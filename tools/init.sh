#!/bin/bash
applications=""
target=""
workingDirectory=""

echo "Project compilation"
echo "==================="
echo "Target: ${target}"
echo "Makefile targets: ${applications}"
echo "Working directory: ${workingDirectory}"
echo

echo "Update files' dates"
echo "==================="
find ${workingDirectory} | xargs -n1 touch -r /root
echo "Ok."
echo

cd ${workingDirectory}
make ${applications} for="${target}"
echo

echo "Automatic shutdown"
echo "=================="
reboot
