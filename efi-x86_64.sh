#!/bin/bash

qemu-system-x86_64 -machine q35 -vga std -net none -usb -device usb-tablet \
	-m 128M -cpu host -enable-kvm -nodefaults -smp 1,cores=1 \
  -serial file:debug.log -global isa-debugcon.iobase=0x402 \
  -drive if=pflash,format=raw,unit=0,readonly,file=OVMF_CODE.fd \
  -drive if=pflash,format=raw,unit=1,file=OVMF_VARS.fd \
  -drive file=fat:rw:hd,format=raw
