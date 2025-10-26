它展示了 linker_script 的作用

instruction

    clang --target=riscv32 \
    -march=rv32ima -mabi=ilp32 -nostdlib \
    -fuse-ld=lld \
    -T simple.ld \
    -o test.elf start.s main.c

    qemu-system-riscv32 -M virt -bios none  -kernel test.elf -nographic

