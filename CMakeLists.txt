cmake_minimum_required(VERSION 3.21)
project(EclipseMenuRecordModeFork VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Cargar Geode SDK (ya configurado por GitHub Actions)
include($ENV{GEODE_SDK}/cmake/Geode.cmake)

# Definir mod Geode
setup_geode_mod(${PROJECT_NAME})

# Archivos del proyecto
target_sources(${PROJECT_NAME} PRIVATE
    src/bot/BotManager.cpp
    src/bot/Recorder.cpp
    src/bot/FrameStepperBridge.cpp
    src/hooks/PlayLayerHooks.cpp
    src/ui/BotTab.cpp
)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
