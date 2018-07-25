#!/usr/bin/env bash
# 寻找build文件夹，不存在就创建
function findBuild(){
    if [ ! -d "./build" ];then
        mkdir ./build
    else
        echo "build文件夹已经存在"
    fi
}

# 进入build文件夹，清除所有的缓存文件
function cmakeOutBuild(){
    cd ./build
    echo "进入build文件夹，即将清除文件缓存"
    rm -rf ./*
    echo "build文件夹缓存清理完毕，即将执行外部构建"
    cmake ..
    echo "外部构建执行完成"
}
# 打包文件
function cmakePackage(){
    cpack --config CPackSourceConfig.cmake
}

# 读取日志文件，读入文件名称
function readLogFile(){
    if [ -e "logfile" ];then
        tmp_path=`echo $(pwd) | sed -n "s#/#\\\\\/#gp"`
        echo "$tmp_path"
        package_name=`sed -n '$p' logfile |sed -n "s/.*CPack:.*\($tmp_path.*tar\.gz\).*generated./\1/gp"`
        package_version=`echo "${package_name}" | sed -n 's/.*pack\/\(.*\)\.tar\.gz/\1/gp'|sed -n 's/\./\\\\./gp'`
        echo "---name:${package_name}\n---version:${package_version}\n"
    else
        echo "file not found"
    fi
}

# 本地提交git仓库，然后推送至远程仓库
function commitToGitHub(){
    if [ ! -n "$package_name" ];then
        echo "不能提交"
    else
        git add ${package_name}
        git commit -m "new version"
        git push
    fi
}

# 本地提交至brew仓库，远程提交
function commitToBrew(){
    if [ ! -n "$package_name" ];then
        echo "不能提交到brew"
    else
        fileSHA256=`openssl dgst -sha256 ${package_name}|sed -n "s/.*= \(.*\)/\1/gp"`
        echo "$fileSHA256"
        cd $(brew --repo rangaofei/saka)/Formula
        sed -i -n "s/sweather-[0-9]\.[0-9]\.[0-9]/sweather-1\.3\.1/g" sweather.rb
        sed -i -n "s/sha256 \".*\"/sha256 \"$fileSHA256\"/g" sweather.rb
        git add sweather.rb
        git commit -m "new version:$package_version"
        git push
        cd -

    fi

}
findBuild
cmakeOutBuild

if [ ! -e "CPackSourceConfig.cmake" ];then
    echo "未找到打包文件，请重新执行此脚本"
else
    echo "已生成打包文件，即将开始打包"
    name=`cmakePackage`
    echo "------------------------------"
    echo "$name"| tee -a ../logfile
fi
cd ..
readLogFile
commitToGitHub
commitToBrew

