/**
 * @file can.h
 * @author Alexis COGNET (alexis.cognet@sii.fr)
 *
 * @brief Implements all functions related to CAN communications (sending and
 * receiving).
 *
 * This file defines an interface to the CAN communication module.
 * There are two type of @ref MESSAGE_TYPE that can be sent.
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_

#include "stm32u5xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

enum MESSAGE_TYPE {
  ALERT_WARNING,
  ALERT_CRITICAL,
};

/**
 * @brief Send a CAN message over CAN Bus.
 *
 * @param hfdcan1 Pointer to an FDCAN_HandleTypeDef structure that contains
 *         the configuration information for the specified FDCAN.
 * @param TxData The data to be sent, in this case it's an error code from 0
 * to 5.
 * @param type The type of message to be sent (see MESSAGE_TYPE enum).
 * @return true if message was successefuly sent, false otherwise.
 */
bool SendMessage(uint8_t TxData[1], enum MESSAGE_TYPE type);

/**
 * @brief Handle received CAN message.
 *
 * This callback called when a new message is received by the Fifo.
 *
 * @param hfdcan Pointer to an FDCAN_HandleTypeDef structure that contains
 *         the configuration information for the specified FDCAN.
 * @param RxFifo0ITs Specifies the event that triggered the callback.
 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,
                               uint32_t RxFifo0ITs);
#endif /* INC_CAN_H_ */
