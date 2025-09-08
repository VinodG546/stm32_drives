
//LED -> PA5
#define     PERIPH_BASEADDR               0x40000000UL
#define 	AHB2_BASEADDR             	  0x48000000UL

#define     GPIOA_BASEADDR				  0x48000000U


#define     RCC_BASEADDR                  0x40021000U
#define 	RCC_AHB2ENR					  (*(volatile unsigned int*)(RCC_BASEADDR + 0x4C))


#define 	MODE_R_OFFSET					0x00UL
#define     GPIOA_MODE_R					(*(volatile unsigned int*)(GPIOA_BASEADDR + MODE_R_OFFSET))

#define     OD_R_OFFSET						0x14
#define     GPIOA_OD_R						(*(volatile unsigned int*)(GPIOA_BASEADDR + OD_R_OFFSET))

#define   	GPIOAEN      					(1U<<0)

#define     LED_PIN                          (1U<<5)

//making moder register 11,10 bit -> 0,1 ->make it as GPIO output mode

int main()
{
	//enable the GPIOA rcc clock
	RCC_AHB2ENR |= GPIOAEN ;

	//set PA5 as output pin
	GPIOA_MODE_R |= (1U<<10);
	GPIOA_MODE_R &= ~(1U<<11);

	while(1)
	{
		//toggle the 5th bit in ODR reg
		GPIOA_OD_R ^= LED_PIN;
		for(int i=0;i<1000000;i++);

	}
}
