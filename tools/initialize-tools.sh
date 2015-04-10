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
check_command sha1sum
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
	dd if=/dev/zero of="${basedir}/${RPI_VERSION}/arch-linux.img" bs=1M count=${IMAGE_SIZE_MO}
	chown ${user}:${group} "${basedir}/${RPI_VERSION}/arch-linux.img"
	
	echo "Creating partitions..."
	echo -e "o\nn\np\n1\n\n+${BOOT_PARTITION_SIZE}\nt\nc\nn\np\n2\n\n\np\nw\n" | fdisk "${basedir}/${RPI_VERSION}/arch-linux.img"
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
		fullUpdateCommands="pacman -Su --noconfirm"
	fi
	
	echo "#!/bin/bash
dhcpcd
pacman -Sy
${fullUpdateCommands}
pacman -S ${ARCADE_MANDATORY_PACKAGES} --noconfirm
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

if [ ! -e "${basedir}/${RPI_VERSION}/arch-linux-devel.img" ]
then
	title "Arch Linux development image initialization"
	cp "${basedir}/${RPI_VERSION}/arch-linux.img" "${basedir}/${RPI_VERSION}/arch-linux-devel.img"
	
	
	title "Development libraries' scripts installation"
	mount_image_partition "${basedir}/${RPI_VERSION}/arch-linux-devel.img" 2 "${basedir}/mnt_devel_libs_install"
	
	autologinServiceFile="${basedir}/mnt_devel_libs_install/etc/systemd/system/getty@tty1.service.d/autologin.conf"
	mkdir -p $(dirname "${autologinServiceFile}")
	echo "[Service]
ExecStart=
ExecStart=-/sbin/agetty --autologin root --noclear %I 38400 linux
Type=idle" > "${autologinServiceFile}"
	chown root:root "${autologinServiceFile}"
	chmod 644 "${autologinServiceFile}"

	echo "#!/bin/bash
dhcpcd
pacman -Sy ${ARCADE_DEVELOPMENT_PACKAGES} --noconfirm
reboot" > "${basedir}/mnt_devel_libs_install/root/.bash_profile"
	chown root:root "${basedir}/mnt_devel_libs_install/root/.bash_profile"
	chmod 640 "${basedir}/mnt_devel_libs_install/root/.bash_profile"
	
	umount_image_partition "${basedir}/mnt_devel_libs_install"
	
	title "Development libraries installation"
	launch_vm "${basedir}/${RPI_VERSION}/arch-linux-devel.img"
	
	title "Development libraries' scripts deletion"
	mount_image_partition "${basedir}/${RPI_VERSION}/arch-linux-devel.img" 2 "${basedir}/mnt_devel_scripts_deletion"
	
	rm -f "${basedir}/mnt_devel_scripts_deletion/etc/systemd/system/getty@tty1.service.d/autologin.conf"
	rm -f "${basedir}/mnt_devel_scripts_deletion/root/.bash_profile"
	
	umount_image_partition "${basedir}/mnt_devel_scripts_deletion"
else
	echo "Arch Linux for development librairies is already installed."
fi

title "Crosstool-ng installation check"
if [ ! -e "${crosstoolngDir}/bin/ct-ng" ]
then
	if [ ! -e "${basedir}/crosstool-ng" ]
	then
		title "Crosstool-ng installation"
		yum install -y gperf gawk bison flex texinfo patch libtool ncurses-devel cvs automake subversion python-devel cmake gcc-c++ glibc-devel glibc-static libstdc++-static
		#libexpat1-devel ??
		
		downloadCrosstoolng=0
		download_file "${CROSSTOOLNG_SHA1_URL}" "${basedir}/crosstoolng.tar.bz2.sha1"
		
		if [ -e "${basedir}/crosstoolng.tar.bz2" ]
		then
			localSha1=$(sha1sum "${basedir}/crosstoolng.tar.bz2" | awk '{print $1}')
			remoteSha1=$(cat "${basedir}/crosstoolng.tar.bz2.sha1" | awk '{print $1}')
			
			if [ "${localSha1}" = "${remoteSha1}" ]
			then
				echo "Crosstool-ng archive is already present. Skipping download."
				downloadCrosstoolng=1
			fi
		fi
		
		if [ ${downloadCrosstoolng} -eq 0 ]
		then
			download_file "${CROSSTOOLNG_URL}" "${basedir}/crosstoolng.tar.bz2"
			
			localSha1=$(sha1sum "${basedir}/crosstoolng.tar.bz2" | awk '{print $1}')
			remoteSha1=$(cat "${basedir}/crosstoolng.tar.bz2.sha1" | awk '{print $1}')
			
			if [ "${localSha1}" != "${remoteSha1}" ]
			then
				echo "Downloaded file checksums do not match. Exiting."
				exit 1
			fi
		fi

		tar xjf "${basedir}/crosstoolng.tar.bz2"
		cd "${basedir}/crosstool-ng-${CROSSTOOLNG_VERSION}"

		if [ ! -e "configure" ]
		then
			./bootstrap
		fi

		./configure --prefix ${CROSSTOOLNG_DIR}

		make
		make install
		cd ${basedir}
		
		# eglibc 2.13 config patch for make 4.0+ support
		mkdir -p "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/patches/eglibc/2_13"
		cp "${basedir}/eglibc_2_13_make_4.patch" "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/patches/eglibc/2_13/eglibc_2_13_make_4.patch"
		chmod 755 "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/patches/eglibc"
		chmod 755 "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/patches/eglibc/2_13"
		chmod 644 "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/patches/eglibc/2_13/eglibc_2_13_make_4.patch"
		
		# Install "sample" config
		mkdir -p "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/samples/arm-bcm2708hardfp-gnueabi"
		cp "${basedir}/bcm2708hardfp-ct-ng.config" "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/samples/arm-bcm2708hardfp-gnueabi/crosstool.config"
		echo "reporter_name=\"Julien vaslet\"
