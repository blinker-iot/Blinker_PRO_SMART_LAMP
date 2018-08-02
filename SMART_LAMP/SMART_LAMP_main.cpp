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
#define BLINKER_BUTTON_PIN 12

#include <Blinker.h>

#include "SMART_LAMP_control.h"

static bool inited = false;
static uint8_t batRead;
static uint8_t batBase;
static uint32_t batFresh = 0;

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
    BLINKER_LOG2("Get user command: ", getData);

    bool isParsed = false;

    if (data.containsKey(BLINKER_CMD_SET)) {
        String setData = data[BLINKER_CMD_SET];

        DynamicJsonBuffer jsonBuffer;
        JsonObject& setJson = jsonBuffer.parseObject(setData);
    }
    else if (data.containsKey(BLINKER_CMD_GET)) {
        String getData = data[BLINKER_CMD_GET];

        DynamicJsonBuffer jsonBuffer;
        JsonObject& getJson = jsonBuffer.parseObject(getData);
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
    BLINKER_LOG1("heartbeat!");
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
    
    BLINKER_LOG1("Button clicked!");
}

/* 
 * Add your code in this function
 * 
 * When button double clicked, device will call this function
 */
void doubleClick()
{
    setBrightness(random(0, 255));
    
    BLINKER_LOG1("Button double clicked!");
}

/* 
 * Add your code in this function
 * 
 * When long press start, device will call this function
 */
void longPressStart()
{
    rainbowDisplay1();

    BLINKER_LOG1("Button long press start!");
}

/* 
 * Add your code in this function
 * 
 * When long press stop and trigged POWERDOWN, device will call this function
 */
void longPressPowerdown()
{
    BLINKER_LOG1("Button long press powerdown!");

    digitalWrite(BLINKER_POWER_3V3_PIN, LOW);
}

/* 
 * Add your code in this function
 * 
 * When long press stop and trigged RESET, device will call this function
 */
void longPressReset()
{
    BLINKER_LOG1("Button long press reset!");
}
#endif

double getBAT()
{
    int sensorValue = analogRead(A0);
    sensorValue += analogRead(A0);
    sensorValue += analogRead(A0);
    sensorValue += analogRead(A0);
    sensorValue += analogRead(A0);
    sensorValue += analogRead(A0);
    sensorValue += analogRead(A0);
    sensorValue += analogRead(A0);

    double voltage = sensorValue * (5.926 / 1023.0 / 8.0);

    // BLINKER_LOG2("bat: ", voltage);

    return voltage;
}

void batCheck()
{
    if ((millis() - batFresh) > BLINKER_BAT_CHECK_TIME)
    {
        batRead = getBAT() * 10;

        if (batBase - batRead > BLINKER_BAT_POWER_USEUP * 10) {
            batBase = batRead;
            BLINKER_ERR_LOG1("BLINKER_BAT_POWER_USEUP");
            BLINKER_LOG6("batBase: ", batBase / 10.0, " v", ", batRead: ", batRead / 10.0, " v");
        }
        else {
            if (batRead > batBase) batBase = batRead;
        }
        
        BLINKER_LOG3("bat: ", batRead / 10.0, " v");
        // BLINKER_LOG_FreeHeap();

        batFresh = millis();

        if (batRead < BLINKER_BAT_POWER_LOW * 10) {
            digitalWrite(BLINKER_POWER_3V3_PIN, LOW);
        }
    }
}

void hardwareInit()
{
    pinMode(BLINKER_POWER_3V3_PIN, OUTPUT);
    digitalWrite(BLINKER_POWER_3V3_PIN, HIGH);
    pinMode(BLINKER_POWER_5V_PIN, OUTPUT);
    digitalWrite(BLINKER_POWER_5V_PIN, HIGH);

    batRead = getBAT() * 10;
}

void LAMP_init()
{
    Serial.begin(115200);

    hardwareInit();
    ledInit();
    
    Blinker.begin(BLINKER_AIR_DETECTOR);

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
}

void LAMP_run()
{
    Blinker.run();

    ledRun();
    batCheck();
}
