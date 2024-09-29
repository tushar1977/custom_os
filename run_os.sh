#!/bin/sh
set -e
. ./clean.sh
. ./build.sh
qemu-system-i386 -kernel sysroot/boot/myos.kernel -serial file:serial.log
