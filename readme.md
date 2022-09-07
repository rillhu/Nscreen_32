# Nscreen_32 development and update #

## For developing, you can esp32-arduino. ##
```
We suggest you use platformIO it support both ESP-idf and arduino-esp32.
Install the platformIO with this guide: https://platformio.org/platformio-ide
```

**Before everything start, please power on your board, and check it if it works.**

## There are 3 demos here: ##

1. tft_espi_graphicstest

```
Used for test the screen only.
```

2. lvgl_benchmark
```
Used for run the benchmark programme. And touch screen driver included.
```

3. lvgl_demo
```
Used for the LVGL demo.And touch screen driver included.
```

###  Note: Adapting 3 old demos

Original Nscreen_32 demos cannot used with latest Arduino framework. I adapt above 3 demos for new `platform_packages: framework-arduinoespressif32` and new version toolchain.

Current toolchain is as below:

```
PACKAGES: 
 - framework-arduinoespressif32 @ 3.20003.0 (2.0.3) 
 - tool-esptoolpy @ 1.30300.0 (3.3.0) 
 - tool-openocd-esp32 @ 2.1100.20220411 (11.0) 
 - toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch3
```

platformio.ini

```
platform_packages =
  ; use a special branch
  ; framework-arduinoespressif32 @ https://github.com/espressif/arduino-esp32.git#idf-release/v4.0
  framework-arduinoespressif32
```

Current libraries version:

```
Dependency Graph
|-- lv_arduino @ 3.0.1 //lvgl 7.8.0 core
|-- TFT_eSPI @ 2.4.61
|   |-- SPIFFS @ 2.0.0
|   |   |-- FS @ 2.0.0
|   |-- FS @ 2.0.0
|   |-- LittleFS @ 2.0.0
|   |   |-- FS @ 2.0.0
|   |-- SPI @ 2.0.0
|-- Wire @ 2.0.0
```



## New demo

4. tft_espi_lvgl820

```
update lvgl to version 8.2.0 and adapt the original demos
```

Current toolchain is as below:

```
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/nscreen-32.html
PLATFORM: Espressif 32 (4.4.0) > YeaCreate NSCREEN-32
HARDWARE: ESP32 240MHz, 320KB RAM, 16MB Flash
DEBUG: Current (cmsis-dap) External (cmsis-dap, esp-prog, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa)
PACKAGES: 
 - framework-arduinoespressif32 @ 3.20003.0 (2.0.3) 
 - tool-esptoolpy @ 1.30300.0 (3.3.0) 
 - tool-openocd-esp32 @ 2.1100.20220411 (11.0) 
 - toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch3
```

Current libraries version:

```
Dependency Graph
|-- lvgl @ 8.2.0
|-- TFT_eSPI @ 2.4.61
|   |-- SPIFFS @ 2.0.0
|   |   |-- FS @ 2.0.0
|   |-- FS @ 2.0.0
|   |-- LittleFS @ 2.0.0
|   |   |-- FS @ 2.0.0
|   |-- SPI @ 2.0.0
Building in release mode
```



- [ ] Update the vertical and landscape support (ROTATION), code need to be clean
- [ ] Enlarge the lvgl dispbuf by dynamic allocate RAM, benchmark test shows FPS could be 22FPS now





### Run results

<img src="https://raw.codehub.cn/p/blogres/d/blogres/git/raw/master/tft_espi_lvgl820-1.jpg?token=sm2nLXppoyGk2mCzUDkRBsdoP2Vx5aELIygB1cXET8" style="zoom:33%;" />

<img src="https://raw.codehub.cn/p/blogres/d/blogres/git/raw/master/tft_espi_lvgl820-2.jpg?token=Sa5zp02nm8RIkoLieQt0SIMvkvQjBpj9SqQBLMIA9D" style="zoom:33%;" />

<img src="https://raw.codehub.cn/p/blogres/d/blogres/git/raw/master/tft_espi_lvgl820-3.jpg?token=vroXEPbiw49iTXO4u60AF3ts4dwczfv4ll89HrECHE" style="zoom:33%;" />

