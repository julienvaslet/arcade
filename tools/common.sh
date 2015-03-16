#!/bin/bash

function expect_root_user ()
{
	if [ $(id -u) != "0" ]
	then
		echo "You must be root (or equivalent) to execute this script."
		echo "Some embedded commands (for instance, mount) need it."
		exit 1
	fi
}

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

function question ()
{
	ret=""
	values=""
	
	if [ $# -gt 1 ]
	then
		for i in $(seq 3 $#)
		do
			if [ -z "${values}" ]
			then
				values=${!i}
			else
				values="${values}\n${!i}"
			fi
		done
	fi
	
	while [ -z "${ret}" ]
	do
		echo -ne "$1 "
		if [ ! -z ${values} ]
		then
			echo -n "[$(echo -e "${values}" | sed ':a;N;$!ba;s/\n/,/g')] "
		fi
		
		read ret
		valid=1
		
		OLDIFS=${IFS}
		IFS=$'\n'
		for v in $(echo -e "${values}")
		do
			if [ "${v}" = "${ret}" ]
			then
				valid=0
				break
			fi
		done
		
		if [ ${valid} -eq 1 ]
		then
			ret=""
		fi
	done
	
	eval "$2=\"${ret}\""
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

function mount_image_partition ()
{
	# Start sector of the N partition
	startsector=$(file $1 | grep -o "startsector\s\+[0-9]\+" | head -n$2 | tail -n1 | cut -f2 -d' ')
	offset=$(echo "${startsector} * 512" | bc)

	mkdir -p $3
	mount -o offset=${offset} $1 $3
}

function umount_image_partition ()
{
	tries=3
	ret=1
	while [ ${ret} != "0" ]
	do
		umount $1
		ret=$?
		tries=$(expr ${tries} - 1)
		
		if [ ${tries} -eq 0 ]
		then
			echo "Unable to unmount \"$1\"."
			return
		fi
		
		if [ "${ret}" != "0" ]
		then
			sleep 1
		fi
	done
	
	rmdir $1
}

function install_package ()
{
	if [ ! -z "$1" ]
	then
		echo "Installing package $*..."
		yum install -y $*
		
		if [ "$?" != 0 ]
		then
			echo "An error has occured during package \"$*\" installation."
			echo "Initialization aborted and must be re-run."
			exit 1
		fi
	else
		echo "No package specified. Ignoring..."
	fi
}

function download_file ()
{
	if [ ! -z "$1" -a ! -z "$2" ]
	then
		if [ -e "$2 " ]
		then
			echo "Removing old file \"$2\"..."
			rm -f "$2"
		fi
		
		echo "Downloading $1..."
		wget -q --show-progress "$1" -O "$2"
		
		if [ "$?" != 0 ]
		then
			echo "An error has occured during downloading \"$1\"."
			echo "Initialization aborted and must be re-run."
			exit 1
		fi
	else
		echo "URL or filename (or both) have not been specified for download."
		exit 2
	fi
}

# Launch a QEMU Environment
function launch_vm ()
{
	if [ -z "${basedir}" ]
	then
		basedir=$(cd `dirname $0`; pwd)
	fi
	
	apply_qemu_patches $1

	qemu-system-arm -kernel ${basedir}/arm1176-kernel -cpu arm1176 -m 256 -M versatilepb -no-reboot -serial stdio -append "root=/dev/sda2 panic=1 rootfstype=ext4 rw" -hda $1
	
	remove_qemu_patches $1
}

# Patches for QEMU Environment
function apply_qemu_patches ()
{
	if [ -z "${basedir}" ]
	then
		basedir=$(cd `dirname $0`; pwd)
	fi
	
	if [ -z "$1" ]
	then
		echo "No image specified for patches application."
		return
	fi
	
	mount_image_partition $1 2 "${basedir}/qemu_patches_root"
	
	cd "${basedir}/qemu_patches_root"

	# Device symlinks
	udevQemuFile="etc/udev/rules.d/90-qemu.rules"
	: > "${udevQemuFile}"
	chown root:root "${udevQemuFile}"
	chmod 644 "${udevQemuFile}"
	echo "KERNEL==\"sda\", SYMLINK+=\"mmcblk0\"
KERNEL==\"sda?\", SYMLINK+=\"mmcblk0p%n\"
KERNEL==\"sda2\", SYMLINK+=\"root\"" >> "${udevQemuFile}"
	
	# Fstab
	sed -i 's|^/dev/mmcblk.*$|#\0|g' "etc/fstab"
	
	# Late-mount
	lateMountServiceLink="etc/systemd/system/multi-user.target.wants/late-mount.service"
	lateMountService="usr/lib/systemd/system/late-mount.service"
	: > "${lateMountService}"
	echo "[Unit]
Description=Late-mount /boot
After=systemd-udevd.service

[Service]
Type=oneshot
ExecStart=/bin/mount /dev/mmcblk0p1 /boot

[Install]
WantedBy=multi-user.target
" >> "${lateMountService}"

	ln -s "${lateMountService}" "${lateMountServiceLink}"
	
	cd ${basedir}
	
	umount_image_partition "${basedir}/qemu_patches_root"
}


function remove_qemu_patches ()
{
	if [ -z "${basedir}" ]
	then
		basedir=$(cd `dirname $0`; pwd)
	fi
	
	if [ -z "$1" ]
	then
		echo "No image specified for patches deletion."
		return
	fi
	
	mount_image_partition $1 2 "${basedir}/qemu_patches_root"
	
	cd "${basedir}/qemu_patches_root"
	
	# Device symlinks
	udevQemuFile="etc/udev/rules.d/90-qemu.rules"
	rm -f "${udevQemuFile}"
	
	# Fstab
	sed -i 's|^#\(/dev/mmcblk.*\)$|\1|g' "etc/fstab"
	
	# Late-mount service
	lateMountServiceLink="etc/systemd/system/multi-user.target.wants/late-mount.service"
	lateMountService="usr/lib/systemd/system/late-mount.service"
	rm -f "${lateMountServiceLink}"
	rm -f "${lateMountService}"
	
	cd ${basedir}
	
	umount_image_partition "${basedir}/qemu_patches_root"
}


