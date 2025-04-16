

#ifndef BQ25120_H
#define BQ25120_H

// Battery Charger
// BQ25120F3A
// 0x6A

#include <stdint.h>
#include <stdbool.h>

#define BQ25_CHARGE_READY (0)
#define BQ25_CHARGE_PROGRESS (1)
#define BQ25_CHARGE_DONE (2)
#define BQ25_CHARGE_FAULT (3)



bool BQ25_ready();

bool BQ25_hasDefaultVout();

void BQ25_toggle_CD(uint16_t cd_pin);

uint8_t BQ25_getStatus(void);

uint8_t BQ25_getChargeStatus(void);

uint8_t BQ25_getFaults(void);

uint8_t BQ25_getFaultsMask(void);


int8_t BQ25_setFaultsMask1(bool mask_vin_overvoltage, bool mask_vin_undervoltage, bool bat_uvlo, bool bat_ocp);

int8_t BQ25_setFaultsMask(bool enable_TS, bool enable_charge_INT);

uint8_t BQ25_getChargeControl(void);

int8_t BQ25_setChargeControl(uint16_t ChargeCurrent_mA, bool disableI2C);

int8_t BQ25_setBatteryVoltage(uint16_t regulation_mV);

int8_t BQ25_setSysVoltage(uint16_t output_mV);

int8_t BQ25_setLoadSwitchVoltage(uint16_t output_mV);

int8_t BQ25_setLimit(uint16_t inputCurrent_mA, uint16_t voltageLow_mV);

uint8_t BQ25_getBatteryStatus();

uint8_t BQ25_getRawBatteryStatus();


uint8_t BQ25_getPushButtonControl(void);
int8_t BQ25_setPushButtonControl(uint8_t val);


#endif // BQ25120_H