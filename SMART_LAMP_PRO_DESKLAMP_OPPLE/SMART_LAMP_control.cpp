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


// #include <Adafruit_NeoPixel.h>
// #ifdef __AVR__
//   #include <avr/power.h>
// #endif

// #define PIN 4

// #define BLINKER_POWER_3V3_PIN 14
// #define BLINKER_POWER_5V_PIN 15

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(BLINKER_WS2812_COUNT, BLINKER_WS2812_PIN, NEO_GRB + NEO_KHZ800);

// #include "OneButton.h"

// OneButton buttonAdd(BLINKER_BUTTON_ADD_PIN, true);
// OneButton buttonSub(BLINKER_BUTTON_SUB_PIN, true);
// OneButton buttonEC0(BLINKER_BUTTON_EC0_PIN, true);
// OneButton buttonEC1(BLINKER_BUTTON_EC1_PIN, true);

static bool     change = false;
static bool     lamp_state = true;
static uint8_t  lampType = BLINKER_LAMP_RAINBOW_CYCLE;
static uint32_t lampStep = 0;
static uint32_t lampSpeed = 100;//BLINKER_LAMP_SPEED_DEFUALT;
static uint32_t _lampSpeed = 100;//BLINKER_LAMP_SPEED_DEFUALT;
static uint32_t freshStart = 0;

static uint32_t gradient_color[BLINKER_MAX_GRADIENT_COUNT] = {0xff0000, 0x00ff00};
static uint8_t  stream_num = 0;
static uint8_t  stream_count = 2;
static uint32_t gradient_color_standard = 0;

static uint32_t standard_color = 0x88ff0088;
static uint32_t sunlight_color = BLINKER_COLOR_TEMP_MIN;
static uint32_t breath_color = 0xffffdbba;
static uint32_t strobe_color = 0x88ff0088;

static uint32_t latest_color = 0;
static uint32_t now_color = 0;
static bool     isGraded = true;

static uint32_t latest_brt = 0;
static uint32_t now_brt = 0;
static bool     isBrt = true;
static uint32_t brtStep = 0;
static uint32_t brtTime = 0;
static uint8_t  brtLumi = 255;

static bool     old_ec0;
static bool     old_ec1;
static bool     now_ec0;
static bool     now_ec1;
static bool     ec0check;

static uint8_t  night_led = 0;
static uint8_t  white_led = 0;
static uint8_t  yellow_led = 0;
static uint8_t  bright_set = 0;
static uint8_t  temp_set = 0;

// static callback_with_uint32_arg_t _lampDelay = NULL;

void setLampBright(uint8_t bright)
{
    bright_set = bright;
} 

void lampShow()
{
    analogWrite(BLINKER_LAMP_NIGHT_LED_PIN, night_led * bright_set / 255);
    analogWrite(BLINKER_LAMP_WHITE_LED_PIN, white_led * bright_set / 255);
    analogWrite(BLINKER_LAMP_YELLOW_LED_PIN, yellow_led * bright_set / 255);
}

void lampColor(uint32_t _c)
{
    night_led = _c >> 16 & 0xFF;
    white_led = _c >>  8 & 0xFF;
    yellow_led = _c       & 0xFF;
}

void lampColor(uint8_t _n, uint8_t _w, uint8_t _y)
{
    night_led = _n;
    white_led = _w;
    yellow_led = _y;
}

bool needFresh()
{
    // strip.show();

    if (_lampSpeed < 1) _lampSpeed = 1;

    // yield();

    if ((millis() - freshStart) >= _lampSpeed) {
        freshStart = millis();
        return true;
    }
    else {
        return false;
    }
}

// void lampFresh(uint32_t ms)
// {
//     ms = ms * BLINKER_WS2812_COUNT / 256;
    
//     // while(!needFresh());

//     if (_lampDelay) {
//         _lampDelay(ms);
//     }
//     else {
//         delay(ms);
//     }
// }

