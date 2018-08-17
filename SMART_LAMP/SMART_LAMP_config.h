#ifndef BLINKER_LAMP_CONFIG_H
#define BLINKER_LAMP_CONFIG_H

#include <Arduino.h>

#define BLINKER_WS2812_PIN              4
#define BLINKER_WS2812_COUNT            9

#define BLINKER_POWER_3V3_PIN           14
#define BLINKER_POWER_5V_PIN            15

#define BLINKER_IIC_SCK_PIN             2
#define BLINKER_IIC_SDA_PIN             0

#define BLINKER_BAT_POWER_LOW           3.5
#define BLINKER_BAT_POWER_HIGH          4.0
#define BLINKER_BAT_POWER_USEUP         0.2
#define BLINKER_BAT_CHECK_TIME          10000UL

#define BLINKER_LAMP_COLOR_R            0
#define BLINKER_LAMP_COLOR_G            1
#define BLINKER_LAMP_COLOR_B            2
#define BLINKER_LAMP_COLOR_BRIGHTNESS   3

#define BLINKER_LAMP_TYPE_COUNT         6

#define BLINKER_LAMP_RAINBOW_CYCLE      0
#define BLINKER_LAMP_RAINBOW            1
#define BLINKER_LAMP_STREAMER           2
#define BLINKER_LAMP_STANDARD           3
#define BLINKER_LAMP_BREATH             4
#define BLINKER_LAMP_RAINBOW_STROBE     5

#define BLINKER_CMD_LAMP_RAINBOW_CYCLE  "raic"
#define BLINKER_CMD_LAMP_RAINBOW        "rai"
#define BLINKER_CMD_LAMP_RAINBOW_STROBE "rais"
#define BLINKER_CMD_LAMP_STANDARD       "std"
#define BLINKER_CMD_LAMP_BREATH         "bre"
#define BLINKER_CMD_LAMP_STREAMER       "stm"

#define BLINKER_CMD_LAMP_MODE           "mode"
#define BLINKER_CMD_LAMP_COLOR          "clr"
#define BLINKER_CMD_LAMP_RUN            "run"
#define BLINKER_CMD_LAMP_SPEED          "spd"

#define BLINKER_CMD_STREAMER_COLOR      "stmc"
#define BLINKER_CMD_STREAMER_COLOR0     "stmc0"
#define BLINKER_CMD_STREAMER_COLOR1     "stmc1"
#define BLINKER_CMD_STREAMER_COLOR2     "stmc2"
#define BLINKER_CMD_STREAMER_COLOR3     "stmc3"

#define BLINKER_LAMP_SPEED_DEFUALT      5000UL
#define BLINKER_LAMP_SPEED_MIN          1000UL
#define BLINKER_LAMP_SPEED_MAX          20000UL

typedef void (*callback_with_uint32_arg_t)(uint32_t data);


// - - - - - - - - | - - - - - - - - | - - - - - - - - | - - - - - - - -
//
//
//

#define BLINKER_EEP_ADDR_LAMP_STATE     2432
#define BLINKER_LAMP_STATE_SIZE         4

#endif