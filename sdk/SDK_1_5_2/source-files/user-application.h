
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

#ifndef SFS_USER_APPLICATION_H
#define SFS_USER_APPLICATION_H
#ifdef __cplusplus
extern "C" {
#endif


/** @brief User defined version number that will be stored in the firmware file header.
 The attribute type is [uint32_t]. The default value is '1'.  */
#ifndef CONFIG_APP_VERSION
#define CONFIG_APP_VERSION             1
#endif

/** @brief Interval between consecutive ``loop()`` calls. Value 0 means ``loop()`` will be 
called as often as possible.
 The attribute type is [uint32_t]. Units are [ms]. The default value is '1'.  */
#ifndef CONFIG_LOOP_INTERVAL
#define CONFIG_LOOP_INTERVAL           1
#endif

#include "user-application-flash.h"
#include "phy-api.h" // look here for UWB UciStatus error codes
#include <stdint.h>
#include "stdbool.h"
#define STANDALONE_API_FUNCTION_COUNT    90
#define STANDALONE_API_FULL_DATE 0x25022490

#define STANDALONE_API_VERSION_MAJOR 1
#define STANDALONE_API_VERSION_MINOR 5
#define STANDALONE_API_VERSION_PATCH 2
#define STANDALONE_API_VERSION_DATE ((25-20) * 0x10 + 2)

#define STANDALONE_API_VERSION ((uint32_t)STANDALONE_API_VERSION_MAJOR << 24 | (uint32_t)STANDALONE_API_VERSION_MINOR << 16 | (uint32_t)STANDALONE_API_VERSION_PATCH << 8 | STANDALONE_API_VERSION_DATE)
#define DDRD  (*((uint32_t*)0x40008000UL))
#define PIND  (*((uint32_t*)0x40008008UL))
#define PORTD (*((uint32_t*)0x4000800CUL))
#define ALTFUN  (*((uint32_t*)0x40008014UL))

#define GPIO0           0x001
#define GPIO1           0x002
#define GPIO2           0x004
#define GPIO3           0x008
#define GPIO4           0x010
#define GPIO5           0x020
#define GPIO6           0x040
#define GPIO7           0x080
#define GPIO8           0x100
#define GPIO9           0x200


#define D0 GPIO5
#define D1 GPIO8
#define D2 GPIO9
#define D3 GPIO4
#define D5 GPIO1
#define D6 GPIO2
#define D7 GPIO3
#define D8 GPIO0


#define PIN_CTS      GPIO8
#define PIN_PA_ENBL  GPIO8
#define PIN_P10      GPIO8
#define PIN_LED_R    GPIO8

#define PIN_RX       GPIO6
#define PIN_P11      GPIO6

#define PIN_TX       GPIO7
#define PIN_P12      GPIO7

#define PIN_CS2      GPIO9
#define PIN_RTS      GPIO9
#define PIN_P13      GPIO9
#define PIN_LED_G    GPIO9

#define PIN_CS1      GPIO0
#define PIN_P14      GPIO0

#define PIN_INT_N    GPIO5
#define PIN_P15      GPIO5

#define PIN_RDY_N    GPIO4
#define PIN_USER_BUTTON  GPIO4
#define PIN_P16      GPIO4

#define PIN_SCK      GPIO1
#define PIN_P17      GPIO1

#define PIN_SDO      GPIO3
#define PIN_P20      GPIO3

#define PIN_SDI      GPIO2
#define PIN_P21      GPIO2

#define SUCCESS (0)

/**__________________________________________________________________________________________**/
/**_____________________________|         Error Codes          |_____________________________**/

/** @brief Global variable errno holds the number of the last error. The value of errno is 0 at 
program startup. Its value can be checked to determine whether a function call (e.g. pinMode()) 
was successful. The value of errno is never set to zero by any system call or library function. */

typedef enum __attribute__ ((__packed__)) ErrnoVal {

/** @brief User application has too low version number. Make sure your user-application.h 
matches the uwb library version. */
  E_VERSION_OUTDATED                                    = 0x1,

/** @brief User application has too high version number. Make sure your user-application.h 
matches the uwb library version. */
  E_VERSION_TOO_HIGH                                    = 0x2,

/** @brief A user application is present, but activation failed. */
  E_VALIDATION_FAILED                                   = 0x3,

/** @brief A user application is present, but its status flag has an unknown value. */
  E_UNKNOWN_STATUS                                      = 0x4,

/** @brief A user application is present, but it is invalidated, possibly after reflashing the 
Primary Firmware. */
  E_APP_INVALIDATED                                     = 0x5,

/** @brief Flash is touched, but there is no valid user application detected. */
  E_NO_VALID_APP                                        = 0x6,

/** @brief No user application detected. */
  E_NO_APP_FOUND                                        = 0x7,

/** @brief Current UWB stack is not valid. */
  E_NO_VALID_STACK                                      = 0x8,

/** @brief The processor stack pointer is outside the valid stack space. */
  E_STACK_OVERFLOW                                      = 0x9,


/** @brief Could not execute power.sleep(). With hard power down, no timer is available for 
wake up. Set wake up time to 0. */
  E_POWER_SLEEP__NO_TIME_ALLOWED                   = 0x010301,

/** @brief Could not execute power.sleep(). With hard power down, possible pins for wake up are 
D8, RX and TX, only falling edge is allowed. */
  E_POWER_SLEEP__FALLING_ONLY                      = 0x010302,

/** @brief Could not execute power.sleep(). At least one wake up source must be specified. */
  E_POWER_SLEEP__NO_WAKEUP_SOURCE                  = 0x010303,

/** @brief Could not execute power.sleep(). The provided power mode is invalid. */
  E_POWER_SLEEP__UNKNOWN_SLEEP_MODE                = 0x010304,

/** @brief Could not execute power.sleep(). The wake up time cannot be in the past. */
  E_POWER_SLEEP__WAKE_UP_TIME_PASSED               = 0x010305,

/** @brief Could not execute power.deepSleep(). At least one wake up source must be specified. */
  E_POWER_DEEP_SLEEP__NO_WAKEUP_SOURCE             = 0x010401,

/** @brief Could not execute power.limitCurrent(). Value is out of range. */
  E_POWER_LIMIT_CURRENT__OUT_OF_RANGE              = 0x010501,

/** @brief Could not execute GPIO.pinMode(). The specified pin number does not map to a 
physical gpio pin. */
  E_GPIO_PIN_MODE__PIN_UNKNOWN                     = 0x030101,

/** @brief Could not execute GPIO.digitalWrite(). The specified pin number does not map to a 
physical GPIO pin. */
  E_GPIO_DIGITAL_WRITE__PIN_UNKNOWN                = 0x030201,

/** @brief Could not execute GPIO.digitalWrite(). The specified pin is configured as an output. 
Use ``pinMode()`` to make it an INPUT and optionally activate pullup or pulldown resistors. */
  E_GPIO_DIGITAL_WRITE__PIN_IS_INPUT               = 0x030202,

/** @brief Could not execute GPIO.digitalRead(). The specified pin number does not map to a 
physical GPIO pin. */
  E_GPIO_DIGITAL_READ__PIN_UNKNOWN                 = 0x030301,

/** @brief Could not execute GPIO.digitalRead(). The specified pin number is configured as an 
output. */
  E_GPIO_DIGITAL_READ__PIN_IS_OUTPUT               = 0x030302,

/** @brief Could not execute GPIO.analogRead(). The ADC is not yet ready. */
  E_GPIO_ANALOG_READ__NOT_READY                    = 0x030401,

/** @brief Could not execute Timer.at(). Cannot schedule another event because the queue is 
maxed out. */
  E_TIMER_AT__QUEUE_FULL                           = 0x050401,

/** @brief Could not execute Timer.atMicroseconds(). Cannot schedule because the period between 
now (see micros()) and the provided time is too long. */
  E_TIMER_AT_MICROSECONDS__OVERFLOW                = 0x050501,

/** @brief Could not execute Timer.atTicks(). Cannot schedule because the period between now 
(see micros()) and the provided time is too long. */
  E_TIMER_AT_TICKS__OVERFLOW                       = 0x050601,

/** @brief Could not execute Interrupt.attachInterrupt(). The specified type of interrupt is 
not valid. */
  E_INTERRUPT_ATTACH_INTERRUPT__WRONG_NUM          = 0x060101,

/** @brief Could not execute Interrupt.attachInterrupt(). The specified GPIO pin is configured 
as an output. Use ``pinMode()`` to make it an input first. */
  E_INTERRUPT_ATTACH_INTERRUPT__GPIO_OUTPUT        = 0x060102,

/** @brief Could not execute Interrupt.attachInterrupt(). Too many interrupts are scheduled. 
Consider removing unused ones via ``detachInterrupt()``. */
  E_INTERRUPT_ATTACH_INTERRUPT__TOO_MANY           = 0x060103,

/** @brief Could not execute Interrupt.attachInterrupt(). The specified mode is not supported. */
  E_INTERRUPT_ATTACH_INTERRUPT__WRONG_MODE         = 0x060104,

/** @brief Could not execute Interrupt.attachInterrupt(). The specified GPIO pin has an ISR 
attached. */
  E_INTERRUPT_ATTACH_INTERRUPT__PIN_CLAIMED        = 0x060105,

/** @brief Could not execute Interrupt.detachInterrupt(). The specified type of interrupt is 
not valid. */
  E_INTERRUPT_DETACH_INTERRUPT__WRONG_NUM          = 0x060201,

/** @brief Could not execute Interrupt.noInterrupts(). The counter value passed to noInterrupts 
was not the latest one. */
  E_INTERRUPT_NO_INTERRUPTS__WRONG_ORDER           = 0x060301,

/** @brief Could not execute Serial.write(). Cannot perform write at this moment. Still busy 
writing previous. */
  E_SERIAL_WRITE__BUSY                             = 0x070501,

/** @brief Could not execute SPI.transfer(). Indicates that the a SPI transaction is still 
being performed. */
  E_SPI_TRANSFER__BUSY                             = 0x080501,

/** @brief Could not execute Crypto.begin(). Couldn't power up the crypto subsystem. Check the 
power connections and power source. */
  E_CRYPTO_BEGIN__FAILED                           = 0x0B0101,

/** @brief Could not execute Crypto.randomBytes(). Indicates that the security subsystem was 
currently busy performing cryptographic operations and, therefore, could not process the 
operation. */
  E_CRYPTO_RANDOM_BYTES__BUSY                      = 0x0B0201,

/** @brief Could not execute Crypto.randomBytes(). Indicates that the random number generator 
failed due to an unspecified internal error condition. */
  E_CRYPTO_RANDOM_BYTES__GENERAL_FAILURE           = 0x0B0202,

/** @brief Could not execute Crypto.randomBytes(). The cryptographic subsystem has not powered 
up. Call ``Crypto.begin()`` first. */
  E_CRYPTO_RANDOM_BYTES__NOT_BEGAN                 = 0x0B0203,

/** @brief Could not execute Crypto.calcCrc(). Indicates that the CRC calculation was 
terminated prematurely due to read error on the input data. This typically indicates that an 
attempt was made to calculate a CRC over a flash memory region containing erased or corrupted 
flash pages. */
  E_CRYPTO_CALC_CRC__READ                          = 0x0B0401,

/** @brief Could not execute Crypto.calcCrc(). Indicates that an invalid parameter was passed. */
  E_CRYPTO_CALC_CRC__BAD_PARAM                     = 0x0B0402,

/** @brief Could not execute Crypto.calcCrc(). Invalid data. */
  E_CRYPTO_CALC_CRC__UNDEFINED                     = 0x0B0403,

/** @brief Could not execute Crypto.configEncryption(). Indicates that the security subsystem 
was currently busy performing cryptographic operations and, therefore, could not process the 
import key operation. */
  E_CRYPTO_CONFIG_ENCRYPTION__BUSY                 = 0x0B0501,

/** @brief Could not execute Crypto.configEncryption(). Indicates that at least one of the 
parameters of this function were invalid. */
  E_CRYPTO_CONFIG_ENCRYPTION__BAD_PARAMETER        = 0x0B0502,

/** @brief Could not execute Crypto.configEncryption(). Indicates that the raw key import 
operation is not allowed for the requested key and/or the current platform state. */
  E_CRYPTO_CONFIG_ENCRYPTION__NOT_PERMITTED        = 0x0B0503,

/** @brief Could not execute Crypto.configEncryption(). Indicates that the import key operation 
failed due to an unspecified internal error condition. No further error information is 
provided, in order to minimize the amount of information that is exposed to potential 
attackers. */
  E_CRYPTO_CONFIG_ENCRYPTION__GENERAL_ERROR        = 0x0B0504,

/** @brief Could not execute Crypto.configDecryption(). Indicates that the security subsystem 
was currently busy performing cryptographic operations and, therefore, could not process the 
import key operation. */
  E_CRYPTO_CONFIG_DECRYPTION__BUSY                 = 0x0B0601,

/** @brief Could not execute Crypto.configDecryption(). Indicates that at least one of the 
parameters of this function were invalid. */
  E_CRYPTO_CONFIG_DECRYPTION__BAD_PARAMETER        = 0x0B0602,

/** @brief Could not execute Crypto.configDecryption(). Indicates that the raw key import 
operation is not allowed for the requested key and/or the current platform state. */
  E_CRYPTO_CONFIG_DECRYPTION__NOT_PERMITTED        = 0x0B0603,

/** @brief Could not execute Crypto.configDecryption(). Indicates that the import key operation 
failed due to an unspecified internal error condition. No further error information is 
provided, in order to minimize the amount of information that is exposed to potential 
attackers. */
  E_CRYPTO_CONFIG_DECRYPTION__GENERAL_ERROR        = 0x0B0604,

/** @brief Could not execute Crypto.encrypt(). Indicates that the security subsystem was 
currently busy performing cryptographic operations and, therefore, could not process the import 
key operation. */
  E_CRYPTO_ENCRYPT__BUSY                           = 0x0B0701,

/** @brief Could not execute Crypto.encrypt(). Indicates that at least one of the parameters of 
this function were invalid. */
  E_CRYPTO_ENCRYPT__BAD_PARAMETER                  = 0x0B0702,

/** @brief Could not execute Crypto.encrypt(). Indicates that the raw key import operation is 
not allowed for the requested key and/or the current platform state. */
  E_CRYPTO_ENCRYPT__NOT_PERMITTED                  = 0x0B0703,

/** @brief Could not execute Crypto.encrypt(). Indicates that the import key operation failed 
due to an unspecified internal error condition. No further error information is provided, in 
order to minimize the amount of information that is exposed to potential attackers. */
  E_CRYPTO_ENCRYPT__GENERAL_ERROR                  = 0x0B0704,

/** @brief Could not execute Crypto.decrypt(). Is reported by authenticated cipher suites, such 
AES in CCM mode, if verification of the authentication tag failed. This error indicates that 
the integrity of the provided ciphertext (and/or its authentication tag) has been compromised 
and, therefore, that the ciphertext is not trustworthy. */
  E_CRYPTO_DECRYPT__VERIFICATION_FAILED            = 0x0B0801,

/** @brief Could not execute Crypto.decrypt(). Indicates that the security subsystem was 
currently busy performing cryptographic operations and, therefore, could not process the import 
key operation. */
  E_CRYPTO_DECRYPT__BUSY                           = 0x0B0802,

/** @brief Could not execute Crypto.decrypt(). Indicates that at least one of the parameters of 
this function were invalid. */
  E_CRYPTO_DECRYPT__BAD_PARAMETER                  = 0x0B0803,

/** @brief Could not execute Crypto.decrypt(). Indicates that the raw key import operation is 
not allowed for the requested key and/or the current platform state. */
  E_CRYPTO_DECRYPT__NOT_PERMITTED                  = 0x0B0804,

/** @brief Could not execute Crypto.decrypt(). Indicates that the import key operation failed 
due to an unspecified internal error condition. No further error information is provided, in 
order to minimize the amount of information that is exposed to potential attackers. */
  E_CRYPTO_DECRYPT__GENERAL_ERROR                  = 0x0B0805,

/** @brief Could not execute EEPROM.read(). The target address is out of range. */
  E_EEPROM_READ__OUT_OF_RANGE                      = 0x0C0101,

/** @brief Could not execute EEPROM.write(). General flash write error. */
  E_EEPROM_WRITE__FAILED                           = 0x0C0201,

/** @brief Could not execute EEPROM.write(). The target address is out of range. */
  E_EEPROM_WRITE__OUT_OF_RANGE                     = 0x0C0202,

/** @brief Could not execute EEPROM.readBytes(). Reading failed, possibly because no data is 
stored at the target address. */
  E_EEPROM_READ_BYTES__FAILED                      = 0x0C0301,

/** @brief Could not execute EEPROM.readBytes(). The target address is out of range. */
  E_EEPROM_READ_BYTES__OUT_OF_RANGE                = 0x0C0302,

/** @brief Could not execute EEPROM.writeBytes(). General flash write error. */
  E_EEPROM_WRITE_BYTES__FAILED                     = 0x0C0401,

/** @brief Could not execute EEPROM.writeBytes(). The target address is out of range. */
  E_EEPROM_WRITE_BYTES__OUT_OF_RANGE               = 0x0C0402,
} ErrnoVal;

// [ Over-the-air (OTA) updates ] //

#define FWUP_PAGE_SIZE (512)

typedef struct __attribute__ ((__packed__)) OtaPacket_t {
    uint32_t version;
    uint32_t appVersion;
    uint32_t totalCrc;
    uint32_t pageCrc;
    uint16_t pageIdx;
    uint16_t pageCount;
    uint8_t pageData[FWUP_PAGE_SIZE];
} OtaPacket_t;

typedef struct __attribute__ ((__packed__)) OtaPayloadNtf {
  uint8_t uciStatus;
  uint16_t payloadLength;
  OtaPacket_t payloadData;
} OtaPayloadNtf;


// If OTAU payload length is OK
// and CRC is OK
// and SDK version is OK
// and appVersion is OK
// then return 1, else 0
#define IS_OTAU_PACKET(payloadNtfPtr, AppVersion, total_crc) \
	(((OtaPayloadNtf*)(payloadNtfPtr))->payloadLength == sizeof(OtaPacket_t) && \
  (((total_crc) == 0) || ((OtaPayloadNtf*)(payloadNtfPtr))->payloadData.totalCrc == (total_crc)) && \
	(((OtaPayloadNtf*)(payloadNtfPtr))->payloadData.version == STANDALONE_API_VERSION) && \
  (((AppVersion) == 0) || ((OtaPayloadNtf*)(payloadNtfPtr))->payloadData.appVersion == (AppVersion)))

#define IS_NEW_OTAU_PACKET(payloadNtfPtr, AppVersion, total_crc) \
	IS_OTAU_PACKET(payloadNtfPtr, AppVersion, total_crc) && (((OtaPayloadNtf*)(payloadNtfPtr))->payloadData.totalCrc != getCurrentCrc())

#define IS_OTAU(payloadNtfPtr) \
  IS_NEW_OTAU_PACKET(payloadNtfPtr, CONFIG_APP_VERSION, 0)


#define GET_OTAU() \
  { \
    delay(10); \
    OtaUpdateReceiveCmd otauCmd = { \
        .appVersion = CONFIG_APP_VERSION, \
        .totalCrc = 0, \
        .timeoutSeconds = 5 \
    }; \
    ASSERT_0(UWB.otaUpdateReceive(&otauCmd)); \
  }

// ============================== //


typedef bool boolean;
typedef uint8_t byte;
#define digitalPinToInterrupt(x) (x)
#if !defined false
  #define false 0
#endif
#if !defined true
  #define true 1
#endif
#ifndef NULL
    #ifdef __cplusplus
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif

    
#define SPISettings(clock, bitOrder, dataMode) \
    ((uint32_t)(clock/1000) << 8) | bitOrder | dataMode


/** [ Ticks to millimeters calculation ]
 * Radio propogation speed: 299704079 m/s. 
 * Time of 1 tick = 1/(128*499.2e6) seconds.
 * mm per tick = 1000 * 299704079 / (128*499.2e6) ~= 4.690381 mm.
 * approximation with integers: 307389 / (2^16) ~= 4.690384 mm.
*/
#define TICKS_TO_MILLIMETERS(ticks_int) (((int64_t)(ticks_int) * 307389) >> 16)


/** [ Ticks and CFO to mm correction calculation ]
 * rxCarrierFreqOffset_int is ticks offset per 2^24 ticks (1/int16 @ 124.8Mhz).
 * Thus, x CFO over period of y ticks means (y * x / (2^24)) ticks offset.
 * Expressed as millimeters offset, this equals:
 * (y * x / (2^24)) * 4.690381 ~= (y * x) / 3577231.
*/
#define TICKS_AND_CFO_TO_MM_CORRECTION(tick_period_int, rxCarrierFreqOffset_int) \
  ((((int64_t)(tick_period_int)) *  (rxCarrierFreqOffset_int)) / 5900000)


/** [ Microseconds to Ticks calculation ]
 * Ticks run at 128*499.2 MHz. This is 63897.6 ticks per microsecond.
*/
#define MICROS_TO_TICKS(uint_us) ((((int64_t)(uint_us)) * 638976) / 10)


/** [ Milliseconds to Ticks calculation ]
 * Ticks run at 128*499.2 MHz. This is 63897600 ticks per millisecond.
*/
#define MILLIS_TO_TICKS(uint_ms) (((int64_t)(uint_ms)) * 63897600)


/** [ Ticks to Microseconds calculation ]
 * Ticks run at 128*499.2 MHz. This is 1/63897.6 microseconds per tick.
*/
#define TICKS_TO_MICROS(uint_ticks) (((int64_t)(uint_ticks) * 10) / 638976)


/** [ Ticks to Milliseconds calculation ]
 * Ticks run at 128*499.2 MHz. This is 1/63897600 milliseconds per tick.
*/
#define TICKS_TO_MILLIS(uint_ticks) (((int64_t)(uint_ticks)) / 63897600)


/** [ Next TX option calulation ]
 * TX can be scheduled at a 124.8Mhz clock. This is once every 512 ticks, i.e. 
 * the last 9 bits of a TX timestamp must equal 0.
*/
#define NEXT_TX_OPTION(uint_ticks) (((uint_ticks) + 0x1FF) & 0xFFFFFFFFFFFFFE00ull)
// Below calculates: 10 * (log10(2) * 10 * (float)overallRxPower) / 0x2000000.
#define GET_RSSI_x10(FirstPathInfoNtf_overallRxPower) \
   ((((int32_t)FirstPathInfoNtf_overallRxPower)) / 1114654)

/**-----------------------------[      Secondary Interface       ]-----------------------------**/

/** @brief This struct defines the user-application header. */
typedef struct __attribute__ ((__packed__)) {
  uint16_t header_id;
  uint8_t type;
  uint8_t status;
  
  uint8_t version_major;
  uint8_t version_minor;
  uint8_t version_patch;
  uint8_t version_date;
  // Implement setup() and loop() in your sketch:
  void (*setup)(void);
  void (*loop)(void);
  // Configuration:
  uint32_t  appVersion;
  uint32_t  loopInterval;
} user_application_header_t;

extern void setup(void); 
extern void loop(void); 
extern uint32_t* p_errno;
#define errno (*p_errno)
extern uint32_t* p_errline;
#define errline (*p_errline)
extern uint32_t* p_dbg_uci;
#define dbg_uci (*p_dbg_uci)

// To detect programming errors and ensure that certain conditions are met in code execution. 
// Use as follows: ASSERT(function_or_value, expected_result). 
// When the function or value in the first argument does not match the expected outcome, 
// the Serial.printError will be used to print the unexpected outcome and code line number.
#define ASSERT(fn,res) {errno=fn; if(errno!=res){errline=__LINE__; Serial.printError();}}

// Similar to ASSERT(function_or_value, expected_result) except a goto statement is added to jump to a user defined label upon an assertion error.
#define ASSERT_GOTO(fn,res,section) {errno=fn; if(errno!=res){errline=__LINE__; Serial.printError(); goto section;}}

// Similar to ASSERT(function_or_value, 0). Useful when you expect a function to return 0 (i.e. SUCCESS).
#define ASSERT_0(fn) {ASSERT(fn, SUCCESS);}

// A switch statement is a control flow statement in programming that allows a program to execute different code blocks based on the value of an expression. We defined a few macros to mimic the regular (lower case) switch syntax in order to handle the bitmask from waitForEvent(timeout_ms) more conveniently. In contrast to the regular switch, all cases are evaluated in the provided order. If there is a match, the bits are cleared from the eventMask before proceding to the next case.
// 
// SWITCH_EVENT(100, millis() > 1000) {
//   if (eventMask == 0) { Serial.print("timeout\n"); }
//   CASE(3): { Serial.print("Triggered event 3\n"); }
//   CASE(1): { Serial.print("Triggered event 1\n"); }
//   CASE(2): { Serial.print("Triggered event 2\n"); }
// }
// 
// In the example above waitForEvent is called with a timeout of 100 milliseconds. This is repeated while the condition millis() > 1000 is met.
// In case a triggerEvent(3) is called somewhere, the lowest two bits are set (b0 and b1) in eventMask. "Triggered event 3" will be printed and b0 and b1 are cleared before moving to CASE(1). Thus, "Triggered event 1" will not be printed when CASE(3) is met. If you would swap CASE(2) and CASE(3), triggerEvent(3) would result in "Triggered event 2" and "Triggered event 1" but not "Triggered event 3". Because of this kind of dependencies, it is advised to always stick to single-bit event identifiers (e.g. triggerEvent(1>>9)).
#define SWITCH_EVENT(timeoutMs, condition) uint32_t eventMask = 0; while(eventMask |= waitForEvent(timeoutMs), condition)
#define CASE(event) if (((eventMask & event) == event) && (eventMask &= ~(uint32_t)event) >= 0) case_ ## event
/**__________________________________________________________________________________________**/
/**_____________________________|      Primary Interface       |_____________________________**/

/**-----------------------------[           Power API            ]-----------------------------**/

 
/** @brief Power on reset was the reason for the reboot. */
#define POWER_ON                                           0x1
 
/** @brief Reset pin was the reason for the reboot. */
#define RESET_PIN                                          0x2
 
/** @brief Reset pin during deep power down was the reason for the reboot. */
#define DPD_RESET_PIN                                      0x3
 
/** @brief Reset pin during hard power down was the reason for the reboot. */
#define HPD_RESET_PIN                                      0x4
 
/** @brief Wake up pin during hard power down was the reason for the reboot. */
#define HPD_WAKE_UP_PIN                                    0x5
 
/** @brief Software master reset was the reason for the reboot. */
#define SOFTWARE_RESET                                     0x6
 
/** @brief Reset caused by power failure on the bias bandgap. */
#define FAIL_BIAS_BANDGAP_VDD                              0x7
 
/** @brief Reset caused by power failure on the CPU retention power domain. */
#define FAIL_CPU_VDD                                       0x8
 
/** @brief Reset caused by brownout on the CPU retention power domain. */
#define BROWNOUT_CPU                                       0x9
 
/** @brief Power failure on the non-volatile power domain was the reason for the reboot. */
#define FAIL_PFLASH_VDD                                    0xA
 
/** @brief Power failure on the buffer power domain was the reason for the reboot. */
#define FAIL_BUF_LOW_VDD                                   0xB
 
/** @brief Reset caused by the watchdog timer. */
#define WATCHDOG                                           0xC
 
/** @brief Failed state change request was the reason for the reboot. */
#define FAIL_STATE_CHANGE                                  0xD
 
/** @brief Wake-up timer from deep power down was the reason for the reboot. */
#define WUP_TIMER                                          0xE
 
/** @brief Snapshot activity from deep power down was the reason for the reboot. */
#define WUP_SNAPSHOT                                       0xF
 
/** @brief Wake up gpio event during hard power down was the reason for the reboot. */
#define DPD_GPIO                                           0x21

/**
 * @brief Indicates the cause of the last reset.
 *
 *
 * @return reason (uint8_t) .
 */
extern const uint8_t (*getResetReason)(void);

/**
 * @brief Resets the device. All global variables will be cleared, except for the one stored in 
the Persistent Register.
 *
 */
extern const void (*reset)(void);

/**
 * @brief Puts the chip into a low power state to save energy (deep power down).
 *
 * @param time (uint32_t) Wake-up time in ms. Non-zero only allowed in Deep Power Down mode. This configures the wake-up timer with the given value.
 * @param pin (uint16_t) Wake-up pin configuration. Please note that at least one wake up source has to be configured. If no wake up source is configured a parameter error will be returned.
 * @param pinEdge (uint8_t) Configures the edge of the wake up pin signal. 0 = falling, 1 = rising edge.
 *
 * @return errnoVal (ErrnoVal) .
 */
extern const ErrnoVal (*sleep)(uint32_t time, uint16_t pin, uint8_t pinEdge);

/**
 * @brief Puts the chip into the lowest power state to save energy (hard power down). Only 
PIN_RX, PIN_TX, and D8 (GPIO0) can be used (FALLING edge only).
 *
 * @param pin (uint16_t) Wake-up pin configuration. Please note that at least one wake up source has to be configured. If no wake up source is configured a parameter error will be returned.
 *
 * @return errnoVal (ErrnoVal) .
 */
extern const ErrnoVal (*deepSleep)(uint16_t pin);

/**
 * @brief Limit the current, to improve battery life. Current Limit can be set between 5 and 20 
mA. When using the SFM10-dev, make sure to desolder the bypass resistor between VBAT and VBUF.
 *
 * @param milliAmps (uint8_t) A value of 0 disables the current limiter. It is advised to connect VBuf and VBat when the current limiter is not used. Its value should be between 0 and 20. The default value is '0'.
 *
 * @return errnoVal (ErrnoVal) .
 */
extern const ErrnoVal (*limitCurrent)(uint8_t milliAmps);

/**
 * @brief When the device is resetted using the reset pin / button, resetted via software, or 
awakens from a deep or hard power down, all data in RAM is cleared. However, there are 4 bytes 
of data that persist and survive a reset. This can be read back as a 32 bit value using this 
function. To store the data, use ``peristentWrite()``. These 32 bits are cleared only after a 
Power-On Reset.
 *
 *
 * @return value (uint32_t) .
 */
extern const uint32_t (*persistentRead)(void);

/**
 * @brief When the device is resetted using the reset pin / button, resetted via software, or 
awakens from a deep or hard power down, all data in RAM is cleared. However, there are 4 bytes 
of data that persist and survive a reset. This can be set as a 32 bit value using this 
function. To read back the data, use ``peristentRead()``. These 32 bits are cleared only after 
a Power-On Reset.
 *
 * @param value (uint32_t) .
 */
extern const void (*persistentWrite)(uint32_t value);


/**-----------------------------[         StackUsage API         ]-----------------------------**/

typedef struct __attribute__ ((__packed__)) {
/**
 * @brief Fills the stack area with a dedicated pattern in order to find the usage later.
 *
 */
  void (*begin)(void);

/**
 * @brief Returns how far the stack has been used since begin() was called.
 *
 *
 * @return byteCount (uint16_t) .
 */
  uint16_t (*findMax)(void);

/**
 * @brief Returns the current stack size.
 *
 *
 * @return byteCount (uint16_t) .
 */
  uint16_t (*current)(void);

/**
 * @brief Returns total stack area (constant).
 *
 *
 * @return byteCount (uint16_t) .
 */
  uint16_t (*limit)(void);

} StackUsage_t;

extern const StackUsage_t StackUsage;

/**--------------------------------------------------------------------------------------------**/

/**-----------------------------[            GPIO API            ]-----------------------------**/

 
/** @brief Digital input without an internal resistor enabled. */
#define INPUT                                              0x00
 
/** @brief Digital output, starting with level set to LOW. */
#define OUTPUT                                             0x10
 
/** @brief Digital input with an internal pullup resistor enabled. */
#define INPUT_PULLUP                                       0x01
 
/** @brief Digital input with an internal pulldown resistor enabled. */
#define INPUT_PULLDOWN                                     0x02
 
/** @brief Digital output, starting with level set to LOW. */
#define OUTPUT_LOW                                         0x10
 
/** @brief Digital output, starting with level set to HIGH. */
#define OUTPUT_HIGH                                        0x11
 
/** @brief Logical 0. */
#define LOW                                                0x00
 
/** @brief Logical 1. */
#define HIGH                                               0x01
 
/** @brief Voltage at VDD BUF (VBUF) pin in millivolts. */
#define VDD_BUF                                            0
 
/** @brief Voltage at VDD GLOB pin in millivolts. */
#define VDD_GLOB                                           1
 
/** @brief Voltage at VDD DIG pin in millivolts. */
#define VDD_DIG                                            2
 
/** @brief Voltage at VDD IO pin in millivolts. */
#define VDD_IO                                             3
 
/** @brief Voltage at positive pin of external NTC, in millivolts. */
#define VDD_NTC_P                                          4
 
/** @brief Voltage at the negative pin of external NTC, in millivolts. */
#define VDD_NTC_N                                          5
 
/** @brief Temperature of external NTC, as a raw calibrated ADC reading (10 bit). */
#define EXT_TEMPERATURE                                    6
 
/** @brief Temperature of internal NTC, in millidegrees. */
#define INT_TEMPERATURE                                    7
 
/** @brief To perform recalibration of the internal ADC. */
#define CALIBRATE_ADC                                      8
 
/** @brief Temperature of external NTC as a raw and uncalibrated ADC value. */
#define EXT_TEMPERATURE_RAW                                0X16
 
/** @brief Temperature of internal NTC as a raw and uncalibrated ADC value. */
#define INT_TEMPERATURE_RAW                                0X17

/**
 * @brief Configures the specified GPIO pin to behave either as an input or an output.
 *
 * @param pinMask (uint16_t) To indicate which pins should have the specified mode. This should be a 10-bit number with each bit representing one of the GPIO pins.
 * @param mode (uint8_t) Pin Mode can be set to INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN, OUTPUT_LOW, or OUTPUT_HIGH.
 *
 * @return errnoVal (ErrnoVal) .
 */
extern const ErrnoVal (*pinMode)(uint16_t pinMask, uint8_t mode);

/**
 * @brief Write a HIGH (0x01) or a LOW (0x00) value to a digital pin. If the pin has been 
configured as an OUTPUT with ``pinMode()``, its voltage will be set to the corresponding value 
(e.g., 3.3V for HIGH, 0V (ground) for LOW). If the pin is configured as an INPUT, this function 
will not have an effect. It is recommended to set the ``pinMode()`` to INPUT_PULLUP to enable 
the internal pull-up resistor or INPUT_PULLDOWN to enable the internal pull-down resistor.
 *
 * @param pinMask (uint16_t) To indicate which pins should be set. This should be a 10-bit number with each bit representing one of the GPIO pins.
 * @param value (uint8_t) HIGH or LOW.
 *
 * @return errnoVal (ErrnoVal) .
 */
extern const ErrnoVal (*digitalWrite)(uint16_t pinMask, uint8_t value);

/**
 * @brief Reads the value from a specified digital pin. The result will be either ``HIGH`` or 
``LOW``.
 *
 * @param pinMask (uint16_t) To indicate which pin should be read.
 *
 * @return value (uint8_t) HIGH or LOW.
 */
extern const uint8_t (*digitalRead)(uint16_t pinMask);

/**
 * @brief Reads the value from the specified analog pin. On the SFM10 boards, all pins are 
connected internally that allows you to read supply voltages and temperatures.
 *
 * @param channel (uint8_t) To indicate which measure you want.
 *
 * @return value (int32_t) .
 */
extern const int32_t (*analogRead)(uint8_t channel);


/**-----------------------------[           Timing API           ]-----------------------------**/

 
/** @brief Will use the calibrated ADC reading of the XTAL NTC. Identical to 
`analogRead(EXT_TEMPERATURE)`. */
#define FROM_TEMPERATURE                                   0

/**
 * @brief Returns the number of milliseconds passed since the module board began running the 
current program. This number will overflow (go back to zero) after approximately 50 days.
 *
 *
 * @return count (uint32_t) Number of milliseconds passed since the module board reset.
 */
extern const uint32_t (*millis)(void);

/**
 * @brief Returns the number of microseconds since the module board began running the current 
program. This number will overflow (go back to zero) after approximately one hour and 11 
minutes.
 *
 *
 * @return count (uint32_t) Number of microseconds passed since the module board reset.
 */
extern const uint32_t (*micros)(void);

/**
 * @brief Returns a pointer to the number of microseconds since the module board began running 
the current program. This number will overflow (go back to zero) after approximately 0.6 
million years.
 *
 *
 * @return count (uint64_t*) Number of microseconds passed since the module board reset.
 */
extern const uint64_t* (*micros64)(void);

/**
 * @brief Pauses / blocks the program for the amount of time (in milliseconds) specified by the 
parameter ``timeMs``. During this period, the UWB subsystem could continue operation or could 
decide to temporarily transition into a sleep mode to save power, depending on the current 
tasks. Certain things do go on while the ``delay()`` function is being executed, because the 
delay function does not disable interrupts.
 *
 * @param timeMs (uint32_t) Amount of time in milliseconds.
 */
extern const void (*delay)(uint32_t timeMs);

/**
 * @brief Pauses / blocks the program for the amount of time (in microseconds) specified by the 
parameter. See ``delay()``.
 *
 * @param time (uint32_t) Amount of time in microseconds.
 */
extern const void (*delayMicroseconds)(uint32_t time);

/**
 * @brief Returns frequency deviation of the crystal corresponding to the measured temperature.
 *
 * @param adc_ext_temperature (int32_t) .
 *
 * @return count (int32_t) Parts per billion.
 */
extern const int32_t (*getXtalPpb)(int32_t adc_ext_temperature);


/**-----------------------------[           Timer API            ]-----------------------------**/

 
/** @brief To indicate that an action must be executed as soon as possible. */
#define ASAP                                               (NULL)
typedef struct __attribute__ ((__packed__)) {
/**
 * @brief Refresh the watchdog timeout per passed milliseconds. This function has to be 
periodically called before the timeout time expires, in order to prevent the watchdog from 
triggering a reset. However, at startup the watchdog is disabled per default.
 *
 * @param timeoutMs (uint32_t) Timeout count in milliseconds. Timeout count of 0 is equivalent to disabling the watchdog. The watchdog is driven with an input clock of 1.2MHz independent of the configured system clock. A watchdog tick is 1ms. Units are [ms].
 */
  void (*setWatchdog)(uint32_t timeoutMs);

/**
 * @brief Set the watchdog counter register back to the value of ``Timer.setWatchdog()``.
 *
 */
  void (*kickWatchdog)(void);

/**
 * @brief Starts the global timer class.
 *
 */
  void (*begin)(void);

/**
 * @brief Schedules a function call at the given time, expressed in milliseconds passed since 
the module board began running the current program (refer ``millis()``). In contrast to the 
other Timer functions, up to 8 timestamps / functions can be queued. To empty the queue 
preemptively, call Timer.end().
 *
 * @param timeMs (uint32_t) Amount of time in milliseconds. Units are [ms].
 * @param function (void (*isr)(void)) The function that is called at the specified time. This function must take no parameters and return nothing.
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*at)(uint32_t timeMs, void (*isr)(void));

/**
 * @brief Similar to ``Timer.at()`` except the given time is in microseconds instead of 
milliseconds.
 *
 * @param timeUs (uint64_t) Amount of time in milliseconds. Units are [us].
 * @param function (void (*isr)(void)) The function that is called at the specified time. This function must take no parameters and return nothing.
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*atMicroseconds)(uint64_t timeUs, void (*isr)(void));

/**
 * @brief Similar to ``Timer.at()`` except the given time is in ticks instead of milliseconds. 
However, the granularity is bound to the XTAL (typically 55.2 MHz). Only a single event 
(including Timer.atMicroseconds()) can be scheduled at a time. .
 *
 * @param timeTicks (uint64_t) Amount of time in ticks (running at 63.8976 GHz, thus 15.65 picoseconds per tick).
 * @param function (void (*isr)(void)) The function that is called at the specified time. This function must take no parameters and return nothing.
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*atTicks)(uint64_t timeTicks, void (*isr)(void));

/**
 * @brief To read the number of currently scheduled using the Timer.at... function(s).
 *
 *
 * @return count (uint8_t) Number of scheduled items.
 */
  uint8_t (*atCount)(void);

/**
 * @brief Schedules a call to the given function at a given constant interval, expressed in 
milliseconds.
 *
 * @param timeMs (uint32_t) Amount of time in milliseconds. Units are [ms].
 * @param function (void (*isr)(void)) The function that is called at the specified time. This function must take no parameters and return nothing.
 */
  void (*every)(uint32_t timeMs, void (*isr)(void));

/**
 * @brief Similar to ``Timer.every()`` except the given time is in microseconds instead of 
milliseconds.
 *
 * @param timeUs (uint32_t) Amount of time in milliseconds. Units are [us].
 * @param function (void (*isr)(void)) The function that is called at the specified time. This function must take no parameters and return nothing.
 */
  void (*everyMicroseconds)(uint32_t timeUs, void (*isr)(void));

/**
 * @brief Stops and resets the global timer class.
 *
 */
  void (*end)(void);

/**
 * @brief Clears the currently scheduled using the Timer.at... function(s).
 *
 */
  void (*clearOneShots)(void);

/**
 * @brief Clears the currently scheduled using the Timer.every... function(s).
 *
 */
  void (*clearPeriodic)(void);

} Timer_t;

extern const Timer_t Timer;

/**--------------------------------------------------------------------------------------------**/

/**-----------------------------[         Interrupt API          ]-----------------------------**/

 
/** @brief The provided callback function will be executed every time an error occurred during 
the execution of a non-UWB API function. */
#define ERROR_INTERRUPT                                    0x1000
 
/** @brief The ISR will be executed every time a GPIO interrupt is generated, irrespective of 
the pin number. To add a pin to the pool of interrupt pins, use ``attachInterrupt( 
PIN_USER_BUTTON, pin_isr, mode)``. If in the latter case pin_isr is set to NULL, only the isr() 
of GPIO_INTERRUPT is executed. If not NULL, pin_isr() is executed before the isr() of 
``GPIO_INTERRUPT``. */
#define GPIO_INTERRUPT                                     0x2000
 
/** @brief The ISR will be executed every time byte(s) areavailable from the Serial port. */
#define SERIAL_INTERRUPT                                   0x3000
 
/** @brief The ISR will be executed every time a SPI transfer was successful. That is, the 
predetermined number of bytes have been transfered (see length parameter). Note that when this 
interrupt is active, the ``SPI.transfer()`` will not be a blocking call anymore. */
#define SPI_TRANSFER_COMPLETE                              0x4000
 
/** @brief The ISR will be executed every time a SPI transfer is started because the host 
asserted the Slave Select line (only applicable when in SPI slave mode). */
#define SPI_TRANSFER_ASSERT                                0x5000
 
/** @brief The ISR will be executed every time a SPI transfer is stopped because the host 
deasserted the Slave Select line (only applicable when in SPI slave mode). */
#define SPI_TRANSFER_DEASSERT                              0x6000
 
/** @brief Triggers when TX has completed. */
#define UWB_TX_DONE_MARKER                                 0x7000
 
/** @brief Triggers when the Start of Frame Delimiter (SFD) is received. THE SFD is positioned 
between the SYNC (preamble) part and the PHR part. The SFD part is used for time-stamping which 
is useful for ranging. */
#define UWB_RX_SFD_MARKER                                  0x8000
 
/** @brief Triggers when the PHR is received. PHR stands for Physical Layer Header. It provides 
information to the receiver about the transmitted payload and other useful physical layer 
information. */
#define UWB_RX_PHR_MARKER                                  0x9000
 
/** @brief Triggers when RX has started. */
#define UWB_RX_START_MARKER                                0xA000
 
/** @brief Triggers when the End of Frame (EOF) is detected. */
#define UWB_RX_EOF_MARKER                                  0xB000
 
/** @brief Triggers when the the voltage of VBUF is too low. Adjust current limiter or add 
capacitance to resolve this. */
#define UWB_VBUF_LOW_INTERRUPT                             0xC000
 
/** @brief The ISR will be executed every time a asynchronous TX was successful. */
#define SERIAL_WRITE_COMPLETE                              0xD000
 
/** @brief The ISR will be executed every time TX result(s) are ready to be read out. 
Equivalent to UWB.onTX(myEvent, NULL). */
#define UWB_TX_INTERRUPT                                   0xC100
 
/** @brief The ISR will be executed every time RX result(s) are ready to be read out. 
Equivalent to UWB.onRX(myEvent, NULL, NULL...). */
#define UWB_RX_INTERRUPT                                   0xC200
 
/** @brief The ISR will be executed every time loopback result(s) are ready to be read out. */
#define UWB_LOOPBACK_INTERRUPT                             0xC300
 
/** @brief Trigger when the pin goes from high to low. This applies to GPIO interrupts only. */
#define FALLING                                            0x00
 
/** @brief Trigger when the pin goes from low to high. This applies to GPIO interrupts only. */
#define RISING                                             0x01
 
/** @brief Arbitrarily chosen value to code the completion of a RX event. */
#define RX_DONE                                            0x01
 
/** @brief Arbitrarily chosen value to code the completion of a TX event. */
#define TX_DONE                                            0x02
 
#define EVENT_TIMEOUT                                      0x0

/**
 * @brief Sets a function to be called when a specific interrupt occurs. This can be a GPIO pin 
that changes state, for example to detect a button press, or the reception of bytes over UART.
 *
 * @param source (uint16_t) Specifies the type of interrupt, which could be for example a specific GPIO pin, or the reception on a UWB message. For example, ``attachInterrupt( PIN_USER_BUTTON), isr, RISING)`` calls isr() every time pin 10 goes from low to high. Note that for a given source only one isr() can be attached. Thus, it is not possible to have both FALLING and RISING interrupts for the same GPIO pin.
 * @param function (void (*isr)(void)) The ISR (the interrupt service routine) is called when the interrupt occurs. This function must take no parameters and return nothing.
 * @param mode (int8_t) Defines when the interrupt should be triggered. For example on the RISING edge of a GPIO pin.
 *
 * @return errnoVal (ErrnoVal) .
 */
extern const ErrnoVal (*attachInterrupt)(uint16_t source, void (*isr)(void), int8_t mode);

/**
 * @brief Turn the given interrupt off.
 *
 * @param source (uint16_t) Specifies the type of interrupt, which could be for example a specific GPIO pin, or the reception of a UWB message.
 *
 * @return errnoVal (ErrnoVal) .
 */
extern const ErrnoVal (*detachInterrupt)(uint16_t source);

/**
 * @brief Disables interrupts until interrupts() is called. Interrupts can slightly disrupt the 
timing of code and may be disabled for particularly critical sections of code.
 *
 *
 * @return counter (uint32_t) Number of stacked noInterrupt calls. This number must be passed 
to interrupts() for reactivation.
 */
extern const uint32_t (*noInterrupts)(void);

/**
 * @brief Re-enables interrupts after they've been disabled by noInterrupts(). Interrupts are 
enabled by default.
 *
 * @param counter (uint32_t) Value that was last returned by noInterrupts().
 *
 * @return counter (uint32_t) Number of stacked noInterrupt calls. This number must be passed 
to interrupts() for reactivation.
 */
extern const uint32_t (*interrupts)(uint32_t counter);

/**
 * @brief Blocks the user application (like ``delay()``) until ``triggerEvent()`` is called 
from an interrupt service routine. This can be used for example to wait in your ``loop()`` 
until an interrupt GPIO pin is triggered, or a UWB packet is received.
 *
 * @param timeMs (uint32_t) As a safety mechanism, when ``triggerEvent()`` is not called within the given timeout, the function returns 0x0 to indicate a timeout has occurred. A timeout value of 0 means that no timeout is used. Units are [ms].
 *
 * @return eventMask (uint32_t) WaitForEvent() returns the ``eventMask`` value that is passed 
to the ``triggerEvent()`` function. Returns 0x0 when a timeout has occurred. In case 
``triggerEvent()`` is called multiple times before ``waitForEvent()``, the resulting 
``eventMask`` returned by ``waitForEvent()`` will be the or-ed total.
 */
extern const uint32_t (*waitForEvent)(uint32_t timeMs);

/**
 * @brief See waitForEvents().
 *
 * @param eventMask (uint32_t) The provided event mask will be returned by the pending ``waitForEvent()`` function. In case ``triggerEvent()`` is called multiple times before ``waitForEvent()``, the resulting ``eventMask`` returned by ``waitForEvent()`` will be the or-ed total. Its value should be between 0x1 and 0xFFFFFFFF. The default value is '0x0'.
 */
extern const void (*triggerEvent)(uint32_t eventMask);

/**
 * @brief Like, triggerEvent() but faster. .
 *
 * @param eventBitIndex (uint8_t) The bit corresponding with the provided zero-based index will be returned by the pending ``waitForEvent()`` function. Its value should be between 0 and 31.
 */
extern const void (*triggerEventBit)(uint8_t eventBitIndex);

/**
 * @brief Returns the number of times the specified bit index was triggered.
 *
 * @param eventBitIndex (uint8_t) To select the event mask bit of interrest. The index is zero-based, e.g. index=1 is to count triggerEvent(0x2) occurrences. Its value should be between 0 and 31.
 *
 * @return eventCount (uint32_t) .
 */
extern const uint32_t (*countEvent)(uint8_t eventBitIndex);

/**
 * @brief Clear pending event bits before a waitForEvent() is called. This way you make sure 
waitForEvent() will not immediately return when events were triggered prior.
 *
 * @param eventMask (uint32_t)  Its value should be between 0x1 and 0xFFFFFFFF. The default value is '0x0'.
 */
extern const void (*clearEvent)(uint32_t eventMask);

/**
 * @brief Reads the current event mask value.
 *
 *
 * @return eventMask (uint32_t) .
 */
extern const uint32_t (*readEvents)(void);


/**-----------------------------[           Serial API           ]-----------------------------**/

typedef struct __attribute__ ((__packed__)) {
/**
 * @brief Sets the data rate in bits per second (baud) for serial data transmission and 
configures the designated RX/TX pins.
 *
 * @param baudRate (uint32_t) Specify the rate at which bits are transmitted. Standard baud rates include 110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 128000 and 256000 bits per second. The default value is '115200'.
 */
  void (*begin)(uint32_t baudRate);

/**
 * @brief Get the number of bytes (characters) available for reading from the serial port. This 
is data that's already arrived and stored in the serial receive buffer (which can hold up to 
256 bytes).
 *
 *
 * @return numberOfBytes (uint16_t)  The default value is '0'.
 */
  uint16_t (*available)(void);

/**
 * @brief Returns the first byte of incoming serial data available (or -1 if no data is 
available).
 *
 *
 * @return byte (int16_t)  The default value is '-1'.
 */
  int16_t (*read)(void);

/**
 * @brief Reads characters from the serial port into a buffer. The function terminates if the 
determined length has been read or a timeout has occurred. ``Serial.readBytes()`` returns the 
number of characters placed in the buffer. A 0 means no valid data was found.
 *
 * @param buffer (uint8_t*) The buffer to store the bytes in.
 * @param length (uint16_t) The number of bytes to read. Its value should be between 1 and 256.
 *
 * @return bytesRead (uint16_t) The number of bytes successfully read.
 */
  uint16_t (*readBytes)(uint8_t* buffer, uint16_t length);

/**
 * @brief Writes binary data to the serial port.
 *
 * @param buffer (const uint8_t*) The buffer containing the bytes to send.
 * @param length (uint16_t) The number of bytes to send. Its value should be between 1 and 256.
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*write)(const uint8_t* buffer, uint16_t length);

/**
 * @brief Returns whether UART is currently busy with transmission. Catch SERIAL_WRITE_COMPLETE 
interrupt.
 *
 *
 * @return isBusy (bool) UART TX is happening.
 */
  bool (*busyWriting)(void);

/**
 * @brief Sets the maximum milliseconds to wait for serial data.
 *
 * @param timeMs (uint32_t)  Units are [ms]. The default value is '100'.
 */
  void (*setTimeout)(uint32_t timeMs);

/**
 * @brief Prints the given byte array as a HEX table.
 *
 * @param buffer (void*) The buffer to print.
 * @param length (uint16_t) The number of bytes to print.
 * @param columns (uint8_t) The number of columns in the table (typically 16).
 */
  void (*hexDump)(void* buffer, uint16_t length, uint8_t columns);

/**
 * @brief Prints data to the serial port as human-readable ASCII text. The C library function 
``int printf(const char *format, ...)`` is used here to send formatted output to stdout. Note 
that output stays in the UART FIFO buffer until a newline character is encountered. With a 
newline character all the bytes in the UART buffer are sent on the wire (UART flush). Also a 
buffer overflow will trigger a UART flush.
 *
 *
 * @return characterCount (int) Total number of characters printed.
 */
  int (*print)(const char*, ... );

/**
 * @brief The C library function ``int sprintf(char *str, const char *format, ...)`` is used 
here. The C Library sprintf() function allows you to create strings with specified formats, 
similar to printf(), but instead of printing to the standard output, it stores the resulting 
string in a character array provided by the user.
 *
 *
 * @return characterCount (int) The number of characters written to the string str, excluding 
the null-terminating character.
 */
  int (*sprint)(char *str , const char*, ... );

/**
 * @brief Prints 64-bit value with decimal notation. Note that the regular ``Serial.print()`` 
has an upper limit of 32-bit values, hence this function. .
 *
 * @param value (int64_t) Number to be printed, for example a 64-bit timestamp value.
 * @param buffer (const uint8_t*) Pointer to where the output should be written to. In case of 0 (NULL), the output will be sent over UART directly.
 *
 * @return characterCount (int) Total number of characters written.
 */
  int (*printInt64)(int64_t value, const uint8_t* buffer);

/**
 * @brief Prints the latest errno change, or ASSERT violation.
 *
 */
  void (*printError)(void);

/**
 * @brief Disables serial communication, allowing the RX and TX pins to be used for general 
input and output. To re-enable serial communication, call ``Serial.begin()``. Note that when 
``Serial.end()`` is called right after ``Serial.print(str)``, there is a chance that str is 
printed only partially.
 *
 */
  void (*end)(void);

} Serial_t;

extern const Serial_t Serial;

/**--------------------------------------------------------------------------------------------**/

/**-----------------------------[            SPI API             ]-----------------------------**/

 
/** @brief BitOrder: Sets the order of the bits shifted out of and into the SPI bus to 
least-significant bit first (default). */
#define LSBFIRST                                           0x00
 
/** @brief BitOrder: Sets the order of the bits shifted out of and into the SPI bus to the 
most-significant bit first. */
#define MSBFIRST                                           0x01
 
/** @brief DataMode: CPOL determines the polarity of the clock. If CPOL is enabled, the clock's 
leading edge is a falling edge, and the trailing edge is a rising edge. */
#define SPI_CPOL                                           0x08
 
/** @brief DataMode: CPHA determines the timing (i.e., phase) of the data bits relative to the 
clock pulses. When CPHA is enabled, data is sampled on the falling edge and shifted out on the 
rising edge. */
#define SPI_CPHA                                           0x04
 
/** @brief DataMode: Clock leading edge is rising (CPOL=0) and data are sampled on the leading 
(first, rising) clock edge (CPHA=0). */
#define SPI_MODE0                                          0x00
 
/** @brief DataMode: Clock leading edge is rising (CPOL=0) and data are sampled on the trailing 
(second, falling) clock edge (CPHA=1). */
#define SPI_MODE1                                          0x04
 
/** @brief DataMode: Clock leading edge is falling (CPOL=1) and data are sampled on the leading 
(first, falling) clock edge (CPHA=0). */
#define SPI_MODE2                                          0x08
 
/** @brief DataMode: Clock leading edge is falling (CPOL=1) and data are sampled on the 
trailing (second, rising) clock edge (CPHA=1). */
#define SPI_MODE3                                          0x0C
 
/** @brief DataMode: Use CS1 pin for slave select. */
#define SPI_CS1                                            0x40
 
/** @brief DataMode: Use CS2 pin for slave select. */
#define SPI_CS2                                            0x80
 
/** @brief Clock: Sets clock speed to 0 to use SPI in slave mode (in which the clock speed is 
determined by the master device). */
#define SPI_SLAVE_MODE                                     0
 
/** @brief DataMode: Chip Select (or Slave select) polarity selection. Active low is the 
default. */
#define SS_ACTIVE_LOW                                      0x00
 
/** @brief DataMode: Chip Select (or Slave select) polarity selection. */
#define SS_ACTIVE_HIGH                                     0x10
 
/** @brief DataMode: By default, the Chip Select gets deactivated when a ``SPI.transfer()`` is 
finished. However, by setting this flag, the Chip Select is kept in the active state until 
``SPI.endTransaction()`` is called. */
#define SS_HOLD_ACTIVE                                     0x20
typedef struct __attribute__ ((__packed__)) {
/**
 * @brief Initializes the SCK, MISO, and MOSI pins.
 *
 */
  void (*begin)(void);

/**
 * @brief Initializes the SPI bus using the defined SPISettings. An SPISettings object is used 
to configure the SPI port for your SPI device. All three parameters are combined to a single 
SPISettings object using ``SPISettings( speedMaximum, bitOrder, dataMode)``. For example, 
``SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0))``.
 *
 * @param settings (SPISettings) Use macro ``SPISettings(clock, bitOrder, dataMode)``.
 */
  void (*beginTransaction)(uint32_t settings);

/**
 * @brief Start receive.
 *
 * @param rxBuffer (uint8_t*) Pointer to buffer for incoming data.
 * @param rxLength (uint16_t) Length of data to be received (number of bytes).
 */
  void (*read)(uint8_t* rxBuffer, uint16_t rxLength);

/**
 * @brief Start receive.
 *
 * @param txBuffer (uint8_t*) Pointer to buffer for outgoing data.
 * @param txLength (uint16_t) Length of data to be written (number of bytes).
 */
  void (*write)(uint8_t* txBuffer, uint16_t txLength);

/**
 * @brief Start simultaneous send and receive.
 *
 * @param txBuffer (uint8_t*) Pointer to buffer for outgoing data.
 * @param rxBuffer (uint8_t*) Pointer to buffer for incoming data.
 * @param length (uint16_t) Length of data to be transmitted/received (number of bytes).
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*transfer)(uint8_t* txBuffer, uint8_t* rxBuffer, uint16_t length);

/**
 * @brief Returns the number of bytes received/transmitted during to the current 
``SPI.transfer``.
 *
 *
 * @return byteCount (uint16_t) The number of bytes transferred since ``SPI.transfer`` was 
called.
 */
  uint16_t (*count)(void);

/**
 * @brief ``SPI.beginTransaction()`` behaves like a locking mechanism to gain the exclusive use 
of the SPI bus, and therefore requires ``SPI.endTransaction()`` to release the bus for others 
to access it.
 *
 */
  void (*endTransaction)(void);

/**
 * @brief Stop using the SPI bus as a master.
 *
 */
  void (*end)(void);

} SPI_t;

extern const SPI_t SPI;

/**--------------------------------------------------------------------------------------------**/

/**-----------------------------[            I2C API             ]-----------------------------**/

 
/** @brief The target peripheral device kept SDA low at the end, communicating the 
acknowledgement (ack). */
#define WRITE_RESULT_ACK                                   0
 
/** @brief The target peripheral device did not respond with an ACK signal. */
#define WRITE_RESULT_NACK                                  -1
 
/** @brief Generation of the clock signal cannot continue, beacuse a peripheral device holds 
the clock line low for too long. */
#define WRITE_RESULT_TIMEOUT                               -2
 
/** @brief The target peripheral device kept SDA low at the end, communicating the 
acknowledgement (ack). */
#define READ_RESULT_ACK                                    0
 
/** @brief The target peripheral device did not respond with an ``ACK`` signal. */
#define READ_RESULT_NACK                                   -1
 
/** @brief Generation of the clock signal cannot continue, beacuse a peripheral device holds 
the clock line low for too long. */
#define READ_RESULT_TIMEOUT                                -2
typedef struct __attribute__ ((__packed__)) {
/**
 * @brief Initializes the module as a (software-) I2C master.
 *
 * @param sdaPinMask (uint16_t) The pin identifier mask for the SDA (data) pin. Because this is a software-I2C implementation, any free GPIO pin can be used.
 * @param sclPinMask (uint16_t) The pin identifier mask for the SCL (clock) pin. Because this is a software-I2C implementation, any free GPIO pin can be used.
 */
  void (*begin)(uint16_t sdaPinMask, uint16_t sclPinMask);

/**
 * @brief Write the provided byte array to the target peripheral device.
 *
 * @param address (uint8_t) The 7-bit address of the slave.
 * @param data (uint8_t*) Byte to send.
 * @param length (uint16_t) Size of the data array. Units are [bytes].
 * @param stop (bool) Indicates whether to send a stop bit at the end of the write (typically set to TRUE).
 *
 * @return result (int8_t) Indicates whether the target peripheral device responded with an 
``ACK`` (0), ``NACK`` (-1), or whether a ``TIMEOUT`` (-2) occurred. A timeout typically occurs 
when a peripheral device holds the clock line low for too long.
 */
  int8_t (*write)(uint8_t address, uint8_t* data, uint16_t length, bool stop);

/**
 * @brief To request bytes from a peripheral device.
 *
 * @param address (uint8_t) The 7-bit address of the slave.
 * @param data (uint8_t*) Buffer to store the received bytes.
 * @param length (uint16_t) How many bytes we want to receive.
 * @param stop (bool) True will send a stop message after the request, releasing the bus. False will continually send a restart after the request, keeping the connection active.
 *
 * @return result (int16_t) Returns the number of successfully received bytes. In case of an 
error or timeout, the value will be negative.
 */
  int16_t (*read)(uint8_t address, uint8_t* data, uint16_t length, bool stop);

/**
 * @brief Deinitializs I2C.
 *
 */
  void (*end)(void);

} I2C_t;

extern const I2C_t I2C;

/**--------------------------------------------------------------------------------------------**/

/**-----------------------------[          Numbers API           ]-----------------------------**/


/**
 * @brief The random function generates pseudo-random numbers. Returns a random number between 
min and max-1.
 *
 * @param min (uint32_t) Lower bound of the random value, inclusive (optional).
 * @param max (uint32_t) Upper bound of the random value, exclusive.
 *
 * @return number (uint32_t) .
 */
extern const uint32_t (*getRandom)(uint32_t min, uint32_t max);

/**
 * @brief Initializes the pseudo-random number generator, causing it to start at an arbitrary 
point in its random sequence.
 *
 * @param seed (uint32_t) Number to initialize the pseudo-random sequence. Use ``randomSeed()`` to initialize the random number generator with a fairly random input, such as a timestamp, ``analogRead()`` on an unconnected pin.
 */
extern const void (*randomSeed)(uint32_t seed);

/**
 * @brief Fast integer square root algorithm with arithmetic rounding of the result. That is, 
if the real answer would have a fractional part of 0.5 or greater, the result is rounded up to 
the next integer.
 *
 * @param value (uint32_t) Unsigned integer for which to find the square root.
 *
 * @return number (uint32_t) Integer square root of the input value.
 */
extern const uint32_t (*usqrt)(uint32_t value);

/**
 * @brief Returns unique device serial number.
 *
 *
 * @return serialNumber (uint32_t) Serial number.
 */
extern const uint32_t (*getSerialNumber)(void);

/**
 * @brief Returns the current total crc of the user application.
 *
 *
 * @return checksum (uint32_t) Checksum.
 */
extern const uint32_t (*getCurrentCrc)(void);

/**
 * @brief Returns the stored antenna delay, measured during production tests.
 *
 *
 * @return delay (uint32_t) Antenna delay in ticks.
 */
extern const uint32_t (*getAntennaDelay)(void);

/**
 * @brief Returns the harware identifier.
 *
 *
 * @return id (uint32_t) Harware identifier.
 */
extern const uint32_t (*getHardwareId)(void);


/**-----------------------------[           Crypto API           ]-----------------------------**/

 
/** @brief Algorithm identifier of the AES-128 algorithm. */
#define CONFIG_ENCRYPTION_ALGORITHM_AES_128                0x0101
 
/** @brief Algorithm identifier of the AES-256 algorithm. */
#define CONFIG_ENCRYPTION_ALGORITHM_AES_256                0x0102
 
/** @brief Algorithm identifier of the ECC algorithm over NIST P-256 curve. */
#define CONFIG_ENCRYPTION_ALGORITHM_ECC_NIST_P256          0x0201
 
/** @brief Algorithm identifier of the ECC algorithm over NIST P-384 curve. */
#define CONFIG_ENCRYPTION_ALGORITHM_ECC_NIST_P384          0x0202
 
/** @brief Algorithm identifier of the ECC algorithm over NIST P-521 curve. */
#define CONFIG_ENCRYPTION_ALGORITHM_ECC_NIST_P521          0x0203
 
/** @brief Electronic Codebook Mode (ECB). ECB mode encryption and decryption is performed 
without any cipher padding. It is the sole responsibility of the user to implement padding (if 
needed). For this mode of operation the size of the plaintext and the size of the ciphertext 
are always the same. This mode can only process ciphertext and/or plaintext sizes that are an 
even multiple of the underlying cipher's block size. */
#define CONFIG_ENCRYPTION_CIPHER_MODE_ECB                  0x01
 
/** @brief Counter Mode (CTR). CTR mode encryption and decryption is performed without any 
cipher padding. For this mode of operation the size of the plaintext buffer and the size of the 
ciphertext buffer are always the same. For this mode of operation the size of the ciphertext 
and/or plaintext can be an arbitrary byte size; it does not have to be an even multiple of the 
underlying cipher's block size. @note This mode of operation requires an initial nonce value 
for the counter. The nonce value is passed via the IV field of the cipher parameter structure. */
#define CONFIG_ENCRYPTION_CIPHER_MODE_CTR                  0x02
 
/** @brief Cipher Block Chaining Mode (CBC) with PKCS#7 padding. For this mode of operation the 
size of the ciphertext buffer can be up to one cipher block larger than the size of the 
plaintext buffer. The implementation automatically applies/removes PKCS#7 padding from the 
plaintext/ciphertext. There are no restrictions on the size of the (unpadded) plaintext; the 
ciphertext size is always an even multiple of the underlying block cipher's size. @note This 
mode of operation requires an Initialization Vector (IV) of exactly one cipher block. @note 
When decrypting PKCS#7 padded ciphertexts particular precautions have to be taken to avoid 
inadvertent creation of padding oracle vulnerabilities. Ciphertexts that have been encrypted 
with PKCS#7 padding should ONLY be decrypted AFTER they have been authenticated. */
#define CONFIG_ENCRYPTION_CIPHER_MODE_CBC                  0x03
 
/** @brief Counter-Mode with CBC-MAC (CCM). For this mode of operation the size of the 
plaintext buffer and the size of the ciphertext buffer are always the same. For this mode of 
operation the size of the ciphertext and/or plaintext can be an arbitrary byte size; it does 
not have to be an even multiple of the underlying cipher's block size. @note This mode of 
operation requires an initial nonce value for the counter. The nonce value is passed via the IV 
field of the cipher parameter structure. @note This mode of operation produces/uses an 
authentication tag. The size of the authentication tag can be any of 4, 6, 8, 10, 12, 14, or 16 
bytes. */
#define CONFIG_ENCRYPTION_CIPHER_MODE_CCM                  0x04
 
/** @brief Cipher Block Chaining Mode (CBC) without padding. CBC mode encryption and decryption 
is performed without any cipher padding. It is the sole responsibility of the user to implement 
padding (if needed). For this mode of operation the size of the plaintext and the size of the 
ciphertext are always the same. This mode can only process ciphertext and/or plaintext sizes 
that are an even multiple of the underlying cipher's block size. @note This mode of operation 
requires an Initialization Vector (IV) of exactly one cipher block. */
#define CONFIG_ENCRYPTION_CIPHER_MODE_CBC_NO_PADDING       0x05
 
/** @brief EAX mode (encrypt-then-authenticate-then-translate) is a mode of operation for 
cryptographic block ciphers. It is an Authenticated Encryption with Associated Data (AEAD) 
algorithm designed to simultaneously provide both authentication and privacy of the message 
(authenticated encryption) with a two-pass scheme, one pass for achieving privacy and one for 
authenticity for each block. */
#define CONFIG_ENCRYPTION_CIPHER_MODE_EAX                  0x06
 
/** @brief Algorithm identifier of the AES-128 algorithm. */
#define CONFIG_DECRYPTION_ALGORITHM_AES_128                0x0101
 
/** @brief Algorithm identifier of the AES-256 algorithm. */
#define CONFIG_DECRYPTION_ALGORITHM_AES_256                0x0102
 
/** @brief Algorithm identifier of the ECC algorithm over NIST P-256 curve. */
#define CONFIG_DECRYPTION_ALGORITHM_ECC_NIST_P256          0x0201
 
/** @brief Algorithm identifier of the ECC algorithm over NIST P-384 curve. */
#define CONFIG_DECRYPTION_ALGORITHM_ECC_NIST_P384          0x0202
 
/** @brief Algorithm identifier of the ECC algorithm over NIST P-521 curve. */
#define CONFIG_DECRYPTION_ALGORITHM_ECC_NIST_P521          0x0203
 
/** @brief Electronic Codebook Mode (ECB). ECB mode encryption and decryption is performed 
without any cipher padding. It is the sole responsibility of the user to implement padding (if 
needed). For this mode of operation the size of the plaintext and the size of the ciphertext 
are always the same. This mode can only process ciphertext and/or plaintext sizes that are an 
even multiple of the underlying cipher's block size. */
#define CONFIG_DECRYPTION_CIPHER_MODE_ECB                  0x01
 
/** @brief Counter Mode (CTR). CTR mode encryption and decryption is performed without any 
cipher padding. For this mode of operation the size of the plaintext buffer and the size of the 
ciphertext buffer are always the same. For this mode of operation the size of the ciphertext 
and/or plaintext can be an arbitrary byte size; it does not have to be an even multiple of the 
underlying cipher's block size. @note This mode of operation requires an initial nonce value 
for the counter. The nonce value is passed via the IV field of the cipher parameter structure. */
#define CONFIG_DECRYPTION_CIPHER_MODE_CTR                  0x02
 
/** @brief Cipher Block Chaining Mode (CBC) with PKCS#7 padding. For this mode of operation the 
size of the ciphertext buffer can be up to one cipher block larger than the size of the 
plaintext buffer. The implementation automatically applies/removes PKCS#7 padding from the 
plaintext/ciphertext. There are no restrictions on the size of the (unpadded) plaintext; the 
ciphertext size is always an even multiple of the underlying block cipher's size. @note This 
mode of operation requires an Initialization Vector (IV) of exactly one cipher block. @note 
When decrypting PKCS#7 padded ciphertexts particular precautions have to be taken to avoid 
inadvertent creation of padding oracle vulnerabilities. Ciphertexts that have been encrypted 
with PKCS#7 padding should ONLY be decrypted AFTER they have been authenticated. */
#define CONFIG_DECRYPTION_CIPHER_MODE_CBC                  0x03
 
/** @brief Counter-Mode with CBC-MAC (CCM). For this mode of operation the size of the 
plaintext buffer and the size of the ciphertext buffer are always the same. For this mode of 
operation the size of the ciphertext and/or plaintext can be an arbitrary byte size; it does 
not have to be an even multiple of the underlying cipher's block size. @note This mode of 
operation requires an initial nonce value for the counter. The nonce value is passed via the IV 
field of the cipher parameter structure. @note This mode of operation produces/uses an 
authentication tag. The size of the authentication tag can be any of 4, 6, 8, 10, 12, 14, or 16 
bytes. */
#define CONFIG_DECRYPTION_CIPHER_MODE_CCM                  0x04
 
/** @brief Cipher Block Chaining Mode (CBC) without padding. CBC mode encryption and decryption 
is performed without any cipher padding. It is the sole responsibility of the user to implement 
padding (if needed). For this mode of operation the size of the plaintext and the size of the 
ciphertext are always the same. This mode can only process ciphertext and/or plaintext sizes 
that are an even multiple of the underlying cipher's block size. @note This mode of operation 
requires an Initialization Vector (IV) of exactly one cipher block. */
#define CONFIG_DECRYPTION_CIPHER_MODE_CBC_NO_PADDING       0x05
 
/** @brief EAX mode (encrypt-then-authenticate-then-translate) is a mode of operation for 
cryptographic block ciphers. It is an Authenticated Encryption with Associated Data (AEAD) 
algorithm designed to simultaneously provide both authentication and privacy of the message 
(authenticated encryption) with a two-pass scheme, one pass for achieving privacy and one for 
authenticity for each block. */
#define CONFIG_DECRYPTION_CIPHER_MODE_EAX                  0x06
typedef struct __attribute__ ((__packed__)) {
/**
 * @brief Powers up the minimal hardware blocks required to use the cryptographic subsystem.
 *
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*begin)(void);

/**
 * @brief This function fills a memory region with random data from a high-quality random 
number generator. The underlying random number generator is a combination of a PRNG, and an 
analog entropy source. Reseeding of the PRNG from the analog entropy source is handled 
automatically by the API function.
 *
 * @param destination (uint8_t *const) Points to the start of the memory region to be filled.
 * @param size (uint32_t) Is the size of the memory region to be filled (in bytes).
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*randomBytes)(uint8_t *const destination, uint32_t size);

/**
 * @brief A cyclic redundancy check (CRC) is an error-detecting code commonly used in digital 
networks and storage devices to detect accidental changes to digital data. There are many 
variants, but they all use polynomial division. With the ``configCrc()`` function, the 
parameters can be set in order to obtain a specific variant. The order of parameters here 
matches most online CRC calculators, for example at www.crccalc.com.
 *
 * @param poly (uint32_t) CRC polynomial value. The default value is '0x04C11DB7'.
 * @param init (uint32_t) CRC initial value. The default value is '0xFFFFFFFF'.
 * @param reflect (uint8_t) If 0 (false), MSB first is used as an CRC input data bit order. If 1, bits are swapped around its center and the output is swapped back. The default value is '1'.
 * @param xorOut (uint32_t) The value to be XORed with the final register value. The default value is '0xFFFFFFFF'.
 */
  void (*configCrc)(uint32_t poly, uint32_t init, uint8_t reflect, uint32_t xorOut);

/**
 * @brief Calculate a CRC-32 checksum with given parameters over an application-visible 
(source) memory region.
 *
 * @param source (uint8_t*) Location of data.
 * @param length (uint32_t) Length of the input data in bytes.
 *
 * @return result (uint32_t) The output CRC value.
 */
  uint32_t (*calcCrc)(uint8_t* source, uint32_t length);

/**
 * @brief Provide the parameters of the encryption operation, including the encryption key, the 
mode of operation, and the initialization value (if needed).
 *
 * @param keyValue (uint8_t*) Raw user-provided key.
 * @param keyLength (uint8_t) Length of the provided key value (number of bytes).
 * @param algorithm (uint16_t) The underlying cryptographic algorithm.
 * @param ivValue (uint8_t*) Initialization Vector (IV). The interpretation of this field depends on the mode of operation selected using the mode field. For cipher modes that do not use an IV (MODE_ECB) this field must be set as a zero sized input data buffer, with NULL data pointer.  The data is not copied to a buffer, so make sure the pointer still points to valid data at the time Crypto.encrypt() is executed.
 * @param ivLength (uint8_t) Length of the provided IV value (number of bytes).
 * @param cipherMode (uint8_t) Mode of operation for the cipher. This field selects the mode of operation to be used by cipher. The interpretation of the iv field depends on the selected mode of operation.
 * @param aadValue (uint8_t*) Additional Authenticated Data (AAD). The interpretation of this field depends on the mode of operation selected using the mode field. It is only used by authenticated encryption (AE) cipher modes. This field must be set as a zero size input data buffer, with NULL data pointer, if an unauthenticated cipher mode (eg. MODE_ECB, MODE_CTR, MODE_CBC) is used. The data is not copied to a buffer, so make sure the pointer still points to valid data at the time Crypto.encrypt() is executed.
 * @param aadLength (uint8_t) Length of the provided AAD value (number of bytes).
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*configEncryption)(uint8_t* keyValue, uint8_t keyLength, uint16_t algorithm, uint8_t* ivValue, uint8_t ivLength, uint8_t cipherMode, uint8_t* aadValue, uint8_t aadLength);

/**
 * @brief Provide the parameters of the decryption operation, including the encryption key, the 
mode of operation, and the initialization value (if needed).
 *
 * @param keyValue (uint8_t*) Raw user-provided key.
 * @param keyLength (uint8_t) Length of the provided key value (number of bytes).
 * @param algorithm (uint16_t) The underlying cryptographic algorithm.
 * @param ivValue (uint8_t*) Initialization Vector (IV). The interpretation of this field depends on the mode of operation selected using the mode field. For cipher modes that do not use an IV (MODE_ECB) this field must be set as a zero sized input data buffer, with NULL data pointer.  The data is not copied to a buffer, so make sure the pointer still points to valid data at the time Crypto.encrypt() is executed.
 * @param ivLength (uint8_t) Length of the provided IV value (number of bytes).
 * @param cipherMode (uint8_t) Mode of operation for the cipher. This field selects the mode of operation to be used by cipher. The interpretation of the iv field depends on the selected mode of operation.
 * @param aadValue (uint8_t*) Additional Authenticated Data (AAD). The interpretation of this field depends on the mode of operation selected using the mode field. It is only used by authenticated encryption (AE) cipher modes. This field must be set as a zero size input data buffer, with NULL data pointer, if an unauthenticated cipher mode (eg. MODE_ECB, MODE_CTR, MODE_CBC) is used. The data is not copied to a buffer, so make sure the pointer still points to valid data at the time Crypto.encrypt() is executed.
 * @param aadLength (uint8_t) Length of the provided AAD value (number of bytes).
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*configDecryption)(uint8_t* keyValue, uint8_t keyLength, uint16_t algorithm, uint8_t* ivValue, uint8_t ivLength, uint8_t cipherMode, uint8_t* aadValue, uint8_t aadLength);

/**
 * @brief Encrypts a contiguous region of memory.
 *
 * @param source (uint8_t*) Location of data to encrypt.
 * @param length (uint16_t) Length of the input data in bytes.
 * @param destination (uint8_t*) Location where the encrypted data should be written to.
 * @param authenticationTag (uint32_t*) Output for the encrypted authentication tag generated by authenticated encryption modes (cipher mode CCM only). This parameter is only required for authenticated encryption modes, and may be omitted (NULL) otherwise.
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*encrypt)(uint8_t* source, uint16_t length, uint8_t* destination, uint32_t* authenticationTag);

/**
 * @brief Decrypts a contiguous region of memory.
 *
 * @param source (uint8_t*) Location of data to decrypt.
 * @param length (uint16_t) Length of the input data in bytes.
 * @param destination (uint8_t*) Location where the decrypted data should be written to.
 * @param authenticationTag (uint32_t*) The encrypted authentication tag generated by authenticated encryption modes (cipher mode CCM only). This parameter is only required for authenticated encryption modes, and may be omitted (NULL) otherwise.
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*decrypt)(uint8_t* source, uint16_t length, uint8_t* destination, uint32_t* authenticationTag);

/**
 * @brief Powers down hardware blocks that are required by the cryptographic subsystem.
 *
 */
  void (*end)(void);

} Crypto_t;

extern const Crypto_t Crypto;

/**--------------------------------------------------------------------------------------------**/

/**-----------------------------[           EEPROM API           ]-----------------------------**/

typedef struct __attribute__ ((__packed__)) {
/**
 * @brief Reads a byte from the EEPROM.
 *
 * @param address (uint16_t) The location to read from, starting from 0. Its value should be equal or smaller than 1023.
 *
 * @return value (uint8_t) The value stored in that location (byte).
 */
  uint8_t (*read)(uint16_t address);

/**
 * @brief Reads a byte from the EEPROM.
 *
 * @param address (uint16_t) The location to read from, starting from 0. Its value should be equal or smaller than 1023.
 * @param value (uint8_t) The value to be stored in that location (byte).
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*write)(uint16_t address, uint8_t value);

/**
 * @brief Reads bytes from the EEPROM.
 *
 * @param buffer (uint8_t*) The buffer to store the bytes in.
 * @param address (uint16_t) The location to read from, starting from 0. Its value should be equal or smaller than 1023.
 * @param length (uint16_t) The number of bytes to read. Its value should be between 1 and 1023.
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*readBytes)(uint8_t* buffer, uint16_t address, uint16_t length);

/**
 * @brief Writes bytes to the EEPROM.
 *
 * @param buffer (uint8_t*) The buffer to store the bytes in.
 * @param address (uint16_t) The location to read from, starting from 0. Its value should be equal or smaller than 1023.
 * @param length (uint16_t) The number of bytes to read. Its value should be between 1 and 1023.
 *
 * @return errnoVal (ErrnoVal) .
 */
  ErrnoVal (*writeBytes)(uint8_t* buffer, uint16_t address, uint16_t length);

} EEPROM_t;

extern const EEPROM_t EEPROM;

/**--------------------------------------------------------------------------------------------**/

/**-----------------------------[          Aliases API           ]-----------------------------**/


/**
 * @brief Returns pointer to current UWB timestamp, which runs at 128*499.2 ticks per 
microsecond. This number will overflow (go back to zero) after approximately 9 years.
 *
 *
 * @return count (uint64_t*) .
 */
extern const uint64_t* (*ticks)(void);

/**
 * @brief Define payload data and payload length as separate function arguments. See also 
UWB.setPayload().
 *
 * @param buffer (uint8_t*) The buffer to store the bytes in.
 * @param length (uint16_t) The number of bytes to read. Its value should be between 1 and 1023.
 *
 * @return errnoVal (UciStatus) .
 */
extern const UciStatus (*setPayload)(uint8_t* buffer, uint16_t length);



#ifdef __cplusplus
} // extern "C"
#endif
#endif /* SFS_USER_APPLICATION_H */


/** End of file. Note: this file is auto generated. Do not edit manually.
*   Copyright SynchronicIT 2025
*/
