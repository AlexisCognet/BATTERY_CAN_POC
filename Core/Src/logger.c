#include "logger.h"

static char *names[LOG_LEVEL_COUNT] = {
    [FAULT] = "[FAULT]",
    [WARN] = "[WARN]",
    [INFO] = "[INFO]",
    [DEBUG] = "[DEBUG]",
};

void LOG(enum LOG_LEVEL level, const char *format, ...) {
  char message[256];
  va_list ap;
  va_start(ap, format);

  vsnprintf(message, 256, format, ap);
  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
  sendUSART1Message("[%02u:%02u:%02u] %s %s\r\n", sTime.Hours, sTime.Minutes,
                    sTime.Seconds, names[level], message);
  va_end(ap);
}