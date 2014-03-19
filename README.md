Laser-Profilometer
==================

Code for a stm32f4 microcontroller (dev board) to control an XY stage and interact with Keyence LT-9010 laser profilometer to create 3D scans of surfaces.

Uses the C++ framework xpcc (see https://github.com/roboterclubaachen/xpcc for code and license). A copy is included because a modified UartHandler is used.

Uses cascaded PID controller to control DC brushed motors equipped with optical rotary encoders.
