
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

#include "user-application.h"

static void __attribute__((used)) user_application_startup() {
  extern uint32_t __text_end__;
  extern uint32_t __data_start__;
  extern uint32_t __data_end__;
  extern uint32_t __bss_start__;
  extern uint32_t __bss_end__;
  uint32_t *data_start_flash = &__text_end__;
  uint32_t *copy_start = &__data_start__;
  while (copy_start < &__data_end__) {
    *copy_start++ = *data_start_flash++;
  }
  uint32_t *bss_start = &__bss_start__;
  while (bss_start < &__bss_end__) {
    *bss_start++ = 0UL;
  }
  setup();
}

int main(void) {
  setup();
  while (1) {
    loop();
  }
  return 0;
}

void __attribute__((weak)) loop() {}

#if !defined(CONFIG_LOOP_VALUE)
  #define CONFIG_LOOP_VALUE loop
#endif

#if !defined(CONFIG_APP_TYPE)
  #define CONFIG_APP_TYPE 0xA0
#endif

#if !defined(CONFIG_APP_STATUS)
  #define CONFIG_APP_STATUS 0x0F
#endif
static const __attribute__((used)) user_application_header_t user_application_header 
  __attribute__((section(".flash.fwHeader"), used)) = { 
  .header_id = 0xDCAC, 
  .type = 0xA0, 
  .status = 0xF, 
  .version_major = STANDALONE_API_VERSION_MAJOR, 
  .version_minor = STANDALONE_API_VERSION_MINOR, 
  .version_patch = STANDALONE_API_VERSION_PATCH, 
  .version_date = STANDALONE_API_VERSION_DATE, 
  .setup = user_application_startup, 
  .loop = loop, 
  .appVersion = CONFIG_APP_VERSION,
  .loopInterval = CONFIG_LOOP_INTERVAL,
}; 

uint32_t* p_errno = (uint32_t*)(0x200080e8);

uint32_t* p_errline = (uint32_t*)(0x20008010);

uint32_t* p_dbg_uci = (uint32_t*)(0x200080ec);
#define PHY_DEBUG Serial.print("PHY_DEBUG: %u, %u\n", (*p_dbg_uci) >> 16, (*p_dbg_uci) & 0xFFFF);



/**
 * @brief Indicates the cause of the last reset.
 *
 * @return reason (uint8_t) .
 */
const uint8_t (*getResetReason)(void) = (void*)(0x00204ecd);

/**
 * @brief Resets the device. All global variables will be cleared, except for the one stored in 
the Persistent Register.
 */
const void (*reset)(void) = (void*)(0x00204ed9);

/**
 * @brief Puts the chip into a low power state to save energy (deep power down).
 *
 * @param time (uint32_t) Wake-up time in ms. Non-zero only allowed in Deep Power Down mode. 
This configures the wake-up timer with the given value.
 * @param pin (uint16_t) Wake-up pin configuration. Please note that at least one wake up 
source has to be configured. If no wake up source is configured a parameter error will be 
returned.
 * @param pinEdge (uint8_t) Configures the edge of the wake up pin signal. 0 = falling, 1 = 
rising edge.
 *
 * @return errnoVal (ErrnoVal) .
 */
const ErrnoVal (*sleep)(uint32_t time, uint16_t pin, uint8_t pinEdge) = (void*)(0x002050cd);

/**
 * @brief Puts the chip into the lowest power state to save energy (hard power down). Only 
PIN_RX, PIN_TX, and D8 (GPIO0) can be used (FALLING edge only).
 *
 * @param pin (uint16_t) Wake-up pin configuration. Please note that at least one wake up 
source has to be configured. If no wake up source is configured a parameter error will be 
returned.
 *
 * @return errnoVal (ErrnoVal) .
 */
const ErrnoVal (*deepSleep)(uint16_t pin) = (void*)(0x002050d9);

/**
 * @brief Limit the current, to improve battery life. Current Limit can be set between 5 and 20 
mA. When using the SFM10-dev, make sure to desolder the bypass resistor between VBAT and VBUF.
 *
 * @param milliAmps (uint8_t) A value of 0 disables the current limiter. It is advised to 
connect VBuf and VBat when the current limiter is not used. The value(s) should be between 0 
and 20. The default value is '0'.
 *
 * @return errnoVal (ErrnoVal) .
 */
const ErrnoVal (*limitCurrent)(uint8_t milliAmps) = (void*)(0x002050e5);

/**
 * @brief When the device is resetted using the reset pin / button, resetted via software, or 
awakens from a deep or hard power down, all data in RAM is cleared. However, there are 4 bytes 
of data that persist and survive a reset. This can be read back as a 32 bit value using this 
function. To store the data, use ``peristentWrite()``. These 32 bits are cleared only after a 
Power-On Reset.
 *
 * @return value (uint32_t) .
 */
const uint32_t (*persistentRead)(void) = (void*)(0x00205119);

/**
 * @brief When the device is resetted using the reset pin / button, resetted via software, or 
awakens from a deep or hard power down, all data in RAM is cleared. However, there are 4 bytes 
of data that persist and survive a reset. This can be set as a 32 bit value using this 
function. To read back the data, use ``peristentRead()``. These 32 bits are cleared only after 
a Power-On Reset.
 *
 * @param value (uint32_t) .
 */
const void (*persistentWrite)(uint32_t value) = (void*)(0x0020511d);

const StackUsage_t StackUsage = {

/**
 * @brief Fills the stack area with a dedicated pattern in order to find the usage later.
 */
  .begin = (void*)(0x002022a5),

/**
 * @brief Returns how far the stack has been used since begin() was called.
 *
 * @return byteCount (uint16_t) .
 */
  .findMax = (void*)(0x002022c5),

/**
 * @brief Returns the current stack size.
 *
 * @return byteCount (uint16_t) .
 */
  .current = (void*)(0x002022e9),

/**
 * @brief Returns total stack area (constant).
 *
 * @return byteCount (uint16_t) .
 */
  .limit = (void*)(0x002022f9),
};


/**
 * @brief Configures the specified GPIO pin to behave either as an input or an output.
 *
 * @param pinMask (uint16_t) To indicate which pins should have the specified mode. This should 
be a 10-bit number with each bit representing one of the GPIO pins.
 * @param mode (uint8_t) Pin Mode can be set to INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN, 
OUTPUT_LOW, or OUTPUT_HIGH.
[mode values]:
  INPUT                  Digital input without an internal resistor enabled.
  OUTPUT                 Digital output, starting with level set to LOW.
  INPUT_PULLUP           Digital input with an internal pullup resistor enabled.
  INPUT_PULLDOWN         Digital input with an internal pulldown resistor enabled.
  OUTPUT_LOW             Digital output, starting with level set to LOW.
  OUTPUT_HIGH            Digital output, starting with level set to HIGH.
 *
 * @return errnoVal (ErrnoVal) .
 */
