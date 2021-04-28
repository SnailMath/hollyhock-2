.section .init
mov.l main_addr, r0
jmp @r0
nop
.align 2
main_addr:
.long _main
load_addr:
.long 0x8cff0000
