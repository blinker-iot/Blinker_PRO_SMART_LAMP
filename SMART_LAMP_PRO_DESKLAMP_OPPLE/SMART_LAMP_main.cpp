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
#define BLINKER_PRO
#define BLINKER_DEBUG_ALL

#define BLINKER_BUTTON
#define BLINKER_BUTTON_PULLDOWN
#define BLINKER_BUTTON_LONGPRESS_POWERDOWN
#define BLINKER_BUTTON_PIN 15

#include <Blinker.h>

#include "SMART_LAMP_control.h"

static bool inited = false;
static bool isLongPress = false;
// static bool powerState = false;
static uint8_t batRead;
static uint8_t batBase;
static uint8_t sunSet = 0;
static uint32_t batFresh = 0;
static uint32_t lamp_bright;

uint8_t colorR, colorG, colorB, colorW;
bool wsState = true;
String wsMode = BLINKER_CMD_COMMON;

/* 
 * Add your command parse code in this function
 * 
 * When get a command and device not parsed this command, 
 * device will call this function with a JsonObject data.
 */
bool dataParse(const JsonObject & data)
{
    String getData;
    data.printTo(getData);
    BLINKER_LOG("Get user command: ", getData);

    bool isParsed = false;

    if (data.containsKey(BLINKER_CMD_SET))
    {
        String setData = data[BLINKER_CMD_SET];

        DynamicJsonBuffer jsonBuffer;
        JsonObject& setJson = jsonBuffer.parseObject(setData);

        if (!setJson.success())
        {
            return false;
        }

        String set_mode = setJson[BLINKER_CMD_LAMP_MODE];

        if (setJson.containsKey(BLINKER_CMD_SWITCH))
        {
            String swData = setJson[BLINKER_CMD_SWITCH];

            if (swData == BLINKER_CMD_ON)
            {
                if (lamp_bright != 0)
                {
                    setBrightness(lamp_bright);
                }
                else
                {
                    setBrightness(255);
                }

                // powerState = true;
            }
            else if (swData == BLINKER_CMD_OFF)
            {
                lamp_bright = getBrightness();
                setBrightness(0);

                // powerState = false;
            }
        }

        if (set_mode == BLINKER_CMD_LAMP_RAINBOW_CYCLE)
        {
            // setLampMode(BLINKER_LAMP_RAINBOW_CYCLE);
            isParsed = true;
        }
        else if (set_mode == BLINKER_CMD_LAMP_RAINBOW)
        {
            // setLampMode(BLINKER_LAMP_RAINBOW);
            isParsed = true;
        }
        else if (set_mode == BLINKER_CMD_LAMP_STANDARD)
        {
            // setLampMode(BLINKER_LAMP_STANDARD);
            
            // if (setJson.containsKey(BLINKER_CMD_LAMP_COLOR))
            // {
            //     uint8_t std_r = setJson[BLINKER_CMD_LAMP_COLOR][0];
            //     uint8_t std_g = setJson[BLINKER_CMD_LAMP_COLOR][1];
            //     uint8_t std_b = setJson[BLINKER_CMD_LAMP_COLOR][2];
            //     uint8_t std_w = setJson[BLINKER_CMD_LAMP_COLOR][3];
                
            //     setStandard(std_w << 24 | std_r << 16 | std_g << 8 | std_b);
            // }

            isParsed = true;
        }
        else if (set_mode == BLINKER_CMD_LAMP_BREATH)
        {
            // setLampMode(BLINKER_LAMP_BREATH);
            
            // if (setJson.containsKey(BLINKER_CMD_LAMP_COLOR))
            // {
            //     uint8_t sun_r = setJson[BLINKER_CMD_LAMP_COLOR][0];
            //     uint8_t sun_g = setJson[BLINKER_CMD_LAMP_COLOR][1];
            //     uint8_t sun_b = setJson[BLINKER_CMD_LAMP_COLOR][2];
            //     uint8_t sun_w = setJson[BLINKER_CMD_LAMP_COLOR][3];
                
            //     setBreath(sun_w << 24 | sun_r << 16 | sun_g << 8 | sun_b);
            // }

            isParsed = true;
        }
        else if (set_mode == BLINKER_CMD_LAMP_STROBE)
        {
            // setLampMode(BLINKER_LAMP_STROBE);
            
            // if (setJson.containsKey(BLINKER_CMD_LAMP_COLOR))
            // {
            //     uint8_t sun_r = setJson[BLINKER_CMD_LAMP_COLOR][0];
            //     uint8_t sun_g = setJson[BLINKER_CMD_LAMP_COLOR][1];
            //     uint8_t sun_b = setJson[BLINKER_CMD_LAMP_COLOR][2];
            //     uint8_t sun_w = setJson[BLINKER_CMD_LAMP_COLOR][3];
                
            //     setStrobe(sun_w << 24 | sun_r << 16 | sun_g << 8 | sun_b);
            // }
            
            isParsed = true;
        }
        else if (set_mode == BLINKER_CMD_LAMP_SUNLIGHT)
        {
            // setLampMode(BLINKER_LAMP_SUNLIGHT);

            if (setJson.containsKey(BLINKER_CMD_LAMP_TEMP))
            {
                uint8_t sun_set = setJson[BLINKER_CMD_LAMP_TEMP];

                sunSet = sun_set;
                
                // uint32_t sun_color = map(sun_set, 0, 255, BLINKER_COLOR_TEMP_MIN, BLINKER_COLOR_TEMP_MAX);
                
                setSunlight(sunSet);
            }
            
            isParsed = true;
        }
        else if (set_mode == BLINKER_CMD_LAMP_GRADIENT)
        {
            // uint8_t state = setJson[BLINKER_CMD_LAMP_RUN];
            // setLampMode(BLINKER_LAMP_GRADIENT, state);
            
            // if (setJson.containsKey(BLINKER_CMD_GRADIENT_COLOR))
            // {
            //     uint32_t stm_clr[BLINKER_MAX_GRADIENT_COUNT];
            //     uint8_t  clr_get;
            //     uint8_t  clr_count = 0;

            //     for (uint8_t num = 0; num < BLINKER_MAX_GRADIENT_COUNT; num++)
            //     {
            //         if (setJson[BLINKER_CMD_GRADIENT_COLOR][num] == 0) break;
            //         else clr_count++;

            //         clr_get = setJson[BLINKER_CMD_GRADIENT_COLOR][num][0];
            //         stm_clr[num] = clr_get;
            //         clr_get = setJson[BLINKER_CMD_GRADIENT_COLOR][num][1];
            //         stm_clr[num] = stm_clr[num] << 8 | clr_get;
            //         clr_get = setJson[BLINKER_CMD_GRADIENT_COLOR][num][2];
            //         stm_clr[num] = stm_clr[num] << 8 | clr_get;
            //     }

            //     setGradient(stm_clr, clr_count);
            // }
            
            isParsed = true;
        }

        if (setJson.containsKey(BLINKER_CMD_LAMP_COLOR))
        {
            // uint32_t set_clr;
            // uint8_t  clr;

            // clr = setJson[BLINKER_CMD_LAMP_COLOR][0];
            // set_clr = clr;
            // clr = setJson[BLINKER_CMD_LAMP_COLOR][1];
            // set_clr = set_clr << 8 | clr;
            // clr = setJson[BLINKER_CMD_LAMP_COLOR][2];
            // set_clr = set_clr << 8 | clr;
            // clr = setJson[BLINKER_CMD_LAMP_COLOR][3];
            // set_clr = set_clr | clr << 24;

            // setColor(set_clr);
        }

        if (setJson.containsKey(BLINKER_CMD_LAMP_SPEED))
        {
            // uint8_t speed = setJson[BLINKER_CMD_LAMP_SPEED];

            // setSpeed(speed);
        }

        if (setJson.containsKey(BLINKER_CMD_LAMP_BRIGHT))
        {
            uint8_t bright = setJson[BLINKER_CMD_LAMP_BRIGHT];

            setBrightness(bright);

            // lamp_bright = getBrightness();
        }
    }
    else if (data.containsKey(BLINKER_CMD_GET))
    {
        String getData = data[BLINKER_CMD_GET];

        DynamicJsonBuffer jsonBuffer;
        JsonObject& getJson = jsonBuffer.parseObject(getData);

        if (!getJson.success())
        {
            return false;
        }
    }    
    
    // Blinker.print(BLINKER_CMD_LAMP_MODE, getMode());
    // Blinker.print(BLINKER_CMD_LAMP_SPEED, getSpeed());
    Blinker.print(BLINKER_CMD_LAMP_BRIGHT, getBrightness());
    Blinker.print(BLINKER_CMD_SWITCH, getBrightness() ? BLINKER_CMD_ON : BLINKER_CMD_OFF);
    // if (getMode() != BLINKER_CMD_LAMP_RAINBOW_CYCLE)
    // {
    //     if (getMode() == BLINKER_CMD_LAMP_GRADIENT)
    //     {
    //         Blinker.printArray(BLINKER_CMD_GRADIENT_COLOR, getColor());
    //     }
    //     else
    //     {
    //         Blinker.printArray(BLINKER_CMD_LAMP_COLOR, getColor());
    //     }
    // }
    
    // if (getMode() == BLINKER_CMD_LAMP_SUNLIGHT)
    // {
        Blinker.print(BLINKER_CMD_LAMP_TEMP, sunSet);
    // }

    if (isParsed) {
        return true;
    }
    else {
        return false;
    }
}

