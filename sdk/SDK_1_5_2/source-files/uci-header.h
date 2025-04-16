
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

#ifndef SFS_UCI_HEADER_H_H
#define SFS_UCI_HEADER_H_H
#ifdef __cplusplus
extern "C" {
#endif


/**__________________________________________________________________________________________**/
/**_____________________________|      UCI Header Helpers      |_____________________________**/

#ifndef UCI_HEADER
typedef union __attribute__ ((__packed__)) UciHeader {
  uint32_t raw;
  struct {
    uint32_t groupIdentifier :    4;
    uint32_t packetBoundaryFlag : 1;
    uint32_t messageType :        3;


    uint32_t opcodeIdentifier :   6;
    uint32_t rfu :                2;  
    
    uint32_t extendedLength :     2;    
    uint32_t rfu2 :               2;    
    uint32_t endWithCrc :         1;
    uint32_t rfu3 :               3;  


    uint32_t payloadLength :      8;
  } ;  
  struct  {
    uint32_t byte0 : 8;
    uint32_t byte1 : 8;
    uint32_t byte2 : 8;
    uint32_t byte3 : 8;
  } ;
} UciHeader;


#define UCI_CMD 0x01
#define UCI_RSP 0x02
#define UCI_NTF 0x03


#define UCI_MT(ptr)  ((UciHeader*)ptr)->messageType
#define UCI_GID(ptr) ((UciHeader*)ptr)->groupIdentifier
#define UCI_OID(ptr) ((UciHeader*)ptr)->opcodeIdentifier
#define UCI_RAW_HEADER(ptr) ((UciHeader*)ptr)->raw

#define SET_UCI_LEN(ptr, L) {((UciHeader*)ptr)->payloadLength = L & 0xFF; }
#define GET_UCI_LEN(ptr) ((uint16_t)(((UciHeader*)ptr)->payloadLength))

#define UCI_HEADER(ptr, u4gid, u6oid, u10len) \
  UCI_RAW_HEADER(ptr) = 0; \
  UCI_GID(ptr) = u4gid; \
  UCI_OID(ptr) = u6oid; \
  SET_UCI_LEN(ptr, u10len)

#define UCI_CMD_HEADER(ptr, u4gid, u6oid, u10len) \
  UCI_HEADER(ptr, u4gid, u6oid, u10len); \
  UCI_MT(ptr) = UCI_CMD

#define UCI_RSP_HEADER(ptr, u4gid, u6oid, u10len) \
  UCI_HEADER(ptr, u4gid, u6oid, u10len); \
  UCI_MT(ptr) = UCI_RSP

#define UCI_NTF_HEADER(ptr, u4gid, u6oid, u10len) \
  UCI_HEADER(ptr, u4gid, u6oid, u10len); \
  UCI_MT(ptr) = UCI_NTF

#define UCI_PACKET_LENGTH_MAX (4+1023+2)

#endif

#ifdef __cplusplus
} // extern "C"
#endif
#endif /* SFS_UCI_HEADER_H_H */
