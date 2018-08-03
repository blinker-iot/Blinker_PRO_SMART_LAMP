#include "SMART_LAMP_control.h"

// #include <WS2812FX.h>

// // Parameter 1 = number of pixels in strip
// // Parameter 2 = Arduino pin number (most are valid)
// // Parameter 3 = pixel type flags, add together as needed:
// //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
// //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
// //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
// //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
// //   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
// WS2812FX ws2812fx = WS2812FX(BLINKER_WS2812_COUNT, BLINKER_WS2812_PIN, NEO_RGB + NEO_KHZ800);


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 4

#define BLINKER_POWER_3V3_PIN 14
#define BLINKER_POWER_5V_PIN 15

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(BLINKER_WS2812_COUNT, BLINKER_WS2812_PIN, NEO_GRB + NEO_KHZ800);

static bool change = false;
static uint8_t lampType = BLINKER_LAMP_RAINBOW_CYCLE;
static uint8_t lampSpeed = 2000UL;
static uint8_t lampStep = 0;

static callback_with_uint32_arg_t _lampDelay = NULL;

void lampFresh(uint32_t ms)
{
    ms = ms / strip.numPixels();
    
    if (_lampDelay) {
        _lampDelay(ms);
    }
    else {
        delay(ms);
    }
}

uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void colorWipe(uint32_t c, uint8_t wait) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        lampFresh(wait);
    }
}

void rainbow() {
    for(uint8_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i + lampStep) & 255));
    }
    lampStep = lampStep + 1 & 0xFF;

    strip.show();
    lampFresh(lampSpeed);
}

void rainbowCycle() {
    for(uint8_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + lampStep) & 255));
    }
    lampStep = lampStep + 1 & 0xFF;

    strip.show();
    lampFresh(lampSpeed);
}

void attachDelay(callback_with_uint32_arg_t newFunc)
{
    _lampDelay = newFunc;
}

void setBrightness(uint8_t bright)
{
    // ws2812fx.setBrightness(bright);

    strip.setBrightness(bright);
}

void modeChange()
{
    // (change ? rainbowDisplay1() : rainbowDisplay2());

    // change = !change;

    lampType = (lampType + 1) % BLINKER_LAMP_TYPE_COUNT;
}

void rainbowDisplay2()
{
    // ws2812fx.setBrightness(255);
    // ws2812fx.setSpeed(5000);
    // ws2812fx.setColor(0x007BFF);
    // ws2812fx.setMode(FX_MODE_RAINBOW);

    lampType = BLINKER_LAMP_RAINBOW;
}

void rainbowDisplay1()
{
    // ws2812fx.setBrightness(255);
    // ws2812fx.setSpeed(1000);
    // ws2812fx.setColor(0x007BFF);
    // ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);

    lampType = BLINKER_LAMP_RAINBOW_CYCLE;
}

void ledInit()
{
    // ws2812fx.init();
    // rainbowDisplay1();
    // ws2812fx.start();

    strip.begin();
    strip.show();
    strip.setBrightness(255);
}

void ledRun()
{
    // ws2812fx.service();

    // (change ? rainbowDisplay1() : rainbowDisplay2());

    // rainbow(20);
    // colorWipe(strip.Color(255, 0, 0), 50);

    switch(lampType) {
        case BLINKER_LAMP_RAINBOW :
            rainbow();
            break;
        case BLINKER_LAMP_RAINBOW_CYCLE :
            rainbowCycle();
            break;
        default :
            break;
    }
}