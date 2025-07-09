# 打开命令行，到此文件所在文件夹下
# 输入命令：
#   mkdir build  # 新建build文件夹
#   cmake -S . -B build
#   cmake --build build

cmake_minimum_required(VERSION 3.5)
project(UvaJudge)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(sourceDir src)

add_executable(judge100_the_3n_plus_1_problem ${sourceDir}/judge100_the_3n_plus_1_problem.cpp)
add_executable(judge10137_the_trip ${sourceDir}/judge10137_the_trip.cpp)
