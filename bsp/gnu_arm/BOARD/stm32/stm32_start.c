//Can't have DEBUG defined due to maro fuck-ups
#undef DEBUG
#include "stm32f10x_type.h"
#include "stm32f10x_lib.h"
#include "stm32f10x_map.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "bits.h"

#define STACK_TOP 0x20000800
#define NVIC_CCR ((volatile unsigned long *)(0xE000ED14))
//Declarations

// VARIABLES

GPIO_InitTypeDef GPIO_InitStructure;

extern int main(int argc, char** argv);

/*Used as checking if static initialization works (will be removed)*/
volatile unsigned int apavariabel = 0x87654321;

/*************************************************************************
 * Function Name: _start
 * Parameters: none
 * Return: Int32U
 *
 * Description: The _start subroutine
 *
 *************************************************************************/
int board_main(void)
{
	int strt_nr = 0;
	char env[100];

	*NVIC_CCR = *NVIC_CCR | 0x200; /* Set STKALIGN in NVIC */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);


	// Configure PC.12 as output push-pull (LED)
	GPIO_WriteBit(GPIOC,GPIO_Pin_12,Bit_SET);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	while(1) {
		strt_nr++;

		GPIOC->BRR |= 0x00001000;
		main(0,(char**)env);
		strt_nr++;
		GPIOC->BSRR |= 0x00001000;
	}
}

void nmi_handler(void)
{
	return ;
}

void hardfault_handler(void)
{
	return ;
}

