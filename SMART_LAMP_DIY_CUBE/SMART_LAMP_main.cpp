/* 
 * BLINKER_PRO is use for professional device
 * 
 * Please make sure you have permission to modify professional device!
 * Please read usermanual first! Thanks!
 * https://doc.blinker.app/
 * 
 * Written by i3water for blinker.
 * Learn more:https://blinker.app/
 */

#define BLINKER_PRINT Serial
#define BLINKER_MQTT
#define BLINKER_ALIGENIE_LIGHT
// #define BLINKER_ESP_SMARTCONFIG
#define BLINKER_DUEROS_LIGHT
// #define BLINKER_DEBUG_ALL

#include <Blinker.h>

char auth[] = "bc5a991c7ec4";
char ssid[] = "mostfun";
char pswd[] = "18038083873";

#define BUTTON_1 "btn-rc"
#define BUTTON_2 "btn-rb"
#define BUTTON_3 "btn-str"
#define BUTTON_4 "btn-std"
#define BUTTON_5 "btn-bre"
#define BUTTON_6 "btn-stb"

BlinkerButton * Button[6];

#define Slider_1 "speed"

BlinkerSlider spdSlider(Slider_1);

#define Number_1 "bat"

BlinkerNumber batNumber(Number_1);

#define RGB_1 "rgb"

BlinkerRGB RGB(RGB_1);

#include <OneButton.h>

#define BLINKER_BUTTON_PIN 15

OneButton button(BLINKER_BUTTON_PIN, false);

#include "SMART_LAMP_control.h"

static bool inited = false;
static bool isLongPress = false;
static uint8_t batRead;
static uint8_t batBase;
static uint32_t lamp_speed;
static uint32_t batFresh = 0;
static uint32_t press_start_time;
static uint16_t pressed_time;

uint8_t colorR, colorG, colorB, colorW;
bool wsState = true;
String wsMode = BLINKER_CMD_COMMON;

String getColor()
{
    uint32_t color = colorR << 16 | colorG << 8 | colorB;

    if (color == 0) {
        color = getPixelColor() & 0x00FFFFFF;
    }

    switch (color)
    {
        case 0xFF0000 :
            return "Red";
        case 0xFFFF00 :
            return "Yellow";
        case 0x0000FF :
            return "Blue";
        case 0x00FF00 :
            return "Green";
        case 0xFFFFFF :
            return "White";
        case 0x000000 :
            return "Black";
        case 0x00FFFF :
            return "Cyan";
        case 0x800080 :
            return "Purple";
        case 0xFFA500 :
            return "Orange";
        default :
            return "White";
    }
}

String getLampMode()
{
    uint8_t get_mode = getMode();

    switch(get_mode) {
        case BLINKER_LAMP_RAINBOW_CYCLE :
            return BLINKER_CMD_MOVIE;
        case BLINKER_LAMP_RAINBOW :
            return BLINKER_CMD_HOLIDAY;
        case BLINKER_LAMP_STREAMER :
            return BLINKER_CMD_MUSIC;
        case BLINKER_LAMP_STANDARD :
            return BLINKER_CMD_COMMON;
        case BLINKER_LAMP_BREATH :
            return BLINKER_CMD_SLEEP;
        case BLINKER_LAMP_RAINBOW_STROBE :
            return BLINKER_CMD_MUSIC;
        default :
            return BLINKER_CMD_COMMON;
    }
}

void aligeniePowerSate(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        // digitalWrite(LED_BUILTIN, HIGH);

        BlinkerAliGenie.powerState("on");
        BlinkerAliGenie.print();

        wsState = true;

        if (colorW == 0) colorW = 255;

        setStandard(255 << 24 | 255 << 16 | 255 << 8 | 255);
        // setBrightness(colorW);
        setMode(BLINKER_LAMP_STANDARD);
    }
    else if (state == BLINKER_CMD_OFF) {
        // digitalWrite(LED_BUILTIN, LOW);

        BlinkerAliGenie.powerState("off");
        BlinkerAliGenie.print();

        wsState = false;

        colorW = 0;
        colorR = 0; colorG = 0; colorB = 0;

        setStandard(colorW << 24 |colorR << 16 | colorG << 8 | colorB);
        // setBrightness(colorW);
        setMode(BLINKER_LAMP_STANDARD);
    }
}

