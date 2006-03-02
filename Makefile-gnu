KILEFILE:=PCM_ST10.Uv2

RLPTH:=../../

# Defines to build with
USR_DEFINES:=PCM,ST10
THIS_MODULE:=pcm_st
CPU_T:=ST10

# The final output name
OUTNAME:=pcm_st

ALL_C:= \
../../Common/COMMONID.C \
../../Common/COMTEXT.C \
../../Common/Down16b/Cannode/ST10F269/DOWN.C \
../../Common/Down16b/DOWN_TXT.C \
../../Common/EEPROM.C \
ADC.C \
ADCMNG.C \
ASC0.C \
CAN1.C \
CAN2.C \
DALLAS.C \
DEVMNG.C \
DISPLAY.C \
GPT2.C \
IO.C \
PCM.C \
PCM_ID.C \
PICMNG.C \
PUMPMOD.C \
PUMPREG.C \
SELFTEST.C \
SERVICE.C \
SPECTEXT.C \
SSC.C \

#Have no idea what this does.....:::---->>>>


ALL_ASM:= \
../../Common/Down16b/down_asm.a66 \
Start167.a66 \


DOX_PROJFILE         := pcm_as.dox
DOX_TAGFILE          := pcm_as.tag
ADDITIONAL_TEXTFILES := pcm_as.txt



include ../../gen_prereq.mk
include ../../gen_targets.mk

Common/Traps.obj: Common/Traps.d
	@echo "<<<<<<Special target>>>>>>!"
	$(CC) ..\Common\Traps.c 'LARGE OPTIMIZE(2,SPEED) ORDER INCDIR(..\..\common\;..\Common\;..\Common\firmfunc\;..\Common\down16b\;..\Common\down16b\cannode\) MOD167 MOD167 MOD167 DEFINE(PCM,CS) PRINT(.\Traps.lst) SRC(.\Traps.asm)'

#	$(ASM) Traps.SRC $(AFLAGS) OBJECT\(../Common/Traps.obj\) EP

#	$(CC) ../Common/Traps.c $(CFLAGS) $(CINCLUDE) SRC\(Traps.asm\)

#../COMMON/TRAPS.OBJ: ../COMMON/TRAPS.D
#	@echo "<<<<<<Special target>>>>>>!"
#	$(CC) ../COMMON/TRAPS.C $(CFLAGS) $(CINCLUDE) SRC\(TRAPS.asm\)
#	$(ASM) TRAPS.asm $(AFLAGS) OBJECT\(../COMMON/TRAPS.OBJ\) EP


include $(FOUND_CDEPS)
# DO NOT DELETE
