#!/bin/bash
basedir=$(cd `dirname $0`; pwd)
tmpDirectory="/tmp"
pidoraVersion="20"
pidoraArch="armv6hl"
pidoraRepo="http://pidora.ca/pidora/releases/${pidoraVersion}/images/"
SDL2Version="2.0.3"

if [ $(id -u) != "0" ]
then
	echo "You must be root (or equivalent) to execute this script."
	echo "Some embedded commands (for instance, mount) need it."
	exit 1
fi

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

function mount_image ()
{
	# Start sector of the last partition
	startsector=$(file $1 | grep -o "startsector\s\+[0-9]\+" | tail -n1 | cut -f2 -d' ')
	offset=$(echo "${startsector} * 512" | bc)

	mkdir -p $2
	mount -o offset=${offset} $1 $2
}

function umount_image ()
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

title "Checking necessary commands"
check_command yum
check_command git
check_command make
check_command rsync

mkdir -p ${basedir}/arm-pi/qemu/{bin,share,lib,libexec,include,etc}
mkdir -p ${basedir}/arm-pi/images

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

# TODO: kernel qemu ?

title "Checking Pidora image"
if [ ! -e "${basedir}/arm-pi/images/pidora.img" ]
then
	echo "Pidora image is not installed."
	
	title "Downloading latest Pidora image"
	pidoraFile=$(curl -s ${pidoraRepo} | grep -o "href=\"Pidora-[^\"]\+" | sed 's/href="//g' | sort -r | head -n1)
	echo "Latest Pidora image is: ${pidoraFile/.zip}"
	
	cd ${basedir}/arm-pi/images
	wget ${pidoraRepo}/${pidoraFile}
	unzip ${pidoraFile}
	mv ${pidoraFile/.zip}.img pidora.img
	rm -f ${pidoraFile} ${pidoraFile/.zip}.img.md5sum
	
	image="${basedir}/arm-pi/images/pidora.img"
	mountpoint="${basedir}/arm-pi/images/mountpoint"
	
	if [ ! -e "${image}" ]
	then
		echo "Unable to download the Pidora image."
		echo "Exiting."
		exit 1
	fi
	
	title "Extending image by 500 mo"
	dd bs=1024k count=500 if=/dev/zero >> ${image}
	startPartition=$(echo -e "unit chs\nprint\nquit\n" | parted ${image} 2>/dev/null | grep "^ 2" | awk '{print $2}')
	endPartition=$(echo -e "unit chs\nprint\nquit\n" | parted ${image} 2>/dev/null | grep "Disk /" | cut -f2 -d':')
	echo -e "unit chs\nrm 2\nmkpart primary ${startPartition} ${endPartition}\nprint\nquit\n" | parted ${image}
	
	title "Configuring Pidora installation"
	mount_image ${image} ${mountpoint}

	# Seems to be bugged
	echo -n "Setting keyboard configuration to french..."
	sed	-i 's/^KEYTABLE=.*$/KEYTABLE="fr-latin1"/' ${mountpoint}/etc/sysconfig/keyboard
	sed	-i 's/^MODEL=.*$/MODEL="pc105"/' ${mountpoint}/etc/sysconfig/keyboard
	sed	-i 's/^LAYOUT=.*$/LAYOUT="fr"/' ${mountpoint}/etc/sysconfig/keyboard
	echo " Ok."
	
	# Seems to be bugged
	echo -n "Setting system locale to french..."
	sed -i 's/^LANG=.*$/LANG="fr_FR.utf8"/' ${mountpoint}/etc/sysconfig/i18n
	if [ $(grep "^LC_ALL=" ${mountpoint}/etc/profile | wc -l) = "0" ]
	then
		echo >> ${mountpoint}/etc/profile
		echo "LC_ALL=fr_FR.utf8" >> ${mountpoint}/etc/profile
		echo "LANG=fr_FR.utf8" >> ${mountpoint}/etc/profile
		echo "export -p LC_ALL" >> ${mountpoint}/etc/profile
		echo "export -p LANG" >> ${mountpoint}/etc/profile
	fi
	echo " Ok."

	echo -n "Setting timezone to Europe/Paris..."
	cd ${mountpoint}/etc
	rm -f localtime
	ln -s ../usr/share/zoneinfo/Europe/Paris localtime
	cd ${basedir}
	echo " Ok."

	echo -n "Setting default boot mode to text-mode..."
	cd ${mountpoint}/etc/systemd/system
	rm -f default.target
	ln -s ../../../lib/systemd/system/multi-user.target default.target
	cd ${basedir}
	echo " Ok."

	echo -n "Desactivating firstboot..."
	echo "RUN_FIRSTBOOT=NO" > ${mountpoint}/etc/sysconfig/firstboot
	echo " Ok."
	
	echo -n "Setting default root password to \"arcade\"..."
	sed	-i 's/^root:.*$/root:$1$ZcsbxZuR$mskYal8zQK2LgCqzH\/sbr0:16410:0:99999:7:::/' ${mountpoint}/etc/shadow-
	cp ${mountpoint}/etc/shadow- ${mountpoint}/etc/shadow
	echo " Ok."

	echo -n "Activating root autologin..."
	cp ${basedir}/auto-configure.sh ${mountpoint}/root/auto-configure.sh
	chmod 750 ${mountpoint}/root/auto-configure.sh
	cp ${basedir}/init.sh ${mountpoint}/root/compile.sh
	chmod 750 ${mountpoint}/root/compile.sh
	
	if [ $(grep "^# Startup script" ${mountpoint}/root/.bashrc | wc -l) = "0" ]
	then
		echo -e "\n# Startup script\n/root/auto-configure.sh" >> ${mountpoint}/root/.bashrc
	else
		sed -i "s|/root/compile\.sh|/root/auto-configure.sh|" ${mountpoint}/root/.bashrc
	fi
	
	serviceFile="${mountpoint}/etc/systemd/system/getty@tty1.service.d/autologin.conf"
	mkdir -p ${mountpoint}/etc/systemd/system/getty@tty1.service.d
	echo "[Service]" > ${serviceFile}
	echo "ExecStart=" >> ${serviceFile}
	echo "ExecStart=-/sbin/agetty --autologin root --noclear %I 38400 linux" >> ${serviceFile}
	echo "Type=idle" >> ${serviceFile}
	echo " Ok."
	
	umount_image ${mountpoint}

	title "Compilation packages installation"
	kernel=${basedir}/arm-pi/images/kernel-qemu
	${basedir}/arm-pi/qemu/bin/qemu-system-arm -cpu arm1176 -hda ${image} -kernel ${kernel} -append "root=/dev/sda2 panic=1 rootfstype=ext4 rw" -m 256 -no-reboot -serial stdio -machine versatilepb
	
	title "End of installation"
	echo "Pidora is now installed and configured."
else
	echo "Pidora image is already installed."
fi

