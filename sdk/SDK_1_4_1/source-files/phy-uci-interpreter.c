
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

#include "phy-uci-interpreter.h"
#include "uci-header.h"

UciStatus UCI_execute(uint8_t* input, uint8_t* output) {

  UciStatus retCode;
  switch (UCI_GID(input)) {
/**__________________________________________________________________________________________**/
/**_____________________________|            Device            |_____________________________**/
    case UCI_DEVICE: {
      switch (UCI_OID(input)) {
        case UCI_DEVICE_RESET_DEVICE: {
          UCI_RSP_HEADER(output, UCI_DEVICE, UCI_DEVICE_RESET_DEVICE, sizeof(ResetDeviceRsp));
          retCode = UWB.resetDevice();
        } break;
        case UCI_DEVICE_GET_DEVICE_INFO: {
          UCI_RSP_HEADER(output, UCI_DEVICE, UCI_DEVICE_GET_DEVICE_INFO, sizeof(GetDeviceInfoRsp));
          retCode = UWB.getDeviceInfo(&(((GetDeviceInfoRspPacket*)output)->payload));
        } break;
        case UCI_DEVICE_ENTER_POWER_MODE: {
          UCI_RSP_HEADER(output, UCI_DEVICE, UCI_DEVICE_ENTER_POWER_MODE, sizeof(EnterPowerModeRsp));
          retCode = UWB.enterPowerMode(&(((EnterPowerModeCmdPacket*)input)->payload));
        } break;
        case UCI_DEVICE_SET_PERSISTENT_REG: {
          UCI_RSP_HEADER(output, UCI_DEVICE, UCI_DEVICE_SET_PERSISTENT_REG, sizeof(SetPersistentRegRsp));
          retCode = UWB.setPersistentReg(&(((SetPersistentRegCmdPacket*)input)->payload));
        } break;
        case UCI_DEVICE_GET_PERSISTENT_REG: {
          UCI_RSP_HEADER(output, UCI_DEVICE, UCI_DEVICE_GET_PERSISTENT_REG, sizeof(GetPersistentRegRsp));
          retCode = UWB.getPersistentReg(&(((GetPersistentRegRspPacket*)output)->payload));
        } break;
        case UCI_DEVICE_OTA_UPDATE_RECEIVE: {
          UCI_RSP_HEADER(output, UCI_DEVICE, UCI_DEVICE_OTA_UPDATE_RECEIVE, sizeof(OtaUpdateReceiveRsp));
          retCode = UWB.otaUpdateReceive(&(((OtaUpdateReceiveCmdPacket*)input)->payload));
        } break;
        case UCI_DEVICE_OTA_UPDATE_TRANSMIT: {
          UCI_RSP_HEADER(output, UCI_DEVICE, UCI_DEVICE_OTA_UPDATE_TRANSMIT, sizeof(OtaUpdateTransmitRsp));
          retCode = UWB.otaUpdateTransmit(&(((OtaUpdateTransmitCmdPacket*)input)->payload));
        } break;
        default: {
          UCI_RSP_HEADER(output, UCI_GID(input), UCI_OID(input), 1);
          retCode = UCI_STATUS_UNKNOWN_OID;
        } break;
      }
    } break;
/**__________________________________________________________________________________________**/
/**_____________________________|        Configuration         |_____________________________**/
    case UCI_CONFIGURATION: {
      switch (UCI_OID(input)) {
        case UCI_CONFIGURATION_SET_CONFIGURATION: {
          UCI_RSP_HEADER(output, UCI_CONFIGURATION, UCI_CONFIGURATION_SET_CONFIGURATION, sizeof(SetConfigurationRsp));
          retCode = UWB.setConfiguration(&(((SetConfigurationCmdPacket*)input)->payload));
        } break;
        case UCI_CONFIGURATION_GET_CONFIGURATION: {
          UCI_RSP_HEADER(output, UCI_CONFIGURATION, UCI_CONFIGURATION_GET_CONFIGURATION, sizeof(GetConfigurationRsp));
          retCode = UWB.getConfiguration(&(((GetConfigurationRspPacket*)output)->payload));
        } break;
        case UCI_CONFIGURATION_STORE_RX_RADIO_CONFIG: {
          UCI_RSP_HEADER(output, UCI_CONFIGURATION, UCI_CONFIGURATION_STORE_RX_RADIO_CONFIG, sizeof(StoreRxRadioConfigRsp));
          retCode = UWB.storeRxRadioConfig(&(((StoreRxRadioConfigCmdPacket*)input)->payload));
        } break;
        case UCI_CONFIGURATION_STORE_TX_RADIO_CONFIG: {
          UCI_RSP_HEADER(output, UCI_CONFIGURATION, UCI_CONFIGURATION_STORE_TX_RADIO_CONFIG, sizeof(StoreTxRadioConfigRsp));
          retCode = UWB.storeTxRadioConfig(&(((StoreTxRadioConfigCmdPacket*)input)->payload));
        } break;
        case UCI_CONFIGURATION_IMPORT_KEY: {
          UCI_RSP_HEADER(output, UCI_CONFIGURATION, UCI_CONFIGURATION_IMPORT_KEY, sizeof(ImportKeyRsp));
          retCode = UWB.importKey(&(((ImportKeyCmdPacket*)input)->payload));
        } break;
        case UCI_CONFIGURATION_GET_TICKS: {
          UCI_RSP_HEADER(output, UCI_CONFIGURATION, UCI_CONFIGURATION_GET_TICKS, sizeof(GetTicksRsp));
          retCode = UWB.getTicks(&(((GetTicksRspPacket*)output)->payload));
        } break;
        case UCI_CONFIGURATION_SET_PAYLOAD: {
          UCI_RSP_HEADER(output, UCI_CONFIGURATION, UCI_CONFIGURATION_SET_PAYLOAD, sizeof(SetPayloadRsp));
          retCode = UWB.setPayload(&(((SetPayloadCmdPacket*)input)->payload));
        } break;
        default: {
          UCI_RSP_HEADER(output, UCI_GID(input), UCI_OID(input), 1);
          retCode = UCI_STATUS_UNKNOWN_OID;
        } break;
      }
    } break;
/**__________________________________________________________________________________________**/
/**_____________________________|           Actions            |_____________________________**/
    case UCI_ACTIONS: {
      switch (UCI_OID(input)) {
        case UCI_ACTIONS_BEGIN: {
          UCI_RSP_HEADER(output, UCI_ACTIONS, UCI_ACTIONS_BEGIN, sizeof(BeginRsp));
          retCode = UWB.begin();
        } break;
        case UCI_ACTIONS_START_TX: {
          UCI_RSP_HEADER(output, UCI_ACTIONS, UCI_ACTIONS_START_TX, sizeof(StartTxRsp));
          retCode = UWB.startTx(&(((StartTxCmdPacket*)input)->payload));
        } break;
        case UCI_ACTIONS_START_RX: {
          UCI_RSP_HEADER(output, UCI_ACTIONS, UCI_ACTIONS_START_RX, sizeof(StartRxRsp));
          retCode = UWB.startRx(&(((StartRxCmdPacket*)input)->payload));
        } break;
        case UCI_ACTIONS_START_CONTINUOUS_RX: {
          UCI_RSP_HEADER(output, UCI_ACTIONS, UCI_ACTIONS_START_CONTINUOUS_RX, sizeof(StartContinuousRxRsp));
          retCode = UWB.startContinuousRx(&(((StartContinuousRxCmdPacket*)input)->payload));
        } break;
        case UCI_ACTIONS_START_LOOPBACK: {
          UCI_RSP_HEADER(output, UCI_ACTIONS, UCI_ACTIONS_START_LOOPBACK, sizeof(StartLoopbackRsp));
          retCode = UWB.startLoopback(&(((StartLoopbackCmdPacket*)input)->payload));
        } break;
        case UCI_ACTIONS_RECALC_RX: {
          UCI_RSP_HEADER(output, UCI_ACTIONS, UCI_ACTIONS_RECALC_RX, sizeof(RecalcRxRsp));
          retCode = UWB.recalcRx(&(((RecalcRxCmdPacket*)input)->payload));
        } break;
        case UCI_ACTIONS_STOP_ACTION: {
          UCI_RSP_HEADER(output, UCI_ACTIONS, UCI_ACTIONS_STOP_ACTION, sizeof(StopActionRsp));
          retCode = UWB.stopAction();
        } break;
        case UCI_ACTIONS_GET_CIR: {
          UCI_RSP_HEADER(output, UCI_ACTIONS, UCI_ACTIONS_GET_CIR, sizeof(GetCirRsp));
          retCode = UWB.getCir(&(((GetCirCmdPacket*)input)->payload), &(((GetCirRspPacket*)output)->payload));
        } break;
        case UCI_ACTIONS_END: {
          UCI_RSP_HEADER(output, UCI_ACTIONS, UCI_ACTIONS_END, sizeof(EndRsp));
          retCode = UWB.end();
        } break;
        default: {
          UCI_RSP_HEADER(output, UCI_GID(input), UCI_OID(input), 1);
          retCode = UCI_STATUS_UNKNOWN_OID;
        } break;
      }
    } break;
    default: { 
      retCode = UCI_STATUS_UNKNOWN_GID;
    } break;
  }
  output[4] = retCode; // always at this postion
  return retCode;
}

/** End of file. Note: this file is auto generated. Do not edit manually.
*   Copyright SynchronicIT 2024
*/
