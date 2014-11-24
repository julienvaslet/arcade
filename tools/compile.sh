#!/bin/bash
basedir=$(cd `dirname $0`; pwd)
projectDirectory=${basedir}/..
image=${basedir}/arm-pi/images/Pidora-2014-R3.img
kernel=${basedir}/arm-pi/images/kernel-qemu
mountpoint=${basedir}/arm-pi/mountpoint
guestDirectory=/root/compile
target=pi
applications="$*"
projectOwner=$(stat -c %U ${projectDirectory})
projectGroup=$(stat -c %G ${projectDirectory})

if [ $(id -u) != "0" ]
then
	echo "You must be root (or equivalent) to execute this script."
	echo "Some embedded commands (for instance, mount) need it."
	exit 1
fi

if [ -z "$applications" ]
then
	echo "usage: $(basename $0) <application0> <application1> ... <applicationN>"
	echo "<applicationI> shall be valid Makefile targets"
	exit 2
fi

function mount_image ()
{
	# Start sector of the last partition
	startsector=$(file ${image} | grep -o "startsector\s\+[0-9]\+" | tail -n1 | cut -f2 -d' ')
	offset=$(echo "${startsector} * 512" | bc)

	mkdir -p ${mountpoint}
	mount -o offset=${offset} ${image} ${mountpoint}
}

function umount_image ()
{
	umount ${mountpoint}
	rm -rf ${mountpoint}
}

mount_image

# Copy auto-compilation script
cp ${basedir}/init.sh ${mountpoint}/root/init.sh
chmod 750 ${mountpoint}/root/init.sh
sed -i "s|^target=|target=${target}|g" ${mountpoint}/root/init.sh
sed -i "s|^applications=|applications=${applications}|g" ${mountpoint}/root/init.sh
sed -i "s|^workingDirectory=|workingDirectory=${guestDirectory}|g" ${mountpoint}/root/init.sh

# Setting auto-compilation @ bash login
if [ $(grep "^/root/init.sh" ${mountpoint}/root/.bashrc | wc -l) = "0" ]
then
	echo -e "\n# Auto-compilation\n/root/init.sh" >> ${mountpoint}/root/.bashrc
fi

# Synchronize projects files
mkdir -p ${mountpoint}/${guestDirectory}
cp -R ${projectDirectory}/{apps,bin,data,get-dependencies.sh,include,Makefile,source,targets} ${mountpoint}${guestDirectory}/

umount_image

# Start VM
${basedir}/arm-pi/qemu/bin/qemu-system-arm -cpu arm1176 -hda ${image} -kernel ${kernel} -append "root=/dev/sda2 panic=1 rootfstype=ext4 rw quiet vga=current" -m 256 -no-reboot -serial stdio -machine versatilepb

mount_image

# Synchronize compiled files
mkdir -p ${projectDirectory}/{bin,lib}/${target}
cp -R ${mountpoint}${guestDirectory}/bin/${target}/* ${projectDirectory}/bin/${target}/
cp -R ${mountpoint}${guestDirectory}/lib/${target}/* ${projectDirectory}/lib/${target}/

chown -R ${projectOwner}:${projectGroup} ${projectDirectory}/{bin,lib}/${target}

umount_image

