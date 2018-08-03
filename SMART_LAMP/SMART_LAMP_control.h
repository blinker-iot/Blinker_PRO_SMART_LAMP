#ifndef BLINKER_LAMP_CONTROL_H
#define BLINKER_LAMP_CONTROL_H

#include "SMART_LAMP_config.h"

void attachDelay(callback_with_uint32_arg_t newFunc);

void lampDelay(uint32_t ms);

void setBrightness(uint8_t bright);

void modeChange();

void rainbowDisplay2();

void rainbowDisplay1();

void ledInit();

void ledRun();

#endif