
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

#ifndef SFS_PHY_H
#define SFS_PHY_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER 
#  define __attribute__( makeVSCodeHappy ) 
#endif

#include "stdint.h"
#include "uci-header.h"

#define PHY_VERSION_MAJOR 0
#define PHY_VERSION_MINOR 4
#define PHY_VERSION_PATCH 0


/** @brief The MT field indicates the contents of the Packet and shall be a 3-bit field. The 
following values are defined: a Command message (CMD, 0x01) which provides instructions to the 
UWB Stack, a Response message (RSP, 0x02) that is always a (direct) response following a 
command, and a notification message (NTF, 0x03) that informs about some state which can occur 
at any time.
 The attribute type is [enum]. */
typedef enum __attribute__ ((__packed__)) UciMessageType { 
    
  /** @brief Control Packet - Command message as payload. */
  UCI_MESSAGE_TYPE_UCI_CMD                           = 0x01,
    
  /** @brief Control Packet - Command message as payload. */
  UCI_MESSAGE_TYPE_UCI_RSP                           = 0x02,
    
  /** @brief Control Packet - Notification message as payload. */
  UCI_MESSAGE_TYPE_UCI_NTF                           = 0x03,
  UCI_MESSAGE_TYPE_ITEM_COUNT = 3
} UciMessageType;



/** @brief Group Identifier.
 The attribute type is [enum]. */
typedef enum __attribute__ ((__packed__)) Uci { 
    
  /** @brief Core functionality like device power management and security. */
  UCI_DEVICE                                         = 0x0,
    
  /** @brief To get/set the configuration. */
  UCI_CONFIGURATION                                  = 0x1,
    
  /** @brief To start/stop phy actions. */
  UCI_ACTIONS                                        = 0x2,
  UCI_ITEM_COUNT = 3
} Uci;



typedef enum __attribute__ ((__packed__)) UciDevice { 
    
  /** @brief Request for device reset. */
  UCI_DEVICE_RESET_DEVICE                            = 0x00,
    
  /** @brief To notify about the current state of the Ultra-wideband subsystem (UWBS). The UWBS 
  shall generate a notification whenever there is an update in the UWBS state. */
  UCI_DEVICE_DEVICE_STATUS_NTF                       = 0x01,
    
  /** @brief To get the UWBS device information. */
  UCI_DEVICE_GET_DEVICE_INFO                         = 0x02,
    
  /** @brief This command puts the IC into a low power state to save energy. */
  UCI_DEVICE_ENTER_POWER_MODE                        = 0x21,
    
  /** @brief To retain 32 bits of data in RAM that survive a device reset. When the device is 
  resetted using the reset pin / button, resetted via software, or awakens from a deep or hard 
  power down, all data in RAM is cleared. However, there are 4 bytes of data that persist and 
  survive a reset. This can be set as a 32 bit value using this function. To read back the data, 
  use getPeristentReg(). These 32 bits are cleared only after a Power-On Reset. */
  UCI_DEVICE_SET_PERSISTENT_REG                      = 0x23,
    
  /** @brief To read back the 32 bits in RAM that survive a reset. See ``setPersistentReg()``. */
  UCI_DEVICE_GET_PERSISTENT_REG                      = 0x24,
    
  /** @brief Receive over the air update packets. */
  UCI_DEVICE_OTA_UPDATE_RECEIVE                      = 0x31,
    
  /** @brief Transmit over the air update packets. */
  UCI_DEVICE_OTA_UPDATE_TRANSMIT                     = 0x32,
  UCI_DEVICE_ITEM_COUNT = 8
} UciDevice;



typedef enum __attribute__ ((__packed__)) UciConfiguration { 
    
  /** @brief To set the UWBS device configurations. */
  UCI_CONFIGURATION_SET_CONFIGURATION                = 0x04,
    
  /** @brief To get the UWBS device configurations. */
  UCI_CONFIGURATION_GET_CONFIGURATION                = 0x05,
    
  /** @brief Stores RX radio settings in secure flash. The index parameter specifies the index of 
  the radio configuration to be stored. */
  UCI_CONFIGURATION_STORE_RX_RADIO_CONFIG            = 0x25,
    
  /** @brief Stores TX radio settings in secure flash. The index parameter specifies the index of 
  the radio configuration to be stored. */
  UCI_CONFIGURATION_STORE_TX_RADIO_CONFIG            = 0x26,
    
  /** @brief This command imports an AES raw key intended for STS generation or UWB payload 
  ciphering into the system. */
  UCI_CONFIGURATION_IMPORT_KEY                       = 0x22,
    
  /** @brief Returns a timestamp from the running timestamp timer. It shall serve as indication 
  for the application to determine timed RX/TX operations. */
  UCI_CONFIGURATION_GET_TICKS                        = 0x02,
    
  /** @brief For the current session, adds (optional) payload data to the upcoming transmission 
  (TX). */
  UCI_CONFIGURATION_SET_PAYLOAD                      = 0x03,
  UCI_CONFIGURATION_ITEM_COUNT = 7
} UciConfiguration;



typedef enum __attribute__ ((__packed__)) UciActions { 
    
  /** @brief To activate the XTAL and UWB circuitry. */
  UCI_ACTIONS_BEGIN                                  = 0x00,
    
  /** @brief To transmit a UWB message at the specified time, or as soon as possible. */
  UCI_ACTIONS_START_TX                               = 0x04,
    
  /** @brief To start listening for a UWB message at the specified time, or as soon as possible. 
  Stop listening using the StopAction command. */
  UCI_ACTIONS_START_RX                               = 0x05,
    
  /** @brief To start listening for a UWB message at the specified time, or as soon as possible, 
  and restart listening each time a message is received. Stop listening using the StopAction 
  command. */
  UCI_ACTIONS_START_CONTINUOUS_RX                    = 0x06,
    
  /** @brief This command is intended for calibrating the device by determining antenna delay. To 
  do so the command starts a loop-back transmission/reception mode according to the current radio 
  configuration. The antenna delay (i.e. difference between the TX and RX timestamps in the 
  LoopbackTimestamp result) should be subtracted from the round-trip time in the two-way-ranging 
  calculation in order to get accurate distance measurements. */
  UCI_ACTIONS_START_LOOPBACK                         = 0x07,
    
  /** @brief This function re-runs ToA Algorithm on the previously captured CIR Data with the 
  input first path security level. */
  UCI_ACTIONS_RECALC_RX                              = 0x09,
    
  /** @brief This function will stop the current UWB TX/RX action (if any). */
  UCI_ACTIONS_STOP_ACTION                            = 0x08,
    
  /** @brief Returns the most recent Channel Input Response (RX). */
  UCI_ACTIONS_GET_CIR                                = 0x17,
    
  /** @brief To get the most recently captured (RX/TX) timestamp event. */
  UCI_ACTIONS_TX_TIMESTAMP_NTF                       = 0x21,
    
  /** @brief To get the most recently captured (RX/TX) timestamp event. */
  UCI_ACTIONS_RX_TIMESTAMP_NTF                       = 0x22,
    
  /** @brief To get the most recently captured (RX/TX) timestamp event. */
  UCI_ACTIONS_LOOPBACK_TIMESTAMP_NTF                 = 0x23,
    
  /** @brief Returns the most recently received (RX) payload. */
  UCI_ACTIONS_PAYLOAD_NTF                            = 0x25,
    
  /** @brief To get the most recently captured (RX) signal strength. */
  UCI_ACTIONS_FIRST_PATH_INFO_NTF                    = 0x26,
    
  /** @brief Returns the most recent Channel Input Response (RX). */
  UCI_ACTIONS_CIR_NTF                                = 0x27,
    
  /** @brief To deactivate the XTAL and UWB circuitry. */
  UCI_ACTIONS_END                                    = 0x01,
  UCI_ACTIONS_ITEM_COUNT = 15
} UciActions;



/** @brief The frequency of IEEE channel in KHz which could be inserted as a Nominal frequency 
of operation on UWB (channel frequency).
 The attribute type is [enum]. */
typedef enum __attribute__ ((__packed__)) UwbChannel { 
    
  /** @brief The frequency of IEEE channel 5 in KHz. */
  UWB_CHANNEL_5                                      = 6489600ul,
    
  /** @brief The frequency of IEEE channel 6 in KHz. */
  UWB_CHANNEL_6                                      = 6988800ul,
    
  /** @brief The frequency of IEEE channel 8 in KHz. */
  UWB_CHANNEL_8                                      = 7488000ul,
    
  /** @brief The frequency of IEEE channel 9 in KHz. */
  UWB_CHANNEL_9                                      = 7987200ul,
  UWB_CHANNEL_ITEM_COUNT = 4
} UwbChannel;



/** @brief Defines Narrow Band Interference Cancellation (NBIC) feature status.
 The attribute type is [enum]. The value(s) should be between 0x00 and 0x02. The default value is '0x00'.  */
typedef enum __attribute__ ((__packed__)) NbInterferenceCancelation { 
    
  /** @brief Narrow Band Interference Cancellation (NBIC) disabled. */
  NB_INTERFERENCE_CANCELATION_DISABLED               = 0x00,
    
  /** @brief Narrow Band Interference Cancellation (NBIC) enabled. */
  NB_INTERFERENCE_CANCELATION_ENABLED                = 0x01,
    
  /** @brief Narrow Band Interference Cancellation (NBIC) enabled, with reduced current 
  consumption and reduced performance. */
  NB_INTERFERENCE_CANCELATION_ENABLED_ECO            = 0x02,
  NB_INTERFERENCE_CANCELATION_ITEM_COUNT = 3
} NbInterferenceCancelation;



/** @brief After the device has powered up and UWB is initialized, the Device State is set to 
DEVICE_STATE_READY.
 The attribute type is [enum]. */
typedef enum __attribute__ ((__packed__)) DeviceState { 
    
  /** @brief UWB is initialized and ready to begin. This state is the first state after power ON. 
  In this state, the device shall accept all Device and Configuration commands. */
  DEVICE_STATE_READY                                 = 0x01,
    
  /** @brief UWB circuitry is activated. In this state, the device shall accept all commands 
  except for specific changes in UWB configuration. */
  DEVICE_STATE_ACTIVE                                = 0x02,
    
  /** @brief An unrecoverable error occurred. A reset is required. */
  DEVICE_STATE_ERROR                                 = 0xFF,
  DEVICE_STATE_ITEM_COUNT = 3
} DeviceState;



/** @brief Indicates the cause of the state change.
 The attribute type is [enum]. */
typedef enum __attribute__ ((__packed__)) DeviceStateReason { 
    
  /** @brief Power on reset was the reason for the reboot. */
  DEVICE_STATE_REASON_POWER_ON                       = 0x1,
    
  /** @brief Reset pin was the reason for the reboot. */
  DEVICE_STATE_REASON_RESET_PIN                      = 0x2,
    
  /** @brief Reset pin during deep power down was the reason for the reboot. */
  DEVICE_STATE_REASON_DPD_RESET_PIN                  = 0x3,
    
  /** @brief Reset pin during hard power down was the reason for the reboot. */
  DEVICE_STATE_REASON_HPD_RESET_PIN                  = 0x4,
    
  /** @brief Wake up pin during hard power down was the reason for the reboot. */
  DEVICE_STATE_REASON_HPD_WUP_PIN                    = 0x5,
    
  /** @brief Software master reset was the reason for the reboot. */
  DEVICE_STATE_REASON_SOFTWARE_RESET                 = 0x6,
    
  /** @brief Reset caused by power failure on the bias bandgap. */
  DEVICE_STATE_REASON_FAIL_VDD_BIAS_BANDGAP          = 0x7,
    
  /** @brief Reset caused by power failure on the CPU retention power domain. */
  DEVICE_STATE_REASON_FAIL_VDD_CPU_RET               = 0x8,
    
  /** @brief Reset caused by brownout on the CPU retention power domain. */
  DEVICE_STATE_REASON_BROWNOUT_CPU_RET               = 0x9,
    
  /** @brief Power failure on the non-volatile power domain was the reason for the reboot. */
  DEVICE_STATE_REASON_FAIL_VDD_PFLASH                = 0xA,
    
  /** @brief Power failure on the buffer power domain was the reason for the reboot. */
  DEVICE_STATE_REASON_FAIL_VDD_BUF_LOW               = 0xB,
    
  /** @brief Reset caused by the watchdog timer. */
  DEVICE_STATE_REASON_WATCHDOG                       = 0xC,
    
  /** @brief Failed state change request was the reason for the reboot. */
  DEVICE_STATE_REASON_FAILED_STATE_CHANGE            = 0xD,
    
  /** @brief Wake-up timer from deep power down was the reason for the reboot. */
  DEVICE_STATE_REASON_WUP_TIMER                      = 0xE,
    
  /** @brief Snapshot activity from deep power down was the reason for the reboot. */
  DEVICE_STATE_REASON_WUP_SNAPSHOT                   = 0xF,
    
  /** @brief The Device State moved from ACTIVE to READY because there are no active sessions 
  anymore. */
  DEVICE_STATE_REASON_NO_ACTIVE_SESSIONS             = 0x10,
    
  /** @brief The UWBS moved from READY to ACTIVE Device State because one or more ranging 
  sessions are activated. */
  DEVICE_STATE_REASON_SESSION_ACTIVE                 = 0x11,
    
  /** @brief Wake up gpio event during hard power down was the reason for the reboot. */
  DEVICE_STATE_REASON_DPD_GPIO                       = 0x21,
    
  /** @brief The Scrambled Timestamp Sequence (STS) is not configured correctly. */
  DEVICE_STATE_REASON_INVALID_STS_CONFIG             = 0x24,
  DEVICE_STATE_REASON_ITEM_COUNT = 19
} DeviceStateReason;



/** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
 The attribute type is [enum]. */
