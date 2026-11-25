#! /bin/sh
#
# Compile and upload the code to an ESP32-DevKit3 development board.
#
# Note:
#
#  1. Connect the development board via USB.
#  2. Platform I/O locates the ESP32 automatically
#
# For further information, please see the PlatformIO Core (CLI)
# documentation at https://docs.platformio.org/en/latest/core/index.html
#
pio run -e esp32dev -t upload

# pio_upload.sh -- END

