cmake_minimum_required(VERSION 3.21)

project(EclipseMenuRecordModeFork VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to your Geode SDK installation.")
endif()

add_library(${PROJECT_NAME} SHARED
    src/bot/BotManager.cpp
    src/bot/Recorder.cpp
    src/bot/FrameStepperBridge.cpp
    src/hooks/PlayLayerHooks.cpp
    src/ui/BotTab.cpp
)

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