typedef enum __attribute__ ((__packed__)) UciStatus { 
    
  /** @brief Indicates a successful command, sub command or firmware function execution. */
  UCI_STATUS_SUCCESS                                 = 0,
    
  /** @brief Intended operation is not supported in the current state. */
  UCI_STATUS_REJECTED                                = 0x01,
    
  /** @brief Intended operation failed to complete. */
  UCI_STATUS_FAILED                                  = 0x02,
    
  /** @brief UCI packet structure is not per spec. */
  UCI_STATUS_SYNTAX_ERROR                            = 0x03,
    
  /** @brief Config ID is correct, and value is not specified. */
  UCI_STATUS_INVALID_PARAM                           = 0x04,
    
  /** @brief Config ID is correct, and value is not in proper range. */
  UCI_STATUS_INVALID_RANGE                           = 0x05,
    
  /** @brief UCI packet payload size is not as per spec. */
  UCI_STATUS_INVALID_MESSAGE_SIZE                    = 0x06,
    
  /** @brief UCI Group ID is not per spec. */
  UCI_STATUS_UNKNOWN_GID                             = 0x07,
    
  /** @brief UCI Opcode ID is not per spec. */
  UCI_STATUS_UNKNOWN_OID                             = 0x08,
    
  /** @brief Config ID is read-only. */
  UCI_STATUS_READ_ONLY                               = 0x09,
    
  /** @brief UWBS request retransmission from AP. */
  UCI_STATUS_COMMAND_RETRY                           = 0x0A,
    
  /** @brief Session does not exist (is not created). */
  UCI_STATUS_ERROR_SESSION_NOT_EXIST                 = 0x11,
    
  /** @brief Session exists (is already created). */
  UCI_STATUS_ERROR_SESSION_DUPLICATE                 = 0x12,
    
  /** @brief Session is active. */
  UCI_STATUS_ERROR_SESSION_ACTIVE                    = 0x13,
    
  /** @brief Max. number of sessions already created. */
  UCI_STATUS_ERROR_MAX_SESSIONS_EXCEEDED             = 0x14,
    
  /** @brief Session is not configured with required app configurations. */
  UCI_STATUS_ERROR_SESSION_NOT_CONFIGURED            = 0x15,
    
  /** @brief Sessions are actively running in UWBS. */
  UCI_STATUS_ERROR_ACTIVE_SESSIONS_ONGOING           = 0x16,
    
  /** @brief Indicates when multicast list is full during one to many ranging. */
  UCI_STATUS_ERROR_MULTICAST_LIST_FULL               = 0x17,
    
  /** @brief Indicates when multicast list is full during one to many ranging. */
  UCI_STATUS_ERROR_ADDRESS_NOT_FOUND                 = 0x18,
    
  /** @brief Indicates when short address is already present. */
  UCI_STATUS_ERROR_ADDRESS_ALREADY_PRESENT           = 0x19,
    
  /** @brief Slot length is not supported in the requested configuration. */
  UCI_STATUS_SLOT_LEN_NOT_SUPPORTED                  = 0x1A,
    
  /** @brief Slot length is not supported in the requested configuration. */
  UCI_STATUS_INVALID_SLOT_PER_RR                     = 0x1B,
    
  /** @brief Radio config requires STS configuration. */
  UCI_STATUS_STS_NOT_CONFIGURED                      = 0x1C,
    
  /** @brief Failed to transmit UWB packet. */
  UCI_STATUS_RANGING_TX_FAILED                       = 0x20,
    
  /** @brief No UWB packet detected by the receiver. */
  UCI_STATUS_RANGING_RX_TIMEOUT                      = 0x21,
    
  /** @brief UWB packet channel decoding error. */
  UCI_STATUS_RANGING_RX_PHY_DEC_FAILED               = 0x22,
    
  /** @brief Failed to detect time of arrival of the UWB packet from CIR samples. */
  UCI_STATUS_RANGING_RX_PHY_TOA_FAILED               = 0x23,
    
  /** @brief UWB packet STS segment mismatch. */
  UCI_STATUS_RANGING_RX_PHY_STS_FAILED               = 0x24,
    
  /** @brief MAC CRC or syntax error. */
  UCI_STATUS_RANGING_RX_MAC_DEC_FAILED               = 0x25,
    
  /** @brief IE syntax error. */
  UCI_STATUS_RANGING_RX_MAC_IE_DEC_FAILED            = 0x26,
    
  /** @brief Expected IE missing in the packet. */
  UCI_STATUS_RANGING_RX_MAC_IE_MISSING               = 0x27,
    
  /** @brief Value is not in proper range: too high. */
  UCI_STATUS_PARAM_TOO_HIGH                          = 0x35,
    
  /** @brief Value is not in proper range: too low. */
  UCI_STATUS_PARAM_TOO_LOW                           = 0x36,
    
  /** @brief The buffer provided by the user is too small to fit the received payload. */
  UCI_STATUS_PAYLOAD_TOO_BIG                         = 0x37,
    
  /** @brief This status indicates that the scheduled action cannot be executed because the 
  provided time lies too far into the future. */
  UCI_STATUS_SCHEDULED_TOO_EARLY                     = 0x38,
    
  /** @brief This status indicates that the scheduled action cannot be executed because the 
  provided time lies not far enough into the future. */
  UCI_STATUS_SCHEDULED_TOO_LATE                      = 0x39,
    
  /** @brief Indicates the returning functions failed without further information. */
  UCI_STATUS_GENERAL_ERROR                           = 0x51,
    
  /** @brief Invalid device unit number. */
  UCI_STATUS_BAD_UNIT_NUMBER                         = 0x55,
    
  /** @brief Bad input instance value. */
  UCI_STATUS_BAD_INSTANCE                            = 0x56,
    
  /** @brief Bad input handle. */
  UCI_STATUS_BAD_HANDLE                              = 0x57,
    
  /** @brief Bad input index. */
  UCI_STATUS_BAD_INDEX                               = 0x58,
    
  /** @brief Invalid input parameter. */
  UCI_STATUS_BAD_PARAMETER                           = 0x59,
    
  /** @brief No instances available. */
  UCI_STATUS_NO_INSTANCES                            = 0x5A,
    
  /** @brief Component is not present. */
  UCI_STATUS_NO_COMPONENT                            = 0x5B,
    
  /** @brief Resource is not available. */
  UCI_STATUS_NO_RESOURCES                            = 0x5C,
    
  /** @brief Instance is already in use. */
  UCI_STATUS_INSTANCE_IN_USE                         = 0x5D,
    
  /** @brief Resource is already in use. */
  UCI_STATUS_RESOURCE_OWNED                          = 0x5E,
    
  /** @brief Caller does not own resource. */
  UCI_STATUS_RESOURCE_NOT_OWNED                      = 0x5F,
    
  /** @brief Inconsistent input params. */
  UCI_STATUS_INCONSISTENT_PARAMS                     = 0x60,
    
  /** @brief Component is not initialized. */
  UCI_STATUS_NOT_INITIALIZED                         = 0x61,
    
  /** @brief Component is not enabled. */
  UCI_STATUS_NOT_ENABLED                             = 0x62,
    
  /** @brief Function is not supported. */
  UCI_STATUS_NOT_SUPPORTED                           = 0x63,
    
  /** @brief Initialization failed. */
  UCI_STATUS_INIT_FAILED                             = 0x64,
    
  /** @brief Component is busy. */
  UCI_STATUS_BUSY                                    = 0x65,
    
  /** @brief Component is not busy. */
  UCI_STATUS_NOT_BUSY                                = 0x66,
    
  /** @brief Read error. */
  UCI_STATUS_READ                                    = 0x67,
    
  /** @brief Write error. */
  UCI_STATUS_WRITE                                   = 0x68,
    
  /** @brief Erase error. */
  UCI_STATUS_ERASE                                   = 0x69,
    
  /** @brief Lock error. */
  UCI_STATUS_LOCK                                    = 0x6A,
    
  /** @brief Unlock error. */
  UCI_STATUS_UNLOCK                                  = 0x6B,
    
  /** @brief Memory allocation failed. */
  UCI_STATUS_OUT_OF_MEMORY                           = 0x6C,
    
  /** @brief Bad virtual address. */
  UCI_STATUS_BAD_VIRT_ADDRESS                        = 0x6D,
    
  /** @brief Bad physical address. */
  UCI_STATUS_BAD_PHYS_ADDRESS                        = 0x6E,
    
  /** @brief Timeout error. */
  UCI_STATUS_TIMEOUT                                 = 0x6F,
    
  /** @brief Data overflow/overrun error. */
  UCI_STATUS_OVERFLOW                                = 0x70,
    
  /** @brief Queue (etc.) is full. */
  UCI_STATUS_FULL                                    = 0x71,
    
  /** @brief Queue, TX payload (etc.) is empty. */
  UCI_STATUS_EMPTY                                   = 0x72,
    
  /** @brief Streaming function failed or session should be started first. */
  UCI_STATUS_NOT_STARTED                             = 0x73,
    
  /** @brief Start function failed. */
  UCI_STATUS_ALREADY_STARTED                         = 0x74,
    
  /** @brief Non-streaming function failed. */
  UCI_STATUS_NOT_STOPPED                             = 0x75,
    
  /** @brief Stop function failed. */
  UCI_STATUS_ALREADY_STOPPED                         = 0x76,
    
  /** @brief Setup function failed. */
  UCI_STATUS_ALREADY_SETUP                           = 0x77,
    
  /** @brief Null input parameter. */
  UCI_STATUS_NULL_PARAMETER                          = 0x78,
    
  /** @brief Null data input function. */
  UCI_STATUS_NULL_DATAINFUNC                         = 0x79,
    
  /** @brief Null data output function. */
  UCI_STATUS_NULL_DATAOUTFUNC                        = 0x7A,
    
  /** @brief Null control function. */
  UCI_STATUS_NULL_CONTROLFUNC                        = 0x7B,
    
  /** @brief Null completion function. */
  UCI_STATUS_NULL_COMPLETIONFUNC                     = 0x7C,
    
  /** @brief Null progress function. */
  UCI_STATUS_NULL_PROGRESSFUNC                       = 0x7D,
    
  /** @brief Null error handler function. */
  UCI_STATUS_NULL_ERRORFUNC                          = 0x7E,
    
  /** @brief Null memory alloc function. */
  UCI_STATUS_NULL_MEMALLOCFUNC                       = 0x7F,
    
  /** @brief Null memory free function. */
  UCI_STATUS_NULL_MEMFREEFUNC                        = 0x80,
    
  /** @brief Null configuration function. */
  UCI_STATUS_NULL_CONFIGFUNC                         = 0x81,
    
  /** @brief Null parent data. */
  UCI_STATUS_NULL_PARENT                             = 0x82,
    
  /** @brief Null in/out descriptor. */
  UCI_STATUS_NULL_IODESC                             = 0x83,
    
  /** @brief Null control descriptor. */
  UCI_STATUS_NULL_CTRLDESC                           = 0x84,
    
  /** @brief Unsupported data class. */
  UCI_STATUS_UNSUPPORTED_DATACLASS                   = 0x85,
    
  /** @brief Unsupported data type. */
  UCI_STATUS_UNSUPPORTED_DATATYPE                    = 0x86,
    
  /** @brief Unsupported data subtype. */
  UCI_STATUS_UNSUPPORTED_DATASUBTYPE                 = 0x87,
    
  /** @brief Invalid/unsupported format. */
  UCI_STATUS_FORMAT                                  = 0x88,
    
  /** @brief Bad input descriptor flags. */
  UCI_STATUS_INPUT_DESC_FLAGS                        = 0x89,
    
  /** @brief Bad output descriptor flags. */
  UCI_STATUS_OUTPUT_DESC_FLAGS                       = 0x8A,
    
  /** @brief Capabilities required ???. */
  UCI_STATUS_CAP_REQUIRED                            = 0x8B,
    
  /** @brief Bad TMAL function table. */
  UCI_STATUS_BAD_TMALFUNC_TABLE                      = 0x8C,
    
  /** @brief Invalid channel identifier. */
  UCI_STATUS_INVALID_CHANNEL_ID                      = 0x8D,
    
  /** @brief Invalid command/request. */
  UCI_STATUS_INVALID_COMMAND                         = 0x8E,
    
  /** @brief Stream mode config conflict. */
  UCI_STATUS_STREAM_MODE_CONFUSION                   = 0x8F,
    
  /** @brief Data underflow/underrun. */
  UCI_STATUS_UNDERRUN                                = 0x90,
    
  /** @brief Empty data packet received. */
  UCI_STATUS_EMPTY_PACKET_RECVD                      = 0x91,
    
  /** @brief Other data input/output err. */
  UCI_STATUS_OTHER_DATAINOUT_ERR                     = 0x92,
    
  /** @brief Stop in progress. */
  UCI_STATUS_STOP_REQUESTED                          = 0x93,
    
  /** @brief UART RX Buffer overflow. */
  UCI_STATUS_LPUART_RX_HW_OVERRUN                    = 0x94,
    
  /** @brief UART RX Noise error. */
  UCI_STATUS_LPUART_NOISE_ERROR                      = 0x95,
    
  /** @brief UART RX Framing error. */
  UCI_STATUS_LPUART_FRAMING_ERROR                    = 0x96,
    
  /** @brief UART RX Parity error. */
  UCI_STATUS_LPUART_PARITY_ERROR                     = 0x97,
    
  /** @brief UART STATUS Flags clear failed. */
  UCI_STATUS_LPUART_FLAG_NO_MANUAL_CLEAR             = 0x98,
    
  /** @brief Assertion failure. */
  UCI_STATUS_ASSERTION                               = 0x99,
    
  /** @brief Highway bandwidth bus error. */
  UCI_STATUS_HIGHWAY_BANDWIDTH                       = 0x9A,
    
  /** @brief Hardware reset failed. */
  UCI_STATUS_HW_RESET_FAILED                         = 0x9B,
    
  /** @brief Bad flags. */
  UCI_STATUS_BAD_FLAGS                               = 0x9D,
    
  /** @brief Bad priority. */
  UCI_STATUS_BAD_PRIORITY                            = 0x9E,
    
  /** @brief Bad reference count. */
  UCI_STATUS_BAD_REFERENCE_COUNT                     = 0x9F,
    
  /** @brief Bad setup. */
  UCI_STATUS_BAD_SETUP                               = 0xA0,
    
  /** @brief Bad stack size. */
  UCI_STATUS_BAD_STACK_SIZE                          = 0xA1,
    
  /** @brief Indicates that a RF RX channel does not have retrievable data. */
  UCI_STATUS_IDLE                                    = 0xA2,
    
  /** @brief Null packet. */
  UCI_STATUS_NULL_PACKET                             = 0xA3,
    
  /** @brief A wrong PFLASH page address has been passed as input. */
  UCI_STATUS_PAGE_FLASH_INVLD_PAGE_ADDR_PARAM        = 0xA4,
    
  /** @brief Indicates flash command execution failed. */
  UCI_STATUS_PAGE_FLASH_CMD_EXECUTION_FAILED         = 0xA5,
    
  /** @brief Indicates ongoing PFLASH operation timed out. */
  UCI_STATUS_PAGE_FLASH_OPERATION_TIMEOUT            = 0xA6,
    
  /** @brief Indicates PFLASH Write word command failed. */
  UCI_STATUS_PAGE_FLASH_WRITE_WORD_CMD_FAILED        = 0xA7,
    
  /** @brief Indicates pflash program page command failed. */
  UCI_STATUS_PAGE_FLASH_PROGRAM_PAGE_CMD_FAILED      = 0xA8,
    
  /** @brief Indicates pflash erase page range command failed. */
  UCI_STATUS_PAGE_FLASH_ERASE_PAGE_RANGE_FAILED      = 0xA9,
    
  /** @brief Indicates that a page flash page contains ECC errors. */
  UCI_STATUS_ECC                                     = 0xAA,
    
  /** @brief Indicates PFLASH margin check failed. */
  UCI_STATUS_PAGE_FLASH_MARGIN_CHECK_FAILED          = 0xAB,
    
  /** @brief Indicates PFLASH blank check failed. */
  UCI_STATUS_PAGE_FLASH_BLANK_CHECK_FAILED           = 0xAC,
    
  /** @brief Indicates PFLASH page corrupted. */
  UCI_STATUS_PAGE_FLASH_PAGE_CORRUPTED               = 0xAD,
    
  /** @brief Indicates PFLASH page programmed. */
  UCI_STATUS_PAGE_FLASH_PAGE_PROGRAMMED              = 0xAE,
    
  /** @brief Indicates PFLASH page erased. */
  UCI_STATUS_PAGE_FLASH_PAGE_ERASED                  = 0xAF,
    
  /** @brief Indicates PFLASH checksum page command failed. */
  UCI_STATUS_PAGE_FLASH_CHECKSUM_PAGE_CMD_FAILED     = 0xB0,
    
  /** @brief Indicates PFLASH Erase page failed. */
  UCI_STATUS_PAGE_FLASH_ERASE_PAGE_FAILED            = 0xB1,
    
  /** @brief A wrong PFLASH word address has been passed as input. */
  UCI_STATUS_PAGE_FLASH_INVLD_WORD_ADDR_PARAM        = 0xB2,
    
  /** @brief Indicates that set read mode command execution failed. */
  UCI_STATUS_PAGE_FLASH_SET_READ_MODE_ERROR          = 0xB3,
    
  /** @brief Indicates that a page flash page initialization has failed. */
  UCI_STATUS_PAGE_FLASH_INIT_FAILED                  = 0xB4,
    
  /** @brief Indicates that the page write failed because all AMU wear-leveling mappings are 
  already in use. */
  UCI_STATUS_PAGE_REPAIR                             = 0xB5,
    
  /** @brief Indicates that an image verification failed. */
  UCI_STATUS_IMAGECHECK_FAILED                       = 0xB6,
    
  /** @brief Indicates the function is activated but the overall state is invalid to execute it 
  correctly. */
  UCI_STATUS_INVALID_STATE                           = 0xB7,
    
  /** @brief Corresponds to Operation whose access is protected. */
  UCI_STATUS_OPERATION_NOT_PERMITTED                 = 0xB8,
    
  /** @brief Power down - clocked off. */
  UCI_STATUS_NOT_CLOCKED                             = 0xB9,
    
  /** @brief Invalid PSDU length. */
  UCI_STATUS_INVALID_PSDU_LEN                        = 0xBA,
    
  /** @brief Invalid timeout time. */
  UCI_STATUS_INVALID_TIMEOUT_TIME                    = 0xBB,
    
  /** @brief Indicates that there was a concurrent access to a resource. */
  UCI_STATUS_CONCURRENT_ACCESS                       = 0xBC,
    
  /** @brief Indicates that updating the page flash AMU configuration failed. */
  UCI_STATUS_STORE_AMU                               = 0xBD,
    
  /** @brief Indicaties that configuring the page flash AMU failed. */
  UCI_STATUS_REMAP_AMU                               = 0xBE,
    
  /** @brief Indicaties AMU initialization failed. */
  UCI_STATUS_INIT_AMU                                = 0xBF,
    
  /** @brief Indicates failed page write due to maring verify error. */
  UCI_STATUS_PAGE_MARGIN_VERIFY                      = 0xC0,
    
  /** @brief Indicaties that AMU is disabled. */
  UCI_STATUS_AMU_DISABLED                            = 0xC1,
    
  /** @brief SW Interface compatibility. */
  UCI_STATUS_COMPATIBILITY                           = 0xC2,
    
  /** @brief Indicates TRIMing used hardware RESET defaults only. */
  UCI_STATUS_TRIM_WITH_HARDWARE_DEFAULTS             = 0xC3,
    
  /** @brief Indicates a CRC error in a transmitted data packet. */
  UCI_STATUS_CRC                                     = 0xC4,
    
  /** @brief Indicates flash inter-page copy failed. */
  UCI_STATUS_FLASH_PAGE_COPY                         = 0xC5,
    
  /** @brief Indicates that report ECC log command execution failed. */
  UCI_STATUS_PAGE_FLASH_REPORT_ECC_LOG_ERROR         = 0xC6,
    
  /** @brief RS input is un-decodable, possible due to too many erasures or unsolvable equation 
  or locator does not split. */
  UCI_STATUS_RS_UNDECODABLE                          = 0xC7,
    
  /** @brief No. of Erasures info not valid. */
  UCI_STATUS_RS_ILLEGAL_ERASURES_NUM                 = 0xC8,
    
  /** @brief Erasure location info not valid. */
  UCI_STATUS_RS_ILLEGAL_ERASURES_LOCATION            = 0xC9,
    
  /** @brief Invalid input parameters to rs decoder. */
  UCI_STATUS_RS_ILLEGAL_PARAMS                       = 0xCA,
    
  /** @brief SECDED decoder error state. */
  UCI_STATUS_SECDED_UNDECODABLE                      = 0xCB,
    
  /** @brief Indicates an undetected ToA startup. */
  UCI_STATUS_TOA_UNDETECTED                          = 0xCC,
    
  /** @brief Signature verification failed. */
  UCI_STATUS_VERIFICATION_FAILED                     = 0xCD,
    
  /** @brief This status indicates invalid configuration. */
  UCI_STATUS_INVALID_CFG                             = 0xCE,
    
  /** @brief This status indicates unavailability of PFLASH stored TRIMming data. */
  UCI_STATUS_NO_TRIM_DATA_ERR                        = 0xCF,
    
  /** @brief This status indicates a failing authentication. */
  UCI_STATUS_AUTHENT_FAIL                            = 0xD1,
    
  /** @brief This function must not be called from within a interrupt service routine. */
  UCI_STATUS_CALL_FROM_ISR_NOT_ALLOWED               = 0xD2,
    
  /** @brief Occurs when the CRC verification checksum is wrong. Note that this status is 
  available only if SET_CRC_CFG_CMD is set to enable. */
  UCI_STATUS_CRC_ERROR                               = 0xF8,
    
  /** @brief Default return code value value. */
  UCI_STATUS_NOT_IMPLEMENTED                         = 0xFE,
  UCI_STATUS_ITEM_COUNT = 163
} UciStatus;




