

# Startup module i386, board=localhost


		.section .exception
		.equ _TOS, 0x10400000		#Temporary top of stack (FIXME)	

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
			mov %ax,0xF0
			jmp 	exception_invalid
.fill 0x100 - (. - _Reserved_0_exception) , 1, 0x0
.endfunc

.func _System_reset_interrupt_exception
_System_reset_interrupt_exception:
			mov	%eax,_TOS	
#			mov     %eax,0x100000
			sub	%eax,4
			mov	%esp,%eax
			xor	%eax,%eax
			xor	%ebx,%ebx
			xor	%ecx,%ecx
			xor	%edx,%edx

	#ifndef __rtems__
			call 	__init
	#endif
			jmp 	main
.fill 0x100 - (. - _System_reset_interrupt_exception) , 1, 0x1
.endfunc

.func _Machine_check_interrupt_exception
_Machine_check_interrupt_exception:
			mov 	%ax,0xF2
			jmp 	exception_invalid
.fill 0x100 - (. - _Machine_check_interrupt_exception) , 1, 0x2
.endfunc

.func _DSI_exception
_DSI_exception:
			mov 	%ax,0xF3
			jmp 	exception_invalid
.fill 0x100 - (. - _DSI_exception) , 1, 0x3
.endfunc

.func _ISI_exception
_ISI_exception:
			mov 	%ax,0xF4
			jmp 	exception_invalid
.fill 0x100 - (. - _ISI_exception) , 1, 0x4
.endfunc

.func _External_Interrupt_exception
_External_Interrupt_exception:
	# Make a jump first because code for storing/restoring context
	# will not fit with in 0x0100 region (would force next entry to break
	# absolute position). 

			jmp 	_External_Interrupt_exception_code
.fill 0x100 - (. - _External_Interrupt_exception) , 1, 0x5
.endfunc

.func _Alignment_exception
_Alignment_exception:
			mov 	%ax,0xF6
			jmp 	exception_invalid
.fill 0x100 - (. - _Alignment_exception) , 1, 0x6
.endfunc

.func _Program_exception
_Program_exception:
			mov 	%ax,0xF7
			jmp 	exception_invalid
.fill 0x100 - (. - _Program_exception) , 1, 0x7
.endfunc

.func _Floating_point_unavailable_exception
_Floating_point_unavailable_exception:
			mov 	%ax,0xF8
			jmp 	exception_invalid
.fill 0x100 - (. - _Floating_point_unavailable_exception) , 1, 0x8
.endfunc

.func _Decrementer_exception
_Decrementer_exception:
			mov 	%ax,0xF9
			jmp 	exception_invalid
.fill 0x100 - (. - _Decrementer_exception) , 1, 0x9
.endfunc

.func __Reserved_1_exception
__Reserved_1_exception:
			mov 	%ax,0xFA
			jmp 	exception_invalid
.fill 0x200 - (. - __Reserved_1_exception) , 1, 0xA
.endfunc

.func _System_call_exception
_System_call_exception:
			mov 	%ax,0xFC
			jmp 	exception_invalid
.fill 0x100 - (. - _System_call_exception) , 1, 0xC
.endfunc

.func _Trace_exception
_Trace_exception:
			mov 	%ax,0xFD
			jmp 	exception_invalid
.fill 0x100 - (. - _Trace_exception) , 1, 0xD
.endfunc

.func _Floating_point_assist_exception
_Floating_point_assist_exception:
			mov 	%ax,0xFE
			jmp 	exception_invalid
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



_External_Interrupt_exception_code:
	#pushall here
	#=====================
	#call		isr_external
	#===================== 
	#popall here

	iret

exception_invalid:
			jmp	exception_invalid
		.end
