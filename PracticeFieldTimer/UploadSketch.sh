#! /bin/sh
#
# Upload practice field timer sketch. 
#
# Parameters (positional on command line):
#
# Number Contents
# ------ ---------------------------------------------------------------
#      1 Serial device connected to the target ESP-32, generally
#        resembling /dev/ttyUSBn, where n is a small number (usually
#        0, 1, 2, or 3, but may be higher)
#
# Typical use from the project directory:
#
#   ./UloadSketch.sh /dev/ttyUSB0
#
# Note that the local configuration and library version are hard wired

/home/eric/Sloeber-V4.4.3/arduinoPlugin/packages/esp32/tools/esptool_py/5.1.0/esptool --chip esp32 --port $1 --baud 921600 --before default-reset --after hard-reset write-flash -z --flash-mode keep --flash-freq keep --flash-size keep 0x1000 /home/eric/GitHub/FirstRobotics/PracticeFieldTimer/Release/PracticeFieldTimer.bootloader.bin 0x8000 /home/eric/GitHub/FirstRobotics/PracticeFieldTimer/Release/PracticeFieldTimer.partitions.bin 0xe000 /home/eric/Sloeber-V4.4.3/arduinoPlugin/packages/esp32/hardware/esp32/3.3.3/tools/partitions/boot_app0.bin 0x10000 /home/eric/GitHub/FirstRobotics/PracticeFieldTimer/Release/PracticeFieldTimer.bin 