void aligenieColor(const String & color)
{
    BLINKER_LOG("need set color: ", color);

    if (color == "Red") {
        colorR = 255;
        colorG = 0;
        colorB = 0;
    }
    else if (color == "Yellow") {
        colorR = 255;
        colorG = 255;
        colorB = 0;
    }
    else if (color == "Blue") {
        colorR = 0;
        colorG = 0;
        colorB = 255;
    }
    else if (color == "Green") {
        colorR = 0;
        colorG = 255;
        colorB = 0;
    }
    else if (color == "White") {
        colorR = 255;
        colorG = 255;
        colorB = 255;
    }
    else if (color == "Black") {
        colorR = 0;
        colorG = 0;
        colorB = 0;
    }
    else if (color == "Cyan") {
        colorR = 0;
        colorG = 255;
        colorB = 255;
    }
    else if (color == "Purple") {
        colorR = 128;
        colorG = 0;
        colorB = 128;
    }
    else if (color == "Orange") {
        colorR = 255;
        colorG = 165;
        colorB = 0;
    }
    
    wsState = true;
    colorW = 255;

    setStandard(colorW << 24 |colorR << 16 | colorG << 8 | colorB);
    // setBrightness(colorW);
    setMode(BLINKER_LAMP_STANDARD);

    BlinkerAliGenie.color(color);
    BlinkerAliGenie.print();
}

void aligenieMode(const String & mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_READING) {
        // Your mode function
        colorR = 255; colorG = 255; colorB = 255;
        setStandard(colorW << 24 | colorR << 16 | colorG << 8 | colorB);
        // setBrightness(colorW);
        setMode(BLINKER_LAMP_STANDARD);
    }
    else if (mode == BLINKER_CMD_MOVIE) {
        // Your mode function
        // setBrightness(colorW);
        setMode(BLINKER_LAMP_RAINBOW_CYCLE);
    }
    else if (mode == BLINKER_CMD_SLEEP) {
        // Your mode function
        setMode(BLINKER_LAMP_BREATH);
    }
    else if (mode == BLINKER_CMD_HOLIDAY) {
        // Your mode function
        setMode(BLINKER_LAMP_RAINBOW);
    }
    else if (mode == BLINKER_CMD_MUSIC) {
        // Your mode function
        setMode(BLINKER_LAMP_STREAMER);
    }
    else if (mode == BLINKER_CMD_COMMON) {
        // Your mode function
        setStandard(colorR << 16 | colorG << 8 | colorB);
        // setBrightness(colorW);
        setMode(BLINKER_LAMP_STANDARD);
    }

    wsMode = mode;

    BlinkerAliGenie.mode(mode);
    BlinkerAliGenie.print();
}

void aligeniecMode(const String & cmode)
{
    BLINKER_LOG("need cancel mode: ", cmode);

    if (cmode == BLINKER_CMD_READING) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_MOVIE) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_SLEEP) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_HOLIDAY) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_MUSIC) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_COMMON) {
        // Your mode function
    }

    wsMode = BLINKER_CMD_COMMON; // new mode

    BlinkerAliGenie.mode(wsMode); // must response
    BlinkerAliGenie.print();
}

void aligenieBright(const String & bright)
{
    BLINKER_LOG("need set brightness: ", bright);

    if (bright == BLINKER_CMD_MAX) {
        colorW = 255;
    }
    else if (bright == BLINKER_CMD_MIN) {
        colorW = 0;
    }
    else {
        colorW = bright.toInt();
    }

    BLINKER_LOG("now set brightness: ", colorW);

    setBrightness(colorW);

    BlinkerAliGenie.brightness(colorW);
    BlinkerAliGenie.print();
}

void aligenieRelativeBright(int32_t bright)
{
    BLINKER_LOG("need set relative brightness: ", bright);

    if (colorW + bright < 255 && colorW + bright >= 0) {
        colorW += bright;
    }

    BLINKER_LOG("now set brightness: ", colorW);

    setBrightness(colorW);

    BlinkerAliGenie.brightness(bright);
    BlinkerAliGenie.print();
}

void aligenieColoTemp(int32_t colorTemp)
{
    BLINKER_LOG("need set colorTemperature: ", colorTemp);

    BlinkerAliGenie.colorTemp(colorTemp);
    BlinkerAliGenie.print();
}

void aligenieRelativeColoTemp(int32_t colorTemp)
{
    BLINKER_LOG("need set relative colorTemperature: ", colorTemp);

    BlinkerAliGenie.colorTemp(colorTemp);
    BlinkerAliGenie.print();
}

