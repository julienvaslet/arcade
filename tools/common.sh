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
