### Pre-requisites

- [CMake](https://cmake.org/download/)
- [Qt6](https://www.qt.io/product/qt6) (Core, SerialPort, Widgets, Charts)

Follow the official documentation to install the above-required software.

### Build

1. Create a build directory

   ```bash
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   ```

2. Build the project
    
   ```bash
   cmake --build build --target reader
   ```

### Configure

All user-configurable settings are located in the `user_config.h` file. The settings include, but are not limited to:

- `PORT_NAME` - The name of the serial port to connect to (the port the Tag is connected to)
- `OUTPUT_FILE` - The name of the file to write the data to
- `READ_CIR` - Whether to read the Channel Impulse Response (CIR) data
- `VISUALIZE_CIR` - Whether to visualize the CIR data

> [!NOTE]
> 
> When changing any of the settings in the SFM-10 firmware or this program, 
> make sure to verify that the changes are compatible with each other 
> (e.g., if the transmission of the CIR is disabled in the firmware, `READ_CIR` should be set to `false` in this program).

### Usage

When the system is ready, run the program:

```bash
./build/reader
```

The program will start reading the data from the Tag and write it to the specified file. 
If `VISUALIZE_CIR` is set to `true`, the program will also display the CIR data in a chart in real-time.
Some additional information will be printed to the console, such as the number of transactions and logging duration.
