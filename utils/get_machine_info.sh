#!/bin/sh

# Incomplete commands, just manually run them...

echo "PROCESSOR INFO:"
echo "#########"
echo

echo "CACHE INFO:" 
echo "#########"
echo 
getconf -a | grep ^LEVEL

echo "TOTAL MEMORY: "
echo "#########"
echo
grep MemTotal /proc/meminfo

echo "OPERATING SYSTEM INFO:"
echo "#########"
echo 
echo $(cat /etc/os-release | grep ^NAME=)
echo $(cat /etc/os-release | grep ^VERSION=)
echo "Linux Kernel Version: "  $(uname-r)