typedef struct __attribute__ ((__packed__)) GenericRsp {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;

} GenericRsp;

/**__________________________________________________________________________________________**/
/**_____________________________|            Device            |_____________________________**/

/**__________________________________________________________________________________________**/
/**_____________________________|         Reset device         |_____________________________**/

/** @brief Request for device reset. */

// OID = 0x00

#define ResetDeviceRsp GenericRsp

typedef struct __attribute__ ((__packed__))  ResetDeviceRspPacket {
  UciHeader header;
  ResetDeviceRsp payload;
} ResetDeviceRspPacket;

typedef struct __attribute__ ((__packed__))  ResetDeviceCmdPacket {
  UciHeader header;
  
} ResetDeviceCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|      Device status ntf       |_____________________________**/

/** @brief To notify about the current state of the Ultra-wideband subsystem (UWBS). The UWBS 
shall generate a notification whenever there is an update in the UWBS state. */

// OID = 0x01


/** @brief To notify about the current state of the Ultra-wideband subsystem (UWBS). The UWBS 
shall generate a notification whenever there is an update in the UWBS state. */
typedef struct __attribute__ ((__packed__)) DeviceStatusNtf {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief After the device has powered up and UWB is initialized, the Device State is set to 
  DEVICE_STATE_READY.
   The attribute type is [enum]. */
  uint8_t deviceState;
  /** @brief Indicates the cause of the state change.
   The attribute type is [enum]. */
  uint8_t deviceStateReason;

} DeviceStatusNtf;


typedef struct __attribute__ ((__packed__))  DeviceStatusNtfPacket {
  UciHeader header;
  DeviceStatusNtf payload;
} DeviceStatusNtfPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|       Get device info        |_____________________________**/

/** @brief To get the UWBS device information. */

// OID = 0x02


/** @brief To get the UWBS device information. */
typedef struct __attribute__ ((__packed__)) GetDeviceInfoRsp {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief Major number of the PHY specification version.
   The attribute type is [hex8]. */
  uint8_t phyMajorVersion;
  /** @brief Minor number of the PHY version. .
   The attribute type is [hex8]. */
  uint8_t phyMinorVersion;
  /** @brief Unique device identifier.
   The attribute type is [hex32]. */
  uint32_t serialNumber;
  /** @brief Major number of the application version.
   The attribute type is [hex8]. */
  uint8_t appMajorVersion;
  /** @brief Minor number of the application version. The bit0-bit3 represents the Patch version 
  and bit4-bit7 represents the Minor version.
   The attribute type is [hex8]. */
  uint8_t appMinorVersion;
  /** @brief Silicon Product identifier (major byte).
   The attribute type is [hex8]. */
  uint8_t chipMajorVersion;
  /** @brief Silicon Typecheck identifier (major byte).
   The attribute type is [hex8]. */
  uint8_t chipMinorVersion;
  /** @brief Major number of the ROM functions version.
   The attribute type is [hex8]. */
  uint8_t romMajorVersion;
  /** @brief Minor number of the ROM functions version. The bit0-bit3 represents the Patch 
  version and bit4-bit7 represents the Minor version.
   The attribute type is [hex8]. */
  uint8_t romMinorVersion;
  /** @brief Major number of the Baseband version.
   The attribute type is [hex8]. */
  uint8_t basebandMajorVersion;
  /** @brief Minor number of the Baseband version.
   The attribute type is [hex8]. */
  uint8_t basebandMinorVersion;
  /** @brief Major number of the DSP version.
   The attribute type is [hex8]. */
  uint8_t dspMajorVersion;
  /** @brief Minor number of the DSP version.
   The attribute type is [hex8]. */
  uint8_t dspMinorVersion;

} GetDeviceInfoRsp;


typedef struct __attribute__ ((__packed__))  GetDeviceInfoRspPacket {
  UciHeader header;
  GetDeviceInfoRsp payload;
} GetDeviceInfoRspPacket;

typedef struct __attribute__ ((__packed__))  GetDeviceInfoCmdPacket {
  UciHeader header;
  
} GetDeviceInfoCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|       Enter power mode       |_____________________________**/

/** @brief This command puts the IC into a low power state to save energy. */

// OID = 0x21


/** @brief "Defined bit-fields used in "wake up pin".
 The attribute type is [uint32]. */
typedef union __attribute__ ((__packed__)) {
  /** @brief Raw uint32_t view of "wake up pin". */
  uint32_t raw;

  /** @brief Bit-field view of "wake up pin". */
  struct {
  
    /** @brief Each of the following pins of the IC can be specified as wake up condition. The HPD 
    mode allows only a sub set of the configurations, whereas the DPD modes allow all 
    configurations. If the bit for rising is set, the wake up occurs on the rising edge of the 
    signal. Please note that this configuration is not valid for mode 0x00 (HPD), in which the wake 
    up only occurs on a low signal level. */
    uint32_t wup                                       : 20;
  };

  /** @brief Bit-field groups of "wake up pin". */
  struct {
  
    /** @brief Wake up on the falling edge of the D8 pin (Supported with HPD). */
    uint32_t wup_d8Falling                             : 1;
  
    /** @brief Wake up on the falling edge of the D5 pin. */
    uint32_t wup_d5Falling                             : 1;
  
    /** @brief Wake up on the falling edge of the D7 pin. */
    uint32_t wup_d6Falling                             : 1;
  
    /** @brief Wake up on the falling edge of the D6 pin. */
    uint32_t wup_d7Falling                             : 1;
  
    /** @brief Wake up on the falling edge of the D3 pin. */
    uint32_t wup_d3Falling                             : 1;
  
    /** @brief Wake up on the falling edge of the D0 pin. */
    uint32_t wup_d0Falling                             : 1;
  
    /** @brief Wake up on the falling edge of the RX pin (Supported with HPD). */
    uint32_t wup_rxFalling                             : 1;
  
    /** @brief  Wake up on the falling edge of the TX pin (Supported with HPD). */
    uint32_t wup_txFalling                             : 1;
  
    /** @brief Wake up on the falling edge of the D1 pin. */
    uint32_t wup_d1Falling                             : 1;
  
    /** @brief Wake up on the falling edge of the D2 pin. */
    uint32_t wup_d2Falling                             : 1;
  
    /** @brief Wake up on the rising edge of the D8 pin. */
    uint32_t wup_d8Rising                              : 1;
  
    /** @brief Wake up on the rising edge of the D5 pin. */
    uint32_t wup_d5Rising                              : 1;
  
    /** @brief Wake up on the rising edge of the D7 pin. */
    uint32_t wup_d6Rising                              : 1;
  
    /** @brief Wake up on the rising edge of the D6 pin. */
    uint32_t wup_d7Rising                              : 1;
  
    /** @brief Wake up on the rising edge of the D3 pin. */
    uint32_t wup_d3Rising                              : 1;
  
    /** @brief Wake up on the rising edge of the D0 pin. */
    uint32_t wup_d0Rising                              : 1;
  
    /** @brief Wake up on the rising edge of the RX pin. */
    uint32_t wup_rxRising                              : 1;
  
    /** @brief  Wake up on the rising edge of the TX pin. */
    uint32_t wup_txRising                              : 1;
  
    /** @brief Wake up on the rising edge of the D1 pin. */
    uint32_t wup_d1Rising                              : 1;
  
    /** @brief Wake up on the rising edge of the D2 pin. */
    uint32_t wup_d2Rising                              : 1;
  };
} UwbDeviceEnterPowerModeCmdPayload_WakeUpPin;

/**
 * @brief Defines values of "wake up pin" fields.
 */
typedef enum __attribute__ ((__packed__))
{  
  /** @brief Wake up on the falling edge of the D8 pin (Supported with HPD). */
  WUP_D8_FALLING                                     =  (1uL << 0),  
  /** @brief Wake up on the falling edge of the D5 pin. */
  WUP_D5_FALLING                                     =  (1uL << 1),  
  /** @brief Wake up on the falling edge of the D7 pin. */
  WUP_D6_FALLING                                     =  (1uL << 2),  
  /** @brief Wake up on the falling edge of the D6 pin. */
  WUP_D7_FALLING                                     =  (1uL << 3),  
  /** @brief Wake up on the falling edge of the D3 pin. */
  WUP_D3_FALLING                                     =  (1uL << 4),  
  /** @brief Wake up on the falling edge of the D0 pin. */
  WUP_D0_FALLING                                     =  (1uL << 5),  
  /** @brief Wake up on the falling edge of the RX pin (Supported with HPD). */
  WUP_RX_FALLING                                     =  (1uL << 6),  
  /** @brief  Wake up on the falling edge of the TX pin (Supported with HPD). */
  WUP_TX_FALLING                                     =  (1uL << 7),  
  /** @brief Wake up on the falling edge of the D1 pin. */
  WUP_D1_FALLING                                     =  (1uL << 8),  
  /** @brief Wake up on the falling edge of the D2 pin. */
  WUP_D2_FALLING                                     =  (1uL << 9),  
  /** @brief Wake up on the rising edge of the D8 pin. */
  WUP_D8_RISING                                      =  (1uL << 10),  
  /** @brief Wake up on the rising edge of the D5 pin. */
  WUP_D5_RISING                                      =  (1uL << 11),  
  /** @brief Wake up on the rising edge of the D7 pin. */
  WUP_D6_RISING                                      =  (1uL << 12),  
  /** @brief Wake up on the rising edge of the D6 pin. */
  WUP_D7_RISING                                      =  (1uL << 13),  
  /** @brief Wake up on the rising edge of the D3 pin. */
  WUP_D3_RISING                                      =  (1uL << 14),  
  /** @brief Wake up on the rising edge of the D0 pin. */
  WUP_D0_RISING                                      =  (1uL << 15),  
  /** @brief Wake up on the rising edge of the RX pin. */
  WUP_RX_RISING                                      =  (1uL << 16),  
  /** @brief  Wake up on the rising edge of the TX pin. */
  WUP_TX_RISING                                      =  (1uL << 17),  
  /** @brief Wake up on the rising edge of the D1 pin. */
  WUP_D1_RISING                                      =  (1uL << 18),  
  /** @brief Wake up on the rising edge of the D2 pin. */
  WUP_D2_RISING                                      =  (1uL << 19),
} UwbDeviceEnterPowerModeCmdPayload_WakeUpPinBitmask;

/**
 * @brief Enum values for "uwbDeviceEnterPowerModeCmdPayload".
 */
