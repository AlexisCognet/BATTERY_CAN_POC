/**
 * @file logger.h
 * @author Alexis Cognet (alexis.cognet@sii.fr)
 *
 * @brief Logging system.
 *
 * This file defines an interface to the logging system.
 * Log messages can be of several @ref LOG_LEVEL "priority levels".
 */
#ifndef LOGGER
#define LOGGER

#include "stm32u5xx_hal.h"
#include "utils.h"

#define LOG_LEVEL_COUNT 4

enum LOG_LEVEL {
  /**
   * @brief Error wich do not make the system crash but some parts of it might
   * not work.
   */
  FAULT,
  /**
   * @brief Event that are not optimal and should be avoided but has little to
   * no effect for the system.
   */
  WARN,
  /**
   * @brief Informations about the BMS or the battery.
   */
  INFO,
  /**
   * @brief Details about the systems.
   */
  DEBUG
};

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;

/**
 * @brief Log a message with the given level over USART1.
 * @note The timestamp start from 00:00:00 when the board is flashed.
 *
 * @param level The priority level of the log.
 * @param format The log message to be displayed.
 *
 */
void LOG(enum LOG_LEVEL level, const char *format, ...);

#endif /* LOGGER */
