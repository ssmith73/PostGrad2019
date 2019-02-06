#include <stm32l4xx.h>

#ifdef __cplusplus
extern "C"
#endif
//void SysTick_Handler(void)
//{
//	HAL_IncTick();
//	HAL_SYSTICK_IRQHandler();
//}

#define DELAY 500
void EXTI15_10_IRQHandler(void);

volatile bool buttonInterrupt = false;

/* Handle PC13 interrupt */
void EXTI15_10_IRQHandler(void) {
	buttonInterrupt = true;
	EXTI->PR1 = 0x2000;
}

void delayMs(int);
void delay1Hz(void);
void initTim2(void);
void initUserSw(void);
void initPbInterrupt(void);


int main(void)
{
	__disable_irq();
	/* Configure the clocks */
	RCC->CR 		 	&= 	0xFFFFFF07;  //Clear ~MSIRANGE bits and MSIRGSEL bit
	RCC->CR 		 	|= 	0x00000088;  //Set MSI to 16MHz and MSIRGSEL bit
	RCC->AHB2ENR 	|= 	0x00000001;  //Enable PA5 clocks
	
	/* Enable PA0 for output */
	GPIOA->MODER 	&= 	0xFFFFF3FF;  //Clear GPIOA[5] MODER bits
	GPIOA->MODER 	|= 	0x00000400;  //Enable GPIOA[5] for output
	
	/* Configure Timer 2
	Assumes 16MHz system clock, rolls over
	at 1Hz intervals */
	//initTim2();
	initUserSw();
	//initPbInterrupt();
	
	RCC->APB2ENR        |= 1; 			//Enable SYSCFG clk (for GPIO interrupt enables)
	SYSCFG->EXTICR[3] 	&= ~0x00F0; 	//CLEAR_BIT the EXTI[13] bits
    SYSCFG->EXTICR[3]   |= 0x20; 		//Enable GPIOC for EXTI[13]
	EXTI->IMR1 					|= 0x2000; 	//Unmask EXTI13
	EXTI->FTSR1 				|= 0x2000;  //Enable falling edge triggered interrupts (pushbutton high to low on push)
	NVIC_EnableIRQ(EXTI15_10_IRQn); //Enable EXTI15-to-10 interrupts
	
  __enable_irq();
	
	for (;;)
	{
		if (buttonInterrupt == true) {
			
			GPIOA->BSRR |= 0x00000020;  	//Set the GPIO
			delayMs(DELAY);
			//for(uint16_t i = 0 ; i < DELAY ; i++) 
				//delay1Hz();
			GPIOA->BSRR |= 0x00200000;    //Clear the GPIO
			delayMs(DELAY);
			
			//for(uint16_t i = 0 ; i < DELAY ; i++)// delay1Hz();
			
			buttonInterrupt = false;
		}
	}
	
}

// delay in mS, off a sysTick - assumes HSI @ 16MHz

void delayMs(int n) {
	uint16_t i;
	/* Configure SysTick 
	Let sysclk (MSI) = 16MHz, 1/sysclk = 62.5nS  - for 1mS delay
	need X * 62.5nS = 0.001, so X= 16000 needs to be the reload value */
	
	SysTick->LOAD = 0x3E80 - 1; /*16000 -1 */
	SysTick->VAL = 0x00000000;  /*Clear the current value register */
	SysTick->CTRL = 0x5;        /*Enable internal clocks source and enable systick */
	for (i = 0;i < n;i++) {
		while ((SysTick->CTRL & 0x10000) == 0) /* wait for reload */
		{}
	}
	SysTick->CTRL = 0; 					//Stop the timer
}

void initTim2() {
	RCC->APB1ENR1 |= 0x1;   		//Enable timer 2
	TIM2->PSC 		= 0x640 - 1; //Prescalar value - divide 16MHz by 1600
	TIM2->ARR 		= 0x2710 - 1; //Reload value
}


void 	delay1Hz() {
	/* Use TIM2 to generate a 1Hz delay
	   Assumes 16MHz sysclk, divide this down by 1600
	   in the prescalar, and then set the reload value for
	   10000 to give the 1Hz - 1/16MHz = 62.5ns
	1600 * 62.5nS = 0.0001S = 100uS, *10000 = 1S
	*/
	TIM2->CNT 		= 0; 							//Clear the count register
	TIM2->CR1 		= 1;  						//Start the counter
	while(!(TIM2->SR & 1)) {}    		//wait for rollover
	TIM2->SR &= ~1; 					  		//Clear UIF bit
	TIM2->CR1 		= 0;  						//Stop the counter
}



void initUserSw() {
	RCC->AHB2ENR 	|= 0x4; 			 		//ENABLE GPIOC port clk
	GPIOC->MODER 	&= 0xF3FFFFFF; 		//Clear GPIOC[13] for input mode
	
}

void initPbInterrupt() {
	
	RCC->APB2ENR        |= 1; 			//Enable SYSCFG clk (for GPIO interrupt enables)
	SYSCFG->EXTICR[3] 	&= ~0x00F0; 	//CLEAR_BIT the EXTI[13] bits
    SYSCFG->EXTICR[3]   |= 0x20; 		//Enable GPIOC for EXTI[13]
	EXTI->IMR1 					|= 0x2000; 	//Unmask EXTI13
	EXTI->FTSR1 				|= 0x2000;  //Enable falling edge triggered interrupts (pushbutton high to low on push)
	NVIC_EnableIRQ(EXTI15_10_IRQn); //Enable EXTI15-to-10 interrupts

}
	 