const ErrnoVal (*pinMode)(uint16_t pinMask, uint8_t mode) = (void*)(0x002026a1);

/**
 * @brief Write a HIGH (0x01) or a LOW (0x00) value to a digital pin. If the pin has been 
configured as an OUTPUT with ``pinMode()``, its voltage will be set to the corresponding value 
(e.g., 3.3V for HIGH, 0V (ground) for LOW). If the pin is configured as an INPUT, this function 
will not have an effect. It is recommended to set the ``pinMode()`` to INPUT_PULLUP to enable 
the internal pull-up resistor or INPUT_PULLDOWN to enable the internal pull-down resistor.
 *
 * @param pinMask (uint16_t) To indicate which pins should be set. This should be a 10-bit 
number with each bit representing one of the GPIO pins.
 * @param value (uint8_t) HIGH or LOW.
[value values]:
  LOW                    Logical 0.
  HIGH                   Logical 1.
 *
 * @return errnoVal (ErrnoVal) .
 */
const ErrnoVal (*digitalWrite)(uint16_t pinMask, uint8_t value) = (void*)(0x002027ed);

/**
 * @brief Reads the value from a specified digital pin. The result will be either ``HIGH`` or 
``LOW``.
 *
 * @param pinMask (uint16_t) To indicate which pin should be read.
 *
 * @return value (uint8_t) HIGH or LOW.
 */
const uint8_t (*digitalRead)(uint16_t pinMask) = (void*)(0x0020284d);

/**
 * @brief Reads the value from the specified analog pin. On the SFM10 boards, all pins are 
connected internally that allows you to read supply voltages and temperatures.
 *
 * @param channel (uint8_t) To indicate which measure you want.
[channel values]:
  VDD_BUF                Voltage at VDD BUF (VBUF) pin in millivolts.
  VDD_GLOB               Voltage at VDD GLOB pin in millivolts.
  VDD_DIG                Voltage at VDD DIG pin in millivolts.
  VDD_IO                 Voltage at VDD IO pin in millivolts.
  VDD_NTC_P              Voltage at positive pin of external NTC, in millivolts.
  VDD_NTC_N              Voltage at the negative pin of external NTC, in millivolts.
  EXT_TEMPERATURE        Temperature of external NTC, as a raw calibrated ADC reading (10 bit).
  INT_TEMPERATURE        Temperature of internal NTC, in millidegrees.
  CALIBRATE_ADC          To perform recalibration of the internal ADC.
  EXT_TEMPERATURE_RAW    Temperature of external NTC as a raw and uncalibrated ADC value.
  INT_TEMPERATURE_RAW    Temperature of internal NTC as a raw and uncalibrated ADC value.
 *
 * @return value (int32_t) .
 */
const int32_t (*analogRead)(uint8_t channel) = (void*)(0x00202891);


/**
 * @brief Returns the number of milliseconds passed since the module board began running the 
current program. This number will overflow (go back to zero) after approximately 50 days.
 *
 * @return count (uint32_t) Number of milliseconds passed since the module board reset.
 */
const uint32_t (*millis)(void) = (void*)(0x00204685);

/**
 * @brief Returns the number of microseconds since the module board began running the current 
program. This number will overflow (go back to zero) after approximately one hour and 11 
minutes.
 *
 * @return count (uint32_t) Number of microseconds passed since the module board reset.
 */
const uint32_t (*micros)(void) = (void*)(0x00204561);

/**
 * @brief Returns a pointer to the number of microseconds since the module board began running 
the current program. This number will overflow (go back to zero) after approximately 0.6 
million years.
 *
 * @return count (uint64_t*) Number of microseconds passed since the module board reset.
 */
const uint64_t* (*micros64)(void) = (void*)(0x002045e1);

/**
 * @brief Pauses / blocks the program for the amount of time (in milliseconds) specified by the 
parameter ``timeMs``. During this period, the UWB subsystem could continue operation or could 
decide to temporarily transition into a sleep mode to save power, depending on the current 
tasks. Certain things do go on while the ``delay()`` function is being executed, because the 
delay function does not disable interrupts.
 *
 * @param timeMs (uint32_t) Amount of time in milliseconds.
 */
const void (*delay)(uint32_t timeMs) = (void*)(0x002046c3);

/**
 * @brief Pauses / blocks the program for the amount of time (in microseconds) specified by the 
parameter. See ``delay()``.
 *
 * @param time (uint32_t) Amount of time in microseconds.
 */
const void (*delayMicroseconds)(uint32_t time) = (void*)(0x0020469b);

/**
 * @brief Returns frequency deviation of the crystal corresponding to the measured temperature.
 *
 * @param adc_ext_temperature (int32_t) .
[adc_ext_temperature values]:
  FROM_TEMPERATURE       Will use the calibrated ADC reading of the XTAL NTC. Identical to `analogRead(EXT_TEMPERATURE)`.
 *
 * @return count (int32_t) Parts per billion.
 */
const int32_t (*getXtalPpb)(int32_t adc_ext_temperature) = (void*)(0x00204779);

