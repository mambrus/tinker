# 1 "/home/ambrmi09/tinker/tinker/bsp/gnu_bfin/startup_gnu.S"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "/home/ambrmi09/tinker/tinker/bsp/gnu_bfin/startup_gnu.S"
# 30 "/home/ambrmi09/tinker/tinker/bsp/gnu_bfin/startup_gnu.S"
        .section .init
        .globl __init
        .type __init,@function
__init:
        .section .fini
        .globl __fini
        .type __fini,@function
__fini:


 .text
 .align 2

 .global __start
__start:


 sp.h = 0xFFB0;
 sp.l = 0x0F00;




 p1.l = __end;
 p1.h = __end;
 p1 -= p0;
 r0 = 0;
# 94 "/home/ambrmi09/tinker/tinker/bsp/gnu_bfin/startup_gnu.S"
    p0.l = START;
    p0.h = START;





    p1.l = (2);


    [P1] = P0;





    R0 = [P0];

    R1.l = 0x8000;

    R0 = R0 | R1;

 [P0] = R0;

 RAISE 15;

 PO.l = WAIT;
 P0.h = WAIT;

 RETI = P0;
 RTI;


 WAIT:
 jump WAIT;

 START:
 [--SP] = RETI;
    p0.l = _boot_card;
    p0.h = _boot_card;

    call (p0);

 p0.l = _exit;
 p0.h = _exit;
 P3 = P4;
 jump (p0)

.global _null_isr
_null_isr:
 rti;

 .data

.global __stack_base__
HeapBase: .dd 0
HeapLimit: .dd 0
__stack_base__: .dd 0
StackLimit: .dd 0
CommandLine: .space 256,0
