# SWeather
一个非常简单的命令行客户端，接口采用的是和风天气接口，感谢。
1. 安装

> 本项目目前只支持homebrew获取方式。

首先获取仓库
```
brew tap rangaofei/saka
```
然后安装该项目
```
brew install sweather
```
2.使用
2.1 设置地区

目前支持到区县级别
```
sweather -setloc <area/city>
```
2.2 查看天气
```
sweather [address]
```
显示包括七天天气、实况天气、逐小时预报、生活指数、更新时间等信息。

直接输入sweather显示默认地区的天气。后边跟上开头不是'-'字符串，直接搜索对应地址的天气
2.2 查看七天天气

```
sweather -forecast
```

2.3 查看当前天气实况
```
sweather -now
```
2.4 查看天气指数
```
sweather -lifestyle
```
2.5 查看逐小时天气预报
```
sweather -hourly
```
2.6 查看地址信息
````
sweather -loc [address]
````
直接输入-loc显示默认地址信息，后边接地址显示所有包含该地址的信息