const Timer_t Timer = {

/**
 * @brief Refresh the watchdog timeout per passed milliseconds. This function has to be 
periodically called before the timeout time expires, in order to prevent the watchdog from 
triggering a reset. However, at startup the watchdog is disabled per default.
 *
 * @param timeoutMs (uint32_t) Timeout count in milliseconds. Timeout count of 0 is equivalent 
to disabling the watchdog. The watchdog is driven with an input clock of 1.2MHz independent of 
the configured system clock. A watchdog tick is 1ms. Units are [ms].
 */
  .setWatchdog = (void*)(0x00203e2d),

/**
 * @brief Set the watchdog counter register back to the value of ``Timer.setWatchdog()``.
 */
  .kickWatchdog = (void*)(0x00203e31),

/**
 * @brief Starts the global timer class.
 */
  .begin = (void*)(0x00203e51),

/**
 * @brief Schedules a function call at the given time, expressed in milliseconds passed since 
the module board began running the current program (refer ``millis()``). In contrast to the 
other Timer functions, up to 8 timestamps / functions can be queued. To empty the queue 
preemptively, call Timer.end().
 *
 * @param timeMs (uint32_t) Amount of time in milliseconds. Units are [ms].
 * @param function (void (*isr)(void)) The function that is called at the specified time. This 
function must take no parameters and return nothing.
 *
 * @return errnoVal (ErrnoVal) .
 */
  .at = (void*)(0x00204161),

/**
 * @brief Similar to ``Timer.at()`` except the given time is in microseconds instead of 
milliseconds.
 *
 * @param timeUs (uint64_t) Amount of time in milliseconds. Units are [us].
 * @param function (void (*isr)(void)) The function that is called at the specified time. This 
function must take no parameters and return nothing.
 *
 * @return errnoVal (ErrnoVal) .
 */
  .atMicroseconds = (void*)(0x002040c5),

/**
 * @brief Similar to ``Timer.at()`` except the given time is in ticks instead of milliseconds. 
However, the granularity is bound to the XTAL (typically 55.2 MHz). Only a single event 
(including Timer.atMicroseconds()) can be scheduled at a time. .
 *
 * @param timeTicks (uint64_t) Amount of time in ticks (running at 63.8976 GHz, thus 15.65 
picoseconds per tick).
[timeTicks values]:
  ASAP                   To indicate that an action must be executed as soon as possible.
 * @param function (void (*isr)(void)) The function that is called at the specified time. This 
function must take no parameters and return nothing.
 *
 * @return errnoVal (ErrnoVal) .
 */
  .atTicks = (void*)(0x00204171),

/**
 * @brief To read the number of currently scheduled using the Timer.at... function(s).
 *
 * @return count (uint8_t) Number of scheduled items.
 */
  .atCount = (void*)(0x00203e65),

/**
 * @brief Schedules a call to the given function at a given constant interval, expressed in 
milliseconds.
 *
 * @param timeMs (uint32_t) Amount of time in milliseconds. Units are [ms].
 * @param function (void (*isr)(void)) The function that is called at the specified time. This 
function must take no parameters and return nothing.
 */
  .every = (void*)(0x0020424d),

/**
 * @brief Similar to ``Timer.every()`` except the given time is in microseconds instead of 
milliseconds.
 *
 * @param timeUs (uint32_t) Amount of time in milliseconds. Units are [us].
 * @param function (void (*isr)(void)) The function that is called at the specified time. This 
function must take no parameters and return nothing.
 */
  .everyMicroseconds = (void*)(0x0020421d),

/**
 * @brief Stops and resets the global timer class.
 */
  .end = (void*)(0x002042a9),

/**
 * @brief Clears the currently scheduled using the Timer.at... function(s).
 */
  .clearOneShots = (void*)(0x00204259),

/**
 * @brief Clears the currently scheduled using the Timer.every... function(s).
 */
  .clearPeriodic = (void*)(0x00204281),
};


/**
 * @brief Sets a function to be called when a specific interrupt occurs. This can be a GPIO pin 
that changes state, for example to detect a button press, or the reception of bytes over UART.
 *
 * @param source (uint16_t) Specifies the type of interrupt, which could be for example a 
specific GPIO pin, or the reception on a UWB message. For example, ``attachInterrupt( 
PIN_USER_BUTTON), isr, RISING)`` calls isr() every time pin 10 goes from low to high. Note that 
for a given source only one isr() can be attached. Thus, it is not possible to have both 
FALLING and RISING interrupts for the same GPIO pin.
[source values]:
  ERROR_INTERRUPT        The provided callback function will be executed every time an error occurred during the execution of a non-UWB API function.
  GPIO_INTERRUPT         The ISR will be executed every time a GPIO interrupt is generated, irrespective of the pin number. To add a pin to the pool of interrupt pins, use ``attachInterrupt( PIN_USER_BUTTON, pin_isr, mode)``. If in the latter case pin_isr is set to NULL, only the isr() of GPIO_INTERRUPT is executed. If not NULL, pin_isr() is executed before the isr() of ``GPIO_INTERRUPT``.
  SERIAL_INTERRUPT       The ISR will be executed every time byte(s) areavailable from the Serial port.
  SPI_TRANSFER_COMPLETE  The ISR will be executed every time a SPI transfer was successful. That is, the predetermined number of bytes have been transfered (see length parameter). Note that when this interrupt is active, the ``SPI.transfer()`` will not be a blocking call anymore.
  SPI_TRANSFER_ASSERT    The ISR will be executed every time a SPI transfer is started because the host asserted the Slave Select line (only applicable when in SPI slave mode).
  SPI_TRANSFER_DEASSERT  The ISR will be executed every time a SPI transfer is stopped because the host deasserted the Slave Select line (only applicable when in SPI slave mode).
  UWB_TX_DONE_MARKER     Triggers when TX has completed.
  UWB_RX_SFD_MARKER      Triggers when the Start of Frame Delimiter (SFD) is received. THE SFD is positioned between the SYNC (preamble) part and the PHR part. The SFD part is used for time-stamping which is useful for ranging.
  UWB_RX_PHR_MARKER      Triggers when the PHR is received. PHR stands for Physical Layer Header. It provides information to the receiver about the transmitted payload and other useful physical layer information.
  UWB_RX_START_MARKER    Triggers when RX has started.
  UWB_RX_EOF_MARKER      Triggers when the End of Frame (EOF) is detected.
  UWB_VBUF_LOW_INTERRUPT Triggers when the the voltage of VBUF is too low. Adjust current limiter or add capacitance to resolve this.
  SERIAL_WRITE_COMPLETE  The ISR will be executed every time a asynchronous TX was successful.
  UWB_TX_INTERRUPT       The ISR will be executed every time TX result(s) are ready to be read out. Equivalent to UWB.onTX(myEvent, NULL).
  UWB_RX_INTERRUPT       The ISR will be executed every time RX result(s) are ready to be read out. Equivalent to UWB.onRX(myEvent, NULL, NULL...).
  UWB_LOOPBACK_INTERRUPT The ISR will be executed every time loopback result(s) are ready to be read out.
 * @param function (void (*isr)(void)) The ISR (the interrupt service routine) is called when 
the interrupt occurs. This function must take no parameters and return nothing.
 * @param mode (int8_t) Defines when the interrupt should be triggered. For example on the 
RISING edge of a GPIO pin.
[mode values]:
  FALLING                Trigger when the pin goes from high to low. This applies to GPIO interrupts only.
  RISING                 Trigger when the pin goes from low to high. This applies to GPIO interrupts only.
 *
 * @return errnoVal (ErrnoVal) .
 */
const ErrnoVal (*attachInterrupt)(uint16_t source, void (*isr)(void), int8_t mode) = (void*)(0x00202d45);

/**
 * @brief Turn the given interrupt off.
 *
 * @param source (uint16_t) Specifies the type of interrupt, which could be for example a 
specific GPIO pin, or the reception of a UWB message.
 *
 * @return errnoVal (ErrnoVal) .
 */
