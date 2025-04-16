#include "user-application.h"
#include "pinout.h"

#include "BQ25120.h"
#include "BQ51222.h"

#include "LIS3DSH.h"

#include "led-animations.h"

#include <stdint.h>


// ============= [ APPLICATION SETTINGS ] =============

#define LOW_BATTERY_VALUE (76)
#define ANTENNA_DELAY (13914)

#define TOO_HOT_TEMPERATURE (50)
#define TX_POWER (33)


// ============= [  VARIABLES  ] =============


bool qPowered = false;
uint8_t chargeStatus;
uint16_t qVoltage;
uint16_t qPower;

uint8_t batteryMonitor;
uint32_t jerk;

PayloadNtf rxPacket;

#define EVENT_START_OTAU (1<<8)


// ============= [    ] =============

void configure_uwb(void) {  
    SetConfigurationCmd config;    
    // Alternatively, use UWB.getConfiguration(&config):
    SetConfiguration_setDefaults(&config); // fill in defaults
    config.txPower = TX_POWER;
    ASSERT_0(UWB.setConfiguration(&config));
    ASSERT_0(UWB.begin());
}


void performOtauTask() {
    GET_OTAU();
    delay(100);
    reset();
}


void checkForOtauOrSettings() {    
  if (IS_OTAU(&rxPacket)) {
      triggerEvent(EVENT_START_OTAU);
      return;
  }
}


void performChargingTask() {
  Timer.setWatchdog(10000);
  configure_uwb();

  int temperature = analogRead(INT_TEMPERATURE);

  uint32_t lastReadTemperature = 0;
  uint32_t lastReadOtau = 0;
  bool tooHot = (temperature/1000) >= TOO_HOT_TEMPERATURE;

  while (tooHot  || chargeStatus != BQ25_CHARGE_READY) {

    
    uint32_t t = millis();
    if (t -lastReadTemperature > 2000 ) {
      lastReadTemperature = t;
      temperature = analogRead(INT_TEMPERATURE);
      Serial.print("Temperature = %i\n", temperature);
      tooHot = (temperature/1000) >= TOO_HOT_TEMPERATURE;

      digitalWrite(PIN_PWR_CD, tooHot);
      if (tooHot) {
        led_showNotification(PIN_LED_RED);
        Serial.print("Too Hot\n");
      }

      qVoltage = BQ51_getVoltage();
      qPower = BQ51_getPower();
      Serial.print("qVoltage = %u mV\n", qVoltage);
      Serial.print("qPower = %u mW\n", qPower);
    }
    if (t -lastReadOtau > 5000 ) {
      lastReadOtau = t;
      UWB.onRxUpdate( checkForOtauOrSettings, NULL, &rxPacket, 1020, NULL, NULL );
      ASSERT_0(UWB.startContinuousRx(ASAP));
      Timer.at(millis() + 120, (void*)UWB.stopAction);
      Serial.print("Check for OTAU/Settings\n");
    }


    switch (chargeStatus)  {
      case BQ25_CHARGE_FAULT:    { led_showStrobo(1000, 100, PIN_LED_RED);  }; break;
      case BQ25_CHARGE_PROGRESS: { led_showBreathing(1500, PIN_LED_RED); }; break;
      case BQ25_CHARGE_DONE:     { led_showStatic(500, PIN_LED_GREEN); }; break;      
    }

    SWITCH_EVENT(1, eventMask) {
        CASE(EVENT_START_OTAU): {performOtauTask();}
    }
    
    chargeStatus = BQ25_getChargeStatus();
    Timer.kickWatchdog();
    
  }
  Serial.print("Proceeding\n");
  delay(100);
   UWB.end();
  Timer.setWatchdog(1000);
}

void configure_power(void);


void wait_for_communication() {
  qPowered = BQ51_ready();
  if (BQ25_ready() == false) {
    Serial.print("Could not communicate with BQ25, wait for QCharger\n");
    Timer.setWatchdog(1000);
    while (true) {
      if (BQ51_ready()) {
        qPowered = true;
        Timer.kickWatchdog();
        break;
      }
      delay(200);
      // Timer.kickWatchdog();
    }
    delay(500);
    if (BQ25_ready()) {
      Serial.print("Success!\n");
      Timer.kickWatchdog();
    } else {
      reset();
    }
  } 
  Timer.setWatchdog(0);
}



