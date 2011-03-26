// LPC2129 Vic Controller
// mtrojer@arrownordic.com
// $WCREV$
//
// Handles IRQ interrupts
// Make sure the instruction 'LDR PC, [PC, #-0x0FF0]' is at
// the IRQ execption.

#include "aeb_types.h"
#include "lpc21xx.h"
#include "lpc2129_vic.h"

static void get_addr_n_cntl(UNS_8 vecaddr, UNS_32 *addr, UNS_32 *cntl)
{
  if(vecaddr<16) {
    *addr = 0xFFFFF100+vecaddr*4;
    *cntl = 0xFFFFF200+vecaddr*4;
  }
  else
    addr=cntl=0;
}

void vic_install_isr(vic_control *vc)
{
  UNS_32 addr, cntl;
  
  get_addr_n_cntl(vc->vecaddr,&addr,&cntl);
  if (addr==0 || cntl==0)
    return;

  VICIntSelect &= ~(1 << vc->vecchannel);

  (*((volatile unsigned long *) addr)) = (unsigned long) vc->func;
  (*((volatile unsigned long *) cntl)) = vc->vecchannel | VIC_CNTL_EN;
}

void vic_enable_int(vic_control *vc)
{
  VICIntEnable |= (1 << vc->vecchannel);
}

void vic_disable_int(vic_control *vc)
{
  VICIntEnClr |= (1 << vc->vecchannel);
}

void vic_remove_isr(vic_control *vc)
{
  UNS_32 addr, cntl;
  
  get_addr_n_cntl(vc->vecaddr,&addr,&cntl);
  if (addr==0 || cntl==0)
    return;

  vic_disable_int(vc);
  (*((volatile unsigned long *) cntl)) &= ~VIC_CNTL_EN;
}

void vic_global_disable_int(void)
{
    asm volatile ("MSR CPSR_c, #0x10|0x80|0x40");
}

void vic_global_enable_int(void)
{
    asm volatile ("MSR CPSR_c, #0x10|0x40");
}