// uint32_t Wheel(byte WheelPos)
// {
//     WheelPos = 255 - WheelPos;
//     if(WheelPos < 85) {
//         return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//     }
//     if(WheelPos < 170) {
//         WheelPos -= 85;
//         return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//     }
//     WheelPos -= 170;
//     return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
// }

// void colorWipe(uint32_t c, uint8_t wait)
// {
//     for(uint16_t i=0; i<strip.numPixels(); i++) {
//         strip.setPixelColor(i, c);
//     }
//     strip.show();
//     // lampFresh(wait);
// }

// void rainbowDisplay()
// {
//     for(uint8_t i=0; i < strip.numPixels(); i++) {
//         strip.setPixelColor(i, Wheel((i + lampStep) & 255));
//     }
//     lampStep = (lampStep + 1) & 0xFF;

//     strip.show();
//     // lampFresh(lampSpeed);
// }

// void rainbowCycleDisplay() {
//     for(uint8_t i=0; i < strip.numPixels(); i++) {
//         strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + lampStep) & 255));
//     }
//     lampStep = (lampStep + 1) & 0xFF;

//     strip.show();
//     // lampFresh(lampSpeed);
// }

uint32_t colorGradient()
{
    int lum = lampStep;

    uint8_t latest_n = (latest_color >> 16 & 0xFF);
    uint8_t latest_w = (latest_color >>  8 & 0xFF);
    uint8_t latest_y = (latest_color       & 0xFF);

    uint8_t now_n = (now_color >> 16 & 0xFF);
    uint8_t now_w = (now_color >>  8 & 0xFF);
    uint8_t now_y = (now_color       & 0xFF);

    uint8_t n = (latest_n + (now_n - latest_n) * (lum) / 256);
    uint8_t w = (latest_w + (now_w - latest_w) * (lum) / 256);
    uint8_t y = (latest_y + (now_y - latest_y) * (lum) / 256);

    uint32_t _delay = 2;

    // for(uint8_t i=0; i < strip.numPixels(); i++) {
    //     strip.setPixelColor(i, r, g, b);
    // }

    // strip.show();

    lampColor(n, w, y);

    lampShow();

    lampStep += 1;
    if(lampStep > 256) 
    {
        isGraded = true;
        lampStep = 256;
    }

    return _delay;
}

// uint32_t streamer(uint32_t c) {
// uint32_t streamer() {
//     // if (!lamp_state) {
//     //     uint8_t r   = (gradient_color_standard >> 16 & 0xFF);
//     //     uint8_t g   = (gradient_color_standard >>  8 & 0xFF);
//     //     uint8_t b   = (gradient_color_standard       & 0xFF);

//     //     for(uint8_t i=0; i < strip.numPixels(); i++) {
//     //         strip.setPixelColor(i, r, g, b);
//     //     }

//     //     strip.show();

//     //     return 0;
//     // }

//     if (lampStep == 0) stream_num = (stream_num + 1) % stream_count;

//     int lum = lampStep;

//     uint16_t start_lum = 64;
//     uint16_t end_lum = 448;

//     uint32_t c = gradient_color[stream_num];

//     uint8_t now_r = (c >> 16 & 0xFF);
//     uint8_t now_g = (c >>  8 & 0xFF);
//     uint8_t now_b = (c       & 0xFF);

//     uint8_t next_num = (stream_num + 1) % stream_count;

//     if (lum < end_lum) next_num = (stream_num + stream_count - 1) % stream_count;

//     uint32_t next_c = gradient_color[next_num];

//     uint8_t next_r = (next_c >> 16 & 0xFF);
//     uint8_t next_g = (next_c >>  8 & 0xFF);
//     uint8_t next_b = (next_c       & 0xFF);

//     uint8_t r, g, b;

