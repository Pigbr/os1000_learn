.section .text.boot
.global _start
_start:
    la sp, _stack_top      # 初始化 stack 指標
    call main
1:  j 1b                  # 無限迴圈，防止返回

# 定義 stack
.section .bss
.space 1024
.global _stack_top        # <- 這裡必須加 global
_stack_top:
