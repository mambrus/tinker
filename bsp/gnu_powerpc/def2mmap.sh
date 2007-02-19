#!/bin/bash

TEMPFILE=/tmp/.$USER.$(echo $0 | sed -e 's/.*\///').$$

echo "// Generated from the file [$1] at [$(date)]"
echo -n "// Script $(echo $0 | sed -e 's/.*\///')"
echo " by Michael Ambrus, <michael.ambrus@gmail.com> (c) 2007"
echo
echo "#ifndef MMAP_REGS_H"
echo "#define MMAP_REGS_H"
echo "#include <sys/types.h>"
echo
echo "#define DMM1    0xFA200000"
echo

convert2mmap_awk='{
	if ($4 == 8)
		print "#define " toupper($1) " (*(volatile __uint8_t *)(" $2"+"$3"))"
	if ($4 == 16)
		print "#define " toupper($1) " (*(volatile __uint16_t *)(" $2"+"$3"))"
	if ($4 == 32)
		print "#define " toupper($1) " (*(volatile __uint32_t *)(" $2"+"$3"))" 
}'

convert2spr_awk='
	!/^;/ { print "#define _" toupper($1) " " $3
	
}'

echo
echo "// Memory mapped registers follows..."
echo

grep DMM1 $1 | awk "$convert2mmap_awk"


echo
echo "// Special Purpose Register ID's (SPR) follows..."
echo "// (to be used with SET_SPR and GET_SPR macros)"
echo
grep SPR $1 | awk "$convert2spr_awk"


echo
echo "#endif //MMAP_REGS_H"


rm -f TEMPFILE