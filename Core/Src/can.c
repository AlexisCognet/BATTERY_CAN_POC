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

bool SendCANMessage(uint8_t TxData[12], enum MESSAGE_TYPE type) {
  FDCAN_TxHeaderTypeDef TxHeader;

  TxHeader.Identifier = 0x100;

  // Use Standard ID not Extended.
  TxHeader.IdType = FDCAN_STANDARD_ID;
  // Send a Data Frame.
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  // Indicate we are sending 12 byte.
  TxHeader.DataLength = FDCAN_DLC_BYTES_12;
  // Allow us to be notified if there is any error in transmission.
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  // Using the same bitrate for both Arbitration and Data Fields
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  // Using the FDCAN standard
  TxHeader.FDFormat = FDCAN_FD_CAN;

  // Not using TxEvent and MessageMarker.
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker = 0;

  HAL_StatusTypeDef status;
  if ((status = HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData)) !=
      HAL_OK) {
    LOG(WARN, "CAN message could not be sent : HAL_status=%d", status);
    return false;
  } else {
    float temperature = TxData[0];
    float intensity = TxData[4];
    float voltage = TxData[8];
    LOG(INFO,
        "Sent CAN message, type=BATTERY_STATUS, Voltage=%dV | "
        "Intensity=%dA | Temperature=%d°C",
        voltage, intensity, temperature);
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
      Error_Handler();
    } else {
      LOG(INFO, "Received CAN message of Id=%u with code=%d.\r\n",
          RxHeader.Identifier, RxData[0]);
    }
  }
}