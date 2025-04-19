# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(openlight)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})

# --- rpi_ws281x ---------------------------------------------------------------
# Locate the rpi_ws281x headers.
# 1) If the library is installed in standard include paths it will be found automatically.
# 2) Otherwise, pass -DRPI_WS281X_INCLUDE_DIR=/path/to/rpi_ws281x when invoking CMake.
find_path(
    RPI_WS281X_INCLUDE_DIR
    NAMES ws2811.h
    PATH_SUFFIXES rpi_ws281x
)

if(NOT RPI_WS281X_INCLUDE_DIR)
    message(FATAL_ERROR "rpi_ws281x headers not found; please set RPI_WS281X_INCLUDE_DIR.")
endif()

include_directories(${RPI_WS281X_INCLUDE_DIR})

set(SOURCES
    main.cpp
    LedStrip.cpp
    FrameProcessing.cpp
    Test.cpp
)

set(CMAKE_BUILD_TYPE Debug)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ws2811)
