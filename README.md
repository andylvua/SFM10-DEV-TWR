## Hardware

### Pre-requisites

- 2+ SynchronicIT SFM-10 Devices
- Micro USB Cables for each device (for power and data transfer)

## Software

### Pre-requisites

- [CMake](https://cmake.org/download/)
- [Python](https://www.python.org/downloads/) (version 3.11 recommended)
- [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
- [CP210X Driver](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads)

Follow the official documentation to install the above-required software.

### Setup

> [!TIP]
> 
> This project has ready-to-use CMake Profiles and Flash Scripts for CLion IDE. If possible, use CLion for the best experience.

When the above software is installed, you can proceed with the following steps:

1. Clone the repository

    ```bash
    git clone git@github.com:andylvua/SFM10-RTLS.git
    ```

2. Install the required Python packages

    ```bash
    pip install -r requirements.txt
    ```

3. Provide the path to your Python interpreter in the `config.env` file

    ```bash
    PYTHON_PATH={path_to_your_python_interpreter}
    ```

### Build

1. Create a build directory

    ```bash
    cmake -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
    ```

2. Build the project
    
    ```bash
    cmake --build cmake-build-release
    ```
    
    > [!NOTE]  
    > 
    > If you want to use different SDK versions, provide the SDK path in the `config.env` file:
    > 
    > ```bash
    > SDK_ROOT={path_to_your_sdk}
    > ```

### Flash

1. Connect the SFM-10 device (or multiple devices) to your computer using a micro USB cable

2. Run the flash script
    
    ```bash
    ./upload.sh
    ```
    
    > [!NOTE]  
    > 
    > By default, the script will flash the firmware to all connected devices. If you want to flash to a specific device(s), provide the device serial number(s) in the `config.env` file:
    > 
    > ```bash
    > SERIAL_PORT=/dev/ttyUSB0,/dev/ttyUSB1
    > ```

## Configuration and Usage

### Configuration

Prior to using the system, you need to configure the firmware. All user-configurable settings are located in the `user_config.h` file. 

1. Set the number of anchors that will be used in the system

    ```c
    #define NUM_ANCHORS {number_of_anchors}
    ```

2. Set the addresses of the anchors

    To determine the address of an anchor, set `SETUP_MODE` to `1` and run the firmware. The address of the anchor will be displayed on the serial monitor (baud rate 115200).

    ```c
    uint16_t anchorAddresses[NUM_ANCHORS] = {0xFFFF, ...};
    ```

Other settings are optional and can be left as default.

### Usage

When the firmware is configured and flashed to the devices, you can use the system.

1. Ensure that all anchors are powered on
2. Connect the tag to a PC
3. By default, if the system is working correctly, the LEDs on the anchors and tag will simultaneously blink
4. The tag will send the logs to the PC via serial communication in encoded format for efficiency
5. To decode and record the logs, you will need to use the `reader` program located in the `reader` directory

> [!NOTE]  
> 
> For the instructions on how to use the `reader` program, refer to the documentation in the `reader` directory
