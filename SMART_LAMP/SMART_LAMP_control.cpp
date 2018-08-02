#include "SMART_LAMP_control.h"

#include <WS2812FX.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(BLINKER_WS2812_COUNT, BLINKER_WS2812_PIN, NEO_RGB + NEO_KHZ800);

static bool change = false;

void setBrightness(uint8 bright)
{
    ws2812fx.setBrightness(bright);
}

void modeChange()
{
    (change ? rainbowDisplay1() : rainbowDisplay2());

    change = !change;
}

void rainbowDisplay2()
{
    ws2812fx.setBrightness(255);
    ws2812fx.setSpeed(5000);
    ws2812fx.setColor(0x007BFF);
    ws2812fx.setMode(FX_MODE_RAINBOW);
}

void rainbowDisplay1()
{
    ws2812fx.setBrightness(255);
    ws2812fx.setSpeed(1000);
    ws2812fx.setColor(0x007BFF);
    ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
}

void ledInit()
{
    ws2812fx.init();
    rainbowDisplay1();
    ws2812fx.start();
}

void ledRun()
{
    ws2812fx.service();
}