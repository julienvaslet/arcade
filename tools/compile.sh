#!/bin/bash
basedir=$(cd `dirname $0`; pwd)
projectDirectory=${basedir}/..
image=${basedir}/arm-pi/images/pidora.img
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
	tries=3
	ret=1
	while [ ${ret} != "0" ]
	do
		umount ${mountpoint}
		ret=$?
		tries=$(expr ${tries} - 1)
		
		if [ ${tries} -eq 0 ]
		then
			echo "Unable to unmount \"${mountpoint}\"."
			return
		fi
		
		if [ "${ret}" != "0" ]
		then
			sleep 1
		fi
	done
	
	rmdir ${mountpoint}
}

mount_image

# Copy auto-compilation script (done by initialization)
#cp ${basedir}/init.sh ${mountpoint}/root/compile.sh
#chmod 750 ${mountpoint}/root/compile.sh

# Set compilation variables
sed -i "s|^target=.*|target=${target}|g" ${mountpoint}/root/compile.sh
sed -i "s|^applications=.*|applications=${applications}|g" ${mountpoint}/root/compile.sh
sed -i "s|^workingDirectory=.*|workingDirectory=${guestDirectory}|g" ${mountpoint}/root/compile.sh

# Synchronize projects files
mkdir -p ${mountpoint}${guestDirectory}
rm -rf ${mountpoint}${guestDirectory}/*
cp -R ${projectDirectory}/{apps,bin,lib,data,get-dependencies.sh,include,Makefile,source,targets} ${mountpoint}${guestDirectory}/

umount_image

# Start VM
${basedir}/arm-pi/qemu/bin/qemu-system-arm -cpu arm1176 -hda ${image} -kernel ${kernel} -append "root=/dev/sda2 panic=1 rootfstype=ext4 rw quiet vga=current" -m 256 -no-reboot -serial stdio -machine versatilepb

mount_image

# Show compilation log
cat ${mountpoint}${guestDirectory}/compilation.log
rm -f ${mountpoint}${guestDirectory}/compilation.log

# Synchronize compiled files
mkdir -p ${projectDirectory}/{bin,lib}/${target}
cp -R ${mountpoint}${guestDirectory}/bin/${target}/* ${projectDirectory}/bin/${target}/
cp -R ${mountpoint}${guestDirectory}/lib/${target}/* ${projectDirectory}/lib/${target}/

chown -R ${projectOwner}:${projectGroup} ${projectDirectory}/{bin,lib}/${target}

umount_image

