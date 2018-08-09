#ifndef BLINKER_LAMP_CONTROL_H
#define BLINKER_LAMP_CONTROL_H

#include "SMART_LAMP_config.h"

void attachDelay(callback_with_uint32_arg_t newFunc);

void lampDelay(uint32_t ms);

void setBrightness(uint8_t bright);

void modeChange();

void rainbow();

void rainbowCycle();

void setMode(uint8_t lamp_mode);

void ledInit();

void ledRun();

#endif