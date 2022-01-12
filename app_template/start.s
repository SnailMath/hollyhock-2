.section .init
the_loading_address:

mov.l main_addr, r0
jmp @r0
nop

.align 2
main_addr:
.long _main
load_addr:
.long the_loading_address 
