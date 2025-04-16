#!/bin/bash

source config.env

if [ -z "$PYTHON_PATH" ]; then
    echo -e "\033[1;31mPYTHON_PATH is not set. Please set it in config.env.\033[0m"
    exit 1
fi
if [ -z "$SDK_ROOT" ]; then
    echo -e "\033[1;31mSDK_ROOT is not set. Please set it in config.env.\033[0m"
    exit 1
fi
if [ ! -f ./cmake-build-release/app.hex ]; then
    echo -e "\033[1;31mapp.hex not found. Please build the project in release mode first.\033[0m"
    exit 1
fi

echo -e "\033[1;32mUploading app.hex\033[0m"

$PYTHON_PATH "$SDK_ROOT"/source-files/upload.py ./cmake-build-release/app.hex "$SERIAL_PORT"
