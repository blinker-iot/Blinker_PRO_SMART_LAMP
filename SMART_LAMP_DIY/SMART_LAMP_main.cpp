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
// #define BLINKER_WIFI
// #define BLINKER_ESP_SMARTCONFIG
#define BLINKER_DEBUG_ALL

#include <Blinker.h>

char auth[] = "c64a7289ui86";
char ssid[] = "有没有wifi";
char pswd[] = "i8888888";

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

#include <modules/OneButton/OneButton.h>

#define BLINKER_BUTTON_PIN 12

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

void button1_callback(const String & state)
{
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // BLINKER_LOG2("get button state: ", state);
    // Blinker.beginFormat();

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

    // Blinker.endFormat();

    setLampMode(BLINKER_LAMP_RAINBOW_CYCLE);
}

void button2_callback(const String & state)
{
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    // BLINKER_LOG2("get button state: ", state);
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
    // BLINKER_LOG2("get button state: ", state);
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
    // BLINKER_LOG2("get button state: ", state);
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
    // BLINKER_LOG2("get button state: ", state);
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
    // BLINKER_LOG2("get button state: ", state);
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
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG2("get slider value: ", value);

    setSpeed(value);
}

void rgb1_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG2("R value: ", r_value);
    BLINKER_LOG2("G value: ", g_value);
    BLINKER_LOG2("B value: ", b_value);
    BLINKER_LOG2("Rrightness value: ", bright_value);

    setStandard(bright_value <<24 | r_value << 16 | g_value << 8 | b_value);
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

    BLINKER_LOG1("heartbeat!");
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
    isLongPress = true;
    press_start_time = millis();

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
    ESP.restart();

    BLINKER_LOG1("Button long press reset!");
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

    BLINKER_LOG1("Button long press stop!");
}

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

        // fas fa-battery-full
        if (batRead >= 41) batNumber.icon("fas fa-battery-full");
        else if (batRead >= 40) batNumber.icon("fas fa-battery-three-quarters");
        else if (batRead >= 39) batNumber.icon("fas fa-battery-half");
        else if (batRead >= 38) batNumber.icon("fas fa-battery-quarter");
        else batNumber.icon("fas fa-battery-empty");
        
        batNumber.color("#00FF00");
        batNumber.unit("v");
        batNumber.text("电量");
        batNumber.print(batRead / 10.0);
        // BLINKER_LOG_FreeHeap();

        batFresh = millis();

        if (batRead < BLINKER_BAT_POWER_LOW * 10) {
            // digitalWrite(BLINKER_POWER_3V3_PIN, LOW);
        }
    }
}

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

    batRead = getBAT() * 10;
}

void LAMP_init()
{
    Serial.begin(115200);

    hardwareInit();
    ledInit();
    
    Blinker.begin(auth, ssid, pswd);
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

    button.attachClick(singalClick);
    button.attachDoubleClick(doubleClick);
    button.attachLongPressStart(longPressStart);
    button.attachLongPressStop(longPressStop);

    attachInterrupt(BLINKER_BUTTON_PIN, buttonTick, CHANGE);

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