typedef enum __attribute__ ((__packed__)) 
{
  
  /** @brief The IC is turned off completely. This mode shows the lowest power consumption. At 
  wake up the regular boot flow is entered. All configurations stored in volatile memory are lost 
  after waking up. Via the Wake up pin Configuration the events can be set up, which cause a wake 
  up. The possible pins for wake up are D8, RX and TX. A wake up via the Timeout parameter is not 
  possible. */
  HARD_POWER_DOWN                                    = 0x01,
  
  /** @brief Hard power down mode with VDD buf charge enabled. */
  HARD_POWER_DOWN_WITH_CHARGE                        = 0x03,
  
  /** @brief The regular DPD mode goes through the regular boot flow at wake up. */
  DEEP_POWER_DOWN                                    = 0x18,
  
  /** @brief Same as 0x18 but additionally turning off the boost mode of the LDO for the always 
  on power domain resulting in a lower power consumption. */
  DEEP_POWER_DOWN_OPTIMIZED                          = 0x1A,

} UwbDeviceEnterPowerModeCmdPayload_PowerMode;


/** @brief This command puts the IC into a low power state to save energy. */
typedef struct __attribute__ ((__packed__)) EnterPowerModeCmd {
  /** @brief Wake-up time in ms. Non-zero only allowed in Power Mode 0x8, 0x9 and 0xA. This 
  configures the wake-up timer with the given value.
   The attribute type is [uint32]. */
  uint32_t wakeUpTime;
  /** @brief Wake-up pin configuration. Please note that at least one wake up source has to be 
  configured. If no wake up source is configured a parameter error will be returned.
   The attribute type is [uint32]. */
  uint32_t wakeUpPin;
  /** @brief The Power Mode determines the low power state, which will be entered by this 
  command.
   The attribute type is [enum]. The value(s) should be between 0x01 and 0x1A. */
  uint8_t powerMode;

} EnterPowerModeCmd;


#define EnterPowerModeRsp GenericRsp

typedef struct __attribute__ ((__packed__))  EnterPowerModeRspPacket {
  UciHeader header;
  EnterPowerModeRsp payload;
} EnterPowerModeRspPacket;

typedef struct __attribute__ ((__packed__))  EnterPowerModeCmdPacket {
  UciHeader header;
  EnterPowerModeCmd payload;
} EnterPowerModeCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|      Set persistent reg      |_____________________________**/

/** @brief To retain 32 bits of data in RAM that survive a device reset. When the device is 
resetted using the reset pin / button, resetted via software, or awakens from a deep or hard 
power down, all data in RAM is cleared. However, there are 4 bytes of data that persist and 
survive a reset. This can be set as a 32 bit value using this function. To read back the data, 
use getPeristentReg(). These 32 bits are cleared only after a Power-On Reset. */

// OID = 0x23


/** @brief To retain 32 bits of data in RAM that survive a device reset. When the device is 
resetted using the reset pin / button, resetted via software, or awakens from a deep or hard 
power down, all data in RAM is cleared. However, there are 4 bytes of data that persist and 
survive a reset. This can be set as a 32 bit value using this function. To read back the data, 
use getPeristentReg(). These 32 bits are cleared only after a Power-On Reset. */
typedef struct __attribute__ ((__packed__)) SetPersistentRegCmd {
  /** @brief User-defined value that should survive a (warm) reset.
   The attribute type is [uint32]. */
  uint32_t value;

} SetPersistentRegCmd;


#define SetPersistentRegRsp GenericRsp

typedef struct __attribute__ ((__packed__))  SetPersistentRegRspPacket {
  UciHeader header;
  SetPersistentRegRsp payload;
} SetPersistentRegRspPacket;

typedef struct __attribute__ ((__packed__))  SetPersistentRegCmdPacket {
  UciHeader header;
  SetPersistentRegCmd payload;
} SetPersistentRegCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|      Get persistent reg      |_____________________________**/

/** @brief To read back the 32 bits in RAM that survive a reset. See ``setPersistentReg()``. */

// OID = 0x24


/** @brief To read back the 32 bits in RAM that survive a reset. See ``setPersistentReg()``. */
typedef struct __attribute__ ((__packed__)) GetPersistentRegRsp {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief User-defined value that should survive a (warm) reset.
   The attribute type is [uint32]. */
  uint32_t value;

} GetPersistentRegRsp;


typedef struct __attribute__ ((__packed__))  GetPersistentRegRspPacket {
  UciHeader header;
  GetPersistentRegRsp payload;
} GetPersistentRegRspPacket;

typedef struct __attribute__ ((__packed__))  GetPersistentRegCmdPacket {
  UciHeader header;
  
} GetPersistentRegCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|      Ota update receive      |_____________________________**/

/** @brief Receive over the air update packets. */

// OID = 0x31


/** @brief Receive over the air update packets. */
typedef struct __attribute__ ((__packed__)) OtaUpdateReceiveCmd {
  /** @brief Only OTA update packets transmitted with a matching app version value will be 
  downloaded by the receiver.
   The attribute type is [uint32]. The default value is '0x0'.  */
  uint32_t appVersion;
  /** @brief The CRC over the total binary, including zero padding. Only OTA update packets with 
  a matching CRC will be downloaded. This allows distribution of multiple firmware versions in 
  parallel. In case a 0 is passed, no selection is made based on a predetermined CRC; instead the 
  first OTA update packet that is received will drive the selection of the remaining packets.
   The attribute type is [uint32]. The default value is '0x0'.  */
  uint32_t totalCrc;
  /** @brief To stop when no single OTAU is downloaded within the specified time, after which the 
  user application continues where it was left off. When 0 is provided, no timeout is set.
   The attribute type is [uint32]. The default value is '0'.  */
  uint32_t timeoutSeconds;

} OtaUpdateReceiveCmd;

#define OTAUPDATERECEIVECMD_APP_VERSION_DEFAULT 0x0
#define OTAUPDATERECEIVECMD_TOTAL_CRC_DEFAULT 0x0
#define OTAUPDATERECEIVECMD_TIMEOUT_SECONDS_DEFAULT 0

#define OtaUpdateReceiveRsp GenericRsp

typedef struct __attribute__ ((__packed__))  OtaUpdateReceiveRspPacket {
  UciHeader header;
  OtaUpdateReceiveRsp payload;
} OtaUpdateReceiveRspPacket;

typedef struct __attribute__ ((__packed__))  OtaUpdateReceiveCmdPacket {
  UciHeader header;
  OtaUpdateReceiveCmd payload;
} OtaUpdateReceiveCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|     Ota update transmit      |_____________________________**/

/** @brief Transmit over the air update packets. */

// OID = 0x32


/** @brief Transmit over the air update packets. */
typedef struct __attribute__ ((__packed__)) OtaUpdateTransmitCmd {
  /** @brief Only OTA update packets transmitted with a matching app version value will be 
  downloaded by the receiver.
   The attribute type is [uint32]. The default value is '0x0'.  */
  uint32_t appVersion;
  /** @brief To specify how many times the distribution off all firmware packets must be repeated 
  before the function terminates. A value of 0 indicates indefinitely; firmware distrubution is 
  stopped only via a device reset (which can be scheduled using ``Timers.at(.., reset)``).
   The attribute type is [uint32]. The default value is '0'.  */
  uint32_t repeatCount;
  /** @brief Memory location of the to-be-distributed user application binary.
   The attribute type is [uint32]. */
  uint32_t fileStartPointer;
  /** @brief Length of the to-be-distributed user application binary as a multiple pof 512 bytes. 
  When the last part has a size less then 512, zeros must be padded to make it 512 bytes.
   The attribute type is [uint8]. */
  uint8_t pageCount;

} OtaUpdateTransmitCmd;

#define OTAUPDATETRANSMITCMD_APP_VERSION_DEFAULT 0x0
#define OTAUPDATETRANSMITCMD_REPEAT_COUNT_DEFAULT 0

#define OtaUpdateTransmitRsp GenericRsp

typedef struct __attribute__ ((__packed__))  OtaUpdateTransmitRspPacket {
  UciHeader header;
  OtaUpdateTransmitRsp payload;
} OtaUpdateTransmitRspPacket;

typedef struct __attribute__ ((__packed__))  OtaUpdateTransmitCmdPacket {
  UciHeader header;
  OtaUpdateTransmitCmd payload;
} OtaUpdateTransmitCmdPacket;

/**__________________________________________________________________________________________**/
/**_____________________________|        Configuration         |_____________________________**/

/**__________________________________________________________________________________________**/
/**_____________________________|      Set configuration       |_____________________________**/

/** @brief To set the UWBS device configurations. */

// OID = 0x04

/**
 * @brief Enum values for "uwbSetConfigCmdPayload".
 */
typedef enum __attribute__ ((__packed__)) 
{
  
  /** @brief Narrow Band Interference Cancellation (NBIC) disabled. */
  UWB_SET_CONFIG_CMD_PAYLOAD_DISABLED                = 0x00,
  
  /** @brief Narrow Band Interference Cancellation (NBIC) enabled. */
  UWB_SET_CONFIG_CMD_PAYLOAD_ENABLED                 = 0x01,
  
  /** @brief Narrow Band Interference Cancellation (NBIC) enabled, with reduced current 
  consumption and reduced performance. */
  UWB_SET_CONFIG_CMD_PAYLOAD_ENABLED_ECO             = 0x02,

} Phy_NbInterferenceCancelation;


/** @brief To set the UWBS device configurations. */
typedef struct __attribute__ ((__packed__)) SetConfigurationCmd {
  /** @brief Reserved for future use.
   The attribute type is [uint8]. The default value is '0'.  */
  uint8_t rfu;
  /** @brief To provide the clock generated from the crystal at the XTAL out pin when the Session 
  is ACTIVE. With this functionality it is possible to supply other devices with a clock signal.
   The attribute type is [hex8]. The default value is '1'.  */
  uint8_t enableXtalOut;
  /** @brief Current Limit between 5 and 20 mA. A value of 0 disables the current limiter.
   The attribute type is [uint8]. The value(s) should be between 0 and 20. The default value is '0'.  */
  uint8_t currentLimit;
  /** @brief Output gating signal for an optional external Power Amplifier (PA) at GPIO8.
   The attribute type is [uint8]. The value(s) should be between 0 and 1. The default value is '0'.  */
  uint8_t enablePASignal;
  /** @brief Indicates whether the timestamps passed with ``startTx()`` or  ``startRx()`` should 
  be checked for validity. If the provided timestamp cannot be met, or it lies more than 5 
  seconds in the future, the startTx or startRx will return an error. Disabling this check 
  results in slightly faster code.
   The attribute type is [uint8]. The value(s) should be between 0 and 1. The default value is '1'.  */
  uint8_t scheduleTimeCheck;
  /** @brief Nominal frequency of operation on UWB.
   The attribute type is [uint32]. Units are [Khz]. The value(s) should be between 6400000 and 8000000. The default value is 'UWB_CHANNEL_5'.  */
  uint32_t channelFrequency;
  /** @brief Parts per billion (ppb) frequency deviation of the crystal corresponding to the 
  measured temperature. Supported range: -256000 to +256000.
   The attribute type is [int32]. The default value is '0'.  */
  int32_t ppb;
  /** @brief The salted hash used for configuring the STS.
   The attribute type is [array]. */
  uint8_t keyHash[16];
  /** @brief 0x00000000 - 0xFFFFFFFF ... The starting STS index to be used.
   The attribute type is [uint32]. The default value is '0'.  */
  uint32_t stsIdx;
  /** @brief This is a user-selected key index used to store the imported key in a table of key 
  handles. This key index can then be referenced by other commands. Eight key slots are 
  available. UWB.importKey() is mandatory to store a key first. In case no STS or no specific key 
  is prefered, set the value to 0xFF and the default key (all zeros) will be imported 
  automatically.
   The attribute type is [uint8]. The default value is '0xFF'.  */
  uint8_t stsKeyIdx;
  /** @brief The index indicating radio and frame configuration to be used for the receive mode 
  (TX).
   The attribute type is [uint8]. The value(s) should be between 0 and 15. The default value is '0'.  */
  uint8_t txRadioCfg;
  /** @brief Selects the SYNC code configuration for the RX/TX action in the ranging action. 0x00 
  means to use the sync code defined by the radio setting. 9 - 32 for the (pre-stored) IEEE 
  compliant SYNC code index. 33 - 51 for the (pre-stored) proprietary SYNC code index.
   The attribute type is [uint8]. The value(s) should be between 0x00 and 51. The default value is '0'.  */
  uint8_t txSyncCode;
  /** @brief TX power. Range: -12 dBm to +30 dBm with 0.25 dBm resolution. When going over +14 
  dBm (value 57 and up), one should consider the regulatory limits. It should be used only with 
  the shorter frames ( smaller than 1 ms). Note that the regulatory limits are specified in 
  average power per millisecond. For every 3 dB that you add, the output power doubles.
   The attribute type is [int8]. The value(s) should be between -48 and 120. The default value is '56'.  */
  int8_t txPower;
  /** @brief Depending on the radio setting the PHR might contain the information if ranging is 
  enabled. This parameter is written to the PHR for those settings. 0x00 Ranging disabled; 0x01 
  Ranging enabled.
   The attribute type is [uint8]. The value(s) should be between 0x00 and 0x01. The default value is '1'.  */
  uint8_t txRangingEnabled;
  /** @brief Selects the offset to apply to the STS index expected for the RX or TX action to be 
  performed. The STS index will be incremented by that offset number. In case there is no STS 
  used for this frame or the STS is configured to be constant, this parameter is ignored.
   The attribute type is [int8]. The default value is '0'.  */
  int8_t txStsIdxIncrement;
  /** @brief Payload cipher key handle index (default: 0xFF, disabled). 0x00-0x07 Enables UWB 
  payload cipher and value is index of key handle to be used for ciphering operations. Please 
  note that if UWB payload ciphering is enabled then encryption/decryption key must be configured 
  in advance and UWB payload size is restricted to 95 bytes. 0x08-0xFF Disables UWB payload 
  ciphering.
   The attribute type is [uint8]. The default value is '0xFF'.  */
  uint8_t txPayloadCipher;
  /** @brief The index indicating radio and frame configuration to be used for the receive mode 
  (RX).
   The attribute type is [uint8]. The value(s) should be between 0 and 15. The default value is '0'.  */
  uint8_t rxRadioCfg;
  /** @brief Selects the SYNC code configuration for the RX/TX action in the ranging action. 0x00 
  means to use the sync code defined by the radio setting. 9 - 32 for the (pre-stored) IEEE 
  compliant SYNC code index. 33 - 51 for the (pre-stored) proprietary SYNC code index.
   The attribute type is [uint8]. The value(s) should be between 0x00 and 43. The default value is '0'.  */
  uint8_t rxSyncCode;
  /** @brief Depending on the radio setting the PHR might contain the information if ranging is 
  enabled. This parameter is written to the PHR for those settings. 0x00 Ranging disabled; 0x01 
  Ranging enabled.
   The attribute type is [uint8]. The value(s) should be between 0x00 and 0x01. The default value is '1'.  */
  uint8_t rxRangingEnabled;
  /** @brief Selects the offset to apply to the STS index expected for the RX or TX action to be 
  performed. The STS index will be incremented by that offset number. In case there is no STS 
  used for this frame or the STS is configured to be constant, this parameter is ignored.
   The attribute type is [int8]. The default value is '0'.  */
  int8_t rxStsIdxIncrement;
  /** @brief Payload cipher key handle index (default: 0xFF, disabled). 0x00-0x07 Enables UWB 
  payload cipher and value is index of key handle to be used for ciphering operations. Please 
  note that if UWB payload ciphering is enabled then encryption/decryption key must be configured 
  in advance and UWB payload size is restricted to 95 bytes. 0x08-0xFF Disables UWB payload 
  ciphering.
   The attribute type is [uint8]. The default value is '0xFF'.  */
  uint8_t rxPayloadCipher;
  /** @brief Defines Narrow Band Interference Cancellation (NBIC) feature status.
   The attribute type is [enum]. The value(s) should be between 0x00 and 0x02. The default value is '0x00'.  */
  uint8_t nbInterferenceCancelation;
  /** @brief Number of samples before the Timestamp pulse (First path index) to be included in 
  the CIR notification. 0 means the first sample is at the First path index.
   The attribute type is [int16]. Units are [samples]. The value(s) should be between -500 and 470. The default value is '-16'.  */
  int16_t channelImpulseResponseStart;
  /** @brief Total number of samples to be included in the CIR notification.
   The attribute type is [uint8]. Units are [samples]. The default value is '32'.  */
  uint8_t channelImpulseResponseLength;
  /** @brief The first path security level allows to make a trade-off between security and 
  sensitivity. A higher value increases security while it also results in sensitivity loss.
   The attribute type is [uint8]. The value(s) should be between 0 and 176. The default value is '64'.  */
  uint8_t rxSecurityLevel;

} SetConfigurationCmd;

