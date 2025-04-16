
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

                                                                  USER APPLICATION version 1.5.2
*/

#ifndef SFS_USER_APPLICATION_FLASH_H
#define SFS_USER_APPLICATION_FLASH_H
#ifdef __cplusplus
extern "C" {
#endif


#define BOOTLOADER_FLASH_START (0x00200000)
#define BOOTLOADER_FLASH_SIZE  (0x1000)
#define BOOTLOADER_RAM_START   (0x20004000)
#define BOOTLOADER_RAM_SIZE    (0x6000)

#define PRIMARY_FLASH_START    (0x00201000)
#define PRIMARY_FLASH_SIZE     (0xEC00)
#define PRIMARY_RAM_START      (0x20008000)
#define PRIMARY_RAM_SIZE       (0x1000)

#define EEPROM_FLASH_START     (0x00201000 + 0xEC00)
#define EEPROM_FLASH_SIZE      (0x400)

#define SECONDARY_FLASH_START  (0x00210000)
#define SECONDARY_FLASH_SIZE   (0x00220000 - SECONDARY_FLASH_START)
#define SECONDARY_RAM_START (0x20004000)
#define SECONDARY_RAM_SIZE (0x5000)

#define STACK_SIZE  (0x800)


#ifdef __cplusplus
} // extern "C"
#endif
#endif /* SFS_USER_APPLICATION_FLASH_H */


/** End of file. Note: this file is auto generated. Do not edit manually.
*   Copyright SynchronicIT 2025
*/
