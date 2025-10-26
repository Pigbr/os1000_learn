int global_data = 42;     // .data
int global_bss;            // .bss

volatile unsigned int *UART0 = (unsigned int *)0x10000000;

void putchar(char c) {
    *UART0 = c;
}

void puts(const char *s) {
    while (*s) putchar(*s++);
}

// 將地址印成十六進位
void print_addr(const char *name, unsigned int addr) {
    puts(name);
    puts(": 0x");
    for (int i = 7; i >= 0; i--) {
        int nibble = (addr >> (i*4)) & 0xF;
        putchar(nibble < 10 ? '0'+nibble : 'A'+nibble-10);
    }
    puts("\n");
}

int main() {
    puts("Hello from bare-metal RISC-V!\n");
    puts("---------------memory allocation-----------------------\n");

    print_addr("main .text", (unsigned int)main);
    print_addr("global_data .data", (unsigned int)&global_data);
    print_addr("global_bss .bss", (unsigned int)&global_bss);

    // stack 地址（從 _stack_top 標籤取得）
    extern unsigned int _stack_top;
    print_addr("stack top", (unsigned int)&_stack_top);

    while (1);
}