#define       SETCONFIGURATIONCMD_RFU_DEFAULT 0
#define SETCONFIGURATIONCMD_ENABLE_XTAL_OUT_DEFAULT 1
#define SETCONFIGURATIONCMD_CURRENT_LIMIT_DEFAULT 0
#define SETCONFIGURATIONCMD_ENABLE_P_A_SIGNAL_DEFAULT 0
#define SETCONFIGURATIONCMD_SCHEDULE_TIME_CHECK_DEFAULT 1
#define SETCONFIGURATIONCMD_CHANNEL_FREQUENCY_DEFAULT UWB_CHANNEL_5
#define       SETCONFIGURATIONCMD_PPB_DEFAULT 0
#define   SETCONFIGURATIONCMD_STS_IDX_DEFAULT 0
#define SETCONFIGURATIONCMD_STS_KEY_IDX_DEFAULT 0xFF
#define SETCONFIGURATIONCMD_TX_RADIO_CFG_DEFAULT 0
#define SETCONFIGURATIONCMD_TX_SYNC_CODE_DEFAULT 0
#define  SETCONFIGURATIONCMD_TX_POWER_DEFAULT 56
#define SETCONFIGURATIONCMD_TX_RANGING_ENABLED_DEFAULT 1
#define SETCONFIGURATIONCMD_TX_STS_IDX_INCREMENT_DEFAULT 0
#define SETCONFIGURATIONCMD_TX_PAYLOAD_CIPHER_DEFAULT 0xFF
#define SETCONFIGURATIONCMD_RX_RADIO_CFG_DEFAULT 0
#define SETCONFIGURATIONCMD_RX_SYNC_CODE_DEFAULT 0
#define SETCONFIGURATIONCMD_RX_RANGING_ENABLED_DEFAULT 1
#define SETCONFIGURATIONCMD_RX_STS_IDX_INCREMENT_DEFAULT 0
#define SETCONFIGURATIONCMD_RX_PAYLOAD_CIPHER_DEFAULT 0xFF
#define SETCONFIGURATIONCMD_NB_INTERFERENCE_CANCELATION_DEFAULT 0x00
#define SETCONFIGURATIONCMD_CHANNEL_IMPULSE_RESPONSE_START_DEFAULT -16
#define SETCONFIGURATIONCMD_CHANNEL_IMPULSE_RESPONSE_LENGTH_DEFAULT 32
#define SETCONFIGURATIONCMD_RX_SECURITY_LEVEL_DEFAULT 64

// helper function to set default values:
extern void SetConfiguration_setDefaults(SetConfigurationCmd* p);

#define SetConfigurationRsp GenericRsp

typedef struct __attribute__ ((__packed__))  SetConfigurationRspPacket {
  UciHeader header;
  SetConfigurationRsp payload;
} SetConfigurationRspPacket;

typedef struct __attribute__ ((__packed__))  SetConfigurationCmdPacket {
  UciHeader header;
  SetConfigurationCmd payload;
} SetConfigurationCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|      Get configuration       |_____________________________**/

/** @brief To get the UWBS device configurations. */

// OID = 0x05


/** @brief To get the UWBS device configurations. */
typedef struct __attribute__ ((__packed__)) GetConfigurationRsp {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief To provide the clock generated from the crystal at the XTAL out pin when the Session 
  is ACTIVE. With this functionality it is possible to supply other devices with a clock signal.
   The attribute type is [hex8]. The default value is '1'.  */
  uint8_t enableXtalOut;
  /** @brief Current Limit between 5 and 20 mA. A value of 0 disables the current limiter.
   The attribute type is [uint8]. The value(s) should be between 0 and 20. The default value is '0'.  */
  uint8_t currentLimit;
  /** @brief Output gating signal for an optional external Power Amplifier (PA) at GPIO8.
   The attribute type is [uint8]. The value(s) should be between 0 and 1. The default value is '0'.  */
  uint8_t enablePASignal;
  /** @brief Indicates whether the timestamps passed with ``startTx()`` or  ``startRx()`` should 
  be checked for validity. If the provided timestamp cannot be met, or it lies more than 5 
  seconds in the future, the startTx or startRx will return an error. Disabling this check 
  results in slightly faster code.
   The attribute type is [uint8]. The value(s) should be between 0 and 1. The default value is '1'.  */
  uint8_t scheduleTimeCheck;
  /** @brief Nominal frequency of operation on UWB.
   The attribute type is [uint32]. Units are [Khz]. The value(s) should be between 6400000 and 8000000. The default value is 'UWB_CHANNEL_5'.  */
  uint32_t channelFrequency;
  /** @brief Parts per billion (ppb) frequency deviation of the crystal corresponding to the 
  measured temperature. Supported range: -256000 to +256000.
   The attribute type is [int32]. The default value is '0'.  */
  int32_t ppb;
  /** @brief The salted hash used for configuring the STS.
   The attribute type is [array]. */
  uint8_t keyHash[16];
  /** @brief 0x00000000 - 0xFFFFFFFF ... The starting STS index to be used.
   The attribute type is [uint32]. The default value is '0'.  */
  uint32_t stsIdx;
  /** @brief This is a user-selected key index used to store the imported key in a table of key 
  handles. This key index can then be referenced by other commands. Eight key slots are 
  available. UWB.importKey() is mandatory to store a key first. In case no STS or no specific key 
  is prefered, set the value to 0xFF and the default key (all zeros) will be imported 
  automatically.
   The attribute type is [uint8]. The default value is '0xFF'.  */
  uint8_t stsKeyIdx;
  /** @brief The index indicating radio and frame configuration to be used for the receive mode 
  (TX).
   The attribute type is [uint8]. The value(s) should be between 0 and 15. The default value is '0'.  */
  uint8_t txRadioCfg;
  /** @brief Selects the SYNC code configuration for the RX/TX action in the ranging action. 0x00 
  means to use the sync code defined by the radio setting. 9 - 32 for the (pre-stored) IEEE 
  compliant SYNC code index. 33 - 51 for the (pre-stored) proprietary SYNC code index.
   The attribute type is [uint8]. The value(s) should be between 0x00 and 51. The default value is '0'.  */
  uint8_t txSyncCode;
  /** @brief TX power. Range: -12 dBm to +30 dBm with 0.25 dBm resolution. When going over +14 
  dBm (value 57 and up), one should consider the regulatory limits. It should be used only with 
  the shorter frames ( smaller than 1 ms). Note that the regulatory limits are specified in 
  average power per millisecond. For every 3 dB that you add, the output power doubles.
   The attribute type is [int8]. The value(s) should be between -48 and 120. The default value is '56'.  */
  int8_t txPower;
  /** @brief Depending on the radio setting the PHR might contain the information if ranging is 
  enabled. This parameter is written to the PHR for those settings. 0x00 Ranging disabled; 0x01 
  Ranging enabled.
   The attribute type is [uint8]. The value(s) should be between 0x00 and 0x01. The default value is '1'.  */
  uint8_t txRangingEnabled;
  /** @brief Selects the offset to apply to the STS index expected for the RX or TX action to be 
  performed. The STS index will be incremented by that offset number. In case there is no STS 
  used for this frame or the STS is configured to be constant, this parameter is ignored.
   The attribute type is [int8]. The default value is '0'.  */
  int8_t txStsIdxIncrement;
  /** @brief Payload cipher key handle index (default: 0xFF, disabled). 0x00-0x07 Enables UWB 
  payload cipher and value is index of key handle to be used for ciphering operations. Please 
  note that if UWB payload ciphering is enabled then encryption/decryption key must be configured 
  in advance and UWB payload size is restricted to 95 bytes. 0x08-0xFF Disables UWB payload 
  ciphering.
   The attribute type is [uint8]. The default value is '0xFF'.  */
  uint8_t txPayloadCipher;
  /** @brief The index indicating radio and frame configuration to be used for the receive mode 
  (RX).
   The attribute type is [uint8]. The value(s) should be between 0 and 15. The default value is '0'.  */
  uint8_t rxRadioCfg;
  /** @brief Selects the SYNC code configuration for the RX/TX action in the ranging action. 0x00 
  means to use the sync code defined by the radio setting. 9 - 32 for the (pre-stored) IEEE 
  compliant SYNC code index. 33 - 51 for the (pre-stored) proprietary SYNC code index.
   The attribute type is [uint8]. The value(s) should be between 0x00 and 43. The default value is '0'.  */
  uint8_t rxSyncCode;
  /** @brief Depending on the radio setting the PHR might contain the information if ranging is 
  enabled. This parameter is written to the PHR for those settings. 0x00 Ranging disabled; 0x01 
  Ranging enabled.
   The attribute type is [uint8]. The value(s) should be between 0x00 and 0x01. The default value is '1'.  */
  uint8_t rxRangingEnabled;
  /** @brief Selects the offset to apply to the STS index expected for the RX or TX action to be 
  performed. The STS index will be incremented by that offset number. In case there is no STS 
  used for this frame or the STS is configured to be constant, this parameter is ignored.
   The attribute type is [int8]. The default value is '0'.  */
  int8_t rxStsIdxIncrement;
  /** @brief Payload cipher key handle index (default: 0xFF, disabled). 0x00-0x07 Enables UWB 
  payload cipher and value is index of key handle to be used for ciphering operations. Please 
  note that if UWB payload ciphering is enabled then encryption/decryption key must be configured 
  in advance and UWB payload size is restricted to 95 bytes. 0x08-0xFF Disables UWB payload 
  ciphering.
   The attribute type is [uint8]. The default value is '0xFF'.  */
  uint8_t rxPayloadCipher;
  /** @brief Defines Narrow Band Interference Cancellation (NBIC) feature status.
   The attribute type is [enum]. The value(s) should be between 0x00 and 0x02. The default value is '0x00'.  */
  uint8_t nbInterferenceCancelation;
  /** @brief Number of samples before the Timestamp pulse (First path index) to be included in 
  the CIR notification. 0 means the first sample is at the First path index.
   The attribute type is [int16]. Units are [samples]. The value(s) should be between -500 and 470. The default value is '-16'.  */
  int16_t channelImpulseResponseStart;
  /** @brief Total number of samples to be included in the CIR notification.
   The attribute type is [uint8]. Units are [samples]. The default value is '32'.  */
  uint8_t channelImpulseResponseLength;
  /** @brief The first path security level allows to make a trade-off between security and 
  sensitivity. A higher value increases security while it also results in sensitivity loss.
   The attribute type is [uint8]. The value(s) should be between 0 and 176. The default value is '64'.  */
  uint8_t rxSecurityLevel;

} GetConfigurationRsp;


typedef struct __attribute__ ((__packed__))  GetConfigurationRspPacket {
  UciHeader header;
  GetConfigurationRsp payload;
} GetConfigurationRspPacket;

typedef struct __attribute__ ((__packed__))  GetConfigurationCmdPacket {
  UciHeader header;
  
} GetConfigurationCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|    Store rx radio config     |_____________________________**/

/** @brief Stores RX radio settings in secure flash. The index parameter specifies the index of 
the radio configuration to be stored. */

// OID = 0x25


/** @brief Stores RX radio settings in secure flash. The index parameter specifies the index of 
the radio configuration to be stored. */
typedef struct __attribute__ ((__packed__)) StoreRxRadioConfigCmd {
  /** @brief The index indicating radio and frame configuration to be used for the receive mode 
  (RX).
   The attribute type is [uint8]. The value(s) should be between 0 and 15. The default value is '0'.  */
  uint8_t rxRadioCfg;
  uint8_t radioCfgData[512];

} StoreRxRadioConfigCmd;

#define STORERXRADIOCONFIGCMD_RX_RADIO_CFG_DEFAULT 0

#define StoreRxRadioConfigRsp GenericRsp

typedef struct __attribute__ ((__packed__))  StoreRxRadioConfigRspPacket {
  UciHeader header;
  StoreRxRadioConfigRsp payload;
} StoreRxRadioConfigRspPacket;

typedef struct __attribute__ ((__packed__))  StoreRxRadioConfigCmdPacket {
  UciHeader header;
  StoreRxRadioConfigCmd payload;
} StoreRxRadioConfigCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|    Store tx radio config     |_____________________________**/

/** @brief Stores TX radio settings in secure flash. The index parameter specifies the index of 
the radio configuration to be stored. */

// OID = 0x26


/** @brief Stores TX radio settings in secure flash. The index parameter specifies the index of 
the radio configuration to be stored. */
typedef struct __attribute__ ((__packed__)) StoreTxRadioConfigCmd {
  /** @brief The index indicating radio and frame configuration to be used for the receive mode 
  (TX).
   The attribute type is [uint8]. The value(s) should be between 0 and 15. The default value is '0'.  */
  uint8_t txRadioCfg;
  uint8_t radioCfgData[256];

} StoreTxRadioConfigCmd;

#define STORETXRADIOCONFIGCMD_TX_RADIO_CFG_DEFAULT 0

#define StoreTxRadioConfigRsp GenericRsp

typedef struct __attribute__ ((__packed__))  StoreTxRadioConfigRspPacket {
  UciHeader header;
  StoreTxRadioConfigRsp payload;
} StoreTxRadioConfigRspPacket;

typedef struct __attribute__ ((__packed__))  StoreTxRadioConfigCmdPacket {
  UciHeader header;
  StoreTxRadioConfigCmd payload;
} StoreTxRadioConfigCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|          Import key          |_____________________________**/

/** @brief This command imports an AES raw key intended for STS generation or UWB payload 
ciphering into the system. */

// OID = 0x22

/**
 * @brief Enum values for "uwbDeviceImportKeyCmdPayload".
 */
typedef enum __attribute__ ((__packed__)) 
{
  
  /** @brief Defines key usage for STS generation. */
  UWB_DEVICE_IMPORT_KEY_CMD_PAYLOAD_STS_GENERATION   = 0x00,
  
  /** @brief Defines key usage for  UWB payload ciphering. */
  UWB_DEVICE_IMPORT_KEY_CMD_PAYLOAD_UWB_PAYLOAD_CIPHERING = 0x01,

} UwbDeviceImportKeyCmdPayload_KeyUsage;


/** @brief This command imports an AES raw key intended for STS generation or UWB payload 
ciphering into the system. */
typedef struct __attribute__ ((__packed__)) ImportKeyCmd {
  /** @brief This is a user-selected key index used to store the imported key in a table of key 
  handles. This key index can then be referenced by other commands. Eight key slots are 
  available.
   The attribute type is [uint8]. */
  uint8_t keyIdx;
  /** @brief Raw key provides the raw cryptographic key. For STS generation a AES-128 key is 
  required for UWB payload ciphering.
   The attribute type is [array]. */
  uint8_t rawKey[16];
  /** @brief Provides the usage of key.
   The attribute type is [uint8]. The default value is '0x00'.  */
  uint8_t keyUsage;

} ImportKeyCmd;

