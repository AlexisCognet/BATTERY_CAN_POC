/*
 *  can.c
 *
 *  Created on: 17 févr. 2026
 *  Author: alexis.cognet
 */

#include "can.h"
#include "logger.h"
#include "main.h"
#include "stm32u5xx.h"
#include "stm32u5xx_hal_def.h"
#include "stm32u5xx_hal_fdcan.h"
#include "utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern FDCAN_HandleTypeDef hfdcan1;

bool SendCANMessage(uint8_t TxData[8], enum MESSAGE_TYPE type) {
  FDCAN_TxHeaderTypeDef TxHeader;

  TxHeader.Identifier = type;
  // Use Standard ID not Extended.
  TxHeader.IdType = FDCAN_STANDARD_ID;
  // Send a Data Frame.
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  // Indicate we are sending x byte.
  if (type == BATTERY_STATUS) {
    TxHeader.DataLength = FDCAN_DLC_BYTES_8;
  } else if (type == TEMPERATURE_STATUS) {
    TxHeader.DataLength = FDCAN_DLC_BYTES_4;
  }
  // Allow us to be notified if there is any error in transmission.
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  // Using the same bitrate for both Arbitration and Data Fields
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  // Using the CAN standard
  TxHeader.FDFormat = FDCAN_CLASSIC_CAN;

  // Not using TxEvent and MessageMarker.
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker = 0;

  HAL_StatusTypeDef status;
  if ((status = HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData)) !=
      HAL_OK) {
    LOG(WARN, "CAN message could not be sent : HAL_status=%d", status);
    return false;
  } else {
    if (type == BATTERY_STATUS) {
      float voltage, intensity;
      memcpy(&voltage, &TxData[4], sizeof(float));
      memcpy(&intensity, &TxData[0], sizeof(float));
      LOG(INFO,
          "Sent CAN message, type=BATTERY_STATUS | Voltage=%.2fV | "
          "Intensity=%.2fA",
          voltage, intensity);
    } else if (type == TEMPERATURE_STATUS) {
      float temperature;
      memcpy(&temperature, &TxData[0], sizeof(float));
      LOG(INFO,
          "Sent CAN message, type=TEMPERATURE_STATUS | Temperature=%.2f°C",
          temperature);
    }
  }
  return true;
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,
                               uint32_t RxFifo0ITs) {
  FDCAN_RxHeaderTypeDef RxHeader;
  uint8_t RxData[1];

  if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
    /* Retreive Rx messages from RX FIFO0 */
    HAL_StatusTypeDef status;
    if ((status = HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader,
                                         RxData)) != HAL_OK) {
      /* Reception Error */
      LOG(WARN,
          "Failure when trying to retrieve CAN message from FIFO : "
          "HAL_status=%d.",
          status);
    } else {
      LOG(INFO, "Received CAN message : Id=0x%x | code=%d.\r\n",
          RxHeader.Identifier, RxData[0]);
    }
  }
}