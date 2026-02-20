/**
 * @file utils.h
 * @author Alexis COGNET (alexis.cognet@sii.fr)
 *
 * @brief Utilitary functions used in a lot of places.
 */

#include "stm32u5xx_hal.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Sends a UART message over USART1.
 *
 *  Send a 256 char maximum message over USART1 (connected to ST-LINK).
 *
 * @param format The message to send by UART.
 */
void sendUSART1Message(const char *format, ...);