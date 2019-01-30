/* *****************************************************************
 *
 * Download latest Blinker library here:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * 
 * Blinker is a cross-hardware, cross-platform solution for the IoT. 
 * It provides APP, device and server support, 
 * and uses public cloud services for data transmission and storage.
 * It can be used in smart home, data monitoring and other fields 
 * to help users build Internet of Things projects better and faster.
 * 
 * Make sure installed 2.5.0 or later ESP8266/Arduino package,
 * if use ESP8266 with Blinker.
 * https://github.com/esp8266/Arduino/releases
 * 
 * Docs: https://doc.blinker.app/
 *       https://github.com/blinker-iot/blinker-doc/wiki
 * 
 * *****************************************************************
 * 
 * Blinker 库下载地址:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
 * 服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
 * 数据监测等领域，可以帮助用户更好更快地搭建物联网项目。
 * 
 * 如果使用 ESP8266 接入 Blinker,
 * 请确保安装了 2.5.0 或更新的 ESP8266/Arduino 支持包。
 * https://github.com/esp8266/Arduino/releases
 * 
 * 文档: https://doc.blinker.app/
 *       https://github.com/blinker-iot/blinker-doc/wiki
 * 
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_ALIGENIE_LIGHT
#define BLINKER_DUEROS_LIGHT
#define BLINKER_ESP_SMARTCONFIG

#include <Blinker.h>

char auth[] = "01abfde74f1c";

uint8_t colorW = 255;
bool wsState = false;
String wsMode = BLINKER_CMD_COMMON;
int breath_step = 0;
uint8_t mode_type = 0;

BlinkerButton power("power");
BlinkerButton mode("mode");

void common()
{
    if (!wsState) return;

    analogWrite(LED_BUILTIN, 255);
}

void blink()
{
    if (!wsState) return;

    analogWrite(LED_BUILTIN, 255);
    delay(1000);
    analogWrite(LED_BUILTIN, 0);
    delay(1000);
}

void breath()
{
    if (!wsState) return;

    int lum = breath_step;

    if (lum > 255) lum = 511 - lum;

    analogWrite(LED_BUILTIN, lum);

    breath_step += 1;

    if (breath_step > (512 - 40)) breath_step = 40;
}

void power_callback(const String & state)
{
    BLINKER_LOG("get power button state: ", state);
    // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    wsState = !wsState;

    if (wsState)
    {
        analogWrite(LED_BUILTIN, 255);
    }
    else
    {
        analogWrite(LED_BUILTIN, 0);
    }
    
    BLINKER_LOG("power state: ", wsState);
}

void mode_callback(const String & state)
{
    BLINKER_LOG("get mode button state: ", state);

    mode_type = (mode_type + 1) % 3;

    wsState = true;

    BLINKER_LOG("mode type: ", mode_type);
}

void aligeniePowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {

        BlinkerAliGenie.powerState("on");
        BlinkerAliGenie.print();

        wsState = true;
    }
    else if (state == BLINKER_CMD_OFF) {

        BlinkerAliGenie.powerState("off");
        BlinkerAliGenie.print();

        wsState = false;

        analogWrite(LED_BUILTIN, 0);
    }
}

void aligenieMode(const String & mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_ALIGENIE_SLEEP) {
        // Your mode function
        mode_type = 2;
    }
    else if (mode == BLINKER_CMD_ALIGENIE_HOLIDAY) {
        // Your mode function
        mode_type = 1;
    }
    else if (mode == BLINKER_CMD_ALIGENIE_COMMON) {
        // Your mode function
        mode_type = 0;
    }

    wsMode = mode;
    wsState = true;

    BlinkerAliGenie.mode(mode);
    BlinkerAliGenie.print();
}

void aligeniecMode(const String & cmode)
{
    BLINKER_LOG("need cancel mode: ", cmode);

    if (cmode == BLINKER_CMD_ALIGENIE_READING) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_ALIGENIE_MOVIE) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_ALIGENIE_SLEEP) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_ALIGENIE_HOLIDAY) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_ALIGENIE_MUSIC) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_ALIGENIE_COMMON) {
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

    BlinkerAliGenie.brightness(bright);
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
            BlinkerAliGenie.mode(wsMode);
            BlinkerAliGenie.brightness(colorW);
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("AliGenie Query Power State");
            BlinkerAliGenie.powerState(wsState ? "on" : "off");
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_MODE_NUMBER :
            BLINKER_LOG("AliGenie Query Mode");
            BlinkerAliGenie.mode(wsMode);
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER :
            BLINKER_LOG("AliGenie Query Brightness");
            BlinkerAliGenie.brightness(colorW);
            BlinkerAliGenie.print();
            break;
        default :
            BlinkerAliGenie.powerState(wsState ? "on" : "off");
            BlinkerAliGenie.mode(wsMode);
            BlinkerAliGenie.brightness(colorW);
            BlinkerAliGenie.print();
            break;
    }
}

void duerPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {

        BlinkerDuerOS.powerState("on");
        BlinkerDuerOS.print();

        wsState = true;
    }
    else if (state == BLINKER_CMD_OFF) {

        BlinkerDuerOS.powerState("off");
        BlinkerDuerOS.print();

        wsState = false;

        analogWrite(LED_BUILTIN, 0);
    }
}

void duerMode(const String & mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_DUEROS_SLEEP) {
        // Your mode function
        mode_type = 2;
    }
    else if (mode == BLINKER_CMD_DUEROS_NIGHT_LIGHT) {
        // Your mode function
        mode_type = 0;
    }
    else if (mode == BLINKER_CMD_DUEROS_ROMANTIC) {
        // Your mode function
        mode_type = 1;
    }

    wsMode = mode;
    wsState = true;

    BlinkerDuerOS.mode(mode);
    BlinkerDuerOS.print();
}

void duercMode(const String & cmode)
{
    BLINKER_LOG("need cancel mode: ", cmode);

    if (cmode == BLINKER_CMD_DUEROS_READING) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_DUEROS_SLEEP) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_DUEROS_ALARM) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_DUEROS_NIGHT_LIGHT) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_DUEROS_ROMANTIC) {
        // Your mode function
    }
    else if (cmode == BLINKER_CMD_DUEROS_READING) {
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

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    Blinker.print(BlinkerTime);
    Blinker.print("millis", BlinkerTime);
}

void setup()
{
    Serial.begin(115200);    
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();

    analogWriteRange(255);
    analogWriteFreq(5000);

    pinMode(LED_BUILTIN, OUTPUT);
    // digitalWrite(LED_BUILTIN, LOW);
    analogWrite(LED_BUILTIN, 0);

    //Blinker.begin(auth, ssid, pswd);
    Blinker.begin(auth);
    Blinker.attachData(dataRead);
    
    BlinkerAliGenie.attachPowerState(aligeniePowerState);
    BlinkerAliGenie.attachMode(aligenieMode);
    BlinkerAliGenie.attachCancelMode(aligeniecMode);
    BlinkerAliGenie.attachBrightness(aligenieBright);
    BlinkerAliGenie.attachRelativeBrightness(aligenieRelativeBright);
    BlinkerAliGenie.attachQuery(aligenieQuery);    
    
    BlinkerDuerOS.attachPowerState(duerPowerState);
    BlinkerDuerOS.attachMode(duerMode);
    BlinkerDuerOS.attachCancelMode(duercMode);
    BlinkerDuerOS.attachBrightness(duerBright);
    BlinkerDuerOS.attachRelativeBrightness(duerRelativeBright);
    BlinkerDuerOS.attachQuery(duerQuery);

    colorW = 0;
    wsState = true;

    power.attach(power_callback);
    mode.attach(mode_callback);
}

void loop()
{
    Blinker.run();

    switch (mode_type)
    {
        case 0:
            common();
            break;
        case 1:
            blink();
            break;
        case 2:
            breath();
            break;
        default:
            common();
            break;
    }
}