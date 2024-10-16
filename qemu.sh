#!/bin/sh
set -e

qemu-system-i386 -s -S -cdrom custom_os.iso
