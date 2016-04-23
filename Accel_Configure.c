#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include <math.h>
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "my_headers.h"



/*
Configure_GPIO
No inputs, no outputs
configures the GPIO setting for the accelerometer interrupt
reads as an input, pulled down to ground --> THIS MAY NEED TO BE CHANGED
*/

GPIO_InitTypeDef GPIO_Init_Accel;

int accel_flag = 0;
float ACC11, ACC21, ACC31, ACC12, ACC22, ACC32, ACC13, ACC23, ACC33, ACC10, ACC20, ACC30;
float normalized[3];
float rotations[3];

float absolute(float input);

void Configure_Accel_GPIO ()
{

	__HAL_RCC_GPIOE_CLK_ENABLE();

	
  GPIO_Init_Accel.Pin = GPIO_PIN_0;
	GPIO_Init_Accel.Mode = GPIO_MODE_IT_RISING;
	GPIO_Init_Accel.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_Init_Accel.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOE, &GPIO_Init_Accel);
	
	
}

LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
LIS3DSH_DRYInterruptConfigTypeDef DRYInterruptConfig;

/*Configure the accelerometer
	*/
void Configure_Accel(void)
{
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;
	LIS3DSH_InitStruct.Axes_Enable = LIS3DSH_X_ENABLE | LIS3DSH_Y_ENABLE | LIS3DSH_Z_ENABLE;
	LIS3DSH_InitStruct.Full_Scale = LIS3DSH_FULLSCALE_4;
	LIS3DSH_InitStruct.AA_Filter_BW = LIS3DSH_AA_BW_200;
	LIS3DSH_InitStruct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;
	LIS3DSH_InitStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL;
	
	
	DRYInterruptConfig.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	DRYInterruptConfig.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	DRYInterruptConfig.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	
	
	LIS3DSH_DataReadyInterruptConfig(&DRYInterruptConfig);
	LIS3DSH_Init(&LIS3DSH_InitStruct);	
	
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
}
/*a function to nromalize the values
these values are given by the configure matlab script
see the Tilt and Angle application document for more info

*/
void normalize(float *array)
{
	ACC11 = 0.1214;
	ACC21 = -0.0056;
	ACC31 = -0.0059;
	
	ACC12 = -0.0039;
	ACC22 = 0.1181;
	ACC32 = -0.0032;
	
	ACC13 =0.0028;
	ACC23 =0.0024;
	ACC33 = 0.1219;
	
	ACC10 = 0.0094;
	ACC20 = 0.0013;
	ACC30 = 0.0394;
	
	array[0] = array[0]*ACC11 + array[1]*ACC12 + array[2]*ACC13 + ACC10;
	array[1] = array[0]*ACC21 + array[1]*ACC22 + array[2]*ACC23 + ACC20;
	array[2] = array[0]*ACC31 + array[1]*ACC32 + array[2]*ACC33 + ACC30;
	
}

/*
calculates the rotation based on x,y,z values
inputs: x,y,z --> floats for the normalized gravitational values
output: rotations --> an array containing the pitch, roll and yaw
*/
float* accelerometer_calculate_rotation(float x, float y, float z) {
	
	//pitch:
	if (y != 0 && z != 0) {
		rotations[0] = absolute((180.0/3.1415)*(atan(x / sqrt(y*y + z*z))));
	}
	
	//roll
	if (x != 0 && z != 0) {
		rotations[1] = absolute((180.0/3.1415)*(atan(y / sqrt(x*x + z*z))));
	}
	//yaw
	if (x != 0 && y != 0) {
		rotations[2] = absolute((180.0/3.1415)*(atan(z / sqrt(x*x + y*y))));
	}
	
	return rotations;
}

/*
handling the interrupt for the accelerometer
*/
void EXTI0_IRQHandler(void)
{
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	osSignalSet	( tid_Thread_Accel,0x01);					//A signal is set to execute accelerometer code
}

float absolute(float input)	//a function to deal with negative angle values
{
	if(input>0)
	{
		return input;
	}
	else
	{
		return input*-1;
	}
}