void configure_power() {
  ASSERT_0(BQ25_setFaultsMask1(false, true, false, false));
  ASSERT_0(BQ25_setFaultsMask(false, true));
  ASSERT_0(BQ25_setSysVoltage(2500));
  ASSERT_0(BQ25_setBatteryVoltage(4200));
  ASSERT_0(BQ25_setChargeControl(160, false));
  ASSERT_0(BQ25_setLoadSwitchVoltage(3000));
  ASSERT_0(BQ25_setLimit(250, 3000));
  ASSERT_0(BQ25_setPushButtonControl((1<<2)));
}


void configure_task() {
  wait_for_communication();
  configure_power();  
  pinMode(PIN_LED_RED, OUTPUT_HIGH);
  pinMode(PIN_LED_GREEN, OUTPUT_HIGH);
  batteryMonitor = BQ25_getBatteryStatus();  
  chargeStatus = BQ25_getChargeStatus();
}


void initialize_tag() {
    uint8_t resetReason = getResetReason();  
    if (resetReason == RESET_PIN) {
      Serial.print("\nFSM10-wearable application - Compile date: " __DATE__ "\n");
      Serial.print("Device 0x%X\n", getSerialNumber());
    }
  pinMode(PIN_PWR_INT, INPUT_PULLUP);
  I2C.begin(PIN_I2C_SDA, PIN_I2C_SCL);  
  pinMode(PIN_PWR_CD, OUTPUT_HIGH);

  // wait for battery charger I2C
  qPowered = BQ51_ready();
  if (qPowered) {
    qVoltage = BQ51_getVoltage();
    qPower = BQ51_getPower();
    // enable charging when VIN is valid:
    digitalWrite(PIN_PWR_CD, LOW);
  }
  
  jerk = LIS3_getJerk(configure_task);

    if (qPowered) {
      Serial.print("qVoltage = %u mV\n", qVoltage);
      Serial.print("qPower = %u mW\n", qPower);
    }

    // 3.31V -> 76% LOW
    // 3.34V -> 76%
    // 3.73 -> 86%
    // 3.86 -> 92%
    // 4.0 -> 94%
    // 4.21 -> 96% HIGH
    Serial.print("batteryMonitor = %u\n", batteryMonitor);  
    // Serial.print("BQ25_getChargeStatus = 0x%x\n", chargeStatus);
    Serial.print("jerk = %u\n", jerk);

  if ((chargeStatus != BQ25_CHARGE_READY)) {
    performChargingTask();
  } 
}



bool led_tagWakeup() {
  uint16_t gpio = batteryMonitor <= LOW_BATTERY_VALUE ? PIN_LED_RED : PIN_LED_GREEN;
  digitalWrite(gpio, LOW);
  return false;
}



void perform_tag_task() {
    Serial.print("tag task\n");
    Timer.setWatchdog(500);
    delay(50);

    sleep(millis() + 1000, PIN_CS1, FALLING);
  
}

            
// ============= [  SETUP  ] =============

void setup() {

  // Start UART at 115200 baud rate
  Serial.begin(115200);
  attachInterrupt(ERROR_EVENT, Serial.printError, 0);

  uint32_t hardwareId = getHardwareId();
  uint8_t resetReason = getResetReason();
  // Serial.print("hardwareId = 0x%x\n", hardwareId);
  if ((resetReason != DEVICE_STATE_REASON_WATCHDOG && resetReason != DEVICE_STATE_REASON_WUP_TIMER)) {
    Serial.print("reset reason = 0x%x\n", resetReason);
  } 

  if (hardwareId == 0xF) {
      Timer.setWatchdog(50);

      initialize_tag();
      led_tagWakeup();
      configure_uwb();
      perform_tag_task();
      UWB.end();
    } else {
    Serial.print("Unknown Hardware ID (0x%X)\n", hardwareId);
  }
}
