/*
The functions for setting up and interfacing with the 7 segment display
All functions here are prototyped in the supporting_functions.h file
*/

#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
/*
	
	PIN MAPPING FOR 7-SEGMENT
	
	PE4 => pin 1 / Digit1
	PE5 => pin 3 / section D
	PE6 => pin 5 / section E
	PE7 => pin 7 / decimal
	PE8 => pin 11 / section F
	PE9 => pin 13 / section C
	PE10 => pin 14 / section A
	PE11 => pin 15 / section G
	PE12 => pin 16 / section B
	PE13 => pin 2 /Digit2
	PE14 => pin 6 / Digit3
	
	
  */

//first, set variables for convenience
uint16_t segment_A = GPIO_PIN_10;
uint16_t segment_B = GPIO_PIN_12;
uint16_t segment_C = GPIO_PIN_9;
uint16_t segment_D = GPIO_PIN_5;
uint16_t segment_E = GPIO_PIN_6;
uint16_t segment_F = GPIO_PIN_8;
uint16_t segment_G = GPIO_PIN_11;

uint16_t segment_decimal = GPIO_PIN_4;
uint16_t digit_1 = GPIO_PIN_4;
uint16_t digit_2 = GPIO_PIN_13;
uint16_t digit_3 = GPIO_PIN_14;
uint16_t decimal = GPIO_PIN_7;

GPIO_InitTypeDef GPIO_Init_Seg;

//the configuration function for the GPIOS for the 7-seg
//all GPIOS are on E and are set here

void Display_Config (void)
{
	__HAL_RCC_GPIOE_CLK_ENABLE();

  GPIO_Init_Seg.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6| GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
	GPIO_Init_Seg.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init_Seg.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_Init_Seg.Pull = GPIO_PULLDOWN;
	
	HAL_GPIO_Init(GPIOE, &GPIO_Init_Seg);
}

void Set_Off(void)
{
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_decimal, GPIO_PIN_RESET);
}


//Set all segments to on
//usually for testing
void Set_All(void)
{
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_decimal, GPIO_PIN_SET);
}

/*Sets a negative symbol on the 7-seg
no inputs or outputs*/

void Set_Neg(void)
{
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_RESET);
	
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_SET);
}

//sets a 0 digit to the 7 segment display
//no inputs or outputs required
void Set_0(void)
{
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_RESET);
}

/*
the function that outputs a 9 at a digit on the 7-seg display
the digit needs to be set elsewhere (or this won't work)
The segments set are A,B,C,D, F and G
E is NOT set
*/
void Set_9(void)
{
	//turn E off
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_RESET);
	
	// turn the rest on
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_SET);
}

/*
the function that outputs an 8 at a digit on the 7-seg display
the digit needs to be set elsewhere (or this won't work)
The segments set are A,B,C,D,E F and G
All are set, its identical to Set_All
*/
void Set_8(void)
{
	Set_All();
}

//sets a 7 digit to the 7 segment display
//no inputs or outputs required
void Set_7(void)
{
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_RESET);
}

//sets a 6 digit to the 7 segment display
//no inputs or outputs required
void Set_6(void)
{
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_RESET);
}

//sets a 5 digit to the 7 segment display
//no inputs or outputs required
void Set_5(void)
{
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_RESET);
}

//sets a 4 digit to the 7 segment display
//no inputs or outputs required
void Set_4(void)
{

	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_RESET);
}

//sets a 3 digit to the 7 segment display
//no inputs or outputs required
void Set_3(void)
{
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_RESET);
}

//sets a 2 digit to the 7 segment display
//no inputs or outputs required
void Set_2(void)
{
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_SET);
}

//sets a 1 digit to the 7 segment display
//no inputs or outputs required
void Set_1(void)
{
	HAL_GPIO_WritePin(GPIOE, segment_A ,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_B, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_C ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, segment_D, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_E, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_F, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, segment_G, GPIO_PIN_RESET);
}

/*functions for controlling the decimal point
same style as the others
*/
void Set_Decimal(int decimal_flag)
{
	if(decimal_flag)
	{HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7 ,GPIO_PIN_SET);}
	else
	{HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7 ,GPIO_PIN_RESET);}
		
}

void Reset_Decimal(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7 ,GPIO_PIN_RESET);
}

//determines what digit to write for the input digit
//input is an integer number between 0-9
//no output, directly sets the 7 seg
//the digit GPIO needs to be set externally, this only decides WHICH digit to write, from input
void Set_Number(int number)
{
	switch(number)
	{
		case 0:
			Set_0();
			break;
		case 1:
			Set_1();
			break;
		case 2:
			Set_2();
			break;		
		case 3:
			Set_3();
			break;
		case 4:
			Set_4();
			break;
		case 5:
			Set_5();
			break;
		case 6:
			Set_6();
			break;
		case 7:
			Set_7();
			break;
		case 8:
			Set_8();
			break;
		case 9:
			Set_9();
			break;
		case 10:		//this is for the case to turn off. display nothing.
			Set_Off();
			break;
		default:
			Set_0();
			break;
	}
	
}

//turns on ALL digits simultaneously
//usually used for testing, thus called
void All_Digits(void)
{
	HAL_GPIO_WritePin(GPIOE, digit_1 ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, digit_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, digit_3 ,GPIO_PIN_SET);
}

//turns on the most significant digit only (usually hundreds digit)
void First_Digit_Set(void)
{
	HAL_GPIO_WritePin(GPIOE, digit_1 ,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, digit_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, digit_3 ,GPIO_PIN_RESET);
}

//turns on the second digit only (tens digit)
void Second_Digit_Set(void)
{
	HAL_GPIO_WritePin(GPIOE, digit_1 ,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, digit_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, digit_3 ,GPIO_PIN_RESET);
}

//turns on the third digit only, (ones digit)
void Third_Digit_Set(void)
{
	HAL_GPIO_WritePin(GPIOE, digit_1 ,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, digit_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, digit_3 ,GPIO_PIN_SET);
}