const ErrnoVal (*detachInterrupt)(uint16_t source) = (void*)(0x00203029);

/**
 * @brief Disables interrupts until interrupts() is called. Interrupts can slightly disrupt the 
timing of code and may be disabled for particularly critical sections of code.
 *
 * @return counter (uint32_t) Number of stacked noInterrupt calls. This number must be passed 
to interrupts() for reactivation.
 */
const uint32_t (*noInterrupts)(void) = (void*)(0x00202ba9);

/**
 * @brief Re-enables interrupts after they've been disabled by noInterrupts(). Interrupts are 
enabled by default.
 *
 * @param counter (uint32_t) Value that was last returned by noInterrupts().
 *
 * @return counter (uint32_t) Number of stacked noInterrupt calls. This number must be passed 
to interrupts() for reactivation.
 */
const uint32_t (*interrupts)(uint32_t counter) = (void*)(0x00202b9d);

/**
 * @brief Blocks the user application (like ``delay()``) until ``triggerEvent()`` is called 
from an interrupt service routine. This can be used for example to wait in your ``loop()`` 
until an interrupt GPIO pin is triggered, or a UWB packet is received.
 *
 * @param timeMs (uint32_t) As a safety mechanism, when ``triggerEvent()`` is not called within 
the given timeout, the function returns 0x0 to indicate a timeout has occurred. A timeout value 
of 0 means that no timeout is used. Units are [ms].
 *
 * @return eventMask (uint32_t) WaitForEvent() returns the ``eventMask`` value that is passed 
to the ``triggerEvent()`` function. Returns 0x0 when a timeout has occurred. In case 
``triggerEvent()`` is called multiple times before ``waitForEvent()``, the resulting 
``eventMask`` returned by ``waitForEvent()`` will be the or-ed total.
 */
const uint32_t (*waitForEvent)(uint32_t timeMs) = (void*)(0x00203245);

/**
 * @brief See waitForEvents().
 *
 * @param eventMask (uint32_t) The provided event mask will be returned by the pending 
``waitForEvent()`` function. In case ``triggerEvent()`` is called multiple times before 
``waitForEvent()``, the resulting ``eventMask`` returned by ``waitForEvent()`` will be the 
or-ed total. The value(s) should be between 0x1 and 0xFFFFFFFF. The default value is '0x0'.
[eventMask values]:
  RX_DONE                Arbitrarily chosen value to code the completion of a RX event.
  TX_DONE                Arbitrarily chosen value to code the completion of a TX event.
  EVENT_TIMEOUT          
 */
const void (*triggerEvent)(uint32_t eventMask) = (void*)(0x00202bbd);

/**
 * @brief Like, triggerEvent() but faster. .
 *
 * @param eventBitIndex (uint8_t) The bit corresponding with the provided zero-based index will 
be returned by the pending ``waitForEvent()`` function. The value(s) should be between 0 and 
31.
 */
const void (*triggerEventBit)(uint8_t eventBitIndex) = (void*)(0x002031b5);

/**
 * @brief Returns the number of times the specified bit index was triggered.
 *
 * @param eventBitIndex (uint8_t) To select the event mask bit of interrest. The index is 
zero-based, e.g. index=1 is to count triggerEvent(0x2) occurrences. The value(s) should be 
between 0 and 31.
 *
 * @return eventCount (uint32_t) .
 */
const uint32_t (*countEvent)(uint8_t eventBitIndex) = (void*)(0x002031c9);

/**
 * @brief Clear pending event bits before a waitForEvent() is called. This way you make sure 
waitForEvent() will not immediately return when events were triggered prior.
 *
 * @param eventMask (uint32_t)  The value(s) should be between 0x1 and 0xFFFFFFFF. The default 
value is '0x0'.
 */
const void (*clearEvent)(uint32_t eventMask) = (void*)(0x00203215);

/**
 * @brief Reads the current event mask value.
 *
 * @return eventMask (uint32_t) .
 */
const uint32_t (*readEvents)(void) = (void*)(0x002031d5);

const Serial_t Serial = {

/**
 * @brief Sets the data rate in bits per second (baud) for serial data transmission and 
configures the designated RX/TX pins.
 *
 * @param baudRate (uint32_t) Specify the rate at which bits are transmitted. Standard baud 
rates include 110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 128000 
and 256000 bits per second. The default value is '115200'.
 */
  .begin = (void*)(0x00204b41),

/**
 * @brief Get the number of bytes (characters) available for reading from the serial port. This 
is data that's already arrived and stored in the serial receive buffer (which can hold up to 
256 bytes).
 *
 * @return numberOfBytes (uint16_t)  The default value is '0'.
 */
  .available = (void*)(0x00204931),

/**
 * @brief Returns the first byte of incoming serial data available (or -1 if no data is 
available).
 *
 * @return byte (int16_t)  The default value is '-1'.
 */
  .read = (void*)(0x00204949),

/**
 * @brief Reads characters from the serial port into a buffer. The function terminates if the 
determined length has been read or a timeout has occurred. ``Serial.readBytes()`` returns the 
number of characters placed in the buffer. A 0 means no valid data was found.
 *
 * @param buffer (uint8_t*) The buffer to store the bytes in.
 * @param length (uint16_t) The number of bytes to read. The value(s) should be between 1 and 
256.
 *
 * @return bytesRead (uint16_t) The number of bytes successfully read.
 */
  .readBytes = (void*)(0x00204985),

/**
 * @brief Writes binary data to the serial port.
 *
 * @param buffer (const uint8_t*) The buffer containing the bytes to send.
 * @param length (uint16_t) The number of bytes to send. The value(s) should be between 1 and 
256.
 *
 * @return errnoVal (ErrnoVal) .
 */
  .write = (void*)(0x00204d01),

/**
 * @brief Returns whether UART is currently busy with transmission. Catch SERIAL_WRITE_COMPLETE 
interrupt.
 *
 * @return isBusy (bool) UART TX is happening.
 */
  .busyWriting = (void*)(0x00204c4d),

/**
 * @brief Sets the maximum milliseconds to wait for serial data.
 *
 * @param timeMs (uint32_t)  Units are [ms]. The default value is '100'.
 */
  .setTimeout = (void*)(0x00204979),

/**
 * @brief Prints the given byte array as a HEX table.
 *
 * @param buffer (void*) The buffer to print.
 * @param length (uint16_t) The number of bytes to print.
 * @param columns (uint8_t) The number of columns in the table (typically 16).
 */
  .hexDump = (void*)(0x002049fd),

/**
 * @brief Prints data to the serial port as human-readable ASCII text. The C library function 
``int printf(const char *format, ...)`` is used here to send formatted output to stdout. Note 
that output stays in the UART FIFO buffer until a newline character is encountered. With a 
newline character all the bytes in the UART buffer are sent on the wire (UART flush). Also a 
buffer overflow will trigger a UART flush.
 *
 * @return characterCount (int) Total number of characters printed.
 */
  .print = (void*)(0x0020ca79),

/**
 * @brief The C library function ``int sprintf(char *str, const char *format, ...)`` is used 
here. The C Library sprintf() function allows you to create strings with specified formats, 
similar to printf(), but instead of printing to the standard output, it stores the resulting 
string in a character array provided by the user.
 *
 * @return characterCount (int) The number of characters written to the string str, excluding 
the null-terminating character.
 */
  .sprint = (void*)(0x0020cd09),

/**
 * @brief Prints 64-bit value with decimal notation. Note that the regular ``Serial.print()`` 
has an upper limit of 32-bit values, hence this function. .
 *
 * @param value (int64_t) Number to be printed, for example a 64-bit timestamp value.
 * @param buffer (const uint8_t*) Pointer to where the output should be written to. In case of 
0 (NULL), the output will be sent over UART directly.
 *
 * @return characterCount (int) Total number of characters written.
 */
  .printInt64 = (void*)(0x00204d1d),

/**
 * @brief Prints the latest errno change, or ASSERT violation.
 */
  .printError = (void*)(0x00204a41),

/**
 * @brief Disables serial communication, allowing the RX and TX pins to be used for general 
input and output. To re-enable serial communication, call ``Serial.begin()``. Note that when 
``Serial.end()`` is called right after ``Serial.print(str)``, there is a chance that str is 
printed only partially.
 */
  .end = (void*)(0x002048f9),
};

