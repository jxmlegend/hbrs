cmake_minimum_required(VERSION 3.5)

project(3531 
    LANGUAGES CXX
    VERSION 2.0.0)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER  arm-hisiv400-linux-gcc)
SET(CMAKE_CXX_COMPILER arm-hisiv400-linux-g++)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_SKIP_RPATH true)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

set(CMAKE_CXX_FLAGS "-g -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-g -Wall ${CMAKE_C_FLAGS}")


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/hbrs
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/include
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lib
)

add_subdirectory(hbrs)