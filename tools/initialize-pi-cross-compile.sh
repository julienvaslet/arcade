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
check_command git
check_command wget
check_command curl
check_command rpm2cpio
check_command cpio
check_command make

mkdir -p ${basedir}/arm-pi

title "Downloading official Raspberry Pi cross-compiling tools"
cd ${tmpDirectory}
git clone git://github.com/raspberrypi/tools.git
cp -R tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/* ${basedir}/arm-pi/
#rm -rf tools
cd ${basedir}

title "Downloading Khronos EGL headers and library"
eglRpm=$(curl -s ${pidoraRepo}/m/ | grep -o "href=\"mesa-libEGL-[0-9][^\"]\+" | sed 's/^href="//g')
eglRpmDevel=$(curl -s ${pidoraRepo}/m/ | grep -o "href=\"mesa-libEGL-devel[^\"]\+" | sed 's/^href="//g')
echo "Current Khronos EGL RPMs are ${eglRpm} and ${eglRpmDevel}"
wget ${pidoraRepo}/m/${eglRpm}
wget ${pidoraRepo}/m/${eglRpmDevel}
cd ${basedir}/arm-pi/arm-bcm2708hardfp-linux-gnueabi/sysroot
rpm2cpio ${basedir}/${eglRpm} | cpio -idm
rpm2cpio ${basedir}/${eglRpmDevel} | cpio -idm
rm -f ${basedir}/${eglRpm} ${basedir}/${eglRpmDevel}
cd ${basedir}

title "Downloading OpenGL ES headers and library"
glesRpm=$(curl -s ${pidoraRepo}/m/ | grep -o "href=\"mesa-libGLES-[0-9][^\"]\+" | sed 's/^href="//g')
glesRpmDevel=$(curl -s ${pidoraRepo}/m/ | grep -o "href=\"mesa-libGLES-devel[^\"]\+" | sed 's/^href="//g')
echo "Current OpenGL ES RPMs are ${glesRpm} and ${glesRpmDevel}"
wget ${pidoraRepo}/m/${glesRpm}
wget ${pidoraRepo}/m/${glesRpmDevel}
cd ${basedir}/arm-pi/arm-bcm2708hardfp-linux-gnueabi/sysroot
rpm2cpio ${basedir}/${glesRpm} | cpio -idm
rpm2cpio ${basedir}/${glesRpmDevel} | cpio -idm
rm -f ${basedir}/${glesRpm} ${basedir}/${glesRpmDevel}
cd ${basedir}

title "Downloading SDL2 ${SDL2Version} release"
wget http://www.libsdl.org/release/SDL2-${SDL2Version}.tar.gz
tar xzf SDL2-${SDL2Version}.tar.gz
rm -f SDL2-${SDL2Version}.tar.gz

title "Compiling SDL2 ${SDL2Version}"
basepath="${basedir}/arm-pi/bin/"
export -p ARCH="arm"
export -p CROSS_TOOLS="arm-bcm2708hardfp-linux-gnueabi-"
export -p CC="${basepath}${CROSS_TOOLS}gcc"
export -p NM="${basepath}${CROSS_TOOLS}nm"
export -p LD="${basepath}${CROSS_TOOLS}ld"
export -p CXX="${basepath}${CROSS_TOOLS}g++"
export -p RANLIB="${basepath}${CROSS_TOOLS}ranlib"
export -p AR="${basepath}${CROSS_TOOLS}ar"

cd SDL2-${SDL2Version}
./configure --disable-pulseaudio --target=arm-linux --host=arm-linux
make

title "Installing SDL2 ${SDL2Version}"
sed -i "s|^prefix\s*=.*|prefix = ${basedir}/arm-pi/arm-bcm2708hardfp-linux-gnueabi/sysroot/usr|g" Makefile
make install

cd ${basedir}
rm -rf SDL2-${SDL2Version}

echo "Installed."