const SPI_t SPI = {

/**
 * @brief Initializes the SCK, MISO, and MOSI pins.
 */
  .begin = (void*)(0x00203355),

/**
 * @brief Initializes the SPI bus using the defined SPISettings. An SPISettings object is used 
to configure the SPI port for your SPI device. All three parameters are combined to a single 
SPISettings object using ``SPISettings( speedMaximum, bitOrder, dataMode)``. For example, 
``SPI.beginTransaction( SPISettings(500000, MSBFIRST, SPI_MODE0))``.
 *
 * @param settings (SPISettings) Use macro ``SPISettings(clock, bitOrder, dataMode)``.
[settings values]:
  LSBFIRST               BitOrder: Sets the order of the bits shifted out of and into the SPI bus to least-significant bit first (default).
  MSBFIRST               BitOrder: Sets the order of the bits shifted out of and into the SPI bus to the most-significant bit first.
  SPI_CPOL               DataMode: CPOL determines the polarity of the clock. If CPOL is enabled, the clock's leading edge is a falling edge, and the trailing edge is a rising edge.
  SPI_CPHA               DataMode: CPHA determines the timing (i.e., phase) of the data bits relative to the clock pulses. When CPHA is enabled, data is sampled on the falling edge and shifted out on the rising edge.
  SPI_MODE0              DataMode: Clock leading edge is rising (CPOL=0) and data are sampled on the leading (first, rising) clock edge (CPHA=0).
  SPI_MODE1              DataMode: Clock leading edge is rising (CPOL=0) and data are sampled on the trailing (second, falling) clock edge (CPHA=1).
  SPI_MODE2              DataMode: Clock leading edge is falling (CPOL=1) and data are sampled on the leading (first, falling) clock edge (CPHA=0).
  SPI_MODE3              DataMode: Clock leading edge is falling (CPOL=1) and data are sampled on the trailing (second, rising) clock edge (CPHA=1).
  SPI_CS1                DataMode: Use CS1 pin for slave select.
  SPI_CS2                DataMode: Use CS2 pin for slave select.
  SPI_SLAVE_MODE         Clock: Sets clock speed to 0 to use SPI in slave mode (in which the clock speed is determined by the master device).
  SS_ACTIVE_LOW          DataMode: Chip Select (or Slave select) polarity selection. Active low is the default.
  SS_ACTIVE_HIGH         DataMode: Chip Select (or Slave select) polarity selection.
  SS_HOLD_ACTIVE         DataMode: By default, the Chip Select gets deactivated when a ``SPI.transfer()`` is finished. However, by setting this flag, the Chip Select is kept in the active state until ``SPI.endTransaction()`` is called.
 */
  .beginTransaction = (void*)(0x00203531),

/**
 * @brief Start receive.
 *
 * @param rxBuffer (uint8_t*) Pointer to buffer for incoming data.
 * @param rxLength (uint16_t) Length of data to be received (number of bytes).
 */
  .read = (void*)(0x002037ed),

/**
 * @brief Start receive.
 *
 * @param txBuffer (uint8_t*) Pointer to buffer for outgoing data.
 * @param txLength (uint16_t) Length of data to be written (number of bytes).
 */
  .write = (void*)(0x002037f7),

/**
 * @brief Start simultaneous send and receive.
 *
 * @param txBuffer (uint8_t*) Pointer to buffer for outgoing data.
 * @param rxBuffer (uint8_t*) Pointer to buffer for incoming data.
 * @param length (uint16_t) Length of data to be transmitted/received (number of bytes).
 *
 * @return errnoVal (ErrnoVal) .
 */
  .transfer = (void*)(0x0020363d),

/**
 * @brief Returns the number of bytes received/transmitted during to the current 
``SPI.transfer``.
 *
 * @return byteCount (uint16_t) The number of bytes transferred since ``SPI.transfer`` was 
called.
 */
  .count = (void*)(0x002035f5),

/**
 * @brief ``SPI.beginTransaction()`` behaves like a locking mechanism to gain the exclusive use 
of the SPI bus, and therefore requires ``SPI.endTransaction()`` to release the bus for others 
to access it.
 */
  .endTransaction = (void*)(0x00203a29),

/**
 * @brief Stop using the SPI bus as a master.
 */
  .end = (void*)(0x00203a3d),
};

