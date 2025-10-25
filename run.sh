#!/bin/bash
set -xue

# QEMU file path
QEMU=qemu-system-riscv32

# Start QEMU
$QEMU -machine virt \
      -bios /home/kevin/Desktop/os1000/openSBI/opensbi-riscv32-generic-fw_dynamic.bin \
      -nographic \
      -serial mon:stdio \
      --no-reboot