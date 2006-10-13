

# Starupt Code for PowerPC must be linked first at Address at which it expects to run.

        .text

        .global _startup
        .func   _startup
_startup:


# Exception Vectors
#  Mapped to Address 0.
#  Absolute addressing mode must be used.
#  Dummy Handlers are implemented as infinite loops which can be modified.


		B main


        .end
