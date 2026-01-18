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
add_executable(judge10137_the_trip            ${sourceDir}/judge10137_the_trip.cpp)
add_executable(judge10142_australian_voting   ${sourceDir}/judge10142_australian_voting.cpp)


# 借鉴Google Test实现
# https://github.com/google/googletest/blob/9156d4caac880b513264ecbe0aa4746a3fead3d7/googletest/cmake/internal_utils.cmake#L273

find_package(Python3)

# py_test(name)
#
# Creates a Python test with the given name whose main module is in
# test/name.py. It does nothing if Python is not installed.
function(py_test name)
  if (NOT Python3_Interpreter_FOUND)
    return()
  endif()

  get_cmake_property(is_multi "GENERATOR_IS_MULTI_CONFIG")
  set(build_dir "${CMAKE_CURRENT_BINARY_DIR}")
  if (is_multi)
    set(build_dir "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
  endif()

  add_test(NAME ${name}
      COMMAND Python3::Interpreter ${CMAKE_CURRENT_SOURCE_DIR}/test/${name}.py
          --build_dir=${build_dir} ${ARGN})

  # Make the Python import path consistent between Bazel and CMake.
  set_tests_properties(${name} PROPERTIES ENVIRONMENT PYTHONPATH=${CMAKE_SOURCE_DIR})
endfunction()


enable_testing()

py_test(judge10142/test10142_test)
py_test(judge-10142-test)
py_test(judge-100-test)
