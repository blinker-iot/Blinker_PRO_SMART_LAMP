#ifndef BLINKER_LAMP_CONTROL_H
#define BLINKER_LAMP_CONTROL_H

#include "SMART_LAMP_config.h"

void attachDelay(callback_with_uint32_arg_t newFunc);

void lampDelay(uint32_t ms);

void setStreamer(uint8_t num, uint32_t color);

void setStreamer(uint32_t *color);

void setBrightness(uint8_t bright);

void setSpeed(uint32_t speed);

void modeChange();

void rainbow();

void rainbowCycle();

void setLampMode(uint8_t lamp_mode);

void ledInit();

void ledRun();

#endif