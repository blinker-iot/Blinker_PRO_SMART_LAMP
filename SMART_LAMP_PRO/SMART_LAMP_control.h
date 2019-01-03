#ifndef BLINKER_LAMP_CONTROL_H
#define BLINKER_LAMP_CONTROL_H

#include "SMART_LAMP_config.h"

// void attachDelay(callback_with_uint32_arg_t newFunc);

// void lampDelay(uint32_t ms);
void resetDisplay(uint16_t _time);

void setStandard(uint32_t color);

void setStreamer(uint8_t num, uint32_t color);

void setStreamer(uint32_t *color);

void setBrightness(uint8_t bright);

void setSpeed(uint8_t speed);

void modeChange();

void rainbow();

void rainbowCycle();

void setLampMode(uint8_t lamp_mode, bool state = true);

void ledInit();

void ledRun();

// void touchInit();

// void touchTick();

#endif