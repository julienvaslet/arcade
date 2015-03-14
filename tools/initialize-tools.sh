#!/bin/bash
basedir=$(cd `dirname $0`; pwd)

source "${basedir}/common.sh"
source "${basedir}/constants.conf"

expect_root_user

### Constants ###
tmpDir="/tmp"
piBinariesDir="/opt/arm-pi"
crosstoolngDir="/opt/crosstool-ng"
crosstoolngUri="https://github.com/crosstool-ng/crosstool-ng.git"

cd ${basedir}

title "Prerequisites"
check_command wget
check_command md5sum
check_command awk
check_command fdisk
check_command losetup
check_command kpartx
check_command mkfs.vfat
check_command mkfs.ext4

RPI_VERSION="RPI"

title "Arch Linux ARM"
if [ ! -e "${basedir}/${RPI_VERSION}/arch-linux.iso" ]
then
	mkdir -p "${basedir}/${RPI_VERSION}"

	url="ARCHLINUX_${RPI_VERSION}_URL"
	urlMd5="ARCHLINUX_${RPI_VERSION}_MD5_URL"
	install_package ${BSDTAR_PACKAGE}
	
	downloadArchLinux=0
	download_file "${!urlMd5}" "${basedir}/${RPI_VERSION}/arch-linux.tar.gz.md5"
	
	if [ -e "${basedir}/${RPI_VERSION}/arch-linux.tar.gz" ]
	then
		localMd5=$(md5sum "${basedir}/${RPI_VERSION}/arch-linux.tar.gz" | awk '{print $1}')
		remoteMd5=$(cat "${basedir}/${RPI_VERSION}/arch-linux.tar.gz.md5" | awk '{print $1}')
		
		if [ "${localMd5}" = "${remoteMd5}" ]
		then
			echo "Arch Linux archive is already present. Skipping download."
			downloadArchLinux=1
		fi
	fi
	
	if [ $downloadArchLinux -eq 0 ]
	then
		download_file "${!url}" "${basedir}/${RPI_VERSION}/arch-linux.tar.gz"
		
		localMd5=$(md5sum "${basedir}/${RPI_VERSION}/arch-linux.tar.gz" | awk '{print $1}')
		remoteMd5=$(cat "${basedir}/${RPI_VERSION}/arch-linux.tar.gz.md5" | awk '{print $1}')
		
		if [ "${localMd5}" != "${remoteMd5}" ]
		then
			echo "Downloaded file checksums do not match. Exiting."
			exit 1
		fi
	fi
	
	echo "Initializing arch-linux.iso file..."
	dd if=/dev/zero of="${basedir}/${RPI_VERSION}/arch-linux.iso" bs=1M count=800
	
	echo "Creating partitions..."
	echo -e "o\nn\np\n1\n\n+100M\nt\nc\nn\np\n2\n\n\np\nw\n" | fdisk "${basedir}/${RPI_VERSION}/arch-linux.iso"
	devloop=$(losetup -f --show "${basedir}/${RPI_VERSION}/arch-linux.iso")
	kpartx -a ${devloop}
	
	# Sleeping for one second... let the system creating loop devices
	sleep 1
	
	if [ ! -e "${devloop/\/dev\//\/dev\/mapper\/}p1" -o ! -e "${devloop/\/dev\//\/dev\/mapper\/}p2" ]
	then
		echo "Could not mount ISO partitions. Aborting initialization."
		
		kpartx -d ${devloop}
		losetup -d ${devloop}
		
		rm -f "${basedir}/${RPI_VERSION}/arch-linux.iso"
		exit 1
	fi
	
	echo "Setting up filesystems..."
	mkfs.vfat ${devloop/\/dev\//\/dev\/mapper\/}p1
	mkfs.ext4 ${devloop/\/dev\//\/dev\/mapper\/}p2
	
	mkdir -p "${basedir}/${RPI_VERSION}/"{boot,root}
	mount ${devloop/\/dev\//\/dev\/mapper\/}p1 "${basedir}/${RPI_VERSION}/boot"
	mount ${devloop/\/dev\//\/dev\/mapper\/}p2 "${basedir}/${RPI_VERSION}/root"
	
	echo "Copying Arch Linux files..."
	bsdtar xzf "${basedir}/${RPI_VERSION}/arch-linux.tar.gz" -C "${basedir}/${RPI_VERSION}/root"
	mv "${basedir}/${RPI_VERSION}"/root/boot/* "${basedir}/${RPI_VERSION}/boot/"
	
	umount "${basedir}/${RPI_VERSION}/boot"
	umount "${basedir}/${RPI_VERSION}/root"
	
	rmdir "${basedir}/${RPI_VERSION}/"{boot,root}
	
	kpartx -d ${devloop}
	losetup -d ${devloop}
else
	echo "Arch Linux image is already installed."
fi


exit

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

