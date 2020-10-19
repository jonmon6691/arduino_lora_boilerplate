#!/usr/bin/env sh

# programmer settings are for "Arduino as programmer"
programmer_port=/dev/ttyUSB1
programmer_baud=19200
programmer_type=stk500v1

# Only tested with Arduino Nano, ymmv
target_board=atmega328p
target_bootloader_path=bootloaders/optiboot_atmega328.hex

# Uncomment and adjust if you installed the IDE using ubuntu software manager, or snap directly
avrdude_bin_path=/snap/arduino/41/hardware/tools/avr/bin/avrdude
avrdude_conf_path=/snap/arduino/41/hardware/tools/avr/etc/avrdude.conf

# Uncomment and adjust if you ran the IDE from source or unpacked from tarball
#avrdude_bin_path=~/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/avrdude
#avrdude_conf_path=~/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf

# Uncomment if you have avrdude installed
#avrdude_bin_path=avrdude

avrdude_flags=-C$(avrdude_conf_path) -v -p$(target_board) -c$(programmer_type) -P$(programmer_port) -b$(programmer_buad) 

# I have NO IDEA if these fuse bit settings are valid for anything other than atmega328!
# Best way to find out yours is to turn on verbose outputs in the arduino IDE and try to flash a bootloader that way and see what it calls
$(avrdude_bin_path) $(avrdude_flags) -e -Ulock:w:0x3F:m -Uefuse:w:0xFD:m -Uhfuse:w:0xDA:m -Ulfuse:w:0xFF:m 
$(avrdude_bin_path) $(avrdude_flags) -Uflash:w:$(target_bootloader_path):i -Ulock:w:0x0F:m
