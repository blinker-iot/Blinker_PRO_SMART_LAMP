#ifndef BLINKER_LAMP_CONFIG_H
#define BLINKER_LAMP_CONFIG_H

#include <Arduino.h>

// #define BLINKER_BUTTON_ADD_PIN          2
// #define BLINKER_BUTTON_SUB_PIN          5
// #define BLINKER_BUTTON_EC0_PIN          0
// #define BLINKER_BUTTON_EC1_PIN          4

#define BLINKER_WS2812_PIN              2
#define BLINKER_WS2812_COUNT            16

// #define BLINKER_POWER_3V3_PIN           14
// #define BLINKER_POWER_5V_PIN            15

// #define BLINKER_IIC_SCK_PIN             2
// #define BLINKER_IIC_SDA_PIN             0

// #define BLINKER_BAT_POWER_LOW           3.5
// #define BLINKER_BAT_POWER_HIGH          4.0
// #define BLINKER_BAT_POWER_USEUP         0.2
// #define BLINKER_BAT_CHECK_TIME          10000UL

#define BLINKER_LAMP_NIGHT_LED_PIN      13
#define BLINKER_LAMP_WHITE_LED_PIN      12
#define BLINKER_LAMP_YELLOW_LED_PIN     14

#define BLINKER_LAMP_NIGHT_PWM_PIN      2
#define BLINKER_LAMP_WHITE_PWM_PIN      4
#define BLINKER_LAMP_YELLOW_PWM_PIN     5

#define BLINKER_LAMP_COLOR_R            0
#define BLINKER_LAMP_COLOR_G            1
#define BLINKER_LAMP_COLOR_B            2
#define BLINKER_LAMP_COLOR_BRIGHTNESS   3

#define BLINKER_LAMP_TYPE_COUNT         6

#define BLINKER_LAMP_RAINBOW_CYCLE      0
#define BLINKER_LAMP_RAINBOW            1
#define BLINKER_LAMP_GRADIENT           2
#define BLINKER_LAMP_STANDARD           3
#define BLINKER_LAMP_BREATH             4
#define BLINKER_LAMP_STROBE             5
#define BLINKER_LAMP_SUNLIGHT           6

#define BLINKER_CMD_LAMP_RAINBOW_CYCLE  "rc"
#define BLINKER_CMD_LAMP_RAINBOW        "rb"
#define BLINKER_CMD_LAMP_STROBE         "stb"
#define BLINKER_CMD_LAMP_STANDARD       "std"
#define BLINKER_CMD_LAMP_BREATH         "bre"
#define BLINKER_CMD_LAMP_GRADIENT       "grd"
#define BLINKER_CMD_LAMP_SUNLIGHT       "sun"

#define BLINKER_MAX_GRADIENT_COUNT      6

#define BLINKER_CMD_LAMP_MODE           "mode"
#define BLINKER_CMD_LAMP_COLOR          "clr"
#define BLINKER_CMD_LAMP_RUN            "run"
#define BLINKER_CMD_LAMP_SPEED          "spd"
#define BLINKER_CMD_LAMP_BRIGHT         "brt"
#define BLINKER_CMD_LAMP_TEMP           "temp"

#define BLINKER_CMD_GRADIENT_COLOR      "grdc"
#define BLINKER_CMD_GRADIENT_COLOR0     "grdc0"
#define BLINKER_CMD_GRADIENT_COLOR1     "grdc1"
#define BLINKER_CMD_GRADIENT_COLOR2     "grdc2"
#define BLINKER_CMD_GRADIENT_COLOR3     "grdc3"

#define BLINKER_LAMP_SPEED_DEFUALT      5000UL
#define BLINKER_LAMP_SPEED_MIN          1000UL
#define BLINKER_LAMP_SPEED_MAX          20000UL

#define BLINKER_COLOR_TEMP_MIN          0xFFA645
#define BLINKER_COLOR_TEMP_MAX          0xFFF9FB

typedef void (*callback_with_uint32_arg_t)(uint32_t data);


// - - - - - - - - | - - - - - - - - | - - - - - - - - | - - - - - - - -
//
//
//

#define BLINKER_EEP_ADDR_LAMP_STATE     2432
#define BLINKER_LAMP_STATE_SIZE         4

#endif