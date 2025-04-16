
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

                                                                  USER APPLICATION version 1.4.1
*/


__BOOTLOADER_FLASH_START = (0x00200000);
__BOOTLOADER_FLASH_SIZE  = (0x1000);
__BOOTLOADER_RAM_START   = (0x20004000);
__BOOTLOADER_RAM_SIZE    = (0x6000);

__PRIMARY_FLASH_START    = (0x00201000);
__PRIMARY_FLASH_SIZE     = (0xEF00);
__PRIMARY_RAM_START      = (0x20008000);
__PRIMARY_RAM_SIZE       = (0x2000);

__EEPROM_FLASH_START     = (0x00201000 + 0xEF00);
__EEPROM_FLASH_SIZE      = (0x0100);

__SECONDARY_FLASH_START  = (0x00210000);
__SECONDARY_FLASH_SIZE   = (0x00220000 - __SECONDARY_FLASH_START);
__SECONDARY_RAM_START    = (0x20004000);
__SECONDARY_RAM_SIZE     = (0x4000);

__IRQ_STACK_SIZE  = (0x600);
__USER_STACK_SIZE  = (0xC00);
__IDLE_STACK_SIZE  = (0x400);

/** End of file. Note: this file is auto generated. Do not edit manually.
*   Copyright SynchronicIT 2024
*/
