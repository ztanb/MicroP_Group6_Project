/*******************************************************************************
  * @file    main.c
  * @author  Ashraf Suyyagh
	* @version V1.2.0
  * @date    17-January-2016
  * @brief   This file demonstrates flasing one LED at an interval of one second
	*          RTX based using CMSIS-RTOS 
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "my_headers.h"					        //our headers

//the prototypes for the threads, in the same style as the LED example
extern void start_Thread_Temp				(void);
extern void start_Thread_Accel			(void);
extern void start_Thread_Keypad			(void);
extern void start_Thread_LED 				(void);
extern void start_Thread_I2C 				(void);

extern void Thread_Temp(void const *argument);
extern void Thread_Accel(void const *argument);
extern void Thread_Keypad(void const *argument);
extern void Thread_LED (void const *argument);
extern void Thread_I2C (void const *argument);

osThreadId tid_Thread_Temp;
osThreadId tid_Thread_Accel;
osThreadId tid_Thread_Keypad;
osThreadId tid_Thread_I2C;   
osThreadId tid_Thread_LED;  

osMutexId temp_mutex;		//mutex for safety
osMutexId accel_mutex;


osMutexDef (temp_mutex);
osMutexDef (accel_mutex);

/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

/**
  * Main function
  */

float test_value = 0;

int main (void) {

  osKernelInitialize();                     /* initialize CMSIS-RTOS          */

  HAL_Init();                               /* Initialize the HAL Library     */

  SystemClock_Config();                     /* Configure the System Clock     */
	//configure the keypad GPIO
	Configure_Key_GPIO();
	
	//configure the timer for the 7seg and keypad
  Timer_Config();
	//Timer_Config4();
	//configure the 7-segment
	Display_Config ();
	
	//configure the accelerometer
	Configure_Accel_GPIO ();
	Configure_Accel();
	
	//configure the temperature ADC
	Configure_Temp_ADC();
	i2c_config();
	
	
	initializeLED_IO();
	
	//Configure the PWM
	

	/* User codes goes here
	3 threads - 1 for each aspect
	- Temp = for the temperature sensor
	- Accel = for the accelerometer
	- Keypad = for the 7segment display and keypad polling
	*/
  
	temp_mutex = osMutexCreate(osMutex(temp_mutex));		//creating the mutex
	accel_mutex = osMutexCreate(osMutex(accel_mutex));
	
 // start_Thread_Temp();
	//start_Thread_Accel(); 
	//start_Thread_Keypad(); 
	//start_Thread_LED ();
	start_Thread_I2C ();
	/* User codes ends here*/
  
	osKernelStart();                          /* start thread execution         */
}
