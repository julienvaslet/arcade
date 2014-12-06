#!/bin/bash

resize2fs /dev/sda2
e2fsck -f /dev/sda2

# Install dependencies
yum install -y make gcc SDL2-* SDL2_image SDL2_image-devel gcc-c++ mesa-*

# Change startup script
sed -i 's/auto-configure\.sh/compile.sh/' /root/.bashrc

reboot