String getAliColor()
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
    uint8_t get_mode = getModeType();

    switch(get_mode) {
        case BLINKER_LAMP_RAINBOW_CYCLE :
            return BLINKER_CMD_MOVIE;
        case BLINKER_LAMP_RAINBOW :
            return BLINKER_CMD_HOLIDAY;
        case BLINKER_LAMP_GRADIENT :
            return BLINKER_CMD_MUSIC;
        case BLINKER_LAMP_STANDARD :
            return BLINKER_CMD_COMMON;
        case BLINKER_LAMP_BREATH :
            return BLINKER_CMD_SLEEP;
        case BLINKER_LAMP_STROBE :
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

        // if (colorW == 0) colorW = 255;

        // setStandard(255 << 24 | 255 << 16 | 255 << 8 | 255);

        if (lamp_bright != 0)
        {
            setBrightness(lamp_bright);
        }
        else
        {
            setBrightness(255);
        }

        // setLampMode(BLINKER_LAMP_STANDARD);
    }
    else if (state == BLINKER_CMD_OFF) {
        // digitalWrite(LED_BUILTIN, LOW);

        BlinkerAliGenie.powerState("off");
        BlinkerAliGenie.print();

        wsState = false;

        // colorW = 0;
        // colorR = 0; colorG = 0; colorB = 0;

        // setStandard(colorW << 24 |colorR << 16 | colorG << 8 | colorB);
        
        lamp_bright = getBrightness();
        setBrightness(0);

        // setLampMode(BLINKER_LAMP_STANDARD);
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

    // setStandard(colorW << 24 |colorR << 16 | colorG << 8 | colorB);
    // // setBrightness(colorW);
    // setLampMode(BLINKER_LAMP_STANDARD);

    BlinkerAliGenie.color(color);
    BlinkerAliGenie.print();
}