void aligenieQuery(int32_t queryCode)
{
    BLINKER_LOG("AliGenie Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("AliGenie Query All");
            BlinkerAliGenie.powerState(wsState ? "on" : "off");
            BlinkerAliGenie.color(getColor());
            BlinkerAliGenie.mode(getLampMode());
            BlinkerAliGenie.colorTemp(50);
            BlinkerAliGenie.brightness(getBrightness());
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("AliGenie Query Power State");
            BlinkerAliGenie.powerState(wsState ? "on" : "off");
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_COLOR_NUMBER :
            BLINKER_LOG("AliGenie Query Color");
            BlinkerAliGenie.color(getColor());
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_MODE_NUMBER :
            BLINKER_LOG("AliGenie Query Mode");
            BlinkerAliGenie.mode(getLampMode());
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_COLORTEMP_NUMBER :
            BLINKER_LOG("AliGenie Query ColorTemperature");
            BlinkerAliGenie.colorTemp(50);
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER :
            BLINKER_LOG("AliGenie Query Brightness");
            BlinkerAliGenie.brightness(getBrightness());
            BlinkerAliGenie.print();
            break;
        default :
            BlinkerAliGenie.powerState(wsState ? "on" : "off");
            BlinkerAliGenie.color(getColor());
            BlinkerAliGenie.mode(getLampMode());
            BlinkerAliGenie.colorTemp(50);
            BlinkerAliGenie.brightness(getBrightness());
            BlinkerAliGenie.print();
            break;
    }
}

void duerPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        // digitalWrite(LED_BUILTIN, HIGH);

        BlinkerDuerOS.powerState("on");
        BlinkerDuerOS.print();

        wsState = true;

        if (colorW == 0) colorW = 255;

        setStandard(255 << 24 | 255 << 16 | 255 << 8 | 255);
        // setBrightness(colorW);
        setMode(BLINKER_LAMP_STANDARD);
    }
    else if (state == BLINKER_CMD_OFF) {
        // digitalWrite(LED_BUILTIN, LOW);

        BlinkerDuerOS.powerState("off");
        BlinkerDuerOS.print();

        wsState = false;

        colorW = 0;
        colorR = 0; colorG = 0; colorB = 0;

        setStandard(colorW << 24 |colorR << 16 | colorG << 8 | colorB);
        // setBrightness(colorW);
        setMode(BLINKER_LAMP_STANDARD);
    }
}

void duerColor(int32_t color)
{
    BLINKER_LOG("need set color: ", color);

    colorR = color >> 16 & 0xFF;
    colorG = color >>  8 & 0xFF;
    colorB = color       & 0xFF;

    BLINKER_LOG("colorR: ", colorR, ", colorG: ", colorG, ", colorB: ", colorB);

    wsState = true;
    colorW = 255;

    setStandard(colorW << 24 |colorR << 16 | colorG << 8 | colorB);
    // setBrightness(colorW);
    setMode(BLINKER_LAMP_STANDARD);

    BlinkerDuerOS.color(color);
    BlinkerDuerOS.print();
}

void duerMode(const String & mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_DUEROS_READING) {
        // Your mode function
        colorR = 255; colorG = 255; colorB = 255;
        setStandard(colorW << 24 | colorR << 16 | colorG << 8 | colorB);
        // setBrightness(colorW);
        setMode(BLINKER_LAMP_STANDARD);
    }
    else if (mode == BLINKER_CMD_DUEROS_ALARM) {
        // Your mode function
        // setBrightness(colorW);
        setMode(BLINKER_LAMP_RAINBOW_CYCLE);
    }
    else if (mode == BLINKER_CMD_DUEROS_SLEEP) {
        // Your mode function
        setMode(BLINKER_LAMP_BREATH);
    }
    else if (mode == BLINKER_CMD_DUEROS_NIGHT_LIGHT) {
        // Your mode function
        setMode(BLINKER_LAMP_RAINBOW);
    }
    else if (mode == BLINKER_CMD_DUEROS_ROMANTIC) {
        // Your mode function
        setMode(BLINKER_LAMP_STREAMER);
    }
    else {
        // Your mode function
        setStandard(colorR << 16 | colorG << 8 | colorB);
        // setBrightness(colorW);
        setMode(BLINKER_LAMP_STANDARD);
    }

    wsMode = mode;

    BlinkerDuerOS.mode(mode);
    BlinkerDuerOS.print();
}

