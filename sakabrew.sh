#!/usr/bin/env bash

if [ ! -d "./build" ];then
mkdir ./build
else
echo "build文件夹已经存在"
fi
cd ./build
echo "进入build文件夹，即将清除文件缓存"
rm -rf ./*
echo "build文件夹缓存清理完毕，即将执行外部构建"
cmake ..
echo "外部构建执行完成"

if [ ! -d "CPackSourceConfig.cmake" ];then
echo "未找到打包文件，请重新执行此脚本"
else
echo "已生成打包文件，即将开始打包"

cpack --configure CPackSourceConfig.cmake
fi