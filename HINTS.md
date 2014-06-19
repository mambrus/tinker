make properhard configure && ./configure -C --host=arm-hixs-eabi --enable-verbose-config MCPU=cortex-m3 BOARD=stm32 CFLAGS="-mthumb -fno-common -O0 -g -mcpu=cortex-m3" && make instal
