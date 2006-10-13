# Startup code for Blackfin

# Starupt Code must be linked first at Address at which it expects to run.

		.bss
		.global _startup
		.align 4;
		.func   _startup
_startup:


# Exception Vectors
#  Mapped to Address 0.
#  Absolute addressing mode must be used.
#  Dummy Handlers are implemented as infinite loops which can be modified.


		JUMP _main


        .end