void aligenieMode(const String & mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_READING) {
        // Your mode function
        colorR = 255; colorG = 255; colorB = 255;
        // setStandard(colorW << 24 | colorR << 16 | colorG << 8 | colorB);
        // // setBrightness(colorW);
        // setLampMode(BLINKER_LAMP_STANDARD);
    }
    else if (mode == BLINKER_CMD_MOVIE) {
        // Your mode function
        // setBrightness(colorW);
        // setLampMode(BLINKER_LAMP_RAINBOW_CYCLE);
    }
    else if (mode == BLINKER_CMD_SLEEP) {
        // Your mode function
        // setLampMode(BLINKER_LAMP_BREATH);
    }
    else if (mode == BLINKER_CMD_HOLIDAY) {
        // Your mode function
        // setLampMode(BLINKER_LAMP_RAINBOW);
    }
    else if (mode == BLINKER_CMD_MUSIC) {
        // Your mode function
        // setLampMode(BLINKER_LAMP_GRADIENT);
    }
    else if (mode == BLINKER_CMD_COMMON) {
        // Your mode function
        // setStandard(colorR << 16 | colorG << 8 | colorB);
        // // setBrightness(colorW);
        // setLampMode(BLINKER_LAMP_STANDARD);
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
    colorW = getBrightness();

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
    colorW = getBrightness();

    BLINKER_LOG("need set relative brightness: ", bright);

    if (colorW + bright < 255 && colorW + bright >= 0) {
        colorW += bright;
    }

    BLINKER_LOG("now set brightness: ", colorW);

    setBrightness(colorW);

    BlinkerAliGenie.brightness(colorW);
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

    setSunlight(getTempSet() + colorTemp/abs(colorTemp) * 10);

    BlinkerAliGenie.colorTemp(getColorTemp());
    BlinkerAliGenie.print();
}

