#ifndef ATA_IDE_H
#define ATA_IDE_H
#include <stdint.h>
#define ATA_DATA_PORT 0x1F0
#define ATA_ERROR_PORT 0x1F1
#define ATA_FEATURE_PORT 0x1F1
#define ATA_SECTOR_COUNT_PORT 0x1F2
#define ATA_SECTOR_LOW_PORT 0x1F3
#define ATA_SECTOR_MID_PORT 0x1F4
#define ATA_SECTOR_HIGH_PORT 0x1F5
#define ATA_DRIVE_PORT 0x1F6
#define ATA_STATUS_PORT 0x1F7
#define ATA_COMMAND_PORT 0x1F7
#define ATA_COMMAND_PORT 0x1F7
#define ATA_ALT_STATUS_PORT 0x3F6
#define ATA_DEVICE_CONTROL_PORT 0x3F6

#define ATA_DEVICE_CONTROL_ENABLE_INTERRUPT 0x00
#define ATA_DEVICE_CONTROL_DISABLE_INTERRUPT 0x02

#define ATA_DRIVE_PRIMARY_MASTER 0xE
#define ATA_DRIVE_PRIMARY_SLAVE 0xF

#define ATA_CMD_IDENTIFY_DEVICE 0xEC
#define ATA_CMD_WRITE 0x30
#define ATA_CMD_READ 0x20

#define ATA_STATUS_BUSY 0x80
#define ATA_STATUS_READY 0x40
#define ATA_STATUS_DEVICE_FAULT 0x20
#define ATA_STATUS_DATA_READY 0x08
#define ATA_STATUS_CORRECTED_ERR 0x04
#define ATA_STATUS_ERR 0x01

#define ATA_ERROR_UNCOREECTABLE_DATA 0x40
#define ATA_ERROR_ID_NOT_FOUND 0x10
#define ATA_ERROR_MEDIA_CHANGE_REQUESTED 0x08
#define ATA_ERROR_COMMAND_ABORTED 0x04
#define ATA_ERROR_ADRESS_NOT_FOUND 0x01

#define ATA_SECTION_SIZE 256

int ata_get_max_storage();

struct ata_settings {
  uint32_t sector_adress;
  uint8_t device_control;
  uint8_t sector_count;
  uint8_t cmd;
};

int ata_read_section(uint16_t out_buffer[ATA_SECTION_SIZE],
                     uint32_t sector_adress);

int ata_write_section(uint16_t in_buffer[ATA_SECTION_SIZE],
                      uint32_t sector_adress);

#endif
