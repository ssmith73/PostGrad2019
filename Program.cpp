#include <stm32l4xx.h>
//#include <stm32_hal_legacy.h>

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
	//HAL_Init();

	RCC->CR 		 	&= 	0xFFFFFF07;  //Clear ~MSIRANGE bits and MSIRGSEL bit
	RCC->CR 		 	|= 	0x00000088;  //Set MSI to 16MHz and MSIRGSEL bit
	RCC->AHB2ENR 	|= 	0x00000001;  //Enable MSI
	GPIOA->MODER 	&= 	0xFFFFF3FF;  //Clear GPIOA[5] MODER bits
	GPIOA->MODER 	|= 	0x00000400;  //Enable GPIOA[5] for output
	
	
//	__GPIOA_CLK_ENABLE();
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	GPIO_InitStructure.Pin = GPIO_PIN_5;
//
//	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
//	GPIO_InitStructure.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	for (;;)
	{
		GPIOA->BSRR |= 0x00000020;
		delayMs(DELAY);
		GPIOA->BSRR |= 0x00200000;
		delayMs(DELAY);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//		HAL_Delay(1000);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//		HAL_Delay(1000);
	}
}
// (Rough) delay in mS, off a 16MHz delay
void delayMs(int n) {
    uint16_t i;
    for(;n>0;n--)
        for(i=0;i<3195;i++);
}
