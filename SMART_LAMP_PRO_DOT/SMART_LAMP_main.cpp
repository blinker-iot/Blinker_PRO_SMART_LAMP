/* 
 * BLINKER_PRO_ESP is use for professional device
 * 
 * Please make sure you have permission to modify professional device!
 * Please read usermanual first! Thanks!
 * https://doc.blinker.app/
 * 
 * Written by i3water for blinker.
 * Learn more:https://blinker.app/
 */

#define BLINKER_PRINT Serial
#define BLINKER_PRO_ESP
#define BLINKER_DEBUG_ALL

#define BLINKER_BUTTON
#define BLINKER_BUTTON_PULLDOWN
#define BLINKER_BUTTON_LONGPRESS_POWERDOWN
#define BLINKER_BUTTON_PIN 12

#include <Blinker.h>

#include "SMART_LAMP_control.h"

char type[] = "Your Device Type";
char auth[] = "Your Device Secret Key";

static bool inited = false;
static bool isLongPress = false;
// static bool powerState = false;
static uint8_t batRead;
static uint8_t batBase;
static uint8_t sunSet = 0;
static uint32_t batFresh = 0;
static uint32_t lamp_bright;

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

        // DynamicJsonBuffer jsonBuffer;
        // JsonObject& setJson = jsonBuffer.parseObject(setData);

        DynamicJsonDocument jsonBuffer(1024);
        DeserializationError error = deserializeJson(jsonBuffer, setData);
        JsonObject setJson = jsonBuffer.as<JsonObject>();

        // if (!setJson.success())
        if (error)
        {
            return false;
        }

        String setMode = setJson[BLINKER_CMD_LAMP_MODE];

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

        if (setMode == BLINKER_CMD_LAMP_RAINBOW_CYCLE)
        {
            setLampMode(BLINKER_LAMP_RAINBOW_CYCLE);
            isParsed = true;
        }
        else if (setMode == BLINKER_CMD_LAMP_RAINBOW)
        {
            setLampMode(BLINKER_LAMP_RAINBOW);
            isParsed = true;
        }
        else if (setMode == BLINKER_CMD_LAMP_STANDARD)
        {
            setLampMode(BLINKER_LAMP_STANDARD);
            
            if (setJson.containsKey(BLINKER_CMD_LAMP_COLOR))
            {
                uint8_t std_r = setJson[BLINKER_CMD_LAMP_COLOR][0];
                uint8_t std_g = setJson[BLINKER_CMD_LAMP_COLOR][1];
                uint8_t std_b = setJson[BLINKER_CMD_LAMP_COLOR][2];
                uint8_t std_w = setJson[BLINKER_CMD_LAMP_COLOR][3];
                
                setStandard(std_w << 24 | std_r << 16 | std_g << 8 | std_b);
            }

            isParsed = true;
        }
        else if (setMode == BLINKER_CMD_LAMP_BREATH)
        {
            setLampMode(BLINKER_LAMP_BREATH);
            
            if (setJson.containsKey(BLINKER_CMD_LAMP_COLOR))
            {
                uint8_t sun_r = setJson[BLINKER_CMD_LAMP_COLOR][0];
                uint8_t sun_g = setJson[BLINKER_CMD_LAMP_COLOR][1];
                uint8_t sun_b = setJson[BLINKER_CMD_LAMP_COLOR][2];
                uint8_t sun_w = setJson[BLINKER_CMD_LAMP_COLOR][3];
                
                setBreath(sun_w << 24 | sun_r << 16 | sun_g << 8 | sun_b);
            }

            isParsed = true;
        }
        else if (setMode == BLINKER_CMD_LAMP_STROBE)
        {
            setLampMode(BLINKER_LAMP_STROBE);
            
            if (setJson.containsKey(BLINKER_CMD_LAMP_COLOR))
            {
                uint8_t sun_r = setJson[BLINKER_CMD_LAMP_COLOR][0];
                uint8_t sun_g = setJson[BLINKER_CMD_LAMP_COLOR][1];
                uint8_t sun_b = setJson[BLINKER_CMD_LAMP_COLOR][2];
                uint8_t sun_w = setJson[BLINKER_CMD_LAMP_COLOR][3];
                
                setStrobe(sun_w << 24 | sun_r << 16 | sun_g << 8 | sun_b);
            }
            
            isParsed = true;
        }
        else if (setMode == BLINKER_CMD_LAMP_SUNLIGHT)
        {
            setLampMode(BLINKER_LAMP_SUNLIGHT);

            if (setJson.containsKey(BLINKER_CMD_LAMP_TEMP))
            {
                uint8_t sun_set = setJson[BLINKER_CMD_LAMP_TEMP];

                sunSet = sun_set;
                
                // uint32_t sun_color = map(sun_set, 0, 255, BLINKER_COLOR_TEMP_MIN, BLINKER_COLOR_TEMP_MAX);
                
                setSunlight(kelvin2RGB(sunSet));
            }
            
            isParsed = true;
        }
        else if (setMode == BLINKER_CMD_LAMP_GRADIENT)
        {
            uint8_t state = setJson[BLINKER_CMD_LAMP_RUN];
            setLampMode(BLINKER_LAMP_GRADIENT, state);
            
            if (setJson.containsKey(BLINKER_CMD_GRADIENT_COLOR))
            {
                uint32_t stm_clr[BLINKER_MAX_GRADIENT_COUNT];
                uint8_t  clr_get;
                uint8_t  clr_count = 0;

                for (uint8_t num = 0; num < BLINKER_MAX_GRADIENT_COUNT; num++)
                {
                    if (setJson[BLINKER_CMD_GRADIENT_COLOR][num] == 0) break;
                    else clr_count++;

                    clr_get = setJson[BLINKER_CMD_GRADIENT_COLOR][num][0];
                    stm_clr[num] = clr_get;
                    clr_get = setJson[BLINKER_CMD_GRADIENT_COLOR][num][1];
                    stm_clr[num] = stm_clr[num] << 8 | clr_get;
                    clr_get = setJson[BLINKER_CMD_GRADIENT_COLOR][num][2];
                    stm_clr[num] = stm_clr[num] << 8 | clr_get;
                }

                setGradient(stm_clr, clr_count);
            }
            
            isParsed = true;
        }

        if (setJson.containsKey(BLINKER_CMD_LAMP_COLOR))
        {
            uint32_t set_clr;
            uint8_t  clr;

            clr = setJson[BLINKER_CMD_LAMP_COLOR][0];
            set_clr = clr;
            clr = setJson[BLINKER_CMD_LAMP_COLOR][1];
            set_clr = set_clr << 8 | clr;
            clr = setJson[BLINKER_CMD_LAMP_COLOR][2];
            set_clr = set_clr << 8 | clr;
            clr = setJson[BLINKER_CMD_LAMP_COLOR][3];
            set_clr = set_clr | clr << 24;

            setColor(set_clr);
        }

        if (setJson.containsKey(BLINKER_CMD_LAMP_SPEED))
        {
            uint8_t speed = setJson[BLINKER_CMD_LAMP_SPEED];

            setSpeed(speed);
        }

        if (setJson.containsKey(BLINKER_CMD_LAMP_BRIGHT))
        {
            uint8_t bright = setJson[BLINKER_CMD_LAMP_BRIGHT];

            setBrightness(bright);
        }
    }
    else if (data.containsKey(BLINKER_CMD_GET))
    {
        String getData = data[BLINKER_CMD_GET];

        // DynamicJsonBuffer jsonBuffer;
        // JsonObject& getJson = jsonBuffer.parseObject(getData);

        DynamicJsonDocument jsonBuffer(1024);
        DeserializationError error = deserializeJson(jsonBuffer, getData);
        JsonObject getJson = jsonBuffer.as<JsonObject>();

        // if (!getJson.success())
        if (error)
        {
            return false;
        }
    }    
    
    Blinker.print(BLINKER_CMD_LAMP_MODE, getMode());
    Blinker.print(BLINKER_CMD_LAMP_SPEED, getSpeed());
    Blinker.print(BLINKER_CMD_LAMP_BRIGHT, getBrightness());
    Blinker.print(BLINKER_CMD_SWITCH, getBrightness() ? BLINKER_CMD_ON : BLINKER_CMD_OFF);
    if (getMode() != BLINKER_CMD_LAMP_RAINBOW_CYCLE)
    {
        if (getMode() == BLINKER_CMD_LAMP_GRADIENT)
        {
            Blinker.printArray(BLINKER_CMD_GRADIENT_COLOR, getColor());
        }
        else
        {
            Blinker.printArray(BLINKER_CMD_LAMP_COLOR, getColor());
        }
    }
    
    if (getMode() == BLINKER_CMD_LAMP_SUNLIGHT)
    {
        Blinker.print(BLINKER_CMD_LAMP_TEMP, sunSet);
    }

    if (isParsed) {
        return true;
    }
    else {
        return false;
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
    Blinker.print(BLINKER_CMD_LAMP_MODE, getMode());
    Blinker.print(BLINKER_CMD_LAMP_SPEED, getSpeed());
    Blinker.print(BLINKER_CMD_LAMP_BRIGHT, getBrightness());
    Blinker.print(BLINKER_CMD_SWITCH, getBrightness() ? BLINKER_CMD_ON : BLINKER_CMD_OFF);
    if (getMode() != BLINKER_CMD_LAMP_RAINBOW_CYCLE)
    {
        if (getMode() == BLINKER_CMD_LAMP_GRADIENT)
        {
            Blinker.printArray(BLINKER_CMD_GRADIENT_COLOR, getColor());
        }
        else
        {
            Blinker.printArray(BLINKER_CMD_LAMP_COLOR, getColor());
        }
    }
    
    if (getMode() == BLINKER_CMD_LAMP_SUNLIGHT)
    {
        Blinker.print(BLINKER_CMD_LAMP_TEMP, sunSet);
    }

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
    rainbowCycle();

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

    digitalWrite(BLINKER_POWER_3V3_PIN, LOW);
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
    pinMode(BLINKER_POWER_3V3_PIN, OUTPUT);
    digitalWrite(BLINKER_POWER_3V3_PIN, HIGH);
    pinMode(BLINKER_POWER_5V_PIN, OUTPUT);
    digitalWrite(BLINKER_POWER_5V_PIN, HIGH);

    // touchInit();

    // batRead = getBAT() * 10;
}

void LAMP_init()
{
    Serial.begin(115200);

    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();

    hardwareInit();
    ledInit();
    
    Blinker.begin(auth, type);

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

    // attachDelay(delays);
}

void LAMP_run()
{
    Blinker.run();

    if (isLongPress) {
        resetDisplay(Blinker.pressedTime());
    }
    else {
        ledRun();
    }

    // touchTick();
    // batCheck();
}
