cmake_minimum_required(VERSION 3.8.2)

set(SOURCE_FILES_FOLDER ${CMAKE_CURRENT_LIST_DIR}/../../source-files/)

include(${SOURCE_FILES_FOLDER}/user-application.cmake)

project(Standalone C ASM)

# When including other h-files, list their directories here:
include_directories(

)

# When including other c-files, list them here:
target_sources(app 
  PRIVATE
  main.c
  ${SOURCE_FILES_FOLDER}/phy-uci-interpreter.c
)

# Set your loop interval in milliseconds here:
add_compile_definitions(CONFIG_LOOP_INTERVAL=1)

# Set your application version number (0..255) here:
add_compile_definitions(CONFIG_APP_VERSION=1)

