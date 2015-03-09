#!/bin/bash
basedir=$(cd `dirname $0`; pwd)

### Constants ###
tmpDir="/tmp"
piBinariesDir="/opt/arm-pi"
crosstoolngDir="/opt/crosstool-ng"
crosstoolngUri="https://github.com/crosstool-ng/crosstool-ng.git"

### Functions ###
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

### Procedure ###
if [ $(id -u) != "0" ]
then
	echo "You must be root (or equivalent) to execute this script."
	echo "Some embedded commands (for instance, yum) need it."
	exit 1
fi

cd ${basedir}

title "Necessary commands checks"
check_command yum
check_command git
check_command make
check_command rsync

title "Crosstool-ng installation check"
if [ ! -e "${crosstoolngDir}/bin/ct-ng" ]
then
	if [ ! -e "${basedir}/crosstool-ng" ]
	then
		title "Crosstool-ng installation"
		yum install -y gperf bison flex texinfo patch libtool ncurses-devel
		git clone ${crosstoolngUri}

		cd ${basedir}/crosstool-ng

		if [ ! -e "configure" ]
		then
			./bootstrap
		fi

		./configure --prefix ${crosstoolngDir}

		make
		make install
		cd ${basedir}
		rm -rf ${basedir}/crosstool-ng
	else
		echo "Crosstool-ng is not installed but \"${basedir}/crosstool-ng\" exists."
		echo "Please remove it before continue."
		exit 1
	fi
else
	echo "Crosstool-ng is installed."
fi

#cp .....config in /tmp/arm-pi/.config
#then ct-ng build

#CT_BINUTILS="binutils"
#CT_PREFIX_DIR="${HOME}/tools/arm-bcm2708/${CT_TARGET}" > CT_PREFIX_DIR="/opt/arm-pi/${CT_TARGET}"