//     if (lum < start_lum) {
//         r = next_r;
//         g = next_g;
//         b = next_b;
//     }
//     else if (lum >= start_lum && lum <= end_lum){
//         r = (next_r - (next_r - now_r) * (lum - start_lum) / (end_lum - start_lum));
//         g = (next_g - (next_g - now_g) * (lum - start_lum) / (end_lum - start_lum));
//         b = (next_b - (next_b - now_b) * (lum - start_lum) / (end_lum - start_lum));
//     }
//     else {
//         r = now_r;
//         g = now_g;
//         b = now_b;
//     }

//     uint32_t _delay = lampSpeed * 2 / 5;

//     if (lum < start_lum || lum > end_lum) _delay = lampSpeed;// * 8 / 5;

//     for(uint8_t i=0; i < strip.numPixels(); i++) {
//         strip.setPixelColor(i, r, g, b);
//     }

//     strip.show();

//     lampStep += 2;
//     if(lampStep > 512) lampStep = 0;

//     return _delay;
// }

uint32_t standard()
{
//     if (!isGraded) return colorGradient();

//     uint8_t lum = (standard_color >> 24 & 0xFF);
//     uint8_t r   = (standard_color >> 16 & 0xFF);
//     uint8_t g   = (standard_color >>  8 & 0xFF);
//     uint8_t b   = (standard_color       & 0xFF);

//     for(uint8_t i=0; i < strip.numPixels(); i++) {
//         strip.setPixelColor(i, r, g, b);
//     }

//     strip.show();

    lampColor(0, 255, 0);

    lampShow();

    return 0;
}

uint32_t sunlight()
{
    if (!isGraded) return colorGradient();

    // uint8_t lum = (sunlight_color >> 24 & 0xFF);
    uint8_t n   = (sunlight_color >> 16 & 0xFF);
    uint8_t w   = (sunlight_color >>  8 & 0xFF);
    uint8_t y   = (sunlight_color       & 0xFF);

    // for(uint8_t i=0; i < strip.numPixels(); i++) {
    //     strip.setPixelColor(i, r, g, b);
    // }

    // strip.show();

    lampColor(n, w, y);

    lampShow();

    return 0;
}

// uint32_t breath()
// {
//     if (!isGraded) return colorGradient();

//     int lum = lampStep;
//     if(lum > 255) lum = 511 - lum; // lum = 15 -> 255 -> 15

//     uint32_t _delay;
//     if(lum == 15) _delay = lampSpeed * 6 / 10;//50; // 970 pause before each breath
//     else if(lum <=  25 && lum > 15)  _delay = lampSpeed * 6 / 10;// * 2;//38; // 19
//     else if(lum <=  50 && lum > 25)  _delay = lampSpeed * 6 / 10;// * 2;//36; // 18
//     else if(lum <=  75 && lum > 50)  _delay = lampSpeed * 4 / 10;// * 2;//28; // 14
//     else if(lum <= 100 && lum > 75)  _delay = lampSpeed * 3 / 10;//20; // 10
//     else if(lum <= 125 && lum > 100) _delay = lampSpeed * 2 / 10;//14; // 7
//     else if(lum <= 150 && lum > 125) _delay = lampSpeed * 1.5 / 10;//11; // 5
//     else _delay = lampSpeed * 1 / 10;//10; // 4

//     // uint32_t color = SEGMENT.colors[0];
//     // uint8_t w = (breath_color >> 24 & 0xFF) * lum / 256;
//     uint8_t r = (breath_color >> 16 & 0xFF);// * lum / 256;
//     uint8_t g = (breath_color >>  8 & 0xFF);// * lum / 256;
//     uint8_t b = (breath_color       & 0xFF);// * lum / 256;
//     for(uint16_t i = 0; i <= strip.numPixels(); i++) {
//         strip.setPixelColor(i, r, g, b);
//     }
//     strip.show();
//     strip.setBrightness(brtLumi * lum / 256);

//     lampStep += 1;
//     if(lampStep > (512 - 40)) lampStep = 40;

//     return _delay;
// }

