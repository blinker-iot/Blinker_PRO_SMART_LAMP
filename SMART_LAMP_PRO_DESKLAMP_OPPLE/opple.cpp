// /* *****************************************************************

//    Download latest Blinker library here:
//    https://github.com/blinker-iot/blinker-library/archive/master.zip


//    Blinker is a cross-hardware, cross-platform solution for the IoT.
//    It provides APP, device and server support,
//    and uses public cloud services for data transmission and storage.
//    It can be used in smart home, data monitoring and other fields
//    to help users build Internet of Things projects better and faster.

//    Make sure installed 2.5.0 or later ESP8266/Arduino package,
//    if use ESP8266 with Blinker.
//    https://github.com/esp8266/Arduino/releases

//    Docs: https://doc.blinker.app/
//          https://github.com/blinker-iot/blinker-doc/wiki

//  * *****************************************************************

//    Blinker 库下载地址:
//    https://github.com/blinker-iot/blinker-library/archive/master.zip

//    Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
//    服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
//    数据监测等领域，可以帮助用户更好更快地搭建物联网项目。

//    如果使用 ESP8266 接入 Blinker,
//    请确保安装了 2.5.0 或更新的 ESP8266/Arduino 支持包。
//    https://github.com/esp8266/Arduino/releases

//    文档: https://doc.blinker.app/
//          https://github.com/blinker-iot/blinker-doc/wiki

//  * *****************************************************************/

// #define BLINKER_WIFI

// #include <Blinker.h>

// //char auth[] = "f85bc0aab801";
// char auth[] = "91b094605887";
// char ssid[] = "mostfun";
// char pswd[] = "18038083873";

// #define Slider_1 "F"
// #define Slider_2 "p2"
// #define Slider_3 "p4"
// #define Slider_4 "p5"



// #define PWM_PIN1 2
// #define PWM_PIN2 4
// #define PWM_PIN3 5
// int32_t PWM1;
// int32_t PWM2;
// int32_t PWM3;

// BlinkerSlider Slider1(Slider_1);
// BlinkerSlider Slider2(Slider_2);
// BlinkerSlider Slider3(Slider_3);
// BlinkerSlider Slider4(Slider_4);

// void slider1_callback(int32_t value)
// {
// //  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//   BLINKER_LOG("get pwm Freq: ", value);
//   analogWriteFreq(value);
//   analogWrite(PWM_PIN1, PWM1);
//   analogWrite(PWM_PIN2, PWM2);
//   analogWrite(PWM_PIN3, PWM3);
// }

// void slider2_callback(int32_t value)
// {
// //  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//   BLINKER_LOG("get slider value: ", value);
//   PWM1 = value;
//   analogWrite(PWM_PIN1, PWM1);
// }

// void slider3_callback(int32_t value)
// {
// //  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//   BLINKER_LOG("get slider value: ", value);
//   PWM2 = value;
//   analogWrite(PWM_PIN2, PWM2);
// }

// void slider4_callback(int32_t value)
// {
// //  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//   BLINKER_LOG("get slider value: ", value);
//   PWM3 = value;
//   analogWrite(PWM_PIN3, PWM3);
// }


// void dataRead(const String & data)
// {
//   BLINKER_LOG("Blinker readString: ", data);

//   Blinker.vibrate();

//   uint32_t BlinkerTime = millis();
//   Blinker.print(BlinkerTime);
//   Blinker.print("millis", BlinkerTime);

//   Slider1.color("#FFFFFF");
//   Slider1.print(random(0, 128));
// }

// void setup()
// {
//   Serial.begin(115200);
//   BLINKER_DEBUG.stream(Serial);
//   Blinker.begin(auth, ssid, pswd);
//   Blinker.attachData(dataRead);
//   analogWriteRange(255);
//   analogWriteFreq(500);
//   analogWrite(PWM_PIN1, 0);
//   analogWrite(PWM_PIN2, 0);
//   analogWrite(PWM_PIN3, 0);
//   pinMode(PWM_PIN1, OUTPUT);
//   pinMode(PWM_PIN2, OUTPUT);
//   pinMode(PWM_PIN3, OUTPUT);
  
//   Slider1.attach(slider1_callback);
//   Slider2.attach(slider2_callback);
//   Slider3.attach(slider3_callback);
//   Slider4.attach(slider4_callback);
// }

// void loop()
// {
//   Blinker.run();
// }
