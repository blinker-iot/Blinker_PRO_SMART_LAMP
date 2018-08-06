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
static uint32_t lampStep = 0;
static uint32_t lampSpeed = BLINKER_LAMP_SPEED_DEFUALT;
static uint32_t freshStart = 0;

static uint32_t stream_color[3] = {0xff0000, 0x00ff00, 0x0000ff};
static uint8_t stream_num = 0;
static uint8_t stream_count = 3;

static uint32_t standard_color = 0x88ff0088;

static callback_with_uint32_arg_t _lampDelay = NULL;

bool needFresh()
{
    yield();

    if (lampSpeed < 256) lampSpeed = 256;

    if ((millis() - freshStart) >= (lampSpeed / 256)) {
        freshStart = millis();
        return true;
    }
    else {
        return false;
    }
}

void lampFresh(uint32_t ms)
{
    ms = ms * BLINKER_WS2812_COUNT / 256;
    
    // while(!needFresh());

    if (_lampDelay) {
        _lampDelay(ms);
    }
    else {
        delay(ms);
    }
}

uint32_t Wheel(byte WheelPos)
{
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

void colorWipe(uint32_t c, uint8_t wait)
{
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        // lampFresh(wait);
    }
}

void rainbow()
{
    for(uint8_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i + lampStep) & 255));
    }
    lampStep = (lampStep + 1) & 0xFF;

    strip.show();
    // lampFresh(lampSpeed);
}

void rainbowCycle() {
    for(uint8_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + lampStep) & 255));
    }
    lampStep = (lampStep + 1) & 0xFF;

    strip.show();
    // lampFresh(lampSpeed);
}

// uint32_t streamer(uint32_t c) {
uint8_t streamer() {
    if (lampStep == 0) stream_num = (stream_num + 1) % stream_count;

    int lum = lampStep;

    uint16_t start_lum = 64;
    uint16_t end_lum = 448;

    uint32_t c = stream_color[stream_num];

    uint8_t now_r = (c >> 16 & 0xFF);
    uint8_t now_g = (c >>  8 & 0xFF);
    uint8_t now_b = (c       & 0xFF);

    uint8_t next_num = (stream_num + 1) % stream_count;

    if (lum < end_lum) next_num = (stream_num + stream_count - 1) % stream_count;

    uint32_t next_c = stream_color[next_num];

    uint8_t next_r = (next_c >> 16 & 0xFF);
    uint8_t next_g = (next_c >>  8 & 0xFF);
    uint8_t next_b = (next_c       & 0xFF);

    uint8_t r, g, b;

    if (lum < start_lum) {
        r = next_r;
        g = next_g;
        b = next_b;
    }
    else if (lum >= start_lum && lum <= end_lum){
        r = (next_r - (next_r - now_r) * (lum - start_lum) / (end_lum - start_lum));
        g = (next_g - (next_g - now_g) * (lum - start_lum) / (end_lum - start_lum));
        b = (next_b - (next_b - now_b) * (lum - start_lum) / (end_lum - start_lum));
    }
    else {
        r = now_r;
        g = now_g;
        b = now_b;
    }

    uint16_t _delay = 5;

    if (lum < start_lum || lum > end_lum) _delay = 20;

    for(uint8_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, r, g, b);
    }

    strip.show();

    lampStep += 2;
    if(lampStep > 512) lampStep = 0;

    return _delay;
}

void standard()
{
    uint8_t lum = (standard_color >> 24 & 0xFF);
    uint8_t r   = (standard_color >> 16 & 0xFF) * lum / 256;
    uint8_t g   = (standard_color >>  8 & 0xFF) * lum / 256;
    uint8_t b   = (standard_color       & 0xFF) * lum / 256;

    for(uint8_t i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, r, g, b);
    }

    strip.show();
}

uint16_t breath()
{
    int lum = lampStep;
    if(lum > 255) lum = 511 - lum; // lum = 15 -> 255 -> 15

    uint16_t _delay;
    if(lum == 15) _delay = 970; // 970 pause before each breath
    else if(lum <=  25 && lum > 15) _delay = 38; // 19
    else if(lum <=  50 && lum > 25) _delay = 36; // 18
    else if(lum <=  75 && lum > 50) _delay = 28; // 14
    else if(lum <= 100 && lum > 75) _delay = 20; // 10
    else if(lum <= 125 && lum > 100) _delay = 14; // 7
    else if(lum <= 150 && lum > 125) _delay = 11; // 5
    else _delay = 10; // 4

    // uint32_t color = SEGMENT.colors[0];
    // uint8_t w = (standard_color >> 24 & 0xFF) * lum / 256;
    uint8_t r = (standard_color >> 16 & 0xFF) * lum / 256;
    uint8_t g = (standard_color >>  8 & 0xFF) * lum / 256;
    uint8_t b = (standard_color       & 0xFF) * lum / 256;
    for(uint16_t i = 0; i <= strip.numPixels(); i++) {
        strip.setPixelColor(i, r, g, b);
    }

    strip.show();

    lampStep += 2;
    if(lampStep > (512-15)) lampStep = 15;

    return _delay;
}

uint16_t rainbowStrobe()
{
    uint32_t c;
    uint16_t _delay;
    if (lampStep % 2) {
        // c = Wheel(lampStep);
        c = Wheel(random(0, 255));
        _delay = 20;
    }
    else {
        c = 0;
        _delay = 1000;
    }

    if (lampStep == 255) lampStep = 0;
    else lampStep++;
    
    uint8_t r = (c >> 16 & 0xFF);
    uint8_t g = (c >>  8 & 0xFF);
    uint8_t b = (c       & 0xFF);
    
    for(uint16_t i = 0; i <= strip.numPixels(); i++) {
        strip.setPixelColor(i, r, g, b);
    }

    strip.show();

    return _delay;
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
    lampStep = 0;

    Serial.println(lampType);
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
    
    if (!needFresh()) return;

    switch(lampType) {
        case BLINKER_LAMP_RAINBOW_CYCLE :
            rainbowCycle();
            break;
        case BLINKER_LAMP_RAINBOW :
            rainbow();
            break;
        case BLINKER_LAMP_RAINBOW_STROBE :
            lampSpeed = rainbowStrobe() * 256;
            break;
        case BLINKER_LAMP_STANDARD :
            standard();
            break;
        case BLINKER_LAMP_BREATH :
            lampSpeed = breath() * 256;
            break;
        case BLINKER_LAMP_STREAMER :
            lampSpeed = streamer() * 256;
            break;
        default :
            break;
    }
}