// uint32_t strobe()
// {
//     uint32_t c;
//     uint32_t _delay;
//     if (lampStep % 2) {
//         // c = Wheel(lampStep);
//         // c = Wheel(random(0, 255));
//         c = strobe_color;
//         // _delay = 5000;//
//         _delay = lampSpeed * 20;
//     }
//     else {
//         c = 0;
//         _delay = lampSpeed * 20;//1000;
//     }

//     if (lampStep == 255) lampStep = 0;
//     else lampStep++;
    
//     uint8_t r = (c >> 16 & 0xFF);
//     uint8_t g = (c >>  8 & 0xFF);
//     uint8_t b = (c       & 0xFF);
    
//     for(uint16_t i = 0; i <= strip.numPixels(); i++) {
//         strip.setPixelColor(i, r, g, b);
//     }

//     strip.show();

//     return _delay;
// }

void resetDisplay(uint16_t _time)
{
    // uint8_t lum = map(_time, 0, 10000, 255, 0);
    // uint8_t r   = 255 - lum;
    // uint8_t g   = lum;
    // uint8_t b   = lum;

    // for(uint8_t i=0; i < strip.numPixels(); i++) {
    //     strip.setPixelColor(i, r, g, b);
    // }

    // strip.show();
}

// void attachDelay(callback_with_uint32_arg_t newFunc)
// {
//     _lampDelay = newFunc;
// }

// void setColor(uint32_t color)
// {
//     switch(lampType) {
//         case BLINKER_LAMP_RAINBOW_CYCLE :
//             break;
//         case BLINKER_LAMP_RAINBOW :
//             break;
//         case BLINKER_LAMP_GRADIENT :
//             break;
//         case BLINKER_LAMP_STANDARD :
//             setStandard(color);
//             break;
//         case BLINKER_LAMP_BREATH :
//             setBreath(color);
//             break;
//         case BLINKER_LAMP_STROBE :
//             setStrobe(color);
//             break;
//         case BLINKER_LAMP_SUNLIGHT :
//             setSunlight(color);
//             break;
//         default :
//             break;
//     }
// }

// void setStandard(uint32_t color)
// {
//     if (color == 0) return;

//     latest_color = strip.getPixelColor(0);
//     now_color = color;
//     isGraded = false;
//     lampStep = 0;

//     standard_color = color;
// }

// void setBreath(uint32_t color)
// {
//     if (color == 0) return;

//     latest_color = strip.getPixelColor(0);
//     now_color = color;
//     isGraded = false;
//     lampStep = 0;

//     breath_color = color;
// }

// void setStrobe(uint32_t color)
// {
//     if (color == 0) return;

//     strobe_color = color;
// }

// uint32_t kelvin2RGB(uint8_t set)
// {
//     uint32_t temperature = map(set, 0, 255, 2500, 6500);

//     uint8_t temp = temperature/100;
//     uint8_t r,g,b;

//     if (temp<=66) {
//         r = 255;
//         g = 99.4708025861 * log(temp) - 161.1195681661;
//         if (temp<=19) {
//             b = 0;
//         } else {
//             b = 138.5177312231 * log(temp-10) - 305.0447927307;
//         }
//     } else {
//         if (temp<60) {
//             r = 0;
//             g = 0;
//         } else {
//             r = 329.698727446 * pow(temp-60, -0.1332047592);
//             g = 288.1221695283 * pow(temp-60, -0.0755148492);
//         }
//         b = 255;
//     }

//     r = (r > 0) ? r : 0;
//     g = (g > 0) ? g : 0;
//     b = (b > 0) ? b : 0;

//     return r << 16 | g << 8 | b;
// }

uint16_t getColorTemp()
{
    return map(temp_set, 0, 255, 3000, 5000);
}

uint8_t getTempSet()
{
    return temp_set;
}

