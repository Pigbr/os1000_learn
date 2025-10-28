#include "kernel.h"
#include "common.h"

extern char __bss[], __bss_end[], __stack_top[];

// sbi call
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid) {
    register long a0 __asm__("a0") = arg0;
    register long a1 __asm__("a1") = arg1;
    register long a2 __asm__("a2") = arg2;
    register long a3 __asm__("a3") = arg3;
    register long a4 __asm__("a4") = arg4;
    register long a5 __asm__("a5") = arg5;
    register long a6 __asm__("a6") = fid;
    register long a7 __asm__("a7") = eid;

    __asm__ __volatile__("ecall"
                         : "=r"(a0), "=r"(a1)
                         : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                           "r"(a6), "r"(a7)
                         : "memory");
    return (struct sbiret){.error = a0, .value = a1};
}

// 調用 sbi call 來 conaole putchar
void putchar(char ch) {
    sbi_call(ch, 0, 0, 0, 0, 0, 0, 1 /* Console Putchar */);
}

__attribute__((naked))
__attribute__((aligned(4)))
void kernel_entry(void) {
    __asm__ __volatile__(
        "csrw sscratch, sp\n"
        "addi sp, sp, -4 * 31\n"
        "sw ra,  4 * 0(sp)\n"
        "sw gp,  4 * 1(sp)\n"
        "sw tp,  4 * 2(sp)\n"
        "sw t0,  4 * 3(sp)\n"
        "sw t1,  4 * 4(sp)\n"
        "sw t2,  4 * 5(sp)\n"
        "sw t3,  4 * 6(sp)\n"
        "sw t4,  4 * 7(sp)\n"
        "sw t5,  4 * 8(sp)\n"
        "sw t6,  4 * 9(sp)\n"
        "sw a0,  4 * 10(sp)\n"
        "sw a1,  4 * 11(sp)\n"
        "sw a2,  4 * 12(sp)\n"
        "sw a3,  4 * 13(sp)\n"
        "sw a4,  4 * 14(sp)\n"
        "sw a5,  4 * 15(sp)\n"
        "sw a6,  4 * 16(sp)\n"
        "sw a7,  4 * 17(sp)\n"
        "sw s0,  4 * 18(sp)\n"
        "sw s1,  4 * 19(sp)\n"
        "sw s2,  4 * 20(sp)\n"
        "sw s3,  4 * 21(sp)\n"
        "sw s4,  4 * 22(sp)\n"
        "sw s5,  4 * 23(sp)\n"
        "sw s6,  4 * 24(sp)\n"
        "sw s7,  4 * 25(sp)\n"
        "sw s8,  4 * 26(sp)\n"
        "sw s9,  4 * 27(sp)\n"
        "sw s10, 4 * 28(sp)\n"
        "sw s11, 4 * 29(sp)\n"

        "csrr a0, sscratch\n"
        "sw a0, 4 * 30(sp)\n"

        "mv a0, sp\n"
        "call handle_trap\n"

        "lw ra,  4 * 0(sp)\n"
        "lw gp,  4 * 1(sp)\n"
        "lw tp,  4 * 2(sp)\n"
        "lw t0,  4 * 3(sp)\n"
        "lw t1,  4 * 4(sp)\n"
        "lw t2,  4 * 5(sp)\n"
        "lw t3,  4 * 6(sp)\n"
        "lw t4,  4 * 7(sp)\n"
        "lw t5,  4 * 8(sp)\n"
        "lw t6,  4 * 9(sp)\n"
        "lw a0,  4 * 10(sp)\n"
        "lw a1,  4 * 11(sp)\n"
        "lw a2,  4 * 12(sp)\n"
        "lw a3,  4 * 13(sp)\n"
        "lw a4,  4 * 14(sp)\n"
        "lw a5,  4 * 15(sp)\n"
        "lw a6,  4 * 16(sp)\n"
        "lw a7,  4 * 17(sp)\n"
        "lw s0,  4 * 18(sp)\n"
        "lw s1,  4 * 19(sp)\n"
        "lw s2,  4 * 20(sp)\n"
        "lw s3,  4 * 21(sp)\n"
        "lw s4,  4 * 22(sp)\n"
        "lw s5,  4 * 23(sp)\n"
        "lw s6,  4 * 24(sp)\n"
        "lw s7,  4 * 25(sp)\n"
        "lw s8,  4 * 26(sp)\n"
        "lw s9,  4 * 27(sp)\n"
        "lw s10, 4 * 28(sp)\n"
        "lw s11, 4 * 29(sp)\n"
        "lw sp,  4 * 30(sp)\n"
        "sret\n"
    );
}

