#include <stdint.h>
#include <stdio.h>
#include "bits.h"

#define SIM_5     (*((volatile uint32_t *) 0x40048038))
#define PCR_PTA4  (*((volatile uint32_t *) 0X40049010))
#define PCR_PTB21 (*((volatile uint32_t *) 0x4004A054))
#define PCR_PTB22 (*((volatile uint32_t *) 0x4004A058))
#define PCR_PTC6  (*((volatile uint32_t *) 0X4004B018))
#define PCR_PTE26 (*((volatile uint32_t *) 0x4004D068))//Pin control registers

#define GPIO_A_BASE_ADDR (((volatile uint32_t *) 0x400FF000))
#define GPIO_B_BASE_ADDR (((volatile uint32_t *) 0x400FF040))
#define GPIO_C_BASE_ADDR (((volatile uint32_t *) 0x400FF080))
#define GPIO_E_BASE_ADDR (((volatile uint32_t *) 0x400FF100))//Pointers to GPIO_B & GPIO_E

typedef struct
{
	uint32_t PDOR;
	uint32_t PSOR;
	uint32_t PCOR;
	uint32_t PTOR;
	uint32_t PDIR;
	uint32_t PDDR;
}GPIO_t;//Base structure for GPIO

#define GPIO_A ((GPIO_t*) GPIO_A_BASE_ADDR)
#define GPIO_B ((GPIO_t*) GPIO_B_BASE_ADDR)
#define GPIO_C ((GPIO_t*) GPIO_C_BASE_ADDR)
#define GPIO_E ((GPIO_t*) GPIO_E_BASE_ADDR)

#define RED_LED_ON      0xFFBFFFFF
#define BLUE_LED_ON     0xFFDFFFFF
#define MAGENTA_LED_ON  0xFF9FFFFF
#define GREEN_LED_ON    0xFBFFFFFF
#define CLEAR_PORT      0xFFFFFFFF
#define RED_PIN     	0x400000
#define BLUE_PIN     	0x200000
#define GREEN_PIN     	0x4000000

#define RED_LED_OFF     RED_PIN
#define BLUE_LED_OFF    BLUE_PIN
#define GREEN_LED_OFF   GREEN_PIN

#define GPIOC_MASK_SW2 0x40;
#define GPIOC_MASK_SW3 0x10;

#define MUX_MODE_1      0x100

#define SYS_CLK_GPIO_A  0x200
#define SYS_CLK_GPIO_B  0x400
#define SYS_CLK_GPIO_C  0x800
#define SYS_CLK_GPIO_E  0x2000

#define MCU_APROX_FRECUENCY 21000000

int main(void)
{
	SIM_5 |= SYS_CLK_GPIO_A;
	SIM_5 |= SYS_CLK_GPIO_B;
	SIM_5 |= SYS_CLK_GPIO_C;
	SIM_5 |= SYS_CLK_GPIO_E;//Bits 10 & 13 of System Clock gating control register on for ports A, B, C & E

	PCR_PTA4  = MUX_MODE_1;
	PCR_PTB21 = MUX_MODE_1;
	PCR_PTB22 = MUX_MODE_1;
	PCR_PTC6  = MUX_MODE_1;
	PCR_PTE26 = MUX_MODE_1;//Bit 8 on for MUX GPIO behavior on PORTS

	GPIO_B -> PDOR = CLEAR_PORT;
	GPIO_B -> PDDR |= RED_PIN;//Bits 21 & 22 of GPIO B on for colors red and blue on LEDRGB
	GPIO_B -> PDDR |= BLUE_PIN;
	GPIO_E -> PDOR = CLEAR_PORT;
	GPIO_E -> PDDR = GREEN_PIN;//Bit 26 of GPIO E on for Green on LEDRGB



    while(1)
    {

    	uint32_t input_value_SW2 = 0;
    	uint32_t input_value_SW3 = 0;
    	uint32_t time_delay = 65000;
    	uint32_t time_delay_third = 3u;


    	input_value_SW2 = GPIO_C->PDIR;
    	/**Masks the GPIOC in the bit of interest*/
    	input_value_SW2 = input_value_SW2 & GPIOC_MASK_SW2;

    	input_value_SW3 = GPIO_A->PDIR;
    	/**Masks the GPIOC in the bit of interest*/
    	input_value_SW3 = input_value_SW3 & GPIOC_MASK_SW3;

    	/**Note that the comparison is not inputValur == False, because it is
    	safer if we switch the arguments*/
    	if(FALSE == input_value_SW2)
    	{
    		time_delay = time_delay/time_delay_third;
    		GPIO_B -> PDOR = RED_LED_ON;
    		GPIO_E -> PDOR = GREEN_LED_ON;
    		GPIO_B -> PDOR = BLUE_LED_ON;
    		SDK_DelayAtLeastUs(time_delay, MCU_APROX_FRECUENCY);
    		GPIO_B -> PDOR |= RED_LED_OFF;
    		GPIO_E -> PDOR |= GREEN_LED_OFF;
    		GPIO_B -> PDOR |= BLUE_LED_OFF;
    	}
    	else if(FALSE == input_value_SW3)
    	{
    		GPIO_B -> PDOR = RED_LED_ON;
    		GPIO_E -> PDOR = GREEN_LED_ON;
    		GPIO_B -> PDOR = BLUE_LED_ON;
    		SDK_DelayAtLeastUs(time_delay, MCU_APROX_FRECUENCY);
    		GPIO_B -> PDOR |= RED_LED_OFF;
    		GPIO_E -> PDOR |= GREEN_LED_OFF;
    		GPIO_B -> PDOR |= BLUE_LED_OFF;
    	}
    	else
    	{
			GPIO_B -> PDOR = RED_LED_ON;
			GPIO_B -> PDOR |= RED_LED_OFF; //turn off red
			SDK_DelayAtLeastUs(time_delay, MCU_APROX_FRECUENCY);
			GPIO_E -> PDOR = GREEN_LED_ON;
			GPIO_E -> PDOR |= GREEN_LED_OFF; //turn off green
			SDK_DelayAtLeastUs(time_delay, MCU_APROX_FRECUENCY);
			GPIO_B -> PDOR = BLUE_LED_ON;
			GPIO_B -> PDOR |= BLUE_LED_OFF; //turn off blue
			SDK_DelayAtLeastUs(time_delay, MCU_APROX_FRECUENCY);
			}
   }
    return 0 ;
}




