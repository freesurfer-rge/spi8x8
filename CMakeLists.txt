cmake_minimum_required(VERSION 3.15)
project("LED 8x8 SPI Driver")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(
  APPEND _warning_opts
  "$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall;-Wextra;-Weffc++;-Werror;>"
  "$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wthread-safety;-Wpedantic;>"
  "$<$<CXX_COMPILER_ID:GNU>:-pedantic;-pedantic-errors;>"
  )

add_compile_options("${_warning_opts}")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}" ${CMAKE_MODULE_PATH})
find_package(pigpio)

set(srcs)

list(APPEND srcs main.cpp)


add_executable(LED8x8)

target_include_directories(LED8x8
    SYSTEM PRIVATE ${pigpio_INCLUDE_DIR})
target_link_libraries(LED8x8 PRIVATE ${pigpiod_if2_LIBRARY})
target_sources(LED8x8 PRIVATE ${srcs})
