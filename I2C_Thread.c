#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "my_headers.h"
//#include <stdio.h>
#include <stdlib.h>

void Thread_I2C (void const *argument);                 // thread function
//osThreadId tid_Thread_I2C;                              // thread id
osThreadDef(Thread_I2C, osPriorityNormal, 1, 0);

//extern uint8_t value_PWM;
uint8_t *ext_value;
uint8_t* data; 
/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_I2C (void) {

  tid_Thread_I2C = osThreadCreate(osThread(Thread_I2C), NULL); // Start LED_Thread
  if (!tid_Thread_I2C) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  for the I2C communicaion
 *---------------------------------------------------------------------------*/

void Thread_I2C (void const *argument) 
		{
			while(1)
				{
					//add transmit and recieve here
					
					//Multiplication and casting for the sending (as requested by phil's code)
					//int temp_temp = abs((int)(temperature *10));	
					//int pitch_temp = abs((int)(rotation_array[0] * 100));
					//int roll_temp = abs((int)(rotation_array[1] * 100)); 

 					/*uint8_t data[4] = {0,0,0,0};
					data[0] = pitch_temp;
					data[1] = roll_temp;
					data[2] = temp_temp;*/
					
					data = "1234567";
					
					Transmit(data);
					//ext_value = Receive();
					//value_PWM = ext_value[0];
					//s_led_mode = ext_value[1];
				}
		}