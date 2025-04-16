#include "BQ25120.h"
#include "user-application.h"
#include "I2C_helpers.h"


#define ADDRESS_BQ25120F3A (0x6A)
#define BQ25_STATUS (0X00)
#define BQ25_FAULTS (0x01)
#define BQ25_FAULTS_MASK (0x02)
#define BQ25_FAST_CHARGE_CONTROL (0x03)
#define BQ25_VOLTAGE_CONTROL (0x05)
#define BQ25_SYS_VOUT (0x06)
#define BQ25_LS_CONTROL (0x07)
#define BQ25_ILIM (0x09)
#define BQ25_BATTERY_MONITOR (0x0A)


#define BQ25_PUSH_BUTTON_CONTROL (0x08)

I2C_FUN(BQ25, ADDRESS_BQ25120F3A);


uint8_t BQ25_getStatus(void) {
    
    return BQ25_read(BQ25_STATUS);
}


uint8_t BQ25_getChargeStatus(void) {
  
  uint8_t status = BQ25_getStatus();
  return status >> 6;

}


uint8_t BQ25_getFaults(void) {
    return BQ25_read(BQ25_FAULTS);
}


uint8_t BQ25_getFaultsMask(void) {
    return BQ25_read(BQ25_FAULTS_MASK);
}


bool BQ25_ready() {
    return (I2C.write(ADDRESS_BQ25120F3A, 0, 0, true) == WRITE_RESULT_ACK);
}


bool BQ25_hasDefaultVout() {
    uint8_t vout = BQ25_read(BQ25_SYS_VOUT);
    return vout == 0xAA;
}


uint8_t BQ25_getChargeControl(void) {
    return BQ25_read(BQ25_FAST_CHARGE_CONTROL);
}

int8_t BQ25_setFaultsMask1(bool mask_vin_overvoltage, 
    bool mask_vin_undervoltage, bool bat_uvlo, bool bat_ocp) {
    uint8_t result = 0;
    if(mask_vin_overvoltage) {
        result |= B3;
    }
    if(mask_vin_undervoltage) {
        result |= B2;
    }
    if(bat_uvlo) {
        result |= B1;
    }
    if(bat_ocp) {
        result |= B0;
    }
    return BQ25_write(BQ25_FAULTS, result);
}

int8_t BQ25_setFaultsMask(bool enable_TS, bool enable_charge_INT) {
    uint8_t result = 0;
    if (enable_TS) {
        result |= B7;
    }
    if (enable_charge_INT) {
        result |= B3;
    }
    
    return BQ25_write(BQ25_FAULTS_MASK, result);
}


int8_t BQ25_setChargeControl(uint16_t ChargeCurrent_mA, bool disableI2C) {
    // disableI2C: When the HZ_MODE bit is written by the host, the I2C interface is disabled if only battery is present. To resume I2C, the CD pin must be toggled.
    uint8_t result = disableI2C;
    if (ChargeCurrent_mA < 40) {
        if (ChargeCurrent_mA > 31) {
            ChargeCurrent_mA = 31;
        }
    } else {
        result |= B7;
        ChargeCurrent_mA /= 10;
        if (ChargeCurrent_mA > 31) {
            ChargeCurrent_mA = 31;
        }
    }
    result |= (ChargeCurrent_mA << 2);
    BQ25_write(BQ25_FAST_CHARGE_CONTROL, result);
}


int8_t BQ25_setBatteryVoltage(uint16_t regulation_mV) {
    BOUND_AND_MAP_RANGE(regulation_mV, 3600, 4650, 10);
    uint8_t result = regulation_mV << 1;
    return BQ25_write(BQ25_VOLTAGE_CONTROL, result);
}


int8_t BQ25_setSysVoltage(uint16_t output_mV) {
    uint8_t result = B5 | B6;
    if (output_mV > 0) {
        result |= B7;
    }

    BOUND_AND_MAP_RANGE(output_mV, 1800, 3300, 100);
    result |= (output_mV << 1);

    return BQ25_write(BQ25_SYS_VOUT, result);
}


int8_t BQ25_setLoadSwitchVoltage(uint16_t output_mV) {
    uint8_t result = 0;
    if (output_mV > 0) {
        result |= B7;
    }

    BOUND_AND_MAP_RANGE(output_mV, 800, 3300, 100);
    result |= output_mV << 2;

    return BQ25_write(BQ25_LS_CONTROL, result);
}


int8_t BQ25_setLimit(uint16_t inputCurrent_mA, uint16_t voltageLow_mV) {
    uint8_t result = 0;
    BOUND_RANGE(inputCurrent_mA, 50, 400);

    BOUND_AND_MAP_RANGE(inputCurrent_mA, 50, 400, 50);
    result |= inputCurrent_mA << 3;

    BOUND_AND_MAP_RANGE(voltageLow_mV, 2000, 3000, 200);
    result |= voltageLow_mV ;
    
    return BQ25_write(BQ25_ILIM, result);
}


void BQ25_toggle_CD(uint16_t cd_pin) {
    pinMode(cd_pin, OUTPUT_LOW);
    delayMicroseconds(10);
    digitalWrite(cd_pin, HIGH);
}


uint8_t BQ25_getRawBatteryStatus() {
    return BQ25_read(BQ25_BATTERY_MONITOR);
}


uint8_t BQ25_getBatteryStatus() {
    BQ25_write(BQ25_BATTERY_MONITOR, B7);
    delay(1);
    uint8_t r = BQ25_read(BQ25_BATTERY_MONITOR);
    uint8_t range = (r & (B5 | B6)) >> 5;
    uint8_t frac = (r & (B2 | B3)) >> 2;
    uint8_t result = 60 + range * 10 + frac * 2;
    return result;
}


uint8_t BQ25_getPushButtonControl(void) {
    return BQ25_read(BQ25_PUSH_BUTTON_CONTROL);
}

int8_t BQ25_setPushButtonControl(uint8_t val) {

    return BQ25_write(BQ25_PUSH_BUTTON_CONTROL, val);
}
