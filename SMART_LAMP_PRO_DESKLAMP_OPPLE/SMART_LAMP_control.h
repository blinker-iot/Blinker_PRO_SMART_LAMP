#ifndef BLINKER_LAMP_CONTROL_H
#define BLINKER_LAMP_CONTROL_H

#include "SMART_LAMP_config.h"

// void attachDelay(callback_with_uint32_arg_t newFunc);

// void lampDelay(uint32_t ms);
void resetDisplay(uint16_t _time);

void setColor(uint32_t color);

void setStandard(uint32_t color);

void setBreath(uint32_t color);

void setStrobe(uint32_t color);

uint32_t kelvin2RGB(uint8_t set);

uint16_t getColorTemp();

uint8_t getTempSet();

void setSunlight(uint32_t color);

// void setGradient(uint8_t num, uint32_t color);

void setGradient(uint32_t *color, uint8_t count);

void setBrightness(uint8_t bright);

uint8_t getBrightness();

uint32_t getPixelColor();

String getColor();

String getMode();

uint8_t getModeType();

void setSpeed(uint8_t speed);

uint8_t getSpeed();

void modeChange();

void rainbow();

void rainbowCycle();

void setLampMode(uint8_t lamp_mode, bool state = true);

void ledInit();

void ledRun();

// void touchInit();

// void touchTick();

#endif