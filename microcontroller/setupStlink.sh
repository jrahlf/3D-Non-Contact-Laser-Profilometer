#!/bin/sh
sudo modprobe -r usb-storage && sudo modprobe usb-storage quirks=483:3744:i
