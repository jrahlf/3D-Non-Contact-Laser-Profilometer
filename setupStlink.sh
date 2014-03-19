#!/bin/sh
## This may be required if STlink V1 is used
sudo modprobe -r usb-storage && sudo modprobe usb-storage quirks=483:3744:i