void duercMode(const String & cmode)
{
    BLINKER_LOG("need cancel mode: ", cmode);

    if (cmode == BLINKER_CMD_DUEROS_READING) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_DUEROS_ALARM) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_DUEROS_SLEEP) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_DUEROS_NIGHT_LIGHT) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_DUEROS_ROMANTIC) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_COMMON) {
        // Your mode function
    }

    wsMode = BLINKER_CMD_COMMON; // new mode

    BlinkerDuerOS.mode(wsMode); // must response
    BlinkerDuerOS.print();
}

void duerBright(const String & bright)
{
    BLINKER_LOG("need set brightness: ", bright);

    if (bright == BLINKER_CMD_MAX) {
        colorW = 255;
    }
    else if (bright == BLINKER_CMD_MIN) {
        colorW = 0;
    }
    else {
        colorW = bright.toInt();
    }

    BLINKER_LOG("now set brightness: ", colorW);

    setBrightness(colorW);

    BlinkerDuerOS.brightness(colorW);
    BlinkerDuerOS.print();
}

void duerRelativeBright(int32_t bright)
{
    BLINKER_LOG("need set relative brightness: ", bright);

    if (colorW + bright < 255 && colorW + bright >= 0) {
        colorW += bright;
    }

    BLINKER_LOG("now set brightness: ", colorW);

    setBrightness(colorW);

    BlinkerDuerOS.brightness(bright);
    BlinkerDuerOS.print();
}

void duerQuery(int32_t queryCode)
{
    BLINKER_LOG("DuerOS Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_TIME_NUMBER :
            BLINKER_LOG("DuerOS Query time");
            BlinkerDuerOS.time(millis());
            BlinkerDuerOS.print();
            break;
        default :
            BlinkerDuerOS.time(millis());
            BlinkerDuerOS.print();
            break;
    }
}

void button1_callback(const String & state)
{
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // BLINKER_LOG("get button state: ", state);
    // Blinker.beginFormat();
    uint32_t n_time = millis();
    for (uint8_t num = 0; num < 6; num++) {
        if (num != 0) {
            Button[num]->color("#000000");
            Button[num]->print();
        }
        else {
            Button[num]->color("#FF0000");
            Button[num]->print();
        }
    }

    spdSlider.print(getSpeed());
    BLINKER_LOG("print use time: ", millis() - n_time);
    // Blinker.endFormat();

    setLampMode(BLINKER_LAMP_RAINBOW_CYCLE);
}

void button2_callback(const String & state)
{
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // BLINKER_LOG("get button state: ", state);
    // Blinker.beginFormat();

    for (uint8_t num = 0; num < 6; num++) {
        if (num != 1) {
            Button[num]->color("#000000");
            Button[num]->print();
        }
        else {
            Button[num]->color("#FF0000");
            Button[num]->print();
        }
    }

    spdSlider.print(getSpeed());

    // Blinker.endFormat();

    setLampMode(BLINKER_LAMP_RAINBOW);
}

void button3_callback(const String & state)
{
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // BLINKER_LOG("get button state: ", state);
    // Blinker.beginFormat();

    for (uint8_t num = 0; num < 6; num++) {
        if (num != 2) {
            Button[num]->color("#000000");
            Button[num]->print();
        }
        else {
            Button[num]->color("#FF0000");
            Button[num]->print();
        }
    }

    spdSlider.print(getSpeed());

    // Blinker.endFormat();

    setLampMode(BLINKER_LAMP_STREAMER);
}

void button4_callback(const String & state)
{
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // BLINKER_LOG("get button state: ", state);
    // Blinker.beginFormat();

    for (uint8_t num = 0; num < 6; num++) {
        if (num != 3) {
            Button[num]->color("#000000");
            Button[num]->print();
        }
        else {
            Button[num]->color("#FF0000");
            Button[num]->print();
        }
    }

    spdSlider.print(getSpeed());

    // Blinker.endFormat();

    setLampMode(BLINKER_LAMP_STANDARD);
}

void button5_callback(const String & state)
{
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // BLINKER_LOG("get button state: ", state);
    // Blinker.beginFormat();

    for (uint8_t num = 0; num < 6; num++) {
        if (num != 4) {
            Button[num]->color("#000000");
            Button[num]->print();
        }
        else {
            Button[num]->color("#FF0000");
            Button[num]->print();
        }
    }

    spdSlider.print(getSpeed());

    // Blinker.endFormat();

    setLampMode(BLINKER_LAMP_BREATH);
}

