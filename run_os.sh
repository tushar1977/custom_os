#!/bin/sh
set -e
. ./build.sh
qemu-system-i386 -kernel sysroot/boot/myos.kernel