void aligenieQuery(int32_t queryCode)
{
    BLINKER_LOG("AliGenie Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("AliGenie Query All");
            BlinkerAliGenie.powerState(getBrightness() ? "on" : "off");
            BlinkerAliGenie.color(getAliColor());
            BlinkerAliGenie.mode(getLampMode());
            BlinkerAliGenie.colorTemp(getColorTemp());
            BlinkerAliGenie.brightness(getBrightness());
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("AliGenie Query Power State");
            BlinkerAliGenie.powerState(getBrightness() ? "on" : "off");
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_COLOR_NUMBER :
            BLINKER_LOG("AliGenie Query Color");
            BlinkerAliGenie.color(getAliColor());
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_MODE_NUMBER :
            BLINKER_LOG("AliGenie Query Mode");
            BlinkerAliGenie.mode(getLampMode());
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_COLORTEMP_NUMBER :
            BLINKER_LOG("AliGenie Query ColorTemperature");
            BlinkerAliGenie.colorTemp(getColorTemp());
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER :
            BLINKER_LOG("AliGenie Query Brightness");
            BlinkerAliGenie.brightness(getBrightness());
            BlinkerAliGenie.print();
            break;
        default :
            BlinkerAliGenie.powerState(getBrightness() ? "on" : "off");
            BlinkerAliGenie.color(getAliColor());
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

        // if (colorW == 0) colorW = 255;

        // setStandard(255 << 24 | 255 << 16 | 255 << 8 | 255);
        
        if (lamp_bright != 0)
        {
            setBrightness(lamp_bright);
        }
        else
        {
            setBrightness(255);
        }

        // setLampMode(BLINKER_LAMP_STANDARD);
    }
    else if (state == BLINKER_CMD_OFF) {
        // digitalWrite(LED_BUILTIN, LOW);

        BlinkerDuerOS.powerState("off");
        BlinkerDuerOS.print();

        wsState = false;

        // colorW = 0;
        // colorR = 0; colorG = 0; colorB = 0;

        // setStandard(colorW << 24 |colorR << 16 | colorG << 8 | colorB);
        
        lamp_bright = getBrightness();
        setBrightness(0);

        // setLampMode(BLINKER_LAMP_STANDARD);
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

    // setStandard(colorW << 24 |colorR << 16 | colorG << 8 | colorB);
    // // setBrightness(colorW);
    // setLampMode(BLINKER_LAMP_STANDARD);

    BlinkerDuerOS.color(color);
    BlinkerDuerOS.print();
}

void duerMode(const String & mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_DUEROS_READING) {
        // Your mode function
        colorR = 255; colorG = 255; colorB = 255;
        // setStandard(colorW << 24 | colorR << 16 | colorG << 8 | colorB);
        // // setBrightness(colorW);
        // setLampMode(BLINKER_LAMP_STANDARD);
    }
    else if (mode == BLINKER_CMD_DUEROS_ALARM) {
        // Your mode function
        // setBrightness(colorW);
        // setLampMode(BLINKER_LAMP_RAINBOW_CYCLE);
    }
    else if (mode == BLINKER_CMD_DUEROS_SLEEP) {
        // Your mode function
        // setLampMode(BLINKER_LAMP_BREATH);
    }
    else if (mode == BLINKER_CMD_DUEROS_NIGHT_LIGHT) {
        // Your mode function
        // setLampMode(BLINKER_LAMP_RAINBOW);
    }
    else if (mode == BLINKER_CMD_DUEROS_ROMANTIC) {
        // Your mode function
        // setLampMode(BLINKER_LAMP_GRADIENT);
    }
    else {
        // Your mode function
        // setStandard(colorR << 16 | colorG << 8 | colorB);
        // setBrightness(colorW);
        // setLampMode(BLINKER_LAMP_STANDARD);
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
    // Blinker.print(BLINKER_CMD_LAMP_MODE, getMode());
    // Blinker.print(BLINKER_CMD_LAMP_SPEED, getSpeed());
    Blinker.print(BLINKER_CMD_LAMP_BRIGHT, getBrightness());
    // Blinker.print(BLINKER_CMD_SWITCH, getBrightness() ? BLINKER_CMD_ON : BLINKER_CMD_OFF);
    // if (getMode() != BLINKER_CMD_LAMP_RAINBOW_CYCLE)
    // {
    //     if (getMode() == BLINKER_CMD_LAMP_GRADIENT)
    //     {
    //         Blinker.printArray(BLINKER_CMD_GRADIENT_COLOR, getColor());
    //     }
    //     else
    //     {
    //         Blinker.printArray(BLINKER_CMD_LAMP_COLOR, getColor());
    //     }
    // }
    
    // if (getMode() == BLINKER_CMD_LAMP_SUNLIGHT)
    // {
        Blinker.print(BLINKER_CMD_LAMP_TEMP, sunSet);
    // }

    BLINKER_LOG("heartbeat!");
}

#if defined(BLINKER_BUTTON)
/* 
 * Blinker provide a button parse function for user if you defined BLINKER_BUTTON
 * 
 * Blinker button can detect singal click/ double click/ long press
 * 
 * Blinker.tick() will run by default, use interrupt will be better
 */
void buttonTick()
{
    Blinker.tick();
}

/* 
 * Add your code in this function
 * 
 * When button clicked, device will call this function
 */
void singalClick()
{
    modeChange();
    
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
    // rainbowCycle();

    BLINKER_LOG("Button long press start!");
}

/* 
 * Add your code in this function
 * 
 * When long press stop, device will call this function
 */
void longPressStop()
{
    isLongPress = false;

    BLINKER_LOG("Button long press stop!");
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
    BLINKER_LOG("Button long press reset!");
}
#endif

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
//         // BLINKER_LOG_FreeHeap();

//         batFresh = millis();

//         if (batRead < BLINKER_BAT_POWER_LOW * 10) {
//             digitalWrite(BLINKER_POWER_3V3_PIN, LOW);
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

    // touchInit();

    // batRead = getBAT() * 10;

    analogWriteRange(255);
    analogWriteFreq(5000);
    analogWrite(BLINKER_LAMP_NIGHT_LED_PIN, 0);
    analogWrite(BLINKER_LAMP_WHITE_LED_PIN, 0);
    analogWrite(BLINKER_LAMP_YELLOW_LED_PIN, 0);
    pinMode(BLINKER_LAMP_NIGHT_LED_PIN, OUTPUT);
    pinMode(BLINKER_LAMP_WHITE_LED_PIN, OUTPUT);
    pinMode(BLINKER_LAMP_YELLOW_LED_PIN, OUTPUT);
}

void LAMP_init()
{
    Serial.begin(115200);

    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();

    hardwareInit();
    ledInit();
    
    Blinker.begin(BLINKER_SMART_LAMP);

    Blinker.attachParse(dataParse);
    Blinker.attachHeartbeat(heartbeat);

#if defined(BLINKER_BUTTON)
    Blinker.attachClick(singalClick);
    Blinker.attachDoubleClick(doubleClick);
    Blinker.attachLongPressStart(longPressStart);
    Blinker.attachLongPressPowerdown(longPressPowerdown);
    Blinker.attachLongPressReset(longPressReset);

    attachInterrupt(BLINKER_BUTTON_PIN, buttonTick, CHANGE);
#endif    

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

    if (isLongPress) {
        // resetDisplay(Blinker.pressedTime());
    }
    else {
        ledRun();
    }

    // touchTick();
    // batCheck();
}