void setSunlight(uint32_t temp)
{
    // if (color == 0) return;
    temp_set = temp;

    uint8_t color_night = 255;
    uint8_t color_white = map(temp, 0, 255, 50, 255);
    uint8_t color_yellow = map(temp, 0, 255, 255, 50);

    latest_color = night_led << 16 | white_led << 8 | yellow_led;//strip.getPixelColor(0);
    now_color = color_night << 16 | color_white << 8 | color_yellow;
    isGraded = false;
    lampStep = 0;

    sunlight_color = now_color;
}

// void setGradient(uint8_t num, uint32_t color)
// {
//     if (num > BLINKER_LAMP_COLOR_B) return;

//     if (color == 0) return;
//     gradient_color[num] = color;
//     gradient_color_standard = color;
// }

// void setGradient(uint32_t *color, uint8_t count)
// {
//     // memcpy(gradient_color, color, sizeof(color));
//     for(uint8_t num = 0; num < count; num++)
//     {
//         gradient_color[num] = color[num];
//     }
//     stream_count = count;
// }

void setBrightness(uint8_t bright)
{
    // strip.setBrightness(bright);

    latest_brt = getBrightness();
    now_brt = bright;
    isBrt = false;
    brtStep = 0;
    brtTime = millis();

    if (lampType == BLINKER_LAMP_BREATH)
    {
        latest_brt = bright_set;//strip.getBrightness();
        brtLumi = bright;
    }
}

uint8_t getBrightness()
{
    return now_brt;//strip.getBrightness();
}

uint32_t getPixelColor()
{
    return 0;//strip.getPixelColor(0);
}

// String getColor()
// {
//     uint32_t _color = 0;

//     uint8_t _color_r = 0;
//     uint8_t _color_g = 0;
//     uint8_t _color_b = 0;

//     String _color_str = "";
    
//     switch(lampType) {
//         case BLINKER_LAMP_GRADIENT :

//             _color_str = "[";

//             for(uint8_t num = 0; num < stream_count; num++)
//             {
//                 _color = gradient_color[num];

//                 _color_r = (_color >> 16 & 0xFF);
//                 _color_g = (_color >>  8 & 0xFF);
//                 _color_b = (_color       & 0xFF);

//                 _color_str += "[" + String(_color_r) + "," + \
//                 String(_color_g) + "," + \
//                 String(_color_b) + "]";

//                 if (num + 1 < stream_count) _color_str += ",";
//             }

//             _color_str += "]";
//             break;
//         case BLINKER_LAMP_STANDARD :
//             _color = standard_color;

//             _color_r = (_color >> 16 & 0xFF);
//             _color_g = (_color >>  8 & 0xFF);
//             _color_b = (_color       & 0xFF);

//             _color_str = "[" + String(_color_r) + "," + \
//                 String(_color_g) + "," + \
//                 String(_color_b) + "]";
//             break;
//         case BLINKER_LAMP_BREATH :
//             _color = breath_color;

//             _color_r = (_color >> 16 & 0xFF);
//             _color_g = (_color >>  8 & 0xFF);
//             _color_b = (_color       & 0xFF);

//             _color_str = "[" + String(_color_r) + "," + \
//                 String(_color_g) + "," + \
//                 String(_color_b) + "]";
//             break;
//         case BLINKER_LAMP_STROBE :
//             _color = strobe_color;

//             _color_r = (_color >> 16 & 0xFF);
//             _color_g = (_color >>  8 & 0xFF);
//             _color_b = (_color       & 0xFF);

//             _color_str = "[" + String(_color_r) + "," + \
//                 String(_color_g) + "," + \
//                 String(_color_b) + "]";
//             break;
//         case BLINKER_LAMP_SUNLIGHT :
//             _color = sunlight_color;

//             _color_r = (_color >> 16 & 0xFF);
//             _color_g = (_color >>  8 & 0xFF);
//             _color_b = (_color       & 0xFF);

//             _color_str = "[" + String(_color_r) + "," + \
//                 String(_color_g) + "," + \
//                 String(_color_b) + "]";
//             break;
//         default :
//             break;
//     }

//     return _color_str;
// }

