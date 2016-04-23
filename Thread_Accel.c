#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "my_headers.h"									//our headers
#include "lis3dsh.h"
#include "stdio.h"
#include <math.h>

/*The functions for creating the accelerometer threads
by jordan Litwin
based on the blink example by Ashraf Suyyagh
*/

void light_set_up(void);
void Thread_Accel (void const *argument);                 // thread function
//osThreadId tid_Thread_Accel;                              // thread id
osThreadDef(Thread_Accel, osPriorityAboveNormal, 1, 0);

GPIO_InitTypeDef GPIO_Init_Light;
	
//variables for storing the acceleration data
float accel_array[3];
float filtered_accel_array[3];
float* rotation_array;

float old_magnitude, new_magnitude = 0;
float delta = 0;
uint8_t tap_state = 0;
int current, last = 0;

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_Accel (void) 
	{

  tid_Thread_Accel = osThreadCreate(osThread(Thread_Accel ), NULL); // Start Thread
  if (!tid_Thread_Accel) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  for reading the accelerometer
All initialization and reading will be done here
 *---------------------------------------------------------------------------*/
	void Thread_Accel (void const *argument) 
		{
		while(1)
			{
				osSignalWait (0x00,20);				//signal will continue code after any signal set
				LIS3DSH_ReadACC(accel_array);
				///kalman filter
				filtered_accel_array[0] = Kalmanfilter_C(accel_array[0]);
				filtered_accel_array[1] = Kalmanfilter_C2(accel_array[1]);
				filtered_accel_array[2] = Kalmanfilter_C3(accel_array[2]);
				
				normalize(filtered_accel_array);
				//mutex for satefy, becuase rotation array is the variable we pass
				osMutexWait(accel_mutex,osWaitForever);
				rotation_array = accelerometer_calculate_rotation(filtered_accel_array[0],filtered_accel_array[1] ,filtered_accel_array[2] );
				osMutexRelease(accel_mutex);
				
				new_magnitude = pow(filtered_accel_array[0],2) + pow(filtered_accel_array[1],2) + pow(filtered_accel_array[2],2);
				delta = new_magnitude - old_magnitude;
				old_magnitude = new_magnitude;
				
				if(delta>500)
				{
					current = HAL_GetTick();
					if((current - last < 400) && (current-last>50))
					{
						tap_state = 1;
						//HAL_GPIO_TogglePin (GPIOD, GPIO_PIN_12);
					}
					else
					{
						tap_state = 0;
					}
					last = current;
				}
				
			}
	}
		

	void light_set_up(void)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();

	
  GPIO_Init_Light.Pin = GPIO_PIN_12;
	GPIO_Init_Light.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init_Light.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_Init_Light.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOD, &GPIO_Init_Light);
	}
