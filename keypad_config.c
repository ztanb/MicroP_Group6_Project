#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"

/**************  Keypad Configuration  *****************
		 __||||||||__
R0	|	#		0		*	|
		|						|
R1	|	9		8		7	|
		|						|
R2	|	6		5		4	|
		|						|
R3	|	3		2		1	|
		|____/-\____|
		  R4   R5	 R6

Pins from left to right: 1 = R4, 2 = R5, 3 = R6, 4 = R0, 5 = R1,
												 6 = R2, 7 = R3, 8 = power

1	&	4	=	#			2	&	4	= 0			3 & 4 = *			
1	&	5	=	9			2	&	5	=	8			3 & 5 = 7			
1	&	6	=	6			2	&	6	=	5			3 & 6 = 4			
1	&	7	=	3			2	&	7	=	2			3 & 7 = 1			

*/

GPIO_InitTypeDef GPIO_Init;

/*********  Variables for readability  ***********/
uint32_t pin_1 = GPIO_PIN_1;
uint32_t pin_2 = GPIO_PIN_2;
uint32_t pin_3 = GPIO_PIN_4;
uint32_t pin_4 = GPIO_PIN_5;
uint32_t pin_5 = GPIO_PIN_6;
uint32_t pin_6 = GPIO_PIN_8;
uint32_t pin_7 = GPIO_PIN_9;
//uint32_t pin_8 = GPIO_PIN_11;

// variables for the pin state
GPIO_PinState pin_value1;
GPIO_PinState pin_value2;
GPIO_PinState pin_value3;
GPIO_PinState pin_value4;
GPIO_PinState pin_value5;
GPIO_PinState pin_value6;
GPIO_PinState pin_value7;
//GPIO_PinState pin_value8;

int angle_mode_flag = 1;
int mode_flag = 0;

void Set_Number(int number);


void Configure_Key_GPIO()
	
{

	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	// GPIO pins chosen in the C range because they are all free pins. the Erange is for the LED
	
  GPIO_Init.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_9; //removed 11
	GPIO_Init.Mode = GPIO_MODE_INPUT; //GPIO_Init.Mode = GPIO_MODE_INPUT; is another option?
	GPIO_Init.Pull = GPIO_PULLDOWN;		//
	GPIO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(GPIOC, &GPIO_Init);
}

/*
Idea: the pins will be polled. When the 'begin' buttin is pressed, it enters a polling while loop.
If  the two pins corresponding to a buton are shorted to 5v, then the pins are read as high and the number
is set in an array.
*/
void read_keypad(void) //an array to store the #'s
{
	int key_flag = 0;
	
	//read the value of the pin. GPIO_PIN_SET = high, GPIO_PIN_RESET = low.
	pin_value1 =	HAL_GPIO_ReadPin(GPIOC, pin_1);		
	pin_value2 =	HAL_GPIO_ReadPin(GPIOC, pin_2);
	pin_value3 =	HAL_GPIO_ReadPin(GPIOC, pin_3);
	pin_value4 =	HAL_GPIO_ReadPin(GPIOC, pin_4);
	pin_value5 =	HAL_GPIO_ReadPin(GPIOC, pin_5);
	pin_value6 =	HAL_GPIO_ReadPin(GPIOC, pin_6);
	pin_value7 =	HAL_GPIO_ReadPin(GPIOC, pin_7);
	
	if ((pin_value3 == GPIO_PIN_SET)&&(pin_value4 == GPIO_PIN_SET)) // '*' is pressed			
			{
				// *  = temperature mode	
				mode_flag = 0;				
				
				//wait for the button to be released 
				while((pin_value3 == GPIO_PIN_SET)&&(pin_value4 == GPIO_PIN_SET)) 
				{
					pin_value3 =	HAL_GPIO_ReadPin(GPIOC, pin_3);
					pin_value4 =	HAL_GPIO_ReadPin(GPIOC, pin_4);
				}
			}

	else if((pin_value1 == GPIO_PIN_SET)&&(pin_value4 == GPIO_PIN_SET)) // '#' is pressed
	{
				//# = acccelerometer mode
				
		while(key_flag != 1) //enters loop to wait for the necessary mode selection
			{		
					pin_value1 =	HAL_GPIO_ReadPin(GPIOC, pin_1);		
					pin_value2 =	HAL_GPIO_ReadPin(GPIOC, pin_2);
					pin_value3 =	HAL_GPIO_ReadPin(GPIOC, pin_3);
					pin_value4 =	HAL_GPIO_ReadPin(GPIOC, pin_4);
					pin_value5 =	HAL_GPIO_ReadPin(GPIOC, pin_5);
					pin_value6 =	HAL_GPIO_ReadPin(GPIOC, pin_6);
					pin_value7 =	HAL_GPIO_ReadPin(GPIOC, pin_7);
					
					if((pin_value3 == GPIO_PIN_SET)&&(pin_value7 == GPIO_PIN_SET)) // '1' is selected
					{
						//1
						angle_mode_flag = 1; //this is pitch
	
						key_flag = 1;		//flag to exit the loop
						
						//wait for the button to be released
						while((pin_value3 == GPIO_PIN_SET)&&(pin_value7 == GPIO_PIN_SET)) 
						{
							pin_value3 =	HAL_GPIO_ReadPin(GPIOC, pin_3);
							pin_value7 =	HAL_GPIO_ReadPin(GPIOC, pin_7);
						}
					}
					else if((pin_value2 == GPIO_PIN_SET)&&(pin_value7 == GPIO_PIN_SET))
					{
							//2
							angle_mode_flag = 2; //this is roll
						
							key_flag = 1;

						//wait for the button to be released
						while((pin_value2 == GPIO_PIN_SET)&&(pin_value7 == GPIO_PIN_SET))
						{
							pin_value2 =	HAL_GPIO_ReadPin(GPIOC, pin_2);
							pin_value7 =	HAL_GPIO_ReadPin(GPIOC, pin_7);
						}
					}
				}	

		mode_flag = 1;		
	}
}

