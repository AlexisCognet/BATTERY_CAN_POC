/**
 * @file utils.h
 * @author Alexis COGNET (alexis.cognet@sii.fr)
 * @brief Utilitary functions used in a lot of places.
 *
 */

#include "utils.h"
#include "logger.h"
#include "stm32u5xx_hal_def.h"

extern UART_HandleTypeDef huart1;

void sendUSART1Message(const char *format, ...) {
  char message[256];
  va_list ap;

  va_start(ap, format);
  vsnprintf(message, 256, format, ap);
  uint16_t len = strlen(message);

  HAL_StatusTypeDef status;
  if ((status =
           HAL_UART_Transmit(&huart1, (uint8_t *)message, len, 1000) != HAL_OK))
    LOG(WARN, "USART1 message failed to be sent : HAL_Status=%d", status);

  va_end(ap);
}