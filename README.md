# SWeather
一个非常简单的命令行客户端，接口采用的是和风天气接口，感谢。
### 1. 安装

> 本项目目前只支持homebrew获取方式。

首先获取仓库
```
brew tap rangaofei/saka
```

![](https://raw.githubusercontent.com/rangaofei/SWeather/master/PIC/1.png)

然后安装该项目
```
brew install sweather
```

![](https://raw.githubusercontent.com/rangaofei/SWeather/master/PIC/2.png)
### 2. 使用

2.1 设置地区

目前支持到区县级别
```
sweather --set-location <area/city>
```

![](https://raw.githubusercontent.com/rangaofei/SWeather/master/PIC/3.png)
2.2 查看天气
```
sweather [-saddress]
```
显示包括七天天气、实况天气、逐小时预报、生活指数、更新时间等信息。
![](https://raw.githubusercontent.com/rangaofei/SWeather/master/PIC/5.png)
直接输入sweather显示默认地区的天气。后边跟上开头不是'-'字符串，直接搜索对应地址的天气

![](https://raw.githubusercontent.com/rangaofei/SWeather/master/PIC/6.png)

![](https://raw.githubusercontent.com/rangaofei/SWeather/master/PIC/7.png)
2.2 查看七天天气

```
sweather -f
```

2.3 查看当前天气实况
```
sweather -n
```
2.4 查看天气指数
```
sweather -l
```
2.5 查看逐小时天气预报
```
sweather -h
```
