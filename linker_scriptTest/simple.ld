ENTRY(_start)

SECTIONS
{
    . = 0x80000000;          /* 程式起始地址 */

    .text : { *(.text*) }    /* 程式碼段 */
    .rodata : { *(.rodata*) }/* read-only data */

    .data : { *(.data*) }    /* 初始化全域變數 */

    .bss : { *(.bss*) }      /* 未初始化全域變數 */

    . = ALIGN(4);
    _stack_top = . + 1024;   /* 定義 stack 頂端 */
}