reporter_url=\"http://www.julienvaslet.fr/\"
reporter_comment=\"Toolchain for the Raspberry Pi (ARMv6), with hard-float.\"" > "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/samples/arm-bcm2708hardfp-gnueabi/reported.by"
		
		chmod 755 "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/samples/arm-bcm2708hardfp-gnueabi"
		chmod 644 "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/samples/arm-bcm2708hardfp-gnueabi/crosstool.config"
		chmod 644 "${CROSSTOOLNG_DIR}/lib/ct-ng.${CROSSTOOLNG_VERSION}/samples/arm-bcm2708hardfp-gnueabi/reported.by"
		
		rm -f "${basedir}/crosstoolng.tar.bz2"
		rm -f "${basedir}/crosstoolng.tar.bz2.sha1"
		rm -rf "${basedir}/crosstool-ng-${CROSSTOOLNG_VERSION}"
	else
		echo "Crosstool-ng is not installed but \"${basedir}/crosstool-ng\" exists."
		echo "Please remove it before continue."
		exit 1
	fi
else
	echo "Crosstool-ng is installed."
fi

title "Cross-compiler installation check"
if [ ! -e "${PI_BINARIES_DIR}/arm-rpi-linux-gnueabi/bin/g++" ]
then
	title "Cross-compiler installation"
	if [ -e "${basedir}/ctngpi.build/" ]
	then
		echo "Directory \"${basedir}/ctngpi.build\" already exists."
		echo "Please remove it to continue compilation."
		exit 1
	fi
	
	mkdir -p "${basedir}/ctngpi.build"
	
	cd "${basedir}/ctngpi.build"
	${CROSSTOOLNG_DIR}/bin/ct-ng arm-bcm2708hardfp-gnueabi

	# Patch installation directory path
	sed -i "s|^CT_PREFIX_DIR=.*|CT_PREFIX_DIR=\"${PI_BINARIES_DIR}/\${CT_TARGET}\"|g" "${basedir}/ctngpi.build/.config"

	# Patch parallel jobs
	sed -i "s|^CT_PARALLEL_JOBS=.*|CT_PARALLEL_JOBS=${PARALLEL_THREADS}|g" "${basedir}/ctngpi.build/.config"

	${CROSSTOOLNG_DIR}/bin/ct-ng build
	
	if [ "$?" != "0" ]
	then
		echo "Compiler compilation seems to fail."
		echo "For futher information, read logs in \"${basedir}/ctngpi.build/build.log\""
		exit 1
	fi
	
	cd ${basedir}
	rm -rf "${basedir}/ctngpi.build"
	echo "Installed."
else
	echo "Directory ${PI_BINARIES_DIR} already exists."
fi

title "Raspberry Pi headers & libraries installation check"
if [ ! -e "${PI_BINARIES_DIR}/RPI" ]
then
	title "Raspberry Pi headers & libraries installation"
	mkdir -p "${PI_BINARIES_DIR}/RPI/"
	
	mount_image_partition "${basedir}/${RPI_VERSION}/arch-linux-devel.img" 2 "${basedir}/mnt_hlibs_copy"
	
	cp -R "${basedir}/mnt_hlibs_copy/"{opt,lib,usr} "${PI_BINARIES_DIR}/RPI/"
	chown -R root:root "${PI_BINARIES_DIR}/RPI"
	find "${PI_BINARIES_DIR}/RPI" -type d -print0 | xargs -0 -n1 -I'{}' chmod 755 {}
	find "${PI_BINARIES_DIR}/RPI" -type f -print0 | xargs -0 -n1 -I'{}' chmod 644 {}
	
	umount_image_partition "${basedir}/mnt_hlibs_copy"

	echo "Headers & libraries copied in \"${PI_BINARIES_DIR}/RPI\"."
	
	title "Symbolic links remapping"
	
	OLDIFS=${IFS}
	IFS=$'\n'
	
	for link in $(find "${PI_BINARIES_DIR}/RPI" -type l)
	do
		if [ "$(ls -l "${link}" | grep "\-> /" | wc -l)" != "0" ]
		then
			destination=$(ls -l "${link}" | grep -o "\->.*" | sed 's/^->\s*//g')
			rm -f "${link}"
			ln -s "${PI_BINARIES_DIR}/RPI${destination}" "${link}"
			echo "${link} remapped."
		fi
	done
	
	IFS=${OLDIFS}
	
else
	echo "Directory \"${PI_BINARIES_DIR}/RPI\" already exists."
	echo "Headers & libraries seems to be installed."
fi

