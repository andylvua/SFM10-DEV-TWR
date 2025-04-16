
/* (c) SynchronicIT B.V. 2021. All rights reserved.                                   05/07/2024
             _____                  _               ______         _             
            / ____|                | |             |  ____|       (_)             TM
           | (___  _   _ _ __   ___| |__  _ __ ___ | |__ _   _ ___ _  ___  _ __  
            \___ \| | | | '_ \ / __| '_ \| '__/ _ \|  __| | | / __| |/ _ \| '_ \ 
            ____) | |_| | | | | (__| | | | | | (_) | |  | |_| \__ \ | (_) | | | |
           |_____/ \__, |_| |_|\___|_| |_|_|  \___/|_|   \__,_|___/_|\___/|_| |_|
                    __/ |                                                        
                   |___/                                 http://www.synchronicit.nl/ 

  This software is confidential and proprietary of SynchronicIT and is subject to the terms and 
  conditions defined in file 'LICENSE.txt', which is part of this source code package. You shall 
  not disclose such Confidential Information and shall use it only in accordance with the terms 
  of the license agreement.

*/

#ifndef SFS_PHY_UCI_INTERPRETER_H_H
#define SFS_PHY_UCI_INTERPRETER_H_H
#ifdef __cplusplus
extern "C" {
#endif

#include "phy-api.h"

UciStatus UCI_execute(uint8_t* input, uint8_t* output);

#ifndef ENABLE_DEVICE_STATUS_NTF
  #define ENABLE_DEVICE_STATUS_NTF 1
#endif

#ifndef ENABLE_TX_TIMESTAMP_NTF
  #define ENABLE_TX_TIMESTAMP_NTF 1
#endif

#ifndef ENABLE_RX_TIMESTAMP_NTF
  #define ENABLE_RX_TIMESTAMP_NTF 1
#endif

#ifndef ENABLE_PAYLOAD_NTF
  #define ENABLE_PAYLOAD_NTF 1
#endif

#ifndef ENABLE_FIRST_PATH_INFO_NTF
  #define ENABLE_FIRST_PATH_INFO_NTF 1
#endif

#ifndef ENABLE_CIR_NTF
  #define ENABLE_CIR_NTF 1
#endif

#ifndef ENABLE_LOOPBACK_TIMESTAMP_NTF
  #define ENABLE_LOOPBACK_TIMESTAMP_NTF 1
#endif


#define ADD_NOTIFICATION_CONTAINERS \
  DeviceStatusNtf deviceStatus; \
  TxTimestampNtf txTimestamp; \
  RxTimestampNtf rxTimestamp; \
  PayloadNtf payload; \
  FirstPathInfoNtf firstPathInfo; \
  CirNtf cir; \
  LoopbackTimestampNtf loopbackTimestamp; \


#if defined(ENABLE_DEVICE_STATUS_NTF) && ENABLE_DEVICE_STATUS_NTF == 1
  #define CONTAINER_DEVICE_STATUS_NTF (&deviceStatus)
#else
   #define CONTAINER_DEVICE_STATUS_NTF (NULL)
#endif
#if defined(ENABLE_TX_TIMESTAMP_NTF) && ENABLE_TX_TIMESTAMP_NTF == 1
  #define CONTAINER_TX_TIMESTAMP_NTF (&txTimestamp)
#else
   #define CONTAINER_TX_TIMESTAMP_NTF (NULL)
#endif
#if defined(ENABLE_RX_TIMESTAMP_NTF) && ENABLE_RX_TIMESTAMP_NTF == 1
  #define CONTAINER_RX_TIMESTAMP_NTF (&rxTimestamp)
#else
   #define CONTAINER_RX_TIMESTAMP_NTF (NULL)
#endif
#if defined(ENABLE_PAYLOAD_NTF) && ENABLE_PAYLOAD_NTF == 1
  #define CONTAINER_PAYLOAD_NTF (&payload)
#else
   #define CONTAINER_PAYLOAD_NTF (NULL)
#endif
#if defined(ENABLE_FIRST_PATH_INFO_NTF) && ENABLE_FIRST_PATH_INFO_NTF == 1
  #define CONTAINER_FIRST_PATH_INFO_NTF (&firstPathInfo)
#else
   #define CONTAINER_FIRST_PATH_INFO_NTF (NULL)
#endif
#if defined(ENABLE_CIR_NTF) && ENABLE_CIR_NTF == 1
  #define CONTAINER_CIR_NTF (&cir)
#else
   #define CONTAINER_CIR_NTF (NULL)
#endif
#if defined(ENABLE_LOOPBACK_TIMESTAMP_NTF) && ENABLE_LOOPBACK_TIMESTAMP_NTF == 1
  #define CONTAINER_LOOPBACK_TIMESTAMP_NTF (&loopbackTimestamp)
#else
   #define CONTAINER_LOOPBACK_TIMESTAMP_NTF (NULL)
#endif


#define ADD_NOTIFICATION_ISR \
  void onDeviceStatusUpdate_ISR(void) { \
    if (CONTAINER_DEVICE_STATUS_NTF) \
    { \
      to_spi_buf(UCI_DEVICE, UCI_DEVICE_DEVICE_STATUS_NTF, (uint8_t*)CONTAINER_DEVICE_STATUS_NTF, sizeof(DeviceStatusNtf)); \
    } \
  } \
  void onTxUpdate_ISR(void) { \
    if (CONTAINER_TX_TIMESTAMP_NTF) \
    { \
      to_spi_buf(UCI_ACTIONS, UCI_ACTIONS_TX_TIMESTAMP_NTF, (uint8_t*)CONTAINER_TX_TIMESTAMP_NTF, sizeof(TxTimestampNtf)); \
    } \
  } \
  void onRxUpdate_ISR(void) { \
    if (CONTAINER_RX_TIMESTAMP_NTF) \
    { \
      to_spi_buf(UCI_ACTIONS, UCI_ACTIONS_RX_TIMESTAMP_NTF, (uint8_t*)CONTAINER_RX_TIMESTAMP_NTF, sizeof(RxTimestampNtf)); \
    } \
    if (CONTAINER_PAYLOAD_NTF) \
    { \
      uint16_t len = 1 + payload.payloadLength + sizeof(payload.payloadLength); \
      to_spi_buf(UCI_ACTIONS, UCI_ACTIONS_PAYLOAD_NTF, (uint8_t*)CONTAINER_PAYLOAD_NTF, len); \
    } \
    if (CONTAINER_FIRST_PATH_INFO_NTF) \
    { \
      to_spi_buf(UCI_ACTIONS, UCI_ACTIONS_FIRST_PATH_INFO_NTF, (uint8_t*)CONTAINER_FIRST_PATH_INFO_NTF, sizeof(FirstPathInfoNtf)); \
    } \
    if (CONTAINER_CIR_NTF) \
    { \
      uint16_t len = cir.cirLength + 4;  \
      to_spi_buf(UCI_ACTIONS, UCI_ACTIONS_CIR_NTF, (uint8_t*)CONTAINER_CIR_NTF, sizeof(CirNtf)); \
    } \
  } \
  void onLoopbackUpdate_ISR(void) { \
    if (CONTAINER_LOOPBACK_TIMESTAMP_NTF) \
    { \
      to_spi_buf(UCI_ACTIONS, UCI_ACTIONS_LOOPBACK_TIMESTAMP_NTF, (uint8_t*)CONTAINER_LOOPBACK_TIMESTAMP_NTF, sizeof(LoopbackTimestampNtf)); \
    } \
  } \


#define SET_NOTIFICATION_HANDLERS \
  UWB.onDeviceStatusUpdate(  \
    onDeviceStatusUpdate_ISR, \
    CONTAINER_DEVICE_STATUS_NTF \
  ); \
  UWB.onTxUpdate(  \
    onTxUpdate_ISR, \
    CONTAINER_TX_TIMESTAMP_NTF \
  ); \
  UWB.onRxUpdate(  \
    onRxUpdate_ISR, \
    CONTAINER_RX_TIMESTAMP_NTF,  \
    CONTAINER_PAYLOAD_NTF, \
    1023,  \
    CONTAINER_FIRST_PATH_INFO_NTF,  \
    CONTAINER_CIR_NTF \
  ); \
  UWB.onLoopbackUpdate(  \
    onLoopbackUpdate_ISR, \
    CONTAINER_LOOPBACK_TIMESTAMP_NTF \
  ); \


#ifdef __cplusplus
} // extern "C"
#endif
#endif /* SFS_PHY_INTERPRET_H */


/** End of file. Note: this file is auto generated. Do not edit manually.
*   Copyright SynchronicIT 2024
*/
