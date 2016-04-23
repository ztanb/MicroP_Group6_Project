#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "my_headers.h"					//our headers

void Thread_Temp (void const *argument);                 // thread function
//osThreadId tid_Thread_Temp;                              // thread id
osThreadDef(Thread_Temp, osPriorityAboveNormal, 1, 0);

float temperature = 0;


/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_Temp (void) 
	{

  tid_Thread_Temp = osThreadCreate(osThread(Thread_Temp ), NULL); // Start LED_Thread
  if (!tid_Thread_Temp) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  temperature
reads the temperature ADC
 *---------------------------------------------------------------------------*/
	void Thread_Temp (void const *argument) 
		{
		while(1)
			{
				osSignalWait(0x00, 10);
				
				osMutexWait(temp_mutex,osWaitForever);
				temperature = Temperature_Read();
				osMutexRelease(temp_mutex);
				
				
			}
	}
