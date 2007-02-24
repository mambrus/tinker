/* bdi2000 supported application (run from RAM):
; SRAM		<0x00000000 - 0x083FFFFF> (4M)
; FLASH		<0x08000000 - 0x00FFFFFF> (16M)
; PRAM		<0x10000000 - 0xFF3FFFFF> (4M)
*/

/* Standalone application (run from flash, no debugger):
; FLASH		<0x00000000 - 0x00FFFFFF> (16M)
; SRAM		<0x08000000 - 0x083FFFFF> (4M)
; PRAM		<0x10000000 - 0xFF3FFFFF> (4M)
*/

/*NOTE: the above must also be reflected in the config file for bdi2000*/

/*
MEMORY
{
	EXCEPT (rx)	: ORIGIN = 0x00000000, LENGTH = 0x00010000
	SRAM_C (rx)	: ORIGIN = 0x00010000, LENGTH = 0x001F0000
	SRAM_D (rw)	: ORIGIN = 0x00200000, LENGTH = 0x00200000
	FLASH  (rx)	: ORIGIN = 0x08000000, LENGTH = 0x01000000
	PRAM   (rwx)	: ORIGIN = 0x10000000, LENGTH = 0x00400000
}
*/


MEMORY
{
	SRAM_C (rx)	: ORIGIN = 0x00000000, LENGTH = 0x00200000
	SRAM_D (rw)	: ORIGIN = 0x00200000, LENGTH = 0x00200000
	FLASH  (rx)	: ORIGIN = 0x08000000, LENGTH = 0x01000000
	PRAM   (rwx)	: ORIGIN = 0x10000000, LENGTH = 0x00400000
}


ENTRY(_System_reset_interrupt_exception)
FORCE_COMMON_ALLOCATION

/*
INHIBIT_COMMON_ALLOCATION
*/



SECTIONS
{
	.text :
	{
		_code_start = .;
		_btext = .;
	/* __EXCEPT_START__ = .; */
		*(.exception);
	/* __EXCEPT_END__ = .; */
		startup_gnu.AO(.text) *(.text);
		*(.rodata)		/* Constant strings and static const */
		*(.rodata.str1.4)
		_etext = .;
		FILL(0x74657874);
		_xtext = .;
	} >SRAM_C

	.eh_frame :
	{
		/*__EXCEPT_START__ = .;*/
		*(.eh_frame)
		/*__EXCEPT_END__ = .;*/
	} >SRAM_C

	.got :
	{
		/*__GOT_START__ = .;*/
		*(.got);
		/*__GOT_END__ = .;*/
	} >SRAM_C


	.sdata :
	{
		_bsdata = .;
		/*__SDATA_START__ = .;*/
		*(.sdata)		/* impure_ptr (struct _reent), function pointers and stuff*/
		/*__SDATA_END__ = .;*/
		_esdata = .;
		FILL(0x73646174);
		_xsdata = .;
	} >SRAM_D

	.got2 :
	{
		__GOT2_START__ = .;
		*(.got2)
		__GOT2_END__   = .;
	} >SRAM_D


	.data :
	{
		_bdata = .;
		*(.data) *(COMMON);
		/*end = .;*/
		_edata = .;
		FILL(0x64617461);
		_xdata = .;
	} >SRAM_D

	/*NOTE Important!*/
	/*Make sure "end" synpol comes last, even after other non mentioned sections */
	/*has been placed according to their default linking rules*/
	.heap :
	{
		end = .;
	} >SRAM_D

	.sbss :
	{
		__SBSS_START__	= .;
		*(.sbss)		/* static variables */
		__SBSS_END__  = .;
	} >PRAM

	.bss :
	{
		_bstart = .;
		*(.bss)
		_bend = .;		
		FILL(0x2E627373);
		_xbss = .;
		/*__CTOR_LIST__	= .;*/
		/*__CTOR_END__	= .;*/
		/*__DTOR_LIST__	= .;*/
		/*__DTOR_END__	= .;*/
		/*__FIXUP_START__	= .;*/
		/*__FIXUP_END__	= .;*/
		/*__SDATA2_START__	= .;*/
		/*__SBSS2_END__	= .;*/
		
	} >PRAM

	_sizeof_t = 0x00200000;
	_sizeof_d = 0x00200000 - SIZEOF ( .sdata );
	_sizeof_b = 0x00400000;
}

/*
  int __SDATA_START__;  int __SDATA2_START__;	OK / eh?
  int __SBSS_END__;     int __SBSS2_END__;	SBSS2 eh?  

  int __GOT_START__;    int __GOT_END__;	OK
  int __GOT2_START__;   int __GOT2_END__;	OK

  int __FIXUP_START__;  int __FIXUP_END__;	eh ?
  int __EXCEPT_START__; int __EXCEPT_END__;	think so yes
  int __init;           int __fini;		OK
  int __CTOR_LIST__;    int __CTOR_END__;	TBD
  int __DTOR_LIST__;    int __DTOR_END__;	TBD
*/

/*	
	.comment : {		*(.comment) 		} >PRAM
	.data : {		*(.data)		} >PRAM
	.debug_abbrev : {	*(.debug_abbrev)	} >PRAM
	.debug_aranges : {	*(.debug_aranges)	} >PRAM
	.debug_frame : {	*(.debug_frame)		} >PRAM
	.debug_info : {		*(.debug_info)		} >PRAM
	.debug_line : {		*(.debug_line)		} >PRAM
	.debug_loc : {		*(.debug_loc)		} >PRAM 
	.debug_pubnames : {	*(.debug_pubnames)	} >PRAM 
	.debug_ranges : {	*(.debug_ranges)	} >PRAM 
	.debug_str : {		*(.debug_str)		} >PRAM 
	.eh_frame : {		*(.eh_frame)		} >PRAM 
	.got : {		*(.got)			} >PRAM 
	.got2 : {		*(.got2)		} >PRAM 
	.rela.dyn : {		*(.rela.dyn)		} >PRAM 
	.rodata : {		*(.rodata)		} >PRAM 
	.rodata.str1.4 : {	*(.rodata.str1.4)	} >PRAM 
	.sbss : {		*(.sbss)		} >PRAM 
	.sbss.1 : {		*(.sbss.1)		} >PRAM 
	.sbss.2 : {		*(.sbss.2)		} >PRAM 
	.sbss.3 : {		*(.sbss.3)		} >PRAM 
	.sbss.4 : {		*(.sbss.4)		} >PRAM 
	.sdata : {		*(.sdata)		} >PRAM 
	.stab : {		*(.stab)		} >PRAM 
	.stabstr : {		*(.stabstr)		} >PRAM 
	
*/
