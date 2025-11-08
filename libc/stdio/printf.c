#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

  while (*format != '\0') {
    size_t maxrem = INT_MAX - written;

    if (format[0] != '%' || format[1] == '%') {
      if (format[0] == '%')
        format++;
      size_t amount = 1;
      while (format[amount] && format[amount] != '%')
        amount++;
      if (maxrem < amount) {
        // TODO: Set errno to EOVERFLOW.
        return -1;
      }
      if (!print(format, amount))
        return -1;
      format += amount;
      written += amount;
      continue;
    }

    const char *format_begun_at = format++;

    if (*format == 'c') {
      format++;
      char c = (char)va_arg(parameters, int /* char promotes to int */);
      if (!maxrem) {
        // TODO: Set errno to EOVERFLOW.
        return -1;
      }
      if (!print(&c, sizeof(c)))
        return -1;
      written++;
    } else if (*format == 's') {
      format++;
      const char *str = va_arg(parameters, const char *);
      size_t len = strlen(str);
      if (maxrem < len) {
        // TODO: Set errno to EOVERFLOW.
        return -1;
      }
      if (!print(str, len))
        return -1;
      written += len;
    } else if (*format == 'u' || *format == 'i') {
      format++;
      bool is_negative = false;
      int val = va_arg(parameters, int);
      if (val == 0) {
        print("0", 1);
        written++;
        continue;
      }
      if (val < 0) {
        val *= -1;
        is_negative = true;
      }
      uint8_t index = 0;
      char str_out[11] = {0};
      while (val > 0) {
        char digit = val % 10 + '0';
        str_out[index] = digit;
        val /= 10;
        index++;
      }
      for (uint8_t local_index = 0; local_index < index / 2; local_index++) {
        char pre_char = str_out[local_index];
        str_out[local_index] = str_out[index - local_index - 1];
        str_out[index - local_index - 1] = pre_char;
      }
      if (is_negative) {
        str_out[index] = '-';
        index++;
      }
      print(str_out, index);
      written += index;
    } else {
      format = format_begun_at;
      size_t len = strlen(format);
      if (maxrem < len) {
        // TODO: Set errno to EOVERFLOW.
        return -1;
      }
      if (!print(format, len))
        return -1;
      written += len;
      format += len;
    }
  }

  va_end(parameters);
  return written;
}
