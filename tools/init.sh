#!/bin/bash
applications=""
target=""
workingDirectory=""

currentDate=$(stat -c %Y $0)

echo "Setting current date"
echo "===================="
date --set="@${currentDate}" &> /dev/null
echo "Current date: $(date)"
echo

echo "Project compilation"
echo "==================="
echo "Target: ${target}"
echo "Makefile targets: ${applications}"
echo "Working directory: ${workingDirectory}"
echo

cd ${workingDirectory}
:> compilation.log
make ${applications} for="${target}" | tee compilation.log
echo

echo "Automatic shutdown"
echo "=================="
reboot