const I2C_t I2C = {

/**
 * @brief Initializes the module as a (software-) I2C master.
 *
 * @param sdaPinMask (uint16_t) The pin identifier mask for the SDA (data) pin. Because this is 
a software-I2C implementation, any free GPIO pin can be used.
 * @param sclPinMask (uint16_t) The pin identifier mask for the SCL (clock) pin. Because this 
is a software-I2C implementation, any free GPIO pin can be used.
 */
  .begin = (void*)(0x00203a91),

/**
 * @brief Write the provided byte array to the target peripheral device.
 *
 * @param address (uint8_t) The 7-bit address of the slave.
 * @param data (uint8_t*) Byte to send.
 * @param length (uint16_t) Size of the data array. Units are [bytes].
 * @param stop (bool) Indicates whether to send a stop bit at the end of the write (typically 
set to TRUE).
 *
 * @return result (int8_t) Indicates whether the target peripheral device responded with an 
``ACK`` (0), ``NACK`` (-1), or whether a ``TIMEOUT`` (-2) occurred. A timeout typically occurs 
when a peripheral device holds the clock line low for too long.
 */
  .write = (void*)(0x00203c85),

/**
 * @brief To request bytes from a peripheral device.
 *
 * @param address (uint8_t) The 7-bit address of the slave.
 * @param data (uint8_t*) Buffer to store the received bytes.
 * @param length (uint16_t) How many bytes we want to receive.
 * @param stop (bool) True will send a stop message after the request, releasing the bus. False 
will continually send a restart after the request, keeping the connection active.
 *
 * @return result (int16_t) Returns the number of successfully received bytes. In case of an 
error or timeout, the value will be negative.
 */
  .read = (void*)(0x00203db9),

/**
 * @brief Deinitializs I2C.
 */
  .end = (void*)(0x00203abd),
};


/**
 * @brief The random function generates pseudo-random numbers. Returns a random number between 
min and max-1.
 *
 * @param min (uint32_t) Lower bound of the random value, inclusive (optional).
 * @param max (uint32_t) Upper bound of the random value, exclusive.
 *
 * @return number (uint32_t) .
 */
const uint32_t (*getRandom)(uint32_t min, uint32_t max) = (void*)(0x00203281);

/**
 * @brief Initializes the pseudo-random number generator, causing it to start at an arbitrary 
point in its random sequence.
 *
 * @param seed (uint32_t) Number to initialize the pseudo-random sequence. Use ``randomSeed()`` 
to initialize the random number generator with a fairly random input, such as a timestamp, 
``analogRead()`` on an unconnected pin.
 */
const void (*randomSeed)(uint32_t seed) = (void*)(0x002032a5);

/**
 * @brief Fast integer square root algorithm with arithmetic rounding of the result. That is, 
if the real answer would have a fractional part of 0.5 or greater, the result is rounded up to 
the next integer.
 *
 * @param value (uint32_t) Unsigned integer for which to find the square root.
 *
 * @return number (uint32_t) Integer square root of the input value.
 */
const uint32_t (*usqrt)(uint32_t value) = (void*)(0x002032a9);

/**
 * @brief Returns unique device serial number.
 *
 * @return serialNumber (uint32_t) Serial number.
 */
const uint32_t (*getSerialNumber)(void) = (void*)(0x002032d5);

/**
 * @brief Returns the current total crc of the user application.
 *
 * @return checksum (uint32_t) Checksum.
 */
const uint32_t (*getCurrentCrc)(void) = (void*)(0x002032e9);

/**
 * @brief Returns the stored antenna delay, measured during production tests.
 *
 * @return delay (uint32_t) Antenna delay in ticks.
 */
const uint32_t (*getAntennaDelay)(void) = (void*)(0x0020330d);

/**
 * @brief Returns the harware identifier.
 *
 * @return id (uint32_t) Harware identifier.
 */
const uint32_t (*getHardwareId)(void) = (void*)(0x00203331);

