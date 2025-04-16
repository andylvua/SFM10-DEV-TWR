
/* (c) SynchronicIT B.V. 2021. All rights reserved.                                   24/02/2025
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

#include "phy-api.h"


const UCI_t UWB = {
  .onDeviceStatusUpdate = (void*)(0x002076a1),
  .resetDevice = (void*)(0x002058ff),
  .getDeviceInfo = (void*)(0x00205909),
  .enterPowerMode = (void*)(0x00205c2d),
  .setPersistentReg = (void*)(0x00205bf1),
  .getPersistentReg = (void*)(0x00205c0f),
  .otaUpdateReceive = (void*)(0x00205271),
  .otaUpdateTransmit = (void*)(0x002055d5),
  .setConfiguration = (void*)(0x002061c5),
  .getConfiguration = (void*)(0x00205c75),
  .storeRxRadioConfig = (void*)(0x00205b55),
  .storeTxRadioConfig = (void*)(0x00205ba1),
  .importKey = (void*)(0x00205a19),
  .getTicks = (void*)(0x00206461),
  .setPayload = (void*)(0x00206499),
  .onTxUpdate = (void*)(0x002075b9),
  .onRxUpdate = (void*)(0x002075e5),
  .onLoopbackUpdate = (void*)(0x0020766d),
  .begin = (void*)(0x00206609),
  .isBusy = (void*)(0x002069d9),
  .startTx = (void*)(0x00206a05),
  .startRx = (void*)(0x00207221),
  .startContinuousRx = (void*)(0x002073e9),
  .startLoopback = (void*)(0x00206859),
  .recalcRx = (void*)(0x002067f9),
  .stopAction = (void*)(0x00207405),
  .readTxTime = (void*)(0x00206b91),
  .readRxTime = (void*)(0x00206c89),
  .readLoopbackTime = (void*)(0x00206df1),
  .readPayload = (void*)(0x00206d89),
  .readFirstPathInfo = (void*)(0x00206e9d),
  .readCir = (void*)(0x00207209),
  .end = (void*)(0x0020743d),
};

const int32_t (*set_ntc_poly_offset)(int32_t offset) = (void*)(0x00201acd);

const int32_t (*set_antenna_delay)(uint32_t ticks) = (void*)(0x00201afb);

const int32_t (*set_hardware_id)(uint32_t id) = (void*)(0x00201b29);

// Caution: Use the SWUP interface at your own risk.
const int32_t (*phscaSwupIntfs_Activate)(const uint32_t comIntfType) = (void*)(0x0020c259);