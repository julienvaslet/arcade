#!/bin/bash
basedir=$(cd `dirname $0`; pwd)

source "${basedir}/common.sh"
source "${basedir}/constants.conf"

expect_root_user

user=$(stat -c %U ${basedir})
group=$(stat -c %G ${basedir})

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
check_command dd
check_command fdisk
check_command losetup
check_command kpartx
check_command mkfs.vfat
check_command mkfs.ext4
check_command fsck.vfat
check_command e2fsck
check_command mount
check_command sed

RPI_VERSION="RPI"

title "QEMU installation"
if [ "$(which qemu-system-arm ; echo $?)" = "1" ]
then
	install_package ${QEMU_PACKAGE}
else
	echo "QEMU is already installed for ARM systems."
fi

title "Arch Linux ARM"
if [ ! -e "${basedir}/${RPI_VERSION}/arch-linux.img" ]
then	
	question "Perform full system update ?" full_update "y" "n"
	
	mkdir -p "${basedir}/${RPI_VERSION}"
	chown ${user}:${group} "${basedir}/${RPI_VERSION}"

	url="ARCHLINUX_${RPI_VERSION}_URL"
	urlMd5="ARCHLINUX_${RPI_VERSION}_MD5_URL"
	install_package ${BSDTAR_PACKAGE}
	
	downloadArchLinux=0
	download_file "${!urlMd5}" "${basedir}/${RPI_VERSION}/arch-linux.tar.gz.md5"
	chown ${user}:${group} "${basedir}/${RPI_VERSION}/arch-linux.tar.gz.md5"
	
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
		chown ${user}:${group} "${basedir}/${RPI_VERSION}/arch-linux.tar.gz"
		
		localMd5=$(md5sum "${basedir}/${RPI_VERSION}/arch-linux.tar.gz" | awk '{print $1}')
		remoteMd5=$(cat "${basedir}/${RPI_VERSION}/arch-linux.tar.gz.md5" | awk '{print $1}')
		
		if [ "${localMd5}" != "${remoteMd5}" ]
		then
			echo "Downloaded file checksums do not match. Exiting."
			exit 1
		fi
	fi
	
	echo "Initializing arch-linux.img file..."
	dd if=/dev/zero of="${basedir}/${RPI_VERSION}/arch-linux.img" bs=1M count=800
	chown ${user}:${group} "${basedir}/${RPI_VERSION}/arch-linux.img"
	
	echo "Creating partitions..."
	echo -e "o\nn\np\n1\n\n+100M\nt\nc\nn\np\n2\n\n\np\nw\n" | fdisk "${basedir}/${RPI_VERSION}/arch-linux.img"
	devloop=$(losetup -f --show "${basedir}/${RPI_VERSION}/arch-linux.img")
	kpartx -a ${devloop}
	
	# Sleeping for one second... let the system creating loop devices
	sleep 1
	
	if [ ! -e "${devloop/\/dev\//\/dev\/mapper\/}p1" -o ! -e "${devloop/\/dev\//\/dev\/mapper\/}p2" ]
	then
		echo "Could not mount image's partitions. Aborting initialization."
		
		kpartx -d ${devloop}
		losetup -d ${devloop}
		
		rm -f "${basedir}/${RPI_VERSION}/arch-linux.img"
		exit 1
	fi
	
	echo "Setting up filesystems..."
	mkfs.vfat ${devloop/\/dev\//\/dev\/mapper\/}p1
	mkfs.ext4 ${devloop/\/dev\//\/dev\/mapper\/}p2
	
	mkdir -p "${basedir}/${RPI_VERSION}/"{r,b}oot
	mount ${devloop/\/dev\//\/dev\/mapper\/}p1 "${basedir}/${RPI_VERSION}/boot"
	mount ${devloop/\/dev\//\/dev\/mapper\/}p2 "${basedir}/${RPI_VERSION}/root"
	
	echo "Copying Arch Linux files..."
	bsdtar -xpf "${basedir}/${RPI_VERSION}/arch-linux.tar.gz" -C "${basedir}/${RPI_VERSION}/root"
	mv "${basedir}/${RPI_VERSION}"/root/boot/* "${basedir}/${RPI_VERSION}/boot/"
	
	echo "Setting keymap to \"${KEYBOARD_KEYMAP}\"..."
	echo "KEYMAP=\"${KEYBOARD_KEYMAP}\"" > "${basedir}/${RPI_VERSION}/root/etc/vconsole.conf"
	chown root:root "${basedir}/${RPI_VERSION}/root/etc/vconsole.conf"
	chmod 644 "${basedir}/${RPI_VERSION}/root/etc/vconsole.conf"
	
	echo "Setting localtime to \"${LOCALTIME}\"..."
	if [ -e "${basedir}/${RPI_VERSION}/root/usr/share/zoneinfo/${LOCALTIME}" ]
	then
		cd "${basedir}/${RPI_VERSION}/root"
		rm -f etc/localtime
		ln -s usr/share/zoneinfo/${LOCALTIME} etc/localtime
		cd "${basedir}"
	else
		echo "Timezone \"${LOCALTIME}\" does not exist. Skipping."
	fi
	
	echo "Copying script for packages installation..."
	
	autologinServiceFile="${basedir}/${RPI_VERSION}/root/etc/systemd/system/getty@tty1.service.d/autologin.conf"
	mkdir -p $(dirname "${autologinServiceFile}")
	echo "[Service]
ExecStart=
ExecStart=-/sbin/agetty --autologin root --noclear %I 38400 linux
Type=idle" > "${autologinServiceFile}"
	chown root:root "${autologinServiceFile}"
	chmod 644 "${autologinServiceFile}"
	
	fullUpdateCommands=""
	
	if [ "${full_update}" = "y" ]
	then
		fullUpdateCommands="pacman -Syu"
	fi

#Establish package list & configure internet access
	
	echo "#!/bin/bash
${fullUpdateCommands}
#pacman -Sy SDL2
reboot" > "${basedir}/${RPI_VERSION}/root/root/.bash_profile"
	chown root:root "${basedir}/${RPI_VERSION}/root/root/.bash_profile"
	chmod 640 "${basedir}/${RPI_VERSION}/root/root/.bash_profile"
	
	umount "${basedir}/${RPI_VERSION}/boot"
	umount "${basedir}/${RPI_VERSION}/root"
	
	rmdir "${basedir}/${RPI_VERSION}/"{boot,root}
	
	echo "Checking filesystems..."
	fsck.vfat -w -r -a -t ${devloop/\/dev\//\/dev\/mapper\/}p1
	e2fsck -p -c -f ${devloop/\/dev\//\/dev\/mapper\/}p2
	
	kpartx -d ${devloop}
	losetup -d ${devloop}
	
	title "ARM1176 Linux Kernel"
	if [ ! -e "${basedir}/arm1176-kernel" ]
	then
		echo "Kernel is not present \"${basedir}/arm1176-kernel\"."
		echo "Compilation is not embedded in this script."
		echo "You may gather one arm1176 kernel @ http://www.xecdesign.com/downloads/linux-qemu/kernel-qemu"
		echo "Aborting initialization."
		
		rm -f "${basedir}/${RPI_VERSION}/arch-linux.img"
		exit 1
	else
		echo "Kernel is already compiled."
	fi

	title "Arch Linux packages installation"
	launch_vm "${basedir}/${RPI_VERSION}/arch-linux.img"
	
	title "Installation & update scripts deletion"
	mount_image_partition "${basedir}/${RPI_VERSION}/arch-linux.img" 2 "${basedir}/mnt_script_deletion"
	
	rm -f "${basedir}/mnt_script_deletion/etc/systemd/system/getty@tty1.service.d/autologin.conf"
	rm -f "${basedir}/mnt_script_deletion/root/.bash_profile"
	
	umount_image_partition "${basedir}/mnt_script_deletion"
	echo "Image cleaned."
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