#define        IMPORTKEYCMD_KEY_USAGE_DEFAULT 0x00

#define ImportKeyRsp GenericRsp

typedef struct __attribute__ ((__packed__))  ImportKeyRspPacket {
  UciHeader header;
  ImportKeyRsp payload;
} ImportKeyRspPacket;

typedef struct __attribute__ ((__packed__))  ImportKeyCmdPacket {
  UciHeader header;
  ImportKeyCmd payload;
} ImportKeyCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|          Get ticks           |_____________________________**/

/** @brief Returns a timestamp from the running timestamp timer. It shall serve as indication 
for the application to determine timed RX/TX operations. */

// OID = 0x02


/** @brief Returns a timestamp from the running timestamp timer. It shall serve as indication 
for the application to determine timed RX/TX operations. */
typedef struct __attribute__ ((__packed__)) GetTicksRsp {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief The timestamp from the timestamp timer indicates at which exact point in time a UWB 
  ranging message was received or transmitted.
   The attribute type is [uint64]. Units are [ticks]. */
  uint64_t timestamp;

} GetTicksRsp;


typedef struct __attribute__ ((__packed__))  GetTicksRspPacket {
  UciHeader header;
  GetTicksRsp payload;
} GetTicksRspPacket;

typedef struct __attribute__ ((__packed__))  GetTicksCmdPacket {
  UciHeader header;
  
} GetTicksCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|         Set payload          |_____________________________**/

/** @brief For the current session, adds (optional) payload data to the upcoming transmission 
(TX). */

// OID = 0x03


/** @brief For the current session, adds (optional) payload data to the upcoming transmission 
(TX). */
typedef struct __attribute__ ((__packed__)) SetPayloadCmd {
  /** @brief The length of the payload (in bytes) stored in the payloadData buffer.
   The attribute type is [uint16]. */
  uint16_t payloadLength;
  /** @brief Payload data.
   The attribute type is [array]. */
  uint8_t payloadData[1020];

} SetPayloadCmd;


#define SetPayloadRsp GenericRsp

typedef struct __attribute__ ((__packed__))  SetPayloadRspPacket {
  UciHeader header;
  SetPayloadRsp payload;
} SetPayloadRspPacket;

typedef struct __attribute__ ((__packed__))  SetPayloadCmdPacket {
  UciHeader header;
  SetPayloadCmd payload;
} SetPayloadCmdPacket;

/**__________________________________________________________________________________________**/
/**_____________________________|           Actions            |_____________________________**/

/**__________________________________________________________________________________________**/
/**_____________________________|            Begin             |_____________________________**/

/** @brief To activate the XTAL and UWB circuitry. */

// OID = 0x00

#define BeginRsp GenericRsp

typedef struct __attribute__ ((__packed__))  BeginRspPacket {
  UciHeader header;
  BeginRsp payload;
} BeginRspPacket;

typedef struct __attribute__ ((__packed__))  BeginCmdPacket {
  UciHeader header;
  
} BeginCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|           Start tx           |_____________________________**/

/** @brief To transmit a UWB message at the specified time, or as soon as possible. */

// OID = 0x04


/** @brief To transmit a UWB message at the specified time, or as soon as possible. */
typedef struct __attribute__ ((__packed__)) StartTxCmd {
  /** @brief The time (in ticks with 15.65ps resolution) when the transmission is started. A 
  value of 0 means transmit as soon as possible. Fractional timestamp resolution is in ticks, 
  with 128 times 499.2 ticks per microsecond, i.e, 15.65 picosecs per tick. However, the TX 
  hardware only supports timed TX operation with 32 ticks (500.8 ps) resolution. Hence the 
  Fractional TS passed as input will be adjusted accordingly.
   The attribute type is [uint64]. Units are [ticks]. */
  uint64_t time;

} StartTxCmd;


#define StartTxRsp GenericRsp

typedef struct __attribute__ ((__packed__))  StartTxRspPacket {
  UciHeader header;
  StartTxRsp payload;
} StartTxRspPacket;

typedef struct __attribute__ ((__packed__))  StartTxCmdPacket {
  UciHeader header;
  StartTxCmd payload;
} StartTxCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|           Start rx           |_____________________________**/

/** @brief To start listening for a UWB message at the specified time, or as soon as possible. 
Stop listening using the StopAction command. */

// OID = 0x05


/** @brief To start listening for a UWB message at the specified time, or as soon as possible. 
Stop listening using the StopAction command. */
typedef struct __attribute__ ((__packed__)) StartRxCmd {
  /** @brief The time (in ticks with 15.65ps resolution) when the reception is started. A value 
  of 0 means start reception as soon as possible.
   The attribute type is [uint64]. Units are [ticks]. */
  uint64_t time;

} StartRxCmd;


#define StartRxRsp GenericRsp

typedef struct __attribute__ ((__packed__))  StartRxRspPacket {
  UciHeader header;
  StartRxRsp payload;
} StartRxRspPacket;

typedef struct __attribute__ ((__packed__))  StartRxCmdPacket {
  UciHeader header;
  StartRxCmd payload;
} StartRxCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|     Start continuous rx      |_____________________________**/

/** @brief To start listening for a UWB message at the specified time, or as soon as possible, 
and restart listening each time a message is received. Stop listening using the StopAction 
command. */

// OID = 0x06


/** @brief To start listening for a UWB message at the specified time, or as soon as possible, 
and restart listening each time a message is received. Stop listening using the StopAction 
command. */
typedef struct __attribute__ ((__packed__)) StartContinuousRxCmd {
  /** @brief The time (in ticks with 15.65ps resolution) when the reception is started. A value 
  of 0 means start reception as soon as possible.
   The attribute type is [uint64]. Units are [ticks]. */
  uint64_t time;

} StartContinuousRxCmd;


#define StartContinuousRxRsp GenericRsp

typedef struct __attribute__ ((__packed__))  StartContinuousRxRspPacket {
  UciHeader header;
  StartContinuousRxRsp payload;
} StartContinuousRxRspPacket;

typedef struct __attribute__ ((__packed__))  StartContinuousRxCmdPacket {
  UciHeader header;
  StartContinuousRxCmd payload;
} StartContinuousRxCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|        Start loopback        |_____________________________**/

/** @brief This command is intended for calibrating the device by determining antenna delay. To 
do so the command starts a loop-back transmission/reception mode according to the current radio 
configuration. The antenna delay (i.e. difference between the TX and RX timestamps in the 
LoopbackTimestamp result) should be subtracted from the round-trip time in the two-way-ranging 
calculation in order to get accurate distance measurements. */

// OID = 0x07


/** @brief This command is intended for calibrating the device by determining antenna delay. To 
do so the command starts a loop-back transmission/reception mode according to the current radio 
configuration. The antenna delay (i.e. difference between the TX and RX timestamps in the 
LoopbackTimestamp result) should be subtracted from the round-trip time in the two-way-ranging 
calculation in order to get accurate distance measurements. */
typedef struct __attribute__ ((__packed__)) StartLoopbackCmd {
  /** @brief The time (in ticks with 15.65ps resolution) when the loopback is started.
   The attribute type is [uint64]. Units are [ticks]. */
  uint64_t time;

} StartLoopbackCmd;


#define StartLoopbackRsp GenericRsp

typedef struct __attribute__ ((__packed__))  StartLoopbackRspPacket {
  UciHeader header;
  StartLoopbackRsp payload;
} StartLoopbackRspPacket;

typedef struct __attribute__ ((__packed__))  StartLoopbackCmdPacket {
  UciHeader header;
  StartLoopbackCmd payload;
} StartLoopbackCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|          Recalc rx           |_____________________________**/

/** @brief This function re-runs ToA Algorithm on the previously captured CIR Data with the 
input first path security level. */

// OID = 0x09


/** @brief This function re-runs ToA Algorithm on the previously captured CIR Data with the 
input first path security level. */
typedef struct __attribute__ ((__packed__)) RecalcRxCmd {
  /** @brief The first path security level allows to make a trade-off between security and 
  sensitivity. A higher value increases security while it also results in sensitivity loss.
   The attribute type is [uint8]. The value(s) should be between 0 and 176. The default value is '64'.  */
  uint8_t rxSecurityLevel;

} RecalcRxCmd;

#define RECALCRXCMD_RX_SECURITY_LEVEL_DEFAULT 64

#define RecalcRxRsp GenericRsp

typedef struct __attribute__ ((__packed__))  RecalcRxRspPacket {
  UciHeader header;
  RecalcRxRsp payload;
} RecalcRxRspPacket;

typedef struct __attribute__ ((__packed__))  RecalcRxCmdPacket {
  UciHeader header;
  RecalcRxCmd payload;
} RecalcRxCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|         Stop action          |_____________________________**/

/** @brief This function will stop the current UWB TX/RX action (if any). */

// OID = 0x08

#define StopActionRsp GenericRsp

typedef struct __attribute__ ((__packed__))  StopActionRspPacket {
  UciHeader header;
  StopActionRsp payload;
} StopActionRspPacket;

typedef struct __attribute__ ((__packed__))  StopActionCmdPacket {
  UciHeader header;
  
} StopActionCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|           Get cir            |_____________________________**/

/** @brief Returns the most recent Channel Input Response (RX). */

// OID = 0x17


/** @brief Returns the most recent Channel Input Response (RX). */
typedef struct __attribute__ ((__packed__)) GetCirRsp {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief Indicates at which sample the first path is detected. A value of 255 means that the 
  first path index is not present in the samples contained.
   The attribute type is [uint8]. The default value is '255'.  */
  uint8_t fpIndex;
  /** @brief The length of the payload (in bytes) stored in the cirData buffer.
   The attribute type is [uint16]. */
  uint16_t cirLength;
  /** @brief The CIR buffer data. Contains at most 255 samples. The cir data consists of a 16 bit 
  real part and 16 bit imaginary part.
   The attribute type is [array]. */
  int16_t cirData[510];

} GetCirRsp;



/** @brief Returns the most recent Channel Input Response (RX). */
typedef struct __attribute__ ((__packed__)) GetCirCmd {
  /** @brief Number of samples before the Timestamp pulse (First path index) to be included in 
  the CIR notification. 0 means the first sample is at the First path index.
   The attribute type is [int16]. Units are [samples]. The value(s) should be between -500 and 470. The default value is '-16'.  */
  int16_t channelImpulseResponseStart;
  /** @brief Total number of samples to be included in the CIR notification.
   The attribute type is [uint8]. Units are [samples]. The default value is '32'.  */
  uint8_t channelImpulseResponseLength;

} GetCirCmd;

#define GETCIRCMD_CHANNEL_IMPULSE_RESPONSE_START_DEFAULT -16
#define GETCIRCMD_CHANNEL_IMPULSE_RESPONSE_LENGTH_DEFAULT 32

typedef struct __attribute__ ((__packed__))  GetCirRspPacket {
  UciHeader header;
  GetCirRsp payload;
} GetCirRspPacket;

typedef struct __attribute__ ((__packed__))  GetCirCmdPacket {
  UciHeader header;
  GetCirCmd payload;
} GetCirCmdPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|       Tx timestamp ntf       |_____________________________**/

/** @brief To get the most recently captured (RX/TX) timestamp event. */

// OID = 0x21


/** @brief To get the most recently captured (RX/TX) timestamp event. */
typedef struct __attribute__ ((__packed__)) TxTimestampNtf {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief The captured Timestamp timer value for when the message is transmitted (TX). The 
  units are in ticks and to convert its value to seconds you have to multiply it by 
  1/(128*499.2e6).
   The attribute type is [uint64]. Units are [ticks]. */
  uint64_t txTimestamp;

} TxTimestampNtf;


typedef struct __attribute__ ((__packed__))  TxTimestampNtfPacket {
  UciHeader header;
  TxTimestampNtf payload;
} TxTimestampNtfPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|       Rx timestamp ntf       |_____________________________**/

/** @brief To get the most recently captured (RX/TX) timestamp event. */

// OID = 0x22


/** @brief To get the most recently captured (RX/TX) timestamp event. */
typedef struct __attribute__ ((__packed__)) RxTimestampNtf {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief The captured Timestamp timer value for when the message is received (RX). The units 
  are in ticks and to convert its value to seconds you have to multiply it by 1/(128*499.2e6).
   The attribute type is [uint64]. Units are [ticks]. */
  uint64_t rxTimestamp;
  /** @brief The device in Receive operation computes the frequency offset(CFO_INT) between 
  transmitter and receiver carrier freq. This value will be provided after the Rx operation. To 
  compute the ppm from this CFO_INT value, the following are the steps: A) Compute CFO in rad/pi 
  per clock cycle: CFO = CFO_INT/2^17. B) Compute CFO(Hz) = (124.8MHz/2)*CFO. C) Compute CFO(ppm) 
  = CFO(Hz) / (fc/1e6), where fc is the carrier frequency and for channel 9 it will be 7.9872GHz.
   The attribute type is [int16]. */
  int16_t rxCarrierFreqOffset;

} RxTimestampNtf;


typedef struct __attribute__ ((__packed__))  RxTimestampNtfPacket {
  UciHeader header;
  RxTimestampNtf payload;
} RxTimestampNtfPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|    Loopback timestamp ntf    |_____________________________**/

/** @brief To get the most recently captured (RX/TX) timestamp event. */

// OID = 0x23


/** @brief To get the most recently captured (RX/TX) timestamp event. */
typedef struct __attribute__ ((__packed__)) LoopbackTimestampNtf {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief The captured Timestamp timer value for when the message is transmitted (TX). The 
  units are in ticks and to convert its value to seconds you have to multiply it by 
  1/(128*499.2e6).
   The attribute type is [uint64]. Units are [ticks]. */
  uint64_t txTimestamp;
  /** @brief The captured Timestamp timer value for when the message is received (RX). The units 
  are in ticks and to convert its value to seconds you have to multiply it by 1/(128*499.2e6).
   The attribute type is [uint64]. Units are [ticks]. */
  uint64_t rxTimestamp;
  /** @brief The device in Receive operation computes the frequency offset(CFO_INT) between 
  transmitter and receiver carrier freq. This value will be provided after the Rx operation. To 
  compute the ppm from this CFO_INT value, the following are the steps: A) Compute CFO in rad/pi 
  per clock cycle: CFO = CFO_INT/2^17. B) Compute CFO(Hz) = (124.8MHz/2)*CFO. C) Compute CFO(ppm) 
  = CFO(Hz) / (fc/1e6), where fc is the carrier frequency and for channel 9 it will be 7.9872GHz.
   The attribute type is [int16]. */
  int16_t rxCarrierFreqOffset;

} LoopbackTimestampNtf;


typedef struct __attribute__ ((__packed__))  LoopbackTimestampNtfPacket {
  UciHeader header;
  LoopbackTimestampNtf payload;
} LoopbackTimestampNtfPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|         Payload ntf          |_____________________________**/

/** @brief Returns the most recently received (RX) payload. */

// OID = 0x25


/** @brief Returns the most recently received (RX) payload. */
typedef struct __attribute__ ((__packed__)) PayloadNtf {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief The length of the payload (in bytes) stored in the payloadData buffer.
   The attribute type is [uint16]. */
  uint16_t payloadLength;
  /** @brief Payload data.
   The attribute type is [array]. */
  uint8_t payloadData[1020];

} PayloadNtf;


typedef struct __attribute__ ((__packed__))  PayloadNtfPacket {
  UciHeader header;
  PayloadNtf payload;
} PayloadNtfPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|     First path info ntf      |_____________________________**/

