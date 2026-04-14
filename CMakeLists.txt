cmake_minimum_required(VERSION 3.21)
project(EclipseMenuRecordModeFork VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK environment variable is not set")
endif()

include($ENV{GEODE_SDK}/cmake/Geode.cmake)

setup_geode_mod(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(${PROJECT_NAME} PRIVATE
    src/bot/BotManager.cpp
    src/bot/Recorder.cpp
    src/bot/FrameStepperBridge.cpp
    src/hooks/PlayLayerHooks.cpp
    src/ui/BotTab.cpp
)