const Crypto_t Crypto = {

/**
 * @brief Powers up the minimal hardware blocks required to use the cryptographic subsystem.
 *
 * @return errnoVal (ErrnoVal) .
 */
  .begin = (void*)(0x00202301),

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
  .randomBytes = (void*)(0x00202345),

/**
 * @brief A cyclic redundancy check (CRC) is an error-detecting code commonly used in digital 
networks and storage devices to detect accidental changes to digital data. There are many 
variants, but they all use polynomial division. With the ``configCrc()`` function, the 
parameters can be set in order to obtain a specific variant. The order of parameters here 
matches most online CRC calculators, for example at www.crccalc.com.
 *
 * @param poly (uint32_t) CRC polynomial value. The default value is '0x04C11DB7'.
 * @param init (uint32_t) CRC initial value. The default value is '0xFFFFFFFF'.
 * @param reflect (uint8_t) If 0 (false), MSB first is used as an CRC input data bit order. If 
1, bits are swapped around its center and the output is swapped back. The default value is '1'.
 * @param xorOut (uint32_t) The value to be XORed with the final register value. The default 
value is '0xFFFFFFFF'.
 */
  .configCrc = (void*)(0x002025e9),

/**
 * @brief Calculate a CRC-32 checksum with given parameters over an application-visible 
(source) memory region.
 *
 * @param source (uint8_t*) Location of data.
 * @param length (uint32_t) Length of the input data in bytes.
 *
 * @return result (uint32_t) The output CRC value.
 */
  .calcCrc = (void*)(0x00202615),

/**
 * @brief Provide the parameters of the encryption operation, including the encryption key, the 
mode of operation, and the initialization value (if needed).
 *
 * @param keyValue (uint8_t*) Raw user-provided key.
 * @param keyLength (uint8_t) Length of the provided key value (number of bytes).
 * @param algorithm (uint16_t) The underlying cryptographic algorithm.
[algorithm values]:
  CONFIG_ENCRYPTION_ALGORITHM_AES_128 Algorithm identifier of the AES-128 algorithm.
  CONFIG_ENCRYPTION_ALGORITHM_AES_256 Algorithm identifier of the AES-256 algorithm.
  CONFIG_ENCRYPTION_ALGORITHM_ECC_NIST_P256 Algorithm identifier of the ECC algorithm over NIST P-256 curve.
  CONFIG_ENCRYPTION_ALGORITHM_ECC_NIST_P384 Algorithm identifier of the ECC algorithm over NIST P-384 curve.
  CONFIG_ENCRYPTION_ALGORITHM_ECC_NIST_P521 Algorithm identifier of the ECC algorithm over NIST P-521 curve.
 * @param ivValue (uint8_t*) Initialization Vector (IV). The interpretation of this field 
depends on the mode of operation selected using the mode field. For cipher modes that do not 
use an IV (MODE_ECB) this field must be set as a zero sized input data buffer, with NULL data 
pointer.  The data is not copied to a buffer, so make sure the pointer still points to valid 
data at the time Crypto.encrypt() is executed.
 * @param ivLength (uint8_t) Length of the provided IV value (number of bytes).
 * @param cipherMode (uint8_t) Mode of operation for the cipher. This field selects the mode of 
operation to be used by cipher. The interpretation of the iv field depends on the selected mode 
of operation.
[cipherMode values]:
  CONFIG_ENCRYPTION_CIPHER_MODE_ECB Electronic Codebook Mode (ECB). ECB mode encryption and decryption is performed without any cipher padding. It is the sole responsibility of the user to implement padding (if needed). For this mode of operation the size of the plaintext and the size of the ciphertext are always the same. This mode can only process ciphertext and/or plaintext sizes that are an even multiple of the underlying cipher's block size.
  CONFIG_ENCRYPTION_CIPHER_MODE_CTR Counter Mode (CTR). CTR mode encryption and decryption is performed without any cipher padding. For this mode of operation the size of the plaintext buffer and the size of the ciphertext buffer are always the same. For this mode of operation the size of the ciphertext and/or plaintext can be an arbitrary byte size; it does not have to be an even multiple of the underlying cipher's block size. @note This mode of operation requires an initial nonce value for the counter. The nonce value is passed via the IV field of the cipher parameter structure.
  CONFIG_ENCRYPTION_CIPHER_MODE_CBC Cipher Block Chaining Mode (CBC) with PKCS#7 padding. For this mode of operation the size of the ciphertext buffer can be up to one cipher block larger than the size of the plaintext buffer. The implementation automatically applies/removes PKCS#7 padding from the plaintext/ciphertext. There are no restrictions on the size of the (unpadded) plaintext; the ciphertext size is always an even multiple of the underlying block cipher's size. @note This mode of operation requires an Initialization Vector (IV) of exactly one cipher block. @note When decrypting PKCS#7 padded ciphertexts particular precautions have to be taken to avoid inadvertent creation of padding oracle vulnerabilities. Ciphertexts that have been encrypted with PKCS#7 padding should ONLY be decrypted AFTER they have been authenticated.
  CONFIG_ENCRYPTION_CIPHER_MODE_CCM Counter-Mode with CBC-MAC (CCM). For this mode of operation the size of the plaintext buffer and the size of the ciphertext buffer are always the same. For this mode of operation the size of the ciphertext and/or plaintext can be an arbitrary byte size; it does not have to be an even multiple of the underlying cipher's block size. @note This mode of operation requires an initial nonce value for the counter. The nonce value is passed via the IV field of the cipher parameter structure. @note This mode of operation produces/uses an authentication tag. The size of the authentication tag can be any of 4, 6, 8, 10, 12, 14, or 16 bytes.
  CONFIG_ENCRYPTION_CIPHER_MODE_CBC_NO_PADDING Cipher Block Chaining Mode (CBC) without padding. CBC mode encryption and decryption is performed without any cipher padding. It is the sole responsibility of the user to implement padding (if needed). For this mode of operation the size of the plaintext and the size of the ciphertext are always the same. This mode can only process ciphertext and/or plaintext sizes that are an even multiple of the underlying cipher's block size. @note This mode of operation requires an Initialization Vector (IV) of exactly one cipher block.
  CONFIG_ENCRYPTION_CIPHER_MODE_EAX EAX mode (encrypt-then-authenticate-then-translate) is a mode of operation for cryptographic block ciphers. It is an Authenticated Encryption with Associated Data (AEAD) algorithm designed to simultaneously provide both authentication and privacy of the message (authenticated encryption) with a two-pass scheme, one pass for achieving privacy and one for authenticity for each block.
 * @param aadValue (uint8_t*) Additional Authenticated Data (AAD). The interpretation of this 
field depends on the mode of operation selected using the mode field. It is only used by 
authenticated encryption (AE) cipher modes. This field must be set as a zero size input data 
buffer, with NULL data pointer, if an unauthenticated cipher mode (eg. MODE_ECB, MODE_CTR, 
MODE_CBC) is used. The data is not copied to a buffer, so make sure the pointer still points to 
valid data at the time Crypto.encrypt() is executed.
 * @param aadLength (uint8_t) Length of the provided AAD value (number of bytes).
 *
 * @return errnoVal (ErrnoVal) .
 */
  .configEncryption = (void*)(0x002023a1),

/**
 * @brief Provide the parameters of the decryption operation, including the encryption key, the 
mode of operation, and the initialization value (if needed).
 *
 * @param keyValue (uint8_t*) Raw user-provided key.
 * @param keyLength (uint8_t) Length of the provided key value (number of bytes).
 * @param algorithm (uint16_t) The underlying cryptographic algorithm.
[algorithm values]:
  CONFIG_DECRYPTION_ALGORITHM_AES_128 Algorithm identifier of the AES-128 algorithm.
  CONFIG_DECRYPTION_ALGORITHM_AES_256 Algorithm identifier of the AES-256 algorithm.
  CONFIG_DECRYPTION_ALGORITHM_ECC_NIST_P256 Algorithm identifier of the ECC algorithm over NIST P-256 curve.
  CONFIG_DECRYPTION_ALGORITHM_ECC_NIST_P384 Algorithm identifier of the ECC algorithm over NIST P-384 curve.
  CONFIG_DECRYPTION_ALGORITHM_ECC_NIST_P521 Algorithm identifier of the ECC algorithm over NIST P-521 curve.
 * @param ivValue (uint8_t*) Initialization Vector (IV). The interpretation of this field 
depends on the mode of operation selected using the mode field. For cipher modes that do not 
use an IV (MODE_ECB) this field must be set as a zero sized input data buffer, with NULL data 
pointer.  The data is not copied to a buffer, so make sure the pointer still points to valid 
data at the time Crypto.encrypt() is executed.
 * @param ivLength (uint8_t) Length of the provided IV value (number of bytes).
 * @param cipherMode (uint8_t) Mode of operation for the cipher. This field selects the mode of 
operation to be used by cipher. The interpretation of the iv field depends on the selected mode 
of operation.
[cipherMode values]:
  CONFIG_DECRYPTION_CIPHER_MODE_ECB Electronic Codebook Mode (ECB). ECB mode encryption and decryption is performed without any cipher padding. It is the sole responsibility of the user to implement padding (if needed). For this mode of operation the size of the plaintext and the size of the ciphertext are always the same. This mode can only process ciphertext and/or plaintext sizes that are an even multiple of the underlying cipher's block size.
  CONFIG_DECRYPTION_CIPHER_MODE_CTR Counter Mode (CTR). CTR mode encryption and decryption is performed without any cipher padding. For this mode of operation the size of the plaintext buffer and the size of the ciphertext buffer are always the same. For this mode of operation the size of the ciphertext and/or plaintext can be an arbitrary byte size; it does not have to be an even multiple of the underlying cipher's block size. @note This mode of operation requires an initial nonce value for the counter. The nonce value is passed via the IV field of the cipher parameter structure.
  CONFIG_DECRYPTION_CIPHER_MODE_CBC Cipher Block Chaining Mode (CBC) with PKCS#7 padding. For this mode of operation the size of the ciphertext buffer can be up to one cipher block larger than the size of the plaintext buffer. The implementation automatically applies/removes PKCS#7 padding from the plaintext/ciphertext. There are no restrictions on the size of the (unpadded) plaintext; the ciphertext size is always an even multiple of the underlying block cipher's size. @note This mode of operation requires an Initialization Vector (IV) of exactly one cipher block. @note When decrypting PKCS#7 padded ciphertexts particular precautions have to be taken to avoid inadvertent creation of padding oracle vulnerabilities. Ciphertexts that have been encrypted with PKCS#7 padding should ONLY be decrypted AFTER they have been authenticated.
  CONFIG_DECRYPTION_CIPHER_MODE_CCM Counter-Mode with CBC-MAC (CCM). For this mode of operation the size of the plaintext buffer and the size of the ciphertext buffer are always the same. For this mode of operation the size of the ciphertext and/or plaintext can be an arbitrary byte size; it does not have to be an even multiple of the underlying cipher's block size. @note This mode of operation requires an initial nonce value for the counter. The nonce value is passed via the IV field of the cipher parameter structure. @note This mode of operation produces/uses an authentication tag. The size of the authentication tag can be any of 4, 6, 8, 10, 12, 14, or 16 bytes.
  CONFIG_DECRYPTION_CIPHER_MODE_CBC_NO_PADDING Cipher Block Chaining Mode (CBC) without padding. CBC mode encryption and decryption is performed without any cipher padding. It is the sole responsibility of the user to implement padding (if needed). For this mode of operation the size of the plaintext and the size of the ciphertext are always the same. This mode can only process ciphertext and/or plaintext sizes that are an even multiple of the underlying cipher's block size. @note This mode of operation requires an Initialization Vector (IV) of exactly one cipher block.
  CONFIG_DECRYPTION_CIPHER_MODE_EAX EAX mode (encrypt-then-authenticate-then-translate) is a mode of operation for cryptographic block ciphers. It is an Authenticated Encryption with Associated Data (AEAD) algorithm designed to simultaneously provide both authentication and privacy of the message (authenticated encryption) with a two-pass scheme, one pass for achieving privacy and one for authenticity for each block.
 * @param aadValue (uint8_t*) Additional Authenticated Data (AAD). The interpretation of this 
field depends on the mode of operation selected using the mode field. It is only used by 
authenticated encryption (AE) cipher modes. This field must be set as a zero size input data 
buffer, with NULL data pointer, if an unauthenticated cipher mode (eg. MODE_ECB, MODE_CTR, 
MODE_CBC) is used. The data is not copied to a buffer, so make sure the pointer still points to 
valid data at the time Crypto.encrypt() is executed.
 * @param aadLength (uint8_t) Length of the provided AAD value (number of bytes).
 *
 * @return errnoVal (ErrnoVal) .
 */
  .configDecryption = (void*)(0x002024bd),

/**
 * @brief Encrypts a contiguous region of memory.
 *
 * @param source (uint8_t*) Location of data to encrypt.
 * @param length (uint16_t) Length of the input data in bytes.
 * @param destination (uint8_t*) Location where the encrypted data should be written to.
 * @param authenticationTag (uint32_t*) Output for the encrypted authentication tag generated 
by authenticated encryption modes (cipher mode CCM only). This parameter is only required for 
authenticated encryption modes, and may be omitted (NULL) otherwise.
 *
 * @return errnoVal (ErrnoVal) .
 */
  .encrypt = (void*)(0x0020243d),

/**
 * @brief Decrypts a contiguous region of memory.
 *
 * @param source (uint8_t*) Location of data to decrypt.
 * @param length (uint16_t) Length of the input data in bytes.
 * @param destination (uint8_t*) Location where the decrypted data should be written to.
 * @param authenticationTag (uint32_t*) The encrypted authentication tag generated by 
authenticated encryption modes (cipher mode CCM only). This parameter is only required for 
authenticated encryption modes, and may be omitted (NULL) otherwise.
 *
 * @return errnoVal (ErrnoVal) .
 */
  .decrypt = (void*)(0x00202559),

/**
 * @brief Powers down hardware blocks that are required by the cryptographic subsystem.
 */
  .end = (void*)(0x0020269d),
};

