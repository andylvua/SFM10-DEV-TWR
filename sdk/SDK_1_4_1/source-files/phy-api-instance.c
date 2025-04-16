
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

#include "phy-api.h"


const UCI_t UWB = {
  .onDeviceStatusUpdate = (void*)(0x00207fb1),
  .resetDevice = (void*)(0x002063ef),
  .getDeviceInfo = (void*)(0x002063f9),
  .enterPowerMode = (void*)(0x0020671d),
  .setPersistentReg = (void*)(0x002066e1),
  .getPersistentReg = (void*)(0x002066ff),
  .otaUpdateReceive = (void*)(0x00205e41),
  .otaUpdateTransmit = (void*)(0x00206191),
  .setConfiguration = (void*)(0x00206c75),
  .getConfiguration = (void*)(0x00206765),
  .storeRxRadioConfig = (void*)(0x00206645),
  .storeTxRadioConfig = (void*)(0x00206691),
  .importKey = (void*)(0x00206509),
  .getTicks = (void*)(0x00206f09),
  .setPayload = (void*)(0x00206f41),
  .onTxUpdate = (void*)(0x00207ec9),
  .onRxUpdate = (void*)(0x00207ef5),
  .onLoopbackUpdate = (void*)(0x00207f7d),
  .begin = (void*)(0x002075c5),
  .startTx = (void*)(0x00207815),
  .startRx = (void*)(0x00207a1d),
  .startContinuousRx = (void*)(0x00207ba9),
  .startLoopback = (void*)(0x00207bc5),
  .recalcRx = (void*)(0x002077b9),
  .stopAction = (void*)(0x00207d45),
  .getCir = (void*)(0x00207a05),
  .end = (void*)(0x00207d7d),
};

const int32_t (*set_ntc_poly_offset)(int32_t offset) = (void*)(0x00201acd);

const int32_t (*set_antenna_delay)(uint32_t ticks) = (void*)(0x00201afb);

const int32_t (*set_hardware_id)(uint32_t id) = (void*)(0x00201b29);

// Caution: Use the SWUP interface at your own risk.
const int32_t (*phscaSwupIntfs_Activate)(const uint32_t comIntfType) = (void*)(0x0020ca4d);