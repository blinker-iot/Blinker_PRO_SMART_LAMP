# SET CMD
```
{
    "set":{
        "mode":"std",
        "run":1,
        "clr":[255,255,255,255],
        "spd":100
    }
}
```
## SET MODE
```
{
    "set":{
        "mode":"std"
    }
}
```
## SET STATE
```
{
    "set":{
        "run":1
    }
}
```
## SET COLOR
```
{
    "set":{
        "clr":[255,255,255,255]
    }
}
```
## SET SPEED
```
{
    "spd":100
}
```
> **mode:**
> BLINKER_CMD_LAMP_RAINBOW_CYCLE  "raic"  
> BLINKER_CMD_LAMP_RAINBOW        "rai"  
> BLINKER_CMD_LAMP_RAINBOW_STROBE "rais"  
> BLINKER_CMD_LAMP_STANDARD       "std"  
> BLINKER_CMD_LAMP_BREATH         "bre"  
> BLINKER_CMD_LAMP_STREAMER       "stm"  

### LAMP_STREAMER
除streamer(多色流光渐变, 渐变到设定颜色时保持一定时间)外, 可以不设置 **run** (state)  
> 暂定四色流光渐变  

在设置streamer的多色时需将state置0, 用户方便观察设定颜色  
```
{
    "set":{
        "mode":"stm",
        "run":0,
        "stmc1":[255,255,255]
    }
}
```
> streamer显示亮度暂定不可调  
> stmc0/stmc1/stmc2/stmc3 对应流光显示颜色  

设置完成:
```
{
    "set":{
        "mode":"stm",
        "run":1,
        "spd":100
    }
}
```
或者:
```
{
    "set":{
        "mode":"stm",
        "run":1,
        "spd":100,
        "stmc":[[255,255,255],[255,255,255],[255,255,255],[255,255,255]]
    }
}
```