void button6_callback(const String & state)
{
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // BLINKER_LOG("get button state: ", state);
    // Blinker.beginFormat();

    for (uint8_t num = 0; num < 6; num++) {
        if (num != 5) {
            Button[num]->color("#000000");
            Button[num]->print();
        }
        else {
            Button[num]->color("#FF0000");
            Button[num]->print();
        }
    }

    spdSlider.print(getSpeed());

    // Blinker.endFormat();

    setLampMode(BLINKER_LAMP_RAINBOW_STROBE);
}

void slider1_callback(int32_t value)
{
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("get slider value: ", value);

    setSpeed(value);
}

void rgb1_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("R value: ", r_value);
    BLINKER_LOG("G value: ", g_value);
    BLINKER_LOG("B value: ", b_value);
    BLINKER_LOG("Rrightness value: ", bright_value);

    setStandard(bright_value << 24 | r_value << 16 | g_value << 8 | b_value);
}

/* 
 * Add your heartbeat message detail in this function
 * 
 * When get heartbeat command {"get": "state"}, device will call this function
 * For example, you can print message back
 * 
 * Every 30s will get a heartbeat command from app 
 */
void heartbeat()
{
    uint32_t now_time = millis();

    for (uint8_t num = 0; num < 6; num++) {
        if (num != getMode()) {
            Button[num]->color("#000000");
            Button[num]->print();
        }
        else {
            Button[num]->color("#FF0000");
            Button[num]->print();
        }
    }

    spdSlider.print(getSpeed());

    BLINKER_LOG("heartbeat!");

    BLINKER_LOG("use time: ", millis() - now_time);
}

/* 
 * Blinker provide a button parse function for user if you defined BLINKER_BUTTON
 * 
 * Blinker button can detect singal click/ double click/ long press
 * 
 * Blinker.tick() will run by default, use interrupt will be better
 */
void buttonTick()
{
    button.tick();
}

/* 
 * Add your code in this function
 * 
 * When button clicked, device will call this function
 */
void singalClick()
{
    modeChange();

    // Blinker.beginFormat();

    for (uint8_t num = 0; num < 6; num++) {
        if (num != getMode()) {
            Button[num]->color("#000000");
            Button[num]->print();
        }
        else {
            Button[num]->color("#FF0000");
            Button[num]->print();
        }
    }

    spdSlider.print(getSpeed());

    // Blinker.endFormat();
    
    BLINKER_LOG("Button clicked!");
}

/* 
 * Add your code in this function
 * 
 * When button double clicked, device will call this function
 */
void doubleClick()
{
    setBrightness(random(0, 255));
    
    BLINKER_LOG("Button double clicked!");
}

/* 
 * Add your code in this function
 * 
 * When long press start, device will call this function
 */
void longPressStart()
{
    isLongPress = true;
    press_start_time = millis();

    BLINKER_LOG("Button long press start!");
}

/* 
 * Add your code in this function
 * 
 * When long press stop and trigged POWERDOWN, device will call this function
 */
void longPressPowerdown()
{
    BLINKER_LOG("Button long press powerdown!");

    // digitalWrite(BLINKER_POWER_3V3_PIN, LOW);
}

/* 
 * Add your code in this function
 * 
 * When long press stop and trigged RESET, device will call this function
 */
void longPressReset()
{
    ESP.restart();

    BLINKER_LOG("Button long press reset!");
}

/* 
 * Add your code in this function
 * 
 * When long press stop, device will call this function
 */
void longPressStop()
{
    isLongPress = false;

    if ((millis() - press_start_time) >= BLINKER_LAMP_LONG_PRESS_RESET) {
        longPressReset();
    }
    else {
        longPressPowerdown();
    }

    BLINKER_LOG("Button long press stop!");
}

// double getBAT()
// {
//     int sensorValue = analogRead(A0);
//     sensorValue += analogRead(A0);
//     sensorValue += analogRead(A0);
//     sensorValue += analogRead(A0);
//     sensorValue += analogRead(A0);
//     sensorValue += analogRead(A0);
//     sensorValue += analogRead(A0);
//     sensorValue += analogRead(A0);

//     double voltage = sensorValue * (5.926 / 1023.0 / 8.0);

//     // BLINKER_LOG("bat: ", voltage);

//     return voltage;
// }

// void batCheck()
// {
//     if ((millis() - batFresh) > BLINKER_BAT_CHECK_TIME)
//     {
//         batRead = getBAT() * 10;

//         if (batBase - batRead > BLINKER_BAT_POWER_USEUP * 10) {
//             batBase = batRead;
//             BLINKER_ERR_LOG("BLINKER_BAT_POWER_USEUP");
//             BLINKER_LOG("batBase: ", batBase / 10.0, " v", ", batRead: ", batRead / 10.0, " v");
//         }
//         else {
//             if (batRead > batBase) batBase = batRead;
//         }
        
//         BLINKER_LOG("bat: ", batRead / 10.0, " v");

//         // fas fa-battery-full
//         if (batRead >= 41) batNumber.icon("fas fa-battery-full");
//         else if (batRead >= 40) batNumber.icon("fas fa-battery-three-quarters");
//         else if (batRead >= 39) batNumber.icon("fas fa-battery-half");
//         else if (batRead >= 38) batNumber.icon("fas fa-battery-quarter");
//         else batNumber.icon("fas fa-battery-empty");
        
//         batNumber.color("#00FF00");
//         batNumber.unit("v");
//         batNumber.text("电量");
//         batNumber.print(batRead / 10.0);
//         // BLINKER_LOG_FreeHeap();

//         batFresh = millis();

//         if (batRead < BLINKER_BAT_POWER_LOW * 10) {
//             // digitalWrite(BLINKER_POWER_3V3_PIN, LOW);
//         }
//     }
// }

// void delays(uint32_t ms)
// {
//     Blinker.delay(ms);
// }

void hardwareInit()
{
    // pinMode(BLINKER_POWER_3V3_PIN, OUTPUT);
    // digitalWrite(BLINKER_POWER_3V3_PIN, HIGH);
    // pinMode(BLINKER_POWER_5V_PIN, OUTPUT);
    // digitalWrite(BLINKER_POWER_5V_PIN, HIGH);

    // batRead = getBAT() * 10;
}

void LAMP_init()
{
    Serial.begin(115200);

    #if defined(BLINKER_PRINT)
        BLINKER_DEBUG.stream(BLINKER_PRINT);
        BLINKER_DEBUG.debugAll();
    #endif

    hardwareInit();
    ledInit();

    button.attachClick(singalClick);
    button.attachDoubleClick(doubleClick);
    button.attachLongPressStart(longPressStart);
    button.attachLongPressStop(longPressStop);

    attachInterrupt(BLINKER_BUTTON_PIN, buttonTick, CHANGE);
    
    Blinker.begin(auth, ssid, pswd);
    // Blinker.begin(auth);
    // Blinker.begin(ssid, pswd);

    Button[0] = new BlinkerButton(BUTTON_1, button1_callback);
    Button[1] = new BlinkerButton(BUTTON_2, button2_callback);
    Button[2] = new BlinkerButton(BUTTON_3, button3_callback);
    Button[3] = new BlinkerButton(BUTTON_4, button4_callback);
    Button[4] = new BlinkerButton(BUTTON_5, button5_callback);
    Button[5] = new BlinkerButton(BUTTON_6, button6_callback);

    spdSlider.attach(slider1_callback);

    RGB.attach(rgb1_callback);

    // Blinker.attachParse(dataParse);
    Blinker.attachHeartbeat(heartbeat);

    BlinkerAliGenie.attachPowerState(aligeniePowerSate);
    BlinkerAliGenie.attachColor(aligenieColor);
    BlinkerAliGenie.attachMode(aligenieMode);
    BlinkerAliGenie.attachCancelMode(aligeniecMode);
    BlinkerAliGenie.attachBrightness(aligenieBright);
    BlinkerAliGenie.attachRelativeBrightness(aligenieRelativeBright);
    BlinkerAliGenie.attachColorTemperature(aligenieColoTemp);
    BlinkerAliGenie.attachRelativeColorTemperature(aligenieRelativeColoTemp);
    BlinkerAliGenie.attachQuery(aligenieQuery);

    BlinkerDuerOS.attachPowerState(duerPowerState);
    BlinkerDuerOS.attachColor(duerColor);
    BlinkerDuerOS.attachMode(duerMode);
    BlinkerDuerOS.attachCancelMode(duercMode);
    BlinkerDuerOS.attachBrightness(duerBright);
    BlinkerDuerOS.attachRelativeBrightness(duerRelativeBright);
    BlinkerDuerOS.attachQuery(duerQuery);

    // attachDelay(delays);
}

void LAMP_run()
{
    Blinker.run();

    button.tick();

    if (isLongPress) {
        resetDisplay(millis() - press_start_time);
    }
    else {
        ledRun();
    }

    // batCheck();
}
