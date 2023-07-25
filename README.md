# Wardriver
[![Docker CI Build](https://github.com/LyndLabs/Wardriver/actions/workflows/docker-image.yml/badge.svg)](https://github.com/LyndLabs/Wardriver/actions/workflows/docker-image.yml)

Wardriver is a wardriving project that supports both ESP8266 (and ESP32 boards soon). This guide will help you set up your development environment and flash the firmware to your device.

## Prerequisites
- ESP8266 board (ESP32 coming soon).
- Docker and esptool installed on your machine.
- You can install it using pip: `pip install esptool` on Mac/Linux. Use the [official espressif releases](https://github.com/espressif/esptool/releases) for other operating systems. 

## Setup

1. Clone this repository to your local machine.

```bash
git clone https://github.com/LyndLabs/Wardriver.git
cd Wardriver/src
```

2. Build the Docker image.

```bash
make build
```

This will create a Docker image with all the necessary tools and libraries to compile the firmware.

## Flashing the Firmware

Before flashing the firmware, make sure your ESP8266 or ESP32 board is connected to your computer.

### 1. ESP32 ONLY: Enter DFU Mode

You can do this in 2 ways:

**Option 1:**

- Plug the board into your computer
- Hold down the 0 button
- Tap & Release the RST button

**Option 2:**

- Hold down the 0 button
- Plug in the board
- Release the 0 button

### Step 2: Find Serial Port & Erase Flash 

Find the Serial Port your board is connected to:

- Windows: Found via Device Manager
- MacOS: `ls /dev/cu*`
- Linux: `ls /dev/tty*`

Your device will appear as something like `ttyUSB0` or `ttyACM0`.

Erase the flash:

```bash
esptool --chip esp32 -p /dev/<your_serial_port> erase_flash
```

Replace `esp32` with `esp8266` if you're using ESP8266 board.

### Step 3: Flashing 

Run the following command to flash the firmware to your board:

```bash
esptool --chip esp32 -p /dev/<your_serial_port> write_flash -z 0x0 ./Wardriver/Wardriver.ino.bin
```

Replace `esp32` with `esp8266` if you're using ESP8266 board.

## Listing All Supported Boards

To list all the supported boards by esptool, you can check the help file:

```bash
esptool -h
```

## Troubleshooting

If you encounter any issues, make sure your board is properly connected and the correct port is specified in the `esptool` command. If the problem persists, please open an issue in this repository.

## Contributing

Contributions are welcome! Please open a pull request with your changes or improvements.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details. 

## Credits

All credit and thanks to Alex Lynd. Docs modded from the DNSDriveBy/Nugget flashing instructions. 
