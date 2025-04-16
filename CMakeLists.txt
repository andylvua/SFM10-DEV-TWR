cmake_minimum_required(VERSION 3.8.2)

set(ENV_FILE ${CMAKE_SOURCE_DIR}/config.env)

if(EXISTS ${ENV_FILE})
    file(STRINGS ${ENV_FILE} CONFIG_LINES)

    foreach(LINE IN LISTS CONFIG_LINES)
        if(LINE MATCHES "^[#]")
            continue()
        endif()
        if(LINE STREQUAL "")
            continue()
        endif()

        string(REGEX MATCH "([^=]+)=(.*)" _match "${LINE}")
        set(VAR_NAME "${CMAKE_MATCH_1}")
        set(VAR_VALUE "${CMAKE_MATCH_2}")

        string(STRIP "${VAR_NAME}" VAR_NAME)
        string(STRIP "${VAR_VALUE}" VAR_VALUE)

        if(VAR_VALUE STREQUAL "")
            continue()
        endif()

        set(${VAR_NAME} "${VAR_VALUE}")
        message(STATUS "Set variable: ${VAR_NAME} = ${VAR_VALUE}")
    endforeach()
else()
    message(FATAL_ERROR "config.env file not found!")
endif()

if(NOT DEFINED SDK_ROOT)
    message(FATAL_ERROR "SDK_ROOT is not set! Please set it in config.env file.")
endif()

set(APPLE FALSE)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)

set(SOURCE_FILES_FOLDER ${SDK_ROOT}/source-files)

include(${SOURCE_FILES_FOLDER}/user-application.cmake)

project(SFM10-RTLS C ASM)

# ignore -Wint-conversion warnings
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-int-conversion -Wno-incompatible-pointer-types")

# When including other h-files, list their directories here:
include_directories(
    shared
)

# When including other c-files, list them here:
target_sources(app
  PRIVATE
  main.c
)

# Set your loop interval in milliseconds here:
add_compile_definitions(CONFIG_LOOP_INTERVAL=1)

# Set your application version number (0..255) here:
add_compile_definitions(CONFIG_APP_VERSION=1)

message(STATUS "Building Standalone application")