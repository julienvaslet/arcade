arcade
======

Arcade project is a C++/OpenGL library and some arcade games which will be playable on a Raspberry Pi.
Project's physical and electronic (for controllers) parts will be linked to this project soon.

ARM compilation
---------------
This repository contains somes scripts to compile from a Fedora installation to a Pidora one via QEMU virtualizer.
The tools/initialize-pi-cross-compile.sh initialize from scratch the QEMU-Pidora installation.
Once successfuly configured, the command "make <application> for=pi" starts the virtualized environment then retrieve compiled binaries.

Virtual joysticks from GPIO
---------------------------
16 GPIO pins (+2 pins for +3.3V and ground) are used to read two players' controllers. Each controller is composed of one 4-directions joystick and 4 push buttons. In the gpio-controllers application, depending on the "data/controllers/gpio.conf" file, as many virtual controllers as sections in the configuration file are created. Associated pins drive the sent events by each controllers.
