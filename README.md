# Display Keyboard

This project is the hardware design and software for a USB keyboard with an
integrated touch display.

Hardware features:

* Mechanical keys.
* ESP32-S2 ([datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s2_datasheet_en.pdf)).
* 2.8" LCD capacitive [touch display](https://www.adafruit.com/product/2770).
* 0.91" vertical [OLED display](https://www.aliexpress.com/item/32672229793.html)
  for volume display.
* Volume control knob.

## Hardware Design

Design is done in [KeyCad](https://kicad.org/). Files are located in
the [pcb](pcb) directory.

## Software Requirements

* [ESP-IDF](https://docs.espressif.com/)
* [VS Code extensions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/vscode-setup.html)
  (optional)

## Building

Configuration (GPIO pins, etc.) is stored in the sdkconfig file. This is
set using menuconfig as so:

```sh
./make.sh config
```

**Setting device port:**

A connected ESP32-S2 is not always given the same device port when
connected to a computer via USB. Follow
[these instructions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html#check-port-on-windows)
to determine your port. When found edit `make.sh` to specify the
port on your computer by modifying the following line:

```sh
PORT=/dev/cu.usbserial-GT4YU06W
```

Build and flash to the attached ESP32-S2 device as so:

```sh
. $HOME/esp/esp-idf/export.sh # only do this once
./make.sh build && ./make.sh flash
```