void handle_trap(struct trap_frame *f) {
    uint32_t scause = READ_CSR(scause);
    uint32_t stval  = READ_CSR(stval);
    uint32_t sepc   = READ_CSR(sepc);

    uint32_t ra  = f->ra;
    uint32_t gp  = f->gp;
    uint32_t tp  = f->tp;
    uint32_t t0  = f->t0;
    uint32_t t1  = f->t1;
    uint32_t t2  = f->t2;
    uint32_t t3  = f->t3;
    uint32_t t4  = f->t4;
    uint32_t t5  = f->t5;
    uint32_t t6  = f->t6;
    uint32_t a0  = f->a0;
    uint32_t a1  = f->a1;
    uint32_t a2  = f->a2;
    uint32_t a3  = f->a3;
    uint32_t a4  = f->a4;
    uint32_t a5  = f->a5;
    uint32_t a6  = f->a6;
    uint32_t a7  = f->a7;
    uint32_t s0  = f->s0;
    uint32_t s1  = f->s1;
    uint32_t s2  = f->s2;
    uint32_t s3  = f->s3;
    uint32_t s4  = f->s4;
    uint32_t s5  = f->s5;
    uint32_t s6  = f->s6;
    uint32_t s7  = f->s7;
    uint32_t s8  = f->s8;
    uint32_t s9  = f->s9;
    uint32_t s10 = f->s10;
    uint32_t s11 = f->s11;
    uint32_t sp  = f->sp;

    PANIC(
        "Unexpected trap!\n"
        "scause=%x  stval=%x  sepc=%x\n"
        "\n"
        "ra=%x  gp=%x  tp=%x  sp=%x\n"
        "t0=%x  t1=%x  t2=%x  t3=%x\n"
        "t4=%x  t5=%x  t6=%x\n"
        "a0=%x  a1=%x  a2=%x  a3=%x\n"
        "a4=%x  a5=%x  a6=%x  a7=%x\n"
        "s0=%x  s1=%x  s2=%x  s3=%x\n"
        "s4=%x  s5=%x  s6=%x  s7=%x\n"
        "s8=%x  s9=%x  s10=%x  s11=%x\n",
        scause, stval, sepc,
        ra, gp, tp, sp,
        t0, t1, t2, t3, t4, t5, t6,
        a0, a1, a2, a3, a4, a5, a6, a7,
        s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11
    );
}


// kernel
void kernel_main(void) {

    printf("---------boot and kernel main start-------------\n");
    // Clear the BSS segment
    memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
    printf("clear bss done\n");

    printf("-------------------myRISC-V OS-------------------\n\n");

    WRITE_CSR(stvec, (uint32_t) kernel_entry); // new
    __asm__ __volatile__("unimp"); // new

    for (;;)
    {
        __asm__ __volatile__("wfi");
    }

}



// boot
__attribute__((section(".text.boot")))
__attribute__((naked))
void boot(void) {
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" // Set the stack pointer
        "j kernel_main\n"       // Jump to the kernel main function
        :
        : [stack_top] "r" (__stack_top) // Pass the stack top address as %[stack_top]
    );
}