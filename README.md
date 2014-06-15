                                                   Michael Ambrus
                                                   Stockholm 2006-10-06
                                                   Helsingborg 2014-07-15
                           TINKER
                           ======

TinKer is a real-time kernel (RTK) for embedded systems.

RTK'S are commonly referred to as a "RTOS", which is strictly speaking
incorrect for most embedded "RTOS" out there ;) )

This package includes kernels sources.

You normally build the project locally (don't install). For GNU based
build-/tool- chains do as follows:

make configure
./configure [<options>]
make

## Configuration & compilation
TinKer has a very flexible configure and build-system due to it's ability to
cope and integrate with may different standard libraries, build tool-chains,
systems and architectures. Configuring the kernel is part of porting and
it's an art in itself. Therefore when configuring like the above, the
build-system will try to determine what it can make use of. Note: This may
vary from system to system depending on how far porting has come for that
particular target. Therefore it's good advice to pass along the following
flag to ./configure: --enable-verbose-config and pay attention to which
packages you get. Worst case your target is completely unsupported, but it's
not hard to get rudimentary scheduling working and in that case, the worst
you'll get is the following:

AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
                   COMPONENTS
                   ==========
TK_COMP_ITC:                             __tk_yes
TK_COMP_PTIMER:                          __tk_no
TK_COMP_KMEM:                            __tk_no
TK_COMP_PTHREAD:                         __tk_no
TK_COMP_POSIX_RT:                        __tk_no
TK_COMP_FILESYS:                         __tk_no

                 PACKAGES & misc
                 ===============
TK_USE_BUILTIN_SORT:                     __tk_yes
VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV

In which case you actually have a complete inner kernel, but the none of the
POSIX interfaces. If you're building for hosted mode, you can easily expand
TinKer:s abilities by letting the build/configure system know a little bit
more about what resources you'd be willing to sacrifice.

Run:

    ./configure --help

...and look especially for the --enable-xxx options. These are both
component enablers/disablers and options to them. Components are named the
same as in the list above and lack an associated value. Try enable one like
for example:

   ./configure: --enable-verbose-config --enable-posix_rt

...for POSIX 1003.1b queues. Look at the configure output. If the component
seems enabled, try compiling:

    make

Each component will keep track of it's dependencies and additional
requirements let you know what other component it depends on depend on
another or if additional ./configure are needed and which one.

## Deliverable - the library

Whence configured, the build system  will build the kernel as a library that
you can link your application against. Without <options> you will build a
native library that will execute the kernel in a process on you host machine
(i.e. a hosted kernel). You can do this if you want to try out TinKer
without actually run it on a cross target.

To build for a certain host system, set the <options> for configure for
example as:
 --host=arm-hixs-elf BOARD=lpx21xxx

This will build several libraries in the directory ./lib/ . However,
only the one named ./lib/tinker.a is the one you'll link against (the
others are intermediate libraries).

Note: that the vendor part is important if you intend to use HIXS. HIXS
is TinKer main system call interface implementation for Newlib.

Just link your application against the library like you usually would. The
principle is the same from bare-metal targets to as green-threads in hosted
mode (i.e. as guest OS as it's now become named) under a host-OS. Hosted
mode can in tern be as a user-space kernel or as a virtual machine with
various levels of efforts getting there off course.

There's one final caution. The order of linking matters, and so does the
order of your header search paths. The latter is a unique feature and is
what makes TinKer able to coexist with other implementations, even though
it's the authors opinion that kernel API:s should never had gotten into
standard libraries in the first place. It's however a common and very nasty
source of errors as header errors will not manifest in a easy to understand
way if there's a conflict.

### Some philosophical rambling
A kernel is a beautiful thing... It's not a data transformer in the usual
sense.  But something that handles control and flow. What matters is
execution behaviour, abilities and performance -and kernels differs **a
lot**. There's no rocket science to it. It's just a matter of how resources
are deployed, utilized and distributed.  You give a little in one end, you
sacrifice something else in the other. But instead of choosing with a
sledgehammer, changing complete systems, to achieve certain run-time
abilities, with tinker one simply rebuilds the library.

Therefore do I ask:

- *Why, oh why, did the standard comity incorperate kernels in standard
  libraries. API:s should had been more than enough.*/


## Wrap-up
Tinker will run on targets based on the normal GNU canonical name
convention. This would imply a 2 dimensional matrix of targets. TinKer
however uses the  vendor part to point out the system (which is normally
pointed out by the last  part of the name). This is because the only
"system" we care about is weather to use **HIXS** or not, and by using the
vendor part we don't have to patch *binutils*, *gcc* nor *gdb*, and focus
only on newlib (Newlib is BTW the library in question when using GNU
toolchain, and *not* glibc).

When you build TinKer you'll also have to consider for which particular
hardware system to run it on (i.e. which board). TinKer really doesn't
care (or know), but you'll have to name the BOARD option when configuring a
build for a cross target. The BOARD option cooresponds to a subdirectory in
each main target directory. You're encouraged to add bourds of you own, this
will not affect neither TinKer's structure or build system in any way. Just
add a new directory, and point it out using the BOARD option.

A GNU target's structure in the file system follows the convention:
./bsp/gnu_<archetecture>

(The architecture implies the main architecture, i.e. variants of
processors following the same architecture all belong to this
directory.)

And the implementation for a specific board would then subsequently be:
./bsp/gnu_<arch>/<BOARD>

Note that normally only one ABI per main architecture is supported (elf
preferably).

Any system call variants are considered to be part of each main
architecture and is not named specifically in the structure (which one
to use is instead handled by inspecting the tool-chain's canonical name
vendor part). Most targets will only have one system implementation
(HIXS) anyway, but ARM TDMI for example has two. (As an embedded designer
you're free to add others for you're own design if you'd like). Just a word
of advise: There are plenty of much more competent kernels out there in
matter of "security", "network stacks" e.t.a. where you may want a clear
separation between kernel and application, and in some cases need to have.
For embedded systems, for example process or memory separation is often
irelevant as the system has one or a very few rigid functions to fulfill and
a crash in one process is almost always as catastrophic as a crash for the
whole system.

### More philosophy
Therefore different aspects are more important than data-separation, which
frankly speaking is just a fall-back for programs that are not deloused
properly (And yes, there are such things as 100% correct programs. They are
just a few magnitudes harder to write and verify and the effort-time scales
badly with complexity, but boy can they execute!).

## Some words about portability
The project limit it's ambition to provide *one* board for each main
target. This is a kernel for embedded systems, and the board adaption is
part of each specific solution anyway. Hence, the board implementation
for each target is very basic and not at all complete (that's your job
;) )

Added to this (now) 4 dimensional matrix, is yet a fifth dimension
naming the tool-chain - and implicitly also another standard C
library. In Tinker's context, a standard C library is part of a
tool-chain. The tool-chain will prefix the architecture in the directory
structure mentioned above. (I.e. all GNU based targets are found in
directories named ./bsp/gnu_<arch>/<BOARD>, where 'gnu' is the
tool-chain prefix).

## Historical
TinKer started it's days further back than it's revision history lets you
know. It has lived in at least 2 incarnations before it's current form. It
started out as a hobby-project for 8-bit CPU:s in the late 80'th
(micro-controllers and CPU's where quite alike back then) and has followed
me though my own evolution through 16, 32 and now 64 bit. This  somewhat
shows in the code. Some design decisions have roots from that time and some
code is messier than usual to be able to cope with the pleura of
combinations and variations possible.

During the good old days, kernels for deeply embedded systems were uncommon.
Until 2004 60% of all embedded kernels were still home-brewed, with horrible
effects as results. There were no standard (and if there were, no-one gave a
rats behind for it, or claimed to follow "another", military classified
(secret?) "one". You basically never knew what you got and I got tired of
the crap. TinKer were in it's second reincarnation (until 2003) inspired by
pSos and had a compatible interface. The newly released POSIX 1003.1c had a
very similar API, and off course we like standards. So from that date and
onward, the primary API is POSIX (subset there-of) even though the old pSos
primitives are still lurking around. The general idea is: You should always
be able to outgrow a TinKer based system and change to something more
powerful without reworking your SW.

All of this somewhat shows in the code. Some design decisions have roots
from that time and some code is messier than usual to be able to cope with
the pleura of combinations and variations possible. At some point one would
probably rewrite and harshly clean-up the code. I suspect no one in their
right minds are using anything else but a GNU tool-chain any-more (whit the
exception for the occasional LLVM-daredevil). But on the good-side, having
to cope with almost anything makes the code quite well tested for compiler
variances and what-nots.

## A short target summary

_As of writing the following targets are supported:_
GNU tool-chain based:
   ARM TDMI          (HIXS based)
   ARM TDMI          (Angel SW based)
   ARM Cortex-m3     (HIXS based)
   ARM Cortex.r4f    (HIXS based)
   ARM Cortex-A8     (HIXS based)
   iX86              (Cygwin, Linux hosted)
   86_64             (Linux hosted)
   PowerPC 840       (HIXS based)
   Blackfin          (Unfinished. Basic dispatching)

Keil tool-chain based
   XC166             (C166 family)

Microsoft Visual studio based
   iX86

Borland              (V5) based
   iX86

_Obsolete targets (not supported anymore):_
IAR
   8051
   Z80

Quite a few more tool-chain/target combos that I don't even recall any
longer are not mentioned.

Should interest occur for the obsolete ports, I'll be happy to revive
them if someone would donate the tools.

For the GNU based targets, you should be able to build TinKer using a any
UNIX like build host like Linux, Cygwin (i.e. Windows) e.t.a. Note however
that you must have GNU based cross tool-chains installed first though.

Enjoy!