// void setSpeed(uint8_t speed)
// {
//     lampSpeed = 255 - speed;
//     // speed = speed * 256;
//     // if (speed < BLINKER_LAMP_SPEED_MIN)
//     //     speed = BLINKER_LAMP_SPEED_MIN;
//     // if (speed > BLINKER_LAMP_SPEED_MAX)
//     //     speed = BLINKER_LAMP_SPEED_MAX;
    
//     // lampSpeed = map(speed, 0, 255, BLINKER_LAMP_SPEED_MAX, BLINKER_LAMP_SPEED_MIN);
//     _lampSpeed = lampSpeed;
// }

// uint8_t getSpeed()
// {
//     return 255 - lampSpeed;//map(lampSpeed, BLINKER_LAMP_SPEED_MAX, BLINKER_LAMP_SPEED_MIN, 0, 255);
// }

void modeChange()
{
    lampType = (lampType + 1) % BLINKER_LAMP_TYPE_COUNT;
    lampStep = 0;
    lampSpeed = 255;
    _lampSpeed = lampSpeed;

    Serial.println(lampType);
}

// void rainbow()
// {
//     lampType = BLINKER_LAMP_RAINBOW;
// }

// void rainbowCycle()
// {
//     lampType = BLINKER_LAMP_RAINBOW_CYCLE;
// }

// void setLampMode(uint8_t lamp_mode, bool state)
// {
//     lampType = lamp_mode;

//     lamp_state = state;
// }

String getMode()
{
    switch(lampType) {
        case BLINKER_LAMP_RAINBOW_CYCLE :
            return BLINKER_CMD_LAMP_RAINBOW_CYCLE;
            break;
        case BLINKER_LAMP_RAINBOW :
            return BLINKER_CMD_LAMP_RAINBOW;
            break;
        case BLINKER_LAMP_GRADIENT :
            return BLINKER_CMD_LAMP_GRADIENT;// * 256;
            break;
        case BLINKER_LAMP_STANDARD :
            return BLINKER_CMD_LAMP_STANDARD;
            break;
        case BLINKER_LAMP_BREATH :
            return BLINKER_CMD_LAMP_BREATH;// * 256;
            break;
        case BLINKER_LAMP_STROBE :
            return BLINKER_CMD_LAMP_STROBE;// * 256;
            break;
        case BLINKER_LAMP_SUNLIGHT :
            return BLINKER_CMD_LAMP_SUNLIGHT;
            break;
        default :
            return BLINKER_CMD_LAMP_RAINBOW_CYCLE;
            break;
    }
}

uint8_t getModeType()
{
    return lampType;
}

void lumiBreath()
{
    if (!isBrt)
    {
        if (millis() - brtTime >= 1)
        {
            brtTime = millis();
            
            uint8_t set_brt = latest_brt + (now_brt - latest_brt) * brtStep / 128;

            brtStep += 4;
            if (brtStep > 128) isBrt = true;

            // strip.setBrightness(set_brt);
            // strip.show();

            setLampBright(set_brt);
            lampShow();

            // Serial.print("SET BRT: ");
            // Serial.println(set_brt);
        }
    }
}

void ledInit()
{
    // strip.begin();
    // strip.show();
    sunlight(128);
    setBrightness(0);
}

void ledRun()
{
    // if (!isBrt)
    // {
        lumiBreath();
    //     return;
    // }

    if (!needFresh()) 
    {
        // strip.show();
        return;
    }

    _lampSpeed = sunlight();

    // switch(lampType) {
    //     case BLINKER_LAMP_RAINBOW_CYCLE :
    //         rainbowCycleDisplay();
    //         break;
    //     case BLINKER_LAMP_RAINBOW :
    //         rainbowDisplay();
    //         break;
    //     case BLINKER_LAMP_GRADIENT :
    //         _lampSpeed = streamer();// * 256;
    //         break;
    //     case BLINKER_LAMP_STANDARD :
    //         _lampSpeed = standard();
    //         break;
    //     case BLINKER_LAMP_BREATH :
    //         _lampSpeed = breath();// * 256;
    //         break;
    //     case BLINKER_LAMP_STROBE :
    //         _lampSpeed = strobe();// * 256;
    //         break;
    //     case BLINKER_LAMP_SUNLIGHT :
    //         _lampSpeed = sunlight();
    //         break;
    //     default :
    //         break;
    // }
}

