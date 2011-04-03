//Something is wrong with the macros under -DDEBUG
//This flag is default on building TinKer (FIXME)
//
//But meanwhile we turn this lag off (FIXME: the macros)

#undef DEBUG
#include "stm32f10x_lib.h"
#include "stm32f10x_map.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "bits.h"

/*
 * Set up PLL. No idea what this code does (yet)
 *
 */
void pll_setup (void)
{
	// 1. Cloking the controller from internal HSI RC (8 MHz)
	RCC_HSICmd(ENABLE);
	// wait until the HSI is ready
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	// 2. Enable ext. high frequency OSC
	RCC_HSEConfig(RCC_HSE_ON);
	// wait until the HSE is ready
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	// 3. Init PLL
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9); // 72MHz
	//  RCC_PLLConfig(RCC_PLLSource_HSE_Div2,RCC_PLLMul_9); // 72MHz
	RCC_PLLCmd(ENABLE);
	// wait until the PLL is ready
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	// 4. Set system clock divders
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	// Flash 1 wait state
	*(vu32 *)0x40022000 = 0x12;
	// 5. Clock system from PLL
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
}
