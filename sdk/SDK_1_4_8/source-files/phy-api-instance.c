
/* (c) SynchronicIT B.V. 2021. All rights reserved.                                   18/09/2024
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
  .onDeviceStatusUpdate = (void*)(0x00207101),
  .resetDevice = (void*)(0x002054bb),
  .getDeviceInfo = (void*)(0x002054c5),
  .enterPowerMode = (void*)(0x002057e9),
  .setPersistentReg = (void*)(0x002057ad),
  .getPersistentReg = (void*)(0x002057cb),
  .otaUpdateReceive = (void*)(0x00204f0d),
  .otaUpdateTransmit = (void*)(0x0020525d),
  .setConfiguration = (void*)(0x00205d7d),
  .getConfiguration = (void*)(0x00205831),
  .storeRxRadioConfig = (void*)(0x00205711),
  .storeTxRadioConfig = (void*)(0x0020575d),
  .importKey = (void*)(0x002055d5),
  .getTicks = (void*)(0x00206019),
  .setPayload = (void*)(0x00206051),
  .onTxUpdate = (void*)(0x00207019),
  .onRxUpdate = (void*)(0x00207045),
  .onLoopbackUpdate = (void*)(0x002070cd),
  .begin = (void*)(0x002066dd),
  .startTx = (void*)(0x00206951),
  .startRx = (void*)(0x00206b65),
  .startContinuousRx = (void*)(0x00206cfd),
  .startLoopback = (void*)(0x00206d19),
  .recalcRx = (void*)(0x002068f5),
  .stopAction = (void*)(0x00206e95),
  .getCir = (void*)(0x00206b4d),
  .end = (void*)(0x00206ecd),
};

const int32_t (*set_ntc_poly_offset)(int32_t offset) = (void*)(0x00201acd);

const int32_t (*set_antenna_delay)(uint32_t ticks) = (void*)(0x00201afb);

const int32_t (*set_hardware_id)(uint32_t id) = (void*)(0x00201b29);

// Caution: Use the SWUP interface at your own risk.
const int32_t (*phscaSwupIntfs_Activate)(const uint32_t comIntfType) = (void*)(0x0020bc71);