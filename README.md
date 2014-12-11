arcade
======

Arcade project is a C++/OpenGL library and some arcade games which will be playable on a Raspberry Pi.
Project's physical and electronic (for controllers) parts will be linked to this project soon.

ARM compilation
-----------------
This repository contains somes scripts to compile from a Fedora installation to a Pidora one via QEMU virtualizer.
The tools/initialize-pi-cross-compile.sh initialize from scratch the QEMU-Pidora installation.
Once successfuly configured, the command "make <application> for=pi" starts the virtualized environment then retrieve compiled binaries.
