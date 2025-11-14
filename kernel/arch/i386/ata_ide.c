#include "kernel/globals.h"
#include "kernel/pic.h"
#include <kernel/ata_ide.h>
#include <stddef.h>
#include <stdint.h>

void ata_process_error(uint8_t ata_error) {
  if (ata_error & ATA_ERROR_ADRESS_NOT_FOUND) {
    write_cur_error("ATA adress not found");
    return;
  }

  if (ata_error & ATA_ERROR_COMMAND_ABORTED) {
    write_cur_error("ATA CMD aborted");
    return;
  }

  if (ata_error & ATA_ERROR_ID_NOT_FOUND) {
    write_cur_error("ATA ID not found");
    return;
  }

  if (ata_error & ATA_ERROR_MEDIA_CHANGE_REQUESTED) {
    write_cur_error("ATA MC requested");
    return;
  }

  if (ata_error & ATA_ERROR_UNCOREECTABLE_DATA) {
    write_cur_error("ATA uncorrectable data");
    return;
  }
}
void ata_read_one_section_into_buffer(uint16_t out_buffer[ATA_SECTION_SIZE]) {
  for (int i = 0; i < ATA_SECTION_SIZE; i++) {
    out_buffer[i] = inw(ATA_DATA_PORT);
  }
}

void ata_write_one_section_from_buffer(uint16_t in_buffer[ATA_SECTION_SIZE]) {
  for (int i = 0; i < ATA_SECTION_SIZE; i++) {
    outw(ATA_DATA_PORT, in_buffer[i]);
  }
}
uint8_t ata_get_status() { return inb(ATA_STATUS_PORT); }
int ata_check_busy() {
  if (ata_get_status() & ATA_STATUS_BUSY) {
    write_cur_error("ATA busy");
    return -1;
  }
  return 0;
}
int ata_check_error() {
  if (ata_get_status() & ATA_STATUS_ERR) {
    uint8_t ata_error = inb(ATA_ERROR_PORT);
    ata_process_error(ata_error);
    return -1;
  }
  return 0;
}

int ata_wait_not_busy() {
  size_t start_time = system_timer_counter;
  uint8_t status = ata_get_status();

  while (status & ATA_STATUS_BUSY) {
    if ((system_timer_counter - start_time) / 18 > MAX_LOOP_SECONDS) {
      write_cur_error("ATA not busy");
      return -1;
    }
    status = ata_get_status();
  }
  return 0;
}

int ata_wait_data_ready() {

  size_t start_time = system_timer_counter;
  uint8_t status = ata_get_status();

  while (!(status & ATA_STATUS_DATA_READY)) {
    if (status & ATA_STATUS_ERR) {
      uint8_t err = inb(ATA_ERROR_PORT);
      ata_process_error(err);
      return -1;
    }
    if ((system_timer_counter - start_time) / 18 > MAX_LOOP_SECONDS) {
      write_cur_error("ATA wait data not ready");
      return -1;
    }
    io_wait();
    status = ata_get_status();
  }
  return 0;
}

int ata_req(struct ata_settings ata_sett) {
  if (ata_check_busy() < 0) {
    return -1;
  }
  outb(ATA_DEVICE_CONTROL_PORT, ata_sett.device_control);
  io_wait();
  outb(ATA_DRIVE_PORT,
       ATA_DRIVE_PRIMARY_MASTER | ((ata_sett.sector_adress >> 24) & 0x0F));
  io_wait();
  outb(ATA_SECTOR_COUNT_PORT, ata_sett.sector_count);
  io_wait();
  outb(ATA_SECTOR_LOW_PORT, (ata_sett.sector_adress) & 0xFF);
  io_wait();
  outb(ATA_SECTOR_MID_PORT, (ata_sett.sector_adress >> 8) & 0xFF);
  io_wait();
  outb(ATA_SECTOR_HIGH_PORT, (ata_sett.sector_adress >> 16) & 0xFF);
  io_wait();
  outb(ATA_COMMAND_PORT, ata_sett.cmd);
  io_wait();
  if (ata_check_error() < 0) {
    return -1;
  }

  return 0;
}

int ata_read_section(uint16_t out_buffer[ATA_SECTION_SIZE],
                     uint32_t sector_adress) {
  struct ata_settings settings = {.cmd = ATA_CMD_READ,
                                  .sector_adress = sector_adress,
                                  .device_control =
                                      ATA_DEVICE_CONTROL_DISABLE_INTERRUPT,
                                  .sector_count = 1};
  int out = ata_req(settings);
  if (out < 0) {
    return out;
  }

  out = ata_wait_data_ready();
  if (out < 0) {
    return out;
  }

  ata_read_one_section_into_buffer(out_buffer);
  return ata_wait_not_busy();
}
int ata_write_section(uint16_t in_buffer[ATA_SECTION_SIZE],
                      uint32_t sector_adress) {
  struct ata_settings settings = {.cmd = ATA_CMD_WRITE,
                                  .sector_adress = sector_adress,
                                  .device_control =
                                      ATA_DEVICE_CONTROL_DISABLE_INTERRUPT,
                                  .sector_count = 1};
  int out = ata_req(settings);
  if (out < 0) {
    return out;
  }
  out = ata_wait_data_ready();
  if (out < 0) {
    return out;
  }
  ata_write_one_section_from_buffer(in_buffer);

  return ata_wait_not_busy();
}

int ata_get_max_storage() {
  if (ata_check_busy() < 0) {
    return -1;
  }
  struct ata_settings settings = {.sector_adress = 0,
                                  .sector_count = 0,
                                  .device_control =
                                      ATA_DEVICE_CONTROL_DISABLE_INTERRUPT,
                                  .cmd = ATA_CMD_IDENTIFY_DEVICE};

  int out = ata_req(settings);
  if (out < 0) {
    return out;
  }
  out = ata_wait_data_ready();
  if (out < 0) {
    return out;
  }

  uint16_t out_bytes[ATA_SECTION_SIZE];
  ata_read_one_section_into_buffer(out_bytes);

  uint32_t disk_segments = (out_bytes[61] << 16) | out_bytes[60];
  int disk_space_gb = (disk_segments * 512) / 1073741824;

  return disk_space_gb;
}
