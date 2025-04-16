
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

#include "user-application.h"
#include "phy-api.h"
/**__________________________________________________________________________________________**/
/**_____________________________|            Device            |_____________________________**/
/**__________________________________________________________________________________________**/
/**_____________________________|        Configuration         |_____________________________**/
void SetConfiguration_setDefaults(SetConfigurationCmd* p){
                       p->rfu = SETCONFIGURATIONCMD_RFU_DEFAULT;
             p->enableXtalOut = SETCONFIGURATIONCMD_ENABLE_XTAL_OUT_DEFAULT;
              p->currentLimit = SETCONFIGURATIONCMD_CURRENT_LIMIT_DEFAULT;
            p->enablePASignal = SETCONFIGURATIONCMD_ENABLE_P_A_SIGNAL_DEFAULT;
         p->scheduleTimeCheck = SETCONFIGURATIONCMD_SCHEDULE_TIME_CHECK_DEFAULT;
          p->channelFrequency = SETCONFIGURATIONCMD_CHANNEL_FREQUENCY_DEFAULT;
                       p->ppb = SETCONFIGURATIONCMD_PPB_DEFAULT;
                    p->stsIdx = SETCONFIGURATIONCMD_STS_IDX_DEFAULT;
                 p->stsKeyIdx = SETCONFIGURATIONCMD_STS_KEY_IDX_DEFAULT;
                p->txRadioCfg = SETCONFIGURATIONCMD_TX_RADIO_CFG_DEFAULT;
                p->txSyncCode = SETCONFIGURATIONCMD_TX_SYNC_CODE_DEFAULT;
                   p->txPower = SETCONFIGURATIONCMD_TX_POWER_DEFAULT;
          p->txRangingEnabled = SETCONFIGURATIONCMD_TX_RANGING_ENABLED_DEFAULT;
         p->txStsIdxIncrement = SETCONFIGURATIONCMD_TX_STS_IDX_INCREMENT_DEFAULT;
           p->txPayloadCipher = SETCONFIGURATIONCMD_TX_PAYLOAD_CIPHER_DEFAULT;
                p->rxRadioCfg = SETCONFIGURATIONCMD_RX_RADIO_CFG_DEFAULT;
                p->rxSyncCode = SETCONFIGURATIONCMD_RX_SYNC_CODE_DEFAULT;
          p->rxRangingEnabled = SETCONFIGURATIONCMD_RX_RANGING_ENABLED_DEFAULT;
         p->rxStsIdxIncrement = SETCONFIGURATIONCMD_RX_STS_IDX_INCREMENT_DEFAULT;
           p->rxPayloadCipher = SETCONFIGURATIONCMD_RX_PAYLOAD_CIPHER_DEFAULT;
 p->nbInterferenceCancelation = SETCONFIGURATIONCMD_NB_INTERFERENCE_CANCELATION_DEFAULT;
p->channelImpulseResponseStart = SETCONFIGURATIONCMD_CHANNEL_IMPULSE_RESPONSE_START_DEFAULT;
p->channelImpulseResponseLength = SETCONFIGURATIONCMD_CHANNEL_IMPULSE_RESPONSE_LENGTH_DEFAULT;
}

/**__________________________________________________________________________________________**/
/**_____________________________|           Actions            |_____________________________**/

/** End of file. Note: this file is auto generated. Do not edit manually.
*   Copyright SynchronicIT 2024
*/
