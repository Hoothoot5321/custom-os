#include "kernel/globals.h"
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char hex_map[6] = {'a', 'b', 'c', 'd', 'e', 'f'};

void reverse_arr(char *buf, size_t length);

void num_to_str(int64_t num, char out_buf[21]);
void num_to_hex_str(uint64_t num, char out_buf[21]);

static bool print(const char *data, size_t length) {
  const unsigned char *bytes = (const unsigned char *)data;
  for (size_t i = 0; i < length; i++)
    if (putchar(bytes[i]) == EOF)
      return false;
  return true;
}

int printf(const char *restrict format, ...) {
  va_list parameters;
  va_start(parameters, format);

  int written = 0;
  char prefix = '\0';

  while (*format != '\0') {
    char cur_char = *format;
    if ((cur_char != '%') && !prefix) {
      print(&cur_char, sizeof(char));
      format++;
      written++;
      continue;
    }
    char next_char = *(format + 1);
    format++;
    if (next_char == 'c') {
      char val = va_arg(parameters, int);
      print(&val, sizeof(val));
      format++;
      written++;
      continue;
    }
    if (next_char == 's') {

      char *val = va_arg(parameters, char *);
      size_t len = strlen(val);
      print(val, len);
      format++;
      written += len;
      continue;
    }
    if (next_char == 'u' || next_char == 'i') {

      char out_buf[21] = {'\0'};
      if (!prefix) {
        int val = va_arg(parameters, int);
        num_to_str((int64_t)val, out_buf);
      }
      if (prefix == 'l') {
        prefix = '\0';
        long val = va_arg(parameters, long);
        num_to_str(val, out_buf);
      }

      if (prefix == 'L') {
        prefix = '\0';
        long long val = va_arg(parameters, long long);
        num_to_str(val, out_buf);
      }
      size_t len = strlen(out_buf);
      print(out_buf, len);
      format++;
      written += len;
      continue;
    }
    if (next_char == 'x') {
      char out_buf[21] = {'\0'};
      if (!prefix) {
        int val = va_arg(parameters, int);
        num_to_hex_str((uint64_t)(uint32_t)val, out_buf);
      }
      if (prefix == 'l') {
        prefix = '\0';
        long val = va_arg(parameters, long);
        num_to_hex_str((uint64_t)(uint32_t)val, out_buf);
      }
      if (prefix == 'L') {
        prefix = '\0';
        long long val = va_arg(parameters, long long);
        num_to_hex_str((uint64_t)val, out_buf);
      }
      size_t len = strlen(out_buf);
      print(out_buf, len);
      format++;
      written += len;
      continue;
    }
    if (next_char == 'l') {
      if (prefix == 'l') {
        prefix = 'L';
        continue;
      }
      prefix = next_char;
      continue;
    }
    va_end(parameters);
    return -1;
  }
  va_end(parameters);
  return written;
}

void num_to_str(int64_t num, char out_buf[21]) {
  bool is_negative = false;
  if (num == 0) {
    out_buf[0] = '0';
    return;
  }
  if (num < 0) {
    is_negative = true;
  }

  uint64_t u_num = is_negative ? -(uint64_t)num : (uint64_t)num;

  uint64_t index = 0;

  while (u_num > 0) {
    uint8_t rest = u_num % 10;
    out_buf[index] = rest + '0';
    u_num /= 10;
    index++;
  }
  if (is_negative) {
    out_buf[index] = '-';
    index++;
  }

  reverse_arr(out_buf, index);
  return;
}

void num_to_hex_str(uint64_t num, char out_buf[21]) {
  if (num == 0) {
    out_buf[0] = '0';
    return;
  }

  uint64_t index = 0;

  while (num > 0) {
    uint8_t rest = num % 16;
    char digit = rest + '0';
    if (rest > 9) {
      digit = hex_map[rest - 10];
    }
    out_buf[index] = digit;
    num /= 16;
    index++;
  }
  reverse_arr(out_buf, index);
  return;
}

void reverse_arr(char *buf, size_t length) {
  for (size_t i = 0; i < length / 2; i++) {
    char temp = buf[length - i - 1];
    buf[length - i - 1] = buf[i];
    buf[i] = temp;
  }
}
