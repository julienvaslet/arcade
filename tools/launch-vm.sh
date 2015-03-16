#!/bin/bash
basedir=$(cd `dirname $0`; pwd)

source "${basedir}/common.sh"
source "${basedir}/constants.conf"

expect_root_user

if [ -z "$1" ]
then
	echo "usage: $0 <raw image file>"
	exit
fi

launch_vm $1