/** @brief To get the most recently captured (RX) signal strength. */

// OID = 0x26


/** @brief To get the most recently captured (RX) signal strength. */
typedef struct __attribute__ ((__packed__)) FirstPathInfoNtf {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief Indicates the first path was detected and the ToA estimation results are valid. The 
  first path detected status is indicated at the least significant bit position. i.e 
  b_firstPathdetected = (bool) (firstPathdetected & 0x1).
   The attribute type is [uint32]. */
  uint32_t firstPathDetected;
  /** @brief Reserved for future use.
   The attribute type is [uint32]. */
  uint32_t rfu1;
  /** @brief Index of first crossing of the detection threshold.
   The attribute type is [uint32]. */
  uint32_t edgeIdx;
  /** @brief Interpolated ToA result (first path lead distance) in samples. ToA result = time / 
  2^15. The first path lead distance is the distance between the end of CIR and the estimated 
  first path.
   The attribute type is [int32]. */
  int32_t fpTime;
  /** @brief Index of strongest path. Variable maxTapIndex is computed from simplified 
  approximation in HW instead of complex CIR computation.
   The attribute type is [uint16]. */
  uint16_t maxTapIndex;
  /** @brief Power at the sample specified by maxTapIndex. Level[dBm] = (log10(2) * 10 * 
  (float)(int16_t)maxTapPower) / 0x200.
   The attribute type is [uint16]. */
  uint16_t maxTapPower;
  /** @brief Index of first path.
   The attribute type is [uint16]. */
  uint16_t firstPathIndex;
  /** @brief Power of the first path. The first-path power is measured at the first-path index 
  found by the ToA algorithm. Level[dBm] = (log10(2) * 10 * (float)(int16_t)firstPathPower) / 
  0x200.
   The attribute type is [int16]. */
  int16_t firstPathPower;
  /** @brief Noise power estimate. Level[dBm] = (log10(2) * 10 * (float)(int16_t)noisePower) / 
  0x2000000.
   The attribute type is [uint32]. */
  uint32_t noisePower;
  /** @brief Detection threshold power level. Level[dBm] = (log10(2) * 10 * 
  (float)(int16_t)detectThrPower) / 0x2000000.
   The attribute type is [int32]. */
  int32_t detectThrPower;
  /** @brief Reserved Fields.
   The attribute type is [uint32]. */
  uint32_t rfu2;
  /** @brief Overall received power computed by the Rx front end. Level[dBm] = (log10(2) * 10 * 
  (float)overallRxPower) / 0x2000000.
   The attribute type is [int32]. */
  int32_t overallRxPower;
  /** @brief Structure for the Physical Header (PHR) field of the UWB frame.
   The attribute type is [uint32]. */
  uint32_t phr;

} FirstPathInfoNtf;


typedef struct __attribute__ ((__packed__))  FirstPathInfoNtfPacket {
  UciHeader header;
  FirstPathInfoNtf payload;
} FirstPathInfoNtfPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|           Cir ntf            |_____________________________**/

/** @brief Returns the most recent Channel Input Response (RX). */

// OID = 0x27


/** @brief Returns the most recent Channel Input Response (RX). */
typedef struct __attribute__ ((__packed__)) CirNtf {
  /** @brief A generic UCI status code. The value of 0x00 indicates no error occurred.
   The attribute type is [enum]. */
  uint8_t uciStatus;
  /** @brief Indicates at which sample the first path is detected. A value of 255 means that the 
  first path index is not present in the samples contained.
   The attribute type is [uint8]. The default value is '255'.  */
  uint8_t fpIndex;
  /** @brief The length of the payload (in bytes) stored in the cirData buffer.
   The attribute type is [uint16]. */
  uint16_t cirLength;
  /** @brief The CIR buffer data. Contains at most 255 samples. The cir data consists of a 16 bit 
  real part and 16 bit imaginary part.
   The attribute type is [array]. */
  int16_t cirData[510];

} CirNtf;


typedef struct __attribute__ ((__packed__))  CirNtfPacket {
  UciHeader header;
  CirNtf payload;
} CirNtfPacket;


/**__________________________________________________________________________________________**/
/**_____________________________|             End              |_____________________________**/

/** @brief To deactivate the XTAL and UWB circuitry. */

// OID = 0x01

#define EndRsp GenericRsp

typedef struct __attribute__ ((__packed__))  EndRspPacket {
  UciHeader header;
  EndRsp payload;
} EndRspPacket;

typedef struct __attribute__ ((__packed__))  EndCmdPacket {
  UciHeader header;
  
} EndCmdPacket;