const EEPROM_t EEPROM = {

/**
 * @brief Reads a byte from the EEPROM.
 *
 * @param address (uint16_t) The location to read from, starting from 0.
 *
 * @return value (uint8_t) The value stored in that location (byte).
 */
  .read = (void*)(0x00204e3d),

/**
 * @brief Reads a byte from the EEPROM.
 *
 * @param address (uint16_t) The location to read from, starting from 0.
 * @param value (uint8_t) The value to be stored in that location (byte).
 *
 * @return errnoVal (ErrnoVal) .
 */
  .write = (void*)(0x00204eb5),

/**
 * @brief Reads bytes from the EEPROM.
 *
 * @param buffer (uint8_t*) The buffer to store the bytes in.
 * @param address (uint16_t) The location to read from, starting from 0.
 * @param length (uint16_t) The number of bytes to read. The value(s) should be between 1 and 
1023.
 *
 * @return errnoVal (ErrnoVal) .
 */
  .readBytes = (void*)(0x00204dcd),

/**
 * @brief Writes bytes to the EEPROM.
 *
 * @param buffer (uint8_t*) The buffer to store the bytes in.
 * @param address (uint16_t) The location to read from, starting from 0.
 * @param length (uint16_t) The number of bytes to read. The value(s) should be between 1 and 
1023.
 *
 * @return errnoVal (ErrnoVal) .
 */
  .writeBytes = (void*)(0x00204e55),
};


/**
 * @brief Returns pointer to current UWB timestamp, which runs at 128*499.2 ticks per 
microsecond. This number will overflow (go back to zero) after approximately 9 years.
 *
 * @return count (uint64_t*) .
 */
const uint64_t* (*ticks)(void) = (void*)(0x00205775);

/**
 * @brief Define payload data and payload length as separate function arguments. See also 
UWB.setPayload().
 *
 * @param buffer (uint8_t*) The buffer to store the bytes in.
 * @param length (uint16_t) The number of bytes to read. The value(s) should be between 1 and 
1023.
 *
 * @return errnoVal (UciStatus) .
 */
const UciStatus (*setPayload)(uint8_t* buffer, uint16_t length) = (void*)(0x002057d9);