// void addClick()
// {
//     lampType = (lampType + 1) % BLINKER_LAMP_TYPE_COUNT;

//     Serial.println("Button add click.");
// }

// void subClick()
// {
//     lampType = (lampType - 1 + BLINKER_LAMP_TYPE_COUNT) % BLINKER_LAMP_TYPE_COUNT;

//     Serial.println("Button sub click.");
// }

// uint8_t ecState = 0;
// uint8_t position = 125;

// void checkState()
// {
//     switch (ecState) {
//         case 1: case 7: case 8: case 14:
//             if (position != 255) position++;
//             return;
//         case 2: case 4: case 11: case 13:
//             if (position != 0) position--;
//             return;
//         case 3: case 12:
//             if (position != 255 && position <= 253) position += 2;
//             else position = 255;
//             return;
//         case 6: case 9:
//             if (position != 0 && position >=2) position -= 2;
//             else position = 0;
//             return;
//     }
// }

// void ec0Click()
// {
//     now_ec0 = !digitalRead(BLINKER_BUTTON_EC0_PIN);
//     now_ec1 = !digitalRead(BLINKER_BUTTON_EC1_PIN);

//     ecState = now_ec1 << 3 | now_ec0 << 2 | old_ec1 << 1 | old_ec0 << 0;

//     checkState();

//     Serial.print("ecState: ");
//     Serial.print(now_ec1);
//     Serial.print(now_ec0);
//     Serial.print(old_ec1);
//     Serial.print(old_ec0);
//     Serial.print(" ");
//     Serial.print(ecState);
//     Serial.print(" position: ");
//     Serial.println(position);
// }

// void ec1Click()
// {
//     now_ec0 = !digitalRead(BLINKER_BUTTON_EC0_PIN);
//     now_ec1 = !digitalRead(BLINKER_BUTTON_EC1_PIN);

//     ecState = now_ec1 << 3 | now_ec0 << 2 | old_ec1 << 1 | old_ec0 << 0;

//     checkState();

//     Serial.print("ecState: ");
//     Serial.print(now_ec1);
//     Serial.print(now_ec0);
//     Serial.print(old_ec1);
//     Serial.print(old_ec0);
//     Serial.print(" ");
//     Serial.print(ecState);
//     Serial.print(" position: ");
//     Serial.println(position);
// }

// void touchTick()
// {
//     // checkState();

//     buttonAdd.tick();
//     buttonSub.tick();
//     // buttonEC0.tick();
//     // buttonEC1.tick();

//     // Serial.println("Button click.");
//     old_ec0 = !digitalRead(BLINKER_BUTTON_EC0_PIN);
//     old_ec1 = !digitalRead(BLINKER_BUTTON_EC1_PIN);
// }

// void touchInit()
// {
//     buttonAdd.attachClick(addClick);
//     buttonSub.attachClick(subClick);
//     buttonEC0.attachClick(ec0Click);
//     buttonEC1.attachClick(ec1Click);

//     attachInterrupt(BLINKER_BUTTON_ADD_PIN, touchTick, CHANGE);
//     attachInterrupt(BLINKER_BUTTON_SUB_PIN, touchTick, CHANGE);
//     attachInterrupt(BLINKER_BUTTON_EC0_PIN, ec0Click, CHANGE);
//     attachInterrupt(BLINKER_BUTTON_EC1_PIN, ec1Click, CHANGE);

//     old_ec0 = !digitalRead(BLINKER_BUTTON_EC0_PIN);
//     old_ec1 = !digitalRead(BLINKER_BUTTON_EC1_PIN);
// }

