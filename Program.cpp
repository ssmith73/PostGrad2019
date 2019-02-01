#include <stm32l4xx.h>

#ifdef __cplusplus
extern "C"
#endif
//void SysTick_Handler(void)
//{
//	HAL_IncTick();
//	HAL_SYSTICK_IRQHandler();
//}

#define DELAY 100

void delayMs(int);
int main(void)
{
	
	RCC->CR 		 	&= 	0xFFFFFF07;  //Clear ~MSIRANGE bits and MSIRGSEL bit
	RCC->CR 		 	|= 	0x00000088;  //Set MSI to 16MHz and MSIRGSEL bit
	RCC->AHB2ENR 	|= 	0x00000001;  //Enable MSI
	GPIOA->MODER 	&= 	0xFFFFF3FF;  //Clear GPIOA[5] MODER bits
	GPIOA->MODER 	|= 	0x00000400;  //Enable GPIOA[5] for output
	

	for (;;)
	{
		GPIOA->BSRR |= 0x00000020;
		delayMs(DELAY);
		GPIOA->BSRR |= 0x00200000;
		delayMs(DELAY);
	}
}

// (Rough) delay in mS, off a 16MHz delay
void delayMs(int n) {
    uint16_t i;
    for(;n>0;n--)
        for(i=0;i<3195;i++);
}
