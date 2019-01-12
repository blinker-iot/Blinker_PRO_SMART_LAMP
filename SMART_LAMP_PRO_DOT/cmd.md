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
        "swi":"on"
    }
}
```
## SET STATUS
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
## SET BRIGHT
```
{
    "brt":100
}
```
> **mode:**
> BLINKER_CMD_LAMP_RAINBOW_CYCLE  "rc"  
> BLINKER_CMD_LAMP_RAINBOW        "rb"  
> BLINKER_CMD_LAMP_STROBE         "stb"  
> BLINKER_CMD_LAMP_STANDARD       "std"  
> BLINKER_CMD_LAMP_BREATH         "bre"  
> BLINKER_CMD_LAMP_GRADIENT       "grd"  
> BLINKER_CMD_LAMP_SUNLIGHT       "sun"

### LAMP_GRADIENT
除GRADIENT(多色流光渐变, 渐变到设定颜色时保持一定时间)外, 可以不设置 **run** (STATUS)  
> 暂定四色流光渐变  

在设置GRADIENT的多色时需将 STATUS 置0, 用户方便观察设定颜色  
```
{
    "set":{
        "mode":"grd",
        "run":0,
        "grdc1":[255,255,255]
    }
}
```
> GRADIENT显示亮度暂定不可调  
> grdc0/grdc1/grdc2/grdc3 对应流光显示颜色  

设置完成:
```
{
    "set":{
        "mode":"grd",
        "run":1,
        "spd":100
    }
}
```
或者:
```
{
    "set":{
        "mode":"grd",
        "run":1,
        "spd":100,
        "grdc":[[255,255,255],[255,255,255],[255,255,255],[255,255,255]]
    }
}
```