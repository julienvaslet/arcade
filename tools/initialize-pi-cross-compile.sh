#!/bin/bash
basedir=$(cd `dirname $0`; pwd)
tmpDirectory="/tmp"
pidoraVersion="20"
pidoraArch="armv6hl"
pidoraRepo="http://pidora.ca/pidora/releases/${pidoraVersion}/packages/${pidoraArch}/os/Packages"
SDL2Version="2.0.3"

function title ()
{
	echo
	echo $1
	length=$(echo $1 | wc -c)
	while [ ${length} -gt 1 ]
	do
		echo -n "="
		length=$(expr ${length} - 1)
	done
	echo
}

function check_command ()
{
	echo -n "Checking command \"$1\"... "
	if [ $(which $1 &>/dev/null;echo $?) = "1" ]
	then
		echo "Not found!"
		exit 1
	else
		echo "ok."
	fi
}

title "Checking necessary commands"
check_command yum
check_command git
check_command make
check_command rsync

mkdir -p ${basedir}/arm-pi/qemu/{bin,share,lib,libexec,include,etc}

title "Checking QEMU installation"
if [ ! -e "${basedir}/arm-pi/qemu/bin/qemu-system-arm" ]
then
	echo "QEMU is not installed."
	
	title "Installing RPM dependencies"
	sudo yum install -y SDL-devel zlib-devel pixman-devel libfdt-devel
	
	title "Installing QEMU from git repository"
    git clone git://git.qemu-project.org/qemu.git
    cd qemu
    ./configure --target-list="arm-softmmu arm-linux-user" --enable-sdl --enable-kvm --prefix="${basedir}/arm-pi/qemu"
	make
	make install
	cd ${basedir}
	rm -rf qemu
else
	echo "QEMU is already installed."
fi

