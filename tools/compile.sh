#!/bin/bash
basedir=$(cd `dirname $0`; pwd)
projectDirectory=${basedir}/..
image=${basedir}/arm-pi/images/Pidora-2014-R3.img
kernel=${basedir}/arm-pi/images/kernel-qemu
mountpoint=${basedir}/arm-pi/mountpoint
guestDirectory=/root/compile
target=pi
applications="$*"

#id -u test

if [ -z "$applications" ]
then
	echo "usage: $(basename $0) <application0> <application1> ... <applicationN>"
	echo "<applicationI> shall be valid Makefile targets"
	exit 1
fi

function mount_image ()
{
	# Start sector of the last partition
	startsector=$(file ${image} | grep -o "startsector\s\+[0-9]\+" | tail -n1 | cut -f2 -d' ')
	offset=$(echo "${startsector} * 512" | bc)

	mkdir -p ${mountpoint}
	sudo mount -o offset=${offset} ${image} ${mountpoint}
}

function umount_image ()
{
	sudo umount ${mountpoint}
	rm -rf ${mountpoint}
}

mount_image

# Copy auto-compilation script
sudo cp ${basedir}/init.sh ${mountpoint}/root/init.sh
sudo chmod 750 ${mountpoint}/root/init.sh
sudo sed -i "s|^target=|target=${target}|g" ${mountpoint}/root/init.sh
sudo sed -i "s|^applications=|applications=${applications}|g" ${mountpoint}/root/init.sh
sudo sed -i "s|^workingDirectory=|workingDirectory=${guestDirectory}|g" ${mountpoint}/root/init.sh

# Setting auto-compilation @ bash login
if [ $(sudo grep "^/root/init.sh" ${mountpoint}/root/.bashrc | wc -l) = "0" ]
then
	sudo bash -c "echo -e \"\n# Auto-compilation\n/root/init.sh\" >> ${mountpoint}/root/.bashrc"
fi

# Synchronize projects files
sudo mkdir -p ${mountpoint}/${guestDirectory}
sudo cp -R ${projectDirectory}/{apps,bin,data,get-dependencies.sh,include,Makefile,source,targets} ${mountpoint}${guestDirectory}/

umount_image

# Start VM
${basedir}/arm-pi/qemu/bin/qemu-system-arm -cpu arm1176 -hda ${image} -kernel ${kernel} -append "root=/dev/sda2 panic=1 rootfstype=ext4 rw quiet vga=current" -m 256 -no-reboot -serial stdio -machine versatilepb

mount_image

# Synchronize compiled files
mkdir -p ${projectDirectory}/{bin,lib}/${target}
cp -R ${mountpoint}${guestDirectory}/bin/${target}/* ${projectDirectory}/bin/${target}/
cp -R ${mountpoint}${guestDirectory}/lib/${target}/* ${projectDirectory}/lib/${target}/

umount_image

