#ifndef BLINKER_LAMP_CONTROL_H
#define BLINKER_LAMP_CONTROL_H

#include "SMART_LAMP_config.h"

void setBrightness(uint8_t bright);

void modeChange();

void rainbowDisplay2();

void rainbowDisplay1();

void ledInit();

void ledRun();

#endif