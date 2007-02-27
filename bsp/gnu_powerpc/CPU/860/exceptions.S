

# Code for OEA exception handling. This code needs to be at a specific 
# address. This is solved by placing it in a section of its own that 
# we let the linker place where we want it.

/* #include "pushpop.h" */

/*
 0x00000 Reserved
 0x00100 System reset interrupt
 0x00200 Machine check interrupt
 0x00300 DSI
 0x00400 ISI
 0x00500 External Interrupt
 0x00600 Alignment
 0x00700  Program
 0x00800  Floating-point unavailable
 0x00900  Decrementer
0x00A00– Reserved
0x00B00
 0x00C00  System call
 0x00D00  Trace
 0x00E00  Floating-point assist
*/
/*Below is ignored (belongs to impl specific stuff...)*/
/*
 0x01000  Software emulation
 0x01100  Instruction TLB miss
 0x01200  Data TLB miss
 0x01300  Instruction TLB error
 0x01400  Data TLB error
0x01500-   Reserved
0x01B00
 0x01C00  Data breakpoint
 0x01D00  Instruction breakpoint
 0x01E00  Peripheral breakpoint
 0x01F00  Nonmaskable development
*/
/*
		.data
		.global oldSP
oldSP:		.word 0x0
		.end 
*/
		.section .exception
		.equ _TOS, 0x1040		#Temporary top of stack (FIXME)	

		.global _Reserved_0_exception
		.global _System_reset_interrupt_exception
		.global _Machine_check_interrupt_exception
		.global _DSI_exception
		.global _ISI_exception
		.global _External_Interrupt_exception
		.global _Alignment_exception
		.global _Program_exception
		.global _Floating_point_unavailable_exception
		.global _Decrementer_exception
		.global __Reserved_1_exception
		.global _System_call_exception
		.global _Trace_exception
		.global _Floating_point_assist_exception


		.align 8

.func _Reserved_0_exception
_Reserved_0_exception:
			li	%r0,0xF0
			b 	exception_invalid
.fill 0x100 - (. - _Reserved_0_exception) , 1, 0x0
.endfunc

.func _System_reset_interrupt_exception
_System_reset_interrupt_exception:
			li	%r1,_TOS	#Load word imidiate (high part of adress, low we don't care)
			slwi	%r1,%r1,16	#Shift left imidiate
			subi    %r1,%r1,0x100	#Subrtact to give a little margin
			xor %r0,%r0,%r0

			xor %r3,%r0,%r0		#Clean regs so we can see usage on context switch
			xor %r4,%r0,%r0
			xor %r5,%r0,%r0
			xor %r6,%r0,%r0
			xor %r7,%r0,%r0
			xor %r8,%r0,%r0
			xor %r9,%r0,%r0
			xor %r10,%r0,%r0
			xor %r11,%r0,%r0
			xor %r12,%r0,%r0
			xor %r13,%r0,%r0
			xor %r14,%r0,%r0
			xor %r15,%r0,%r0
			xor %r16,%r0,%r0
			xor %r17,%r0,%r0
			xor %r18,%r0,%r0
			xor %r19,%r0,%r0
			xor %r20,%r0,%r0
			xor %r21,%r0,%r0
			xor %r22,%r0,%r0
			xor %r23,%r0,%r0
			xor %r24,%r0,%r0
			xor %r25,%r0,%r0
			xor %r26,%r0,%r0
			xor %r27,%r0,%r0
			xor %r28,%r0,%r0
			xor %r29,%r0,%r0
			xor %r30,%r0,%r0
			xor %r31,%r0,%r0
	#ifndef __rtems__
			bl __init
	#endif
			b main
.fill 0x100 - (. - _System_reset_interrupt_exception) , 1, 0x1
.endfunc

.func _Machine_check_interrupt_exception
_Machine_check_interrupt_exception:
			li	%r0,0xF2
			b 	exception_invalid
.fill 0x100 - (. - _Machine_check_interrupt_exception) , 1, 0x2
.endfunc

.func _DSI_exception
_DSI_exception:
			li	%r0,0xF3
			b 	exception_invalid
.fill 0x100 - (. - _DSI_exception) , 1, 0x3
.endfunc

.func _ISI_exception
_ISI_exception:
			li	%r0,0xF4
			b 	exception_invalid
.fill 0x100 - (. - _ISI_exception) , 1, 0x4
.endfunc

.func _External_Interrupt_exception
_External_Interrupt_exception:
	# Make a jump first because code for storing/restoring context
	# will not fit with in 0x0100 region (would force next entry to break
	# absolute position). 

#	isync
#	eieio

	B _External_Interrupt_exception_code
.fill 0x100 - (. - _External_Interrupt_exception) , 1, 0x5
.endfunc

.func _Alignment_exception
_Alignment_exception:
			li	%r0,0xF6
			b 	exception_invalid
.fill 0x100 - (. - _Alignment_exception) , 1, 0x6
.endfunc

.func _Program_exception
_Program_exception:
			li	%r0,0xF7
			b 	exception_invalid
.fill 0x100 - (. - _Program_exception) , 1, 0x7
.endfunc

.func _Floating_point_unavailable_exception
_Floating_point_unavailable_exception:
			li	%r0,0xF8
			b 	exception_invalid
.fill 0x100 - (. - _Floating_point_unavailable_exception) , 1, 0x8
.endfunc

.func _Decrementer_exception
_Decrementer_exception:
			li	%r0,0xF9
			b 	exception_invalid
.fill 0x100 - (. - _Decrementer_exception) , 1, 0x9
.endfunc

.func __Reserved_1_exception
__Reserved_1_exception:
			li	%r0,0xFA
			b 	exception_invalid
.fill 0x200 - (. - __Reserved_1_exception) , 1, 0xA
.endfunc

.func _System_call_exception
_System_call_exception:
			li	%r0,0xFC
			b 	exception_invalid
.fill 0x100 - (. - _System_call_exception) , 1, 0xC
.endfunc

.func _Trace_exception
_Trace_exception:
			li	%r0,0xFD
			b 	exception_invalid
.fill 0x100 - (. - _Trace_exception) , 1, 0xD
.endfunc

.func _Floating_point_assist_exception
_Floating_point_assist_exception:
			li	%r0,0xFE
			b 	exception_invalid
.fill 0x100 - (. - _Floating_point_assist_exception) , 1, 0xE
.endfunc

		.size _Reserved_0_exception , 			0x00100
		.size _System_reset_interrupt_exception , 	0x00100
		.size _Machine_check_interrupt_exception , 	0x00100
		.size _DSI_exception , 				0x00100
		.size _ISI_exception , 				0x00100
		.size _External_Interrupt_exception , 		0x00100
		.size _Alignment_exception ,		 	0x00100
		.size _Program_exception , 			0x00100
		.size _Floating_point_unavailable_exception ,	0x00100
		.size _Decrementer_exception , 			0x00100
		.size __Reserved_1_exception , 			   0x00200
		.size _System_call_exception , 			0x00100
		.size _Trace_exception , 			0x00100
		.size _Floating_point_assist_exception , 	0x00100


_External_Interrupt_exception_code_macroversion:
	# Interrupts are disabled at this point.
	
	# create an isr stack frame before calling C function
/*	PUSHALL_ASM*/

	#=====================
	bl	isr_external
	#===================== 

	# unwind isr stack frame
/*	POPALL_ASM*/

	rfi

_External_Interrupt_exception_code_nasty:
	# Interrupts are disabled at this point.
	
	# create an isr stack frame before calling C function
	isync			#Instruction cache sync
	sync	0		#Data cach sync
	isync			#Instruction cache sync
	sync	0		#Data cach sync
/*	PUSHALL_ASM*/
	isync			#Instruction cache sync
	sync	0		#Data cach sync
	isync			#Instruction cache sync
	sync	0		#Data cach sync



	#=====================
	bl	isr_external
	#===================== 

	# unwind isr stack frame
	isync			#Instruction cache sync
	sync	0		#Data cach sync
	isync			#Instruction cache sync
	sync	0		#Data cach sync
/*	POPALL_ASM*/
	isync			#Instruction cache sync
	sync	0		#Data cach sync
	isync			#Instruction cache sync
	sync	0		#Data cach sync

	rfi


#define SPADDING ( 16 + 4 + 8)
#define S_SZ (0x90 + SPADDING)

_External_Interrupt_exception_code:
	# Interrupts are disabled at this point.

	#Avoid adjustment using add on popping (will affect CR and XER registers occasionally)
	stwu	%r1,-4(%r1)

	#We'll be using r0 for this operation, so save this first
	stwu	%r0,-4(%r1)

	#Save flags and counter registers (cos we want then poped last)
	mfcr   %r0
	stwu	%r0,-4(%r1)
	mfxer	%r0
	stwu	%r0,-4(%r1)
	mfctr   %r0
	stwu	%r0,-4(%r1)

	#Save the link register
	mflr	%r0
	stwu	%r0,-4(%r1)


	#push the rest of the GPR's
	stwu	%r31,-4(%r1)
	stwu	%r30,-4(%r1)

	stwu	%r29,-4(%r1)
	stwu	%r28,-4(%r1)
	stwu	%r27,-4(%r1)
	stwu	%r26,-4(%r1)
	stwu	%r25,-4(%r1)
	stwu	%r24,-4(%r1)
	stwu	%r23,-4(%r1)
	stwu	%r22,-4(%r1)
	stwu	%r21,-4(%r1)
	stwu	%r20,-4(%r1)

	stwu	%r19,-4(%r1)
	stwu	%r18,-4(%r1)
	stwu	%r17,-4(%r1)
	stwu	%r16,-4(%r1)
	stwu	%r15,-4(%r1)
	stwu	%r14,-4(%r1)
	stwu	%r13,-4(%r1)
	stwu	%r12,-4(%r1)
	stwu	%r11,-4(%r1)
	stwu	%r10,-4(%r1)

	stwu	%r9,-4(%r1)
	stwu	%r8,-4(%r1)
	stwu	%r7,-4(%r1)
	stwu	%r6,-4(%r1)
	stwu	%r5,-4(%r1)
	stwu	%r4,-4(%r1)
	stwu	%r3,-4(%r1)
	stwu	%r2,-4(%r1)
						#	stwu	%r1,-4(%r1)
						#	stwu	%r0,-4(%r1)
	#C function believe parts of the current stack is ok for local variables
	#back the pointer to allocate extra space
	addi	%r1,%r1,-40

	#R1 (i.e. sp) should now be on a good address
	#=====================
	bl	isr_external
	#===================== 

	# unwind isr stack frame
	addi	%r1,%r1,40

	addi	%r1,%r1,-4
						#	lwzu	%r0,4(%r1)
						#	lwzu	%r1,4(%r1)
	lwzu	%r2,4(%r1)
	lwzu	%r3,4(%r1)
	lwzu	%r4,4(%r1)
	lwzu	%r5,4(%r1)
	lwzu	%r6,4(%r1)
	lwzu	%r7,4(%r1)
	lwzu	%r8,4(%r1)
	lwzu	%r9,4(%r1)

	lwzu	%r10,4(%r1)
	lwzu	%r11,4(%r1)
	lwzu	%r12,4(%r1)
	lwzu	%r13,4(%r1)
	lwzu	%r14,4(%r1)
	lwzu	%r15,4(%r1)
	lwzu	%r16,4(%r1)
	lwzu	%r17,4(%r1)
	lwzu	%r18,4(%r1)
	lwzu	%r19,4(%r1)

	lwzu	%r20,4(%r1)
	lwzu	%r21,4(%r1)
	lwzu	%r22,4(%r1)
	lwzu	%r23,4(%r1)
	lwzu	%r24,4(%r1)
	lwzu	%r25,4(%r1)
	lwzu	%r26,4(%r1)
	lwzu	%r27,4(%r1)
	lwzu	%r28,4(%r1)
	lwzu	%r29,4(%r1)

	lwzu	%r30,4(%r1)
	lwzu	%r31,4(%r1)

	#Restore Link register
	lwzu	%r0,4(%r1)
	mtlr	%r0

	#Restore flags and counters
	lwzu	%r0,4(%r1)
	mtctr	%r0
	lwzu	%r0,4(%r1)
	mtxer	%r0
	lwzu	%r0,4(%r1)
	mtcr	%r0


	#Restore R0 and stack-pointer
	lwzu	%r0,4(%r1)
	lwz	%r1,4(%r1)

	rfi


		.end