/**__________________________________________________________________________________________**/
/**                             |           UCI API            |                             **/
/**_____________________________|          Standalone          |_____________________________**/
typedef struct __attribute__ ((__packed__)) { 
/**__________________________________________________________________________________________**/
/**_____________________________|            Device            |_____________________________**/
/**
 * @brief To set a callback function and to provide a data container for when an update is 
ready regarding device status.
 *
 * @param[in] deviceStatusNtf To notify about the current state of the Ultra-wideband subsystem 
(UWBS). The UWBS shall generate a notification whenever there is an update in the UWBS state.
 */
  void (*onDeviceStatusUpdate)(void (*callbackFunction)(void), DeviceStatusNtf*);
/**
 * @brief Request for device reset.
 *
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*resetDevice)();
/**
 * @brief To get the UWBS device information.
 *
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 * @param[out] phyMajorVersion (hex8) Major number of the PHY specification version.
 * @param[out] phyMinorVersion (hex8) Minor number of the PHY version. .
 * @param[out] serialNumber (hex32) Unique device identifier.
 * @param[out] appMajorVersion (hex8) Major number of the application version.
 * @param[out] appMinorVersion (hex8) Minor number of the application version. The bit0-bit3 
represents the Patch version and bit4-bit7 represents the Minor version.
 * @param[out] chipMajorVersion (hex8) Silicon Product identifier (major byte).
 * @param[out] chipMinorVersion (hex8) Silicon Typecheck identifier (major byte).
 * @param[out] romMajorVersion (hex8) Major number of the ROM functions version.
 * @param[out] romMinorVersion (hex8) Minor number of the ROM functions version. The bit0-bit3 
represents the Patch version and bit4-bit7 represents the Minor version.
 * @param[out] basebandMajorVersion (hex8) Major number of the Baseband version.
 * @param[out] basebandMinorVersion (hex8) Minor number of the Baseband version.
 * @param[out] dspMajorVersion (hex8) Major number of the DSP version.
 * @param[out] dspMinorVersion (hex8) Minor number of the DSP version.
 */
  UciStatus (*getDeviceInfo)(GetDeviceInfoRsp* output);
/**
 * @brief This command puts the IC into a low power state to save energy.
 *
 * @param[in] wakeUpTime (uint32) Wake-up time in ms. Non-zero only allowed in Power Mode 0x8, 0x9 and 0xA. This configures the wake-up timer with the given value.
 * @param[in] wakeUpPin (uint32) Wake-up pin configuration. Please note that at least one wake up source has to be configured. If no wake up source is configured a parameter error will be returned.
 * @param[in] powerMode (enum power mode) The Power Mode determines the low power state, which will be entered by this command. Its value should be between 0x01 and 0x1A.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*enterPowerMode)(EnterPowerModeCmd* input);
/**
 * @brief To retain 32 bits of data in RAM that survive a device reset. When the device is 
resetted using the reset pin / button, resetted via software, or awakens from a deep or hard 
power down, all data in RAM is cleared. However, there are 4 bytes of data that persist and 
survive a reset. This can be set as a 32 bit value using this function. To read back the data, 
use getPeristentReg(). These 32 bits are cleared only after a Power-On Reset.
 *
 * @param[in] value (uint32) User-defined value that should survive a (warm) reset.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*setPersistentReg)(SetPersistentRegCmd* input);
/**
 * @brief To read back the 32 bits in RAM that survive a reset. See ``setPersistentReg()``.
 *
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 * @param[out] value (uint32) User-defined value that should survive a (warm) reset.
 */
  UciStatus (*getPersistentReg)(GetPersistentRegRsp* output);
/**
 * @brief Receive over the air update packets.
 *
 * @param[in] appVersion (uint32) Only OTA update packets transmitted with a matching app version value will be downloaded by the receiver. The default value is '0x0'.
 * @param[in] totalCrc (uint32) The CRC over the total binary, including zero padding. Only OTA update packets with a matching CRC will be downloaded. This allows distribution of multiple firmware versions in parallel. In case a 0 is passed, no selection is made based on a predetermined CRC; instead the first OTA update packet that is received will drive the selection of the remaining packets. The default value is '0x0'.
 * @param[in] timeoutSeconds (uint32) To stop when no single OTAU is downloaded within the specified time, after which the user application continues where it was left off. When 0 is provided, no timeout is set. The default value is '0'.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*otaUpdateReceive)(OtaUpdateReceiveCmd* input);
/**
 * @brief Transmit over the air update packets.
 *
 * @param[in] appVersion (uint32) Only OTA update packets transmitted with a matching app version value will be downloaded by the receiver. The default value is '0x0'.
 * @param[in] repeatCount (uint32) To specify how many times the distribution off all firmware packets must be repeated before the function terminates. A value of 0 indicates indefinitely; firmware distrubution is stopped only via a device reset (which can be scheduled using ``Timers.at(.., reset)``). The default value is '0'.
 * @param[in] fileStartPointer (uint32) Memory location of the to-be-distributed user application binary.
 * @param[in] pageCount (uint8) Length of the to-be-distributed user application binary as a multiple pof 512 bytes. When the last part has a size less then 512, zeros must be padded to make it 512 bytes.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*otaUpdateTransmit)(OtaUpdateTransmitCmd* input);
/**__________________________________________________________________________________________**/
/**_____________________________|        Configuration         |_____________________________**/
/**
 * @brief To set the UWBS device configurations.
 *
 * @param[in] rfu (uint8) Reserved for future use. The default value is '0'.
 * @param[in] enableXtalOut (hex8) To provide the clock generated from the crystal at the XTAL out pin when the Session is ACTIVE. With this functionality it is possible to supply other devices with a clock signal. Its value should be equal or smaller than 1. The default value is '1'.
 * @param[in] currentLimit (uint8) Current Limit between 5 and 20 mA. A value of 0 disables the current limiter. Its value should be between 0 and 20. The default value is '0'.
 * @param[in] enablePASignal (uint8) Output gating signal for an optional external Power Amplifier (PA) at GPIO8. Its value should be between 0 and 1. The default value is '0'.
 * @param[in] scheduleTimeCheck (uint8) Indicates whether the timestamps passed with ``startTx()`` or  ``startRx()`` should be checked for validity. If the provided timestamp cannot be met, or it lies more than 5 seconds in the future, the startTx or startRx will return an error. Disabling this check results in slightly faster code. Its value should be between 0 and 1. The default value is '1'.
 * @param[in] channelFrequency (uint32) Nominal frequency of operation on UWB. Units are [Khz]. Its value should be between 6400000 and 8000000. The default value is 'UWB_CHANNEL_5'.
 * @param[in] ppb (int32) Parts per billion (ppb) frequency deviation of the crystal corresponding to the measured temperature. Supported range= -256000 to +256000. The default value is '0'.
 * @param[in] keyHash (array) The salted hash used for configuring the STS.
 * @param[in] stsIdx (uint32) 0x00000000 - 0xFFFFFFFF ... The starting STS index to be used. The default value is '0'.
 * @param[in] stsKeyIdx (uint8) This is a user-selected key index used to store the imported key in a table of key handles. This key index can then be referenced by other commands. Eight key slots are available. UWB.importKey() is mandatory to store a key first. In case no STS or no specific key is prefered, set the value to 0xFF and the default key (all zeros) will be imported automatically. The default value is '0xFF'.
 * @param[in] txRadioCfg (uint8) The index indicating radio and frame configuration to be used for the receive mode (TX). Its value should be between 0 and 15. The default value is '0'.
 * @param[in] txSyncCode (uint8) Selects the SYNC code configuration for the RX/TX action in the ranging action. 0x00 means to use the sync code defined by the radio setting. 9 - 32 for the (pre-stored) IEEE compliant SYNC code index. 33 - 51 for the (pre-stored) proprietary SYNC code index. Its value should be between 0x00 and 51. The default value is '0'.
 * @param[in] txPower (int8) TX power. Range= -12 dBm to +30 dBm with 0.25 dBm resolution. When going over +14 dBm (value 57 and up), one should consider the regulatory limits. It should be used only with the shorter frames ( smaller than 1 ms). Note that the regulatory limits are specified in average power per millisecond. For every 3 dB that you add, the output power doubles. Its value should be between -48 and 120. The default value is '56'.
 * @param[in] txRangingEnabled (uint8) Depending on the radio setting the PHR might contain the information if ranging is enabled. This parameter is written to the PHR for those settings. 0x00 Ranging disabled; 0x01 Ranging enabled. Its value should be between 0x00 and 0x01. The default value is '1'.
 * @param[in] txStsIdxIncrement (int8) Selects the offset to apply to the STS index expected for the RX or TX action to be performed. The STS index will be incremented by that offset number. In case there is no STS used for this frame or the STS is configured to be constant, this parameter is ignored. The default value is '0'.
 * @param[in] txPayloadCipher (uint8) Payload cipher key handle index (default= 0xFF, disabled). 0x00-0x07 Enables UWB payload cipher and value is index of key handle to be used for ciphering operations. Please note that if UWB payload ciphering is enabled then encryption/decryption key must be configured in advance and UWB payload size is restricted to 95 bytes. 0x08-0xFF Disables UWB payload ciphering. Its value should be equal or smaller than 0xFF. The default value is '0xFF'.
 * @param[in] rxRadioCfg (uint8) The index indicating radio and frame configuration to be used for the receive mode (RX). Its value should be between 0 and 15. The default value is '0'.
 * @param[in] rxSyncCode (uint8) Selects the SYNC code configuration for the RX/TX action in the ranging action. 0x00 means to use the sync code defined by the radio setting. 9 - 32 for the (pre-stored) IEEE compliant SYNC code index. 33 - 51 for the (pre-stored) proprietary SYNC code index. Its value should be between 0x00 and 43. The default value is '0'.
 * @param[in] rxRangingEnabled (uint8) Depending on the radio setting the PHR might contain the information if ranging is enabled. This parameter is written to the PHR for those settings. 0x00 Ranging disabled; 0x01 Ranging enabled. Its value should be between 0x00 and 0x01. The default value is '1'.
 * @param[in] rxStsIdxIncrement (int8) Selects the offset to apply to the STS index expected for the RX or TX action to be performed. The STS index will be incremented by that offset number. In case there is no STS used for this frame or the STS is configured to be constant, this parameter is ignored. The default value is '0'.
 * @param[in] rxPayloadCipher (uint8) Payload cipher key handle index (default= 0xFF, disabled). 0x00-0x07 Enables UWB payload cipher and value is index of key handle to be used for ciphering operations. Please note that if UWB payload ciphering is enabled then encryption/decryption key must be configured in advance and UWB payload size is restricted to 95 bytes. 0x08-0xFF Disables UWB payload ciphering. Its value should be equal or smaller than 0xFF. The default value is '0xFF'.
 * @param[in] nbInterferenceCancelation (enum nb interference cancelation) Defines Narrow Band Interference Cancellation (NBIC) feature status. Its value should be between 0x00 and 0x02. The default value is '0x00'.
 * @param[in] channelImpulseResponseStart (int16) Number of samples before the Timestamp pulse (First path index) to be included in the CIR notification. 0 means the first sample is at the First path index. Units are [samples]. Its value should be between -500 and 470. The default value is '-16'.
 * @param[in] channelImpulseResponseLength (uint8) Total number of samples to be included in the CIR notification. Units are [samples]. The default value is '32'.
 * @param[in] rxSecurityLevel (uint8) The first path security level allows to make a trade-off between security and sensitivity. A higher value increases security while it also results in sensitivity loss. Its value should be between 0 and 176. The default value is '64'.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*setConfiguration)(SetConfigurationCmd* input);
/**
 * @brief To get the UWBS device configurations.
 *
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 * @param[out] enableXtalOut (hex8) To provide the clock generated from the crystal at the XTAL 
out pin when the Session is ACTIVE. With this functionality it is possible to supply other 
devices with a clock signal. Its value should be equal or smaller than 1. The default value is 
'1'.
 * @param[out] currentLimit (uint8) Current Limit between 5 and 20 mA. A value of 0 disables 
the current limiter. Its value should be between 0 and 20. The default value is '0'.
 * @param[out] enablePASignal (uint8) Output gating signal for an optional external Power 
Amplifier (PA) at GPIO8. Its value should be between 0 and 1. The default value is '0'.
 * @param[out] scheduleTimeCheck (uint8) Indicates whether the timestamps passed with 
``startTx()`` or  ``startRx()`` should be checked for validity. If the provided timestamp 
cannot be met, or it lies more than 5 seconds in the future, the startTx or startRx will return 
an error. Disabling this check results in slightly faster code. Its value should be between 0 
and 1. The default value is '1'.
 * @param[out] channelFrequency (uint32) Nominal frequency of operation on UWB. Units are 
[Khz]. Its value should be between 6400000 and 8000000. The default value is 'UWB_CHANNEL_5'.
 * @param[out] ppb (int32) Parts per billion (ppb) frequency deviation of the crystal 
corresponding to the measured temperature. Supported range= -256000 to +256000. The default 
value is '0'.
 * @param[out] keyHash (array) The salted hash used for configuring the STS.
 * @param[out] stsIdx (uint32) 0x00000000 - 0xFFFFFFFF ... The starting STS index to be used. 
The default value is '0'.
 * @param[out] stsKeyIdx (uint8) This is a user-selected key index used to store the imported 
key in a table of key handles. This key index can then be referenced by other commands. Eight 
key slots are available. UWB.importKey() is mandatory to store a key first. In case no STS or 
no specific key is prefered, set the value to 0xFF and the default key (all zeros) will be 
imported automatically. The default value is '0xFF'.
 * @param[out] txRadioCfg (uint8) The index indicating radio and frame configuration to be used 
for the receive mode (TX). Its value should be between 0 and 15. The default value is '0'.
 * @param[out] txSyncCode (uint8) Selects the SYNC code configuration for the RX/TX action in 
the ranging action. 0x00 means to use the sync code defined by the radio setting. 9 - 32 for 
the (pre-stored) IEEE compliant SYNC code index. 33 - 51 for the (pre-stored) proprietary SYNC 
code index. Its value should be between 0x00 and 51. The default value is '0'.
 * @param[out] txPower (int8) TX power. Range= -12 dBm to +30 dBm with 0.25 dBm resolution. 
When going over +14 dBm (value 57 and up), one should consider the regulatory limits. It should 
be used only with the shorter frames ( smaller than 1 ms). Note that the regulatory limits are 
specified in average power per millisecond. For every 3 dB that you add, the output power 
doubles. Its value should be between -48 and 120. The default value is '56'.
 * @param[out] txRangingEnabled (uint8) Depending on the radio setting the PHR might contain 
the information if ranging is enabled. This parameter is written to the PHR for those settings. 
0x00 Ranging disabled; 0x01 Ranging enabled. Its value should be between 0x00 and 0x01. The 
default value is '1'.
 * @param[out] txStsIdxIncrement (int8) Selects the offset to apply to the STS index expected 
for the RX or TX action to be performed. The STS index will be incremented by that offset 
number. In case there is no STS used for this frame or the STS is configured to be constant, 
this parameter is ignored. The default value is '0'.
 * @param[out] txPayloadCipher (uint8) Payload cipher key handle index (default= 0xFF, 
disabled). 0x00-0x07 Enables UWB payload cipher and value is index of key handle to be used for 
ciphering operations. Please note that if UWB payload ciphering is enabled then 
encryption/decryption key must be configured in advance and UWB payload size is restricted to 
95 bytes. 0x08-0xFF Disables UWB payload ciphering. Its value should be equal or smaller than 
0xFF. The default value is '0xFF'.
 * @param[out] rxRadioCfg (uint8) The index indicating radio and frame configuration to be used 
for the receive mode (RX). Its value should be between 0 and 15. The default value is '0'.
 * @param[out] rxSyncCode (uint8) Selects the SYNC code configuration for the RX/TX action in 
the ranging action. 0x00 means to use the sync code defined by the radio setting. 9 - 32 for 
the (pre-stored) IEEE compliant SYNC code index. 33 - 51 for the (pre-stored) proprietary SYNC 
code index. Its value should be between 0x00 and 43. The default value is '0'.
 * @param[out] rxRangingEnabled (uint8) Depending on the radio setting the PHR might contain 
the information if ranging is enabled. This parameter is written to the PHR for those settings. 
0x00 Ranging disabled; 0x01 Ranging enabled. Its value should be between 0x00 and 0x01. The 
default value is '1'.
 * @param[out] rxStsIdxIncrement (int8) Selects the offset to apply to the STS index expected 
for the RX or TX action to be performed. The STS index will be incremented by that offset 
number. In case there is no STS used for this frame or the STS is configured to be constant, 
this parameter is ignored. The default value is '0'.
 * @param[out] rxPayloadCipher (uint8) Payload cipher key handle index (default= 0xFF, 
disabled). 0x00-0x07 Enables UWB payload cipher and value is index of key handle to be used for 
ciphering operations. Please note that if UWB payload ciphering is enabled then 
encryption/decryption key must be configured in advance and UWB payload size is restricted to 
95 bytes. 0x08-0xFF Disables UWB payload ciphering. Its value should be equal or smaller than 
0xFF. The default value is '0xFF'.
 * @param[out] nbInterferenceCancelation (enum nb interference cancelation) Defines Narrow Band 
Interference Cancellation (NBIC) feature status. Its value should be between 0x00 and 0x02. The 
default value is '0x00'.
 * @param[out] channelImpulseResponseStart (int16) Number of samples before the Timestamp pulse 
(First path index) to be included in the CIR notification. 0 means the first sample is at the 
First path index. Units are [samples]. Its value should be between -500 and 470. The default 
value is '-16'.
 * @param[out] channelImpulseResponseLength (uint8) Total number of samples to be included in 
the CIR notification. Units are [samples]. The default value is '32'.
 * @param[out] rxSecurityLevel (uint8) The first path security level allows to make a trade-off 
between security and sensitivity. A higher value increases security while it also results in 
sensitivity loss. Its value should be between 0 and 176. The default value is '64'.
 */
  UciStatus (*getConfiguration)(GetConfigurationRsp* output);
/**
 * @brief Stores RX radio settings in secure flash. The index parameter specifies the index of 
the radio configuration to be stored.
 *
 * @param[in] rxRadioCfg (uint8) The index indicating radio and frame configuration to be used for the receive mode (RX). Its value should be between 0 and 15. The default value is '0'.
 * @param[in] radioCfgData (array) .
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*storeRxRadioConfig)(StoreRxRadioConfigCmd* input);
/**
 * @brief Stores TX radio settings in secure flash. The index parameter specifies the index of 
the radio configuration to be stored.
 *
 * @param[in] txRadioCfg (uint8) The index indicating radio and frame configuration to be used for the receive mode (TX). Its value should be between 0 and 15. The default value is '0'.
 * @param[in] radioCfgData (array) .
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*storeTxRadioConfig)(StoreTxRadioConfigCmd* input);
/**
 * @brief This command imports an AES raw key intended for STS generation or UWB payload 
ciphering into the system.
 *
 * @param[in] keyIdx (uint8) This is a user-selected key index used to store the imported key in a table of key handles. This key index can then be referenced by other commands. Eight key slots are available. Its value should be equal or smaller than 0x07.
 * @param[in] rawKey (array) Raw key provides the raw cryptographic key. For STS generation a AES-128 key is required for UWB payload ciphering.
 * @param[in] keyUsage (uint8) Provides the usage of key. Its value should be equal or smaller than 1. The default value is '0x00'.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*importKey)(ImportKeyCmd* input);
/**
 * @brief Returns a timestamp from the running timestamp timer. It shall serve as indication 
for the application to determine timed RX/TX operations.
 *
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 * @param[out] timestamp (uint64) The timestamp from the timestamp timer indicates at which 
exact point in time a UWB ranging message was received or transmitted. Units are [ticks].
 */
  UciStatus (*getTicks)(GetTicksRsp* output);
/**
 * @brief For the current session, adds (optional) payload data to the upcoming transmission 
(TX).
 *
 * @param[in] payloadLength (uint16) The length of the payload (in bytes) stored in the payloadData buffer. Its value should be equal or smaller than 1020.
 * @param[in] payloadData (array) Payload data.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*setPayload)(SetPayloadCmd* input);
/**__________________________________________________________________________________________**/
/**_____________________________|           Actions            |_____________________________**/
/**
 * @brief To set a callback function and to provide a data container for when an update is 
ready regarding tx.
 *
 * @param[in] txTimestampNtf To get the most recently captured (RX/TX) timestamp event.
 */
  void (*onTxUpdate)(void (*callbackFunction)(void), TxTimestampNtf*);
/**
 * @brief To set a callback function and to provide a data container for when an update is 
ready regarding rx.
 *
 * @param[in] rxTimestampNtf To get the most recently captured (RX/TX) timestamp event.
 * @param[in] payloadNtf Returns the most recently received (RX) payload.
 * @param[in] payloadNtfCapacity (uint16_t) Maximum size in number of bytes.
 * @param[in] firstPathInfoNtf To get the most recently captured (RX) signal strength.
 * @param[in] cirNtf Returns the most recent Channel Input Response (RX).
 */
  void (*onRxUpdate)(void (*callbackFunction)(void), RxTimestampNtf*, PayloadNtf*, uint16_t, FirstPathInfoNtf*, CirNtf*);
/**
 * @brief To set a callback function and to provide a data container for when an update is 
ready regarding loopback.
 *
 * @param[in] loopbackTimestampNtf To get the most recently captured (RX/TX) timestamp event.
 */
  void (*onLoopbackUpdate)(void (*callbackFunction)(void), LoopbackTimestampNtf*);
/**
 * @brief To activate the XTAL and UWB circuitry.
 *
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*begin)();
/**
 * @brief To transmit a UWB message at the specified time, or as soon as possible.
 *
 * @param[in] time (uint64) The time (in ticks with 15.65ps resolution) when the transmission is started. A value of 0 means transmit as soon as possible. Fractional timestamp resolution is in ticks, with 128 times 499.2 ticks per microsecond, i.e, 15.65 picosecs per tick. However, the TX hardware only supports timed TX operation with 32 ticks (500.8 ps) resolution. Hence the Fractional TS passed as input will be adjusted accordingly. Units are [ticks]. Its value should be equal or greater than 0x00.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*startTx)(StartTxCmd* input);
/**
 * @brief To start listening for a UWB message at the specified time, or as soon as possible. 
Stop listening using the StopAction command.
 *
 * @param[in] time (uint64) The time (in ticks with 15.65ps resolution) when the transmission is started. A value of 0 means transmit as soon as possible. Fractional timestamp resolution is in ticks, with 128 times 499.2 ticks per microsecond, i.e, 15.65 picosecs per tick. However, the TX hardware only supports timed TX operation with 32 ticks (500.8 ps) resolution. Hence the Fractional TS passed as input will be adjusted accordingly. Units are [ticks]. Its value should be equal or greater than 0x00.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*startRx)(StartRxCmd* input);
/**
 * @brief To start listening for a UWB message at the specified time, or as soon as possible, 
and restart listening each time a message is received. Stop listening using the StopAction 
command.
 *
 * @param[in] time (uint64) The time (in ticks with 15.65ps resolution) when the transmission is started. A value of 0 means transmit as soon as possible. Fractional timestamp resolution is in ticks, with 128 times 499.2 ticks per microsecond, i.e, 15.65 picosecs per tick. However, the TX hardware only supports timed TX operation with 32 ticks (500.8 ps) resolution. Hence the Fractional TS passed as input will be adjusted accordingly. Units are [ticks]. Its value should be equal or greater than 0x00.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*startContinuousRx)(StartContinuousRxCmd* input);
/**
 * @brief This command is intended for calibrating the device by determining antenna delay. To 
do so the command starts a loop-back transmission/reception mode according to the current radio 
configuration. The antenna delay (i.e. difference between the TX and RX timestamps in the 
LoopbackTimestamp result) should be subtracted from the round-trip time in the two-way-ranging 
calculation in order to get accurate distance measurements.
 *
 * @param[in] time (uint64) The time (in ticks with 15.65ps resolution) when the transmission is started. A value of 0 means transmit as soon as possible. Fractional timestamp resolution is in ticks, with 128 times 499.2 ticks per microsecond, i.e, 15.65 picosecs per tick. However, the TX hardware only supports timed TX operation with 32 ticks (500.8 ps) resolution. Hence the Fractional TS passed as input will be adjusted accordingly. Units are [ticks]. Its value should be equal or greater than 0x00.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*startLoopback)(StartLoopbackCmd* input);
/**
 * @brief This function re-runs ToA Algorithm on the previously captured CIR Data with the 
input first path security level.
 *
 * @param[in] rxSecurityLevel (uint8) The first path security level allows to make a trade-off between security and sensitivity. A higher value increases security while it also results in sensitivity loss. Its value should be between 0 and 176. The default value is '64'.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*recalcRx)(RecalcRxCmd* input);
/**
 * @brief This function will stop the current UWB TX/RX action (if any).
 *
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*stopAction)();
/**
 * @brief Returns the most recent Channel Input Response (RX).
 *
 * @param[in] channelImpulseResponseStart (int16) Number of samples before the Timestamp pulse (First path index) to be included in the CIR notification. 0 means the first sample is at the First path index. Units are [samples]. Its value should be between -500 and 470. The default value is '-16'.
 * @param[in] channelImpulseResponseLength (uint8) Total number of samples to be included in the CIR notification. Units are [samples]. The default value is '32'.
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 * @param[out] fpIndex (uint8) Indicates at which sample the first path is detected. A value of 
255 means that the first path index is not present in the samples contained. The default value 
is '255'.
 * @param[out] cirLength (uint16) The length of the payload (in bytes) stored in the cirData 
buffer. Its value should be equal or smaller than 1020.
 * @param[out] cirData (array) The CIR buffer data. Contains at most 255 samples. The cir data 
consists of a 16 bit real part and 16 bit imaginary part.
 */
  UciStatus (*getCir)(GetCirCmd* input, GetCirRsp* output);
/**
 * @brief To deactivate the XTAL and UWB circuitry.
 *
 * @param[out] uciStatus (enum uci status) A generic UCI status code. The value of 0x00 
indicates no error occurred.
 */
  UciStatus (*end)();
} UCI_t;

extern const UCI_t UWB;

#ifdef __cplusplus
} // extern "C"
#endif
#endif /* SFS_PHY_H */


/** End of file. Note: this file is auto generated. Do not edit manually.
*   Copyright SynchronicIT 2024
*/
