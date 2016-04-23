#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "my_headers.h"					//our headers

/*All the necessary files to configure the timer
*/

TIM_HandleTypeDef TIM_Handle;
TIM_HandleTypeDef TIM_Handle4;

uint8_t value_PWM;

int counter = 0;
int pwm_counter = 0;
int counter_val = 0;
int limit = 0;

/*
A function to control the timer
sets up all necessary values, such as prescaler, counter type, frequency and period
note that the sampling rate = base rate/ (period*prescaler), base rate = 84 MHz for tim3
Just needs to be faster than 1KHz for the human eye!

no inputs or outputs required
*/
void Timer_Config(void)
{
	__TIM3_CLK_ENABLE();
    TIM_Handle.Init.Prescaler = 2400;
    TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM_Handle.Init.Period = 35;
    TIM_Handle.Instance = TIM3;   //Same timer whose clocks we enabled
    HAL_TIM_Base_Init(&TIM_Handle);     // Init timer
    HAL_TIM_Base_Start_IT(&TIM_Handle); // start timer interrupts
	
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
}
void Timer_Config4(void)
{
	__TIM4_CLK_ENABLE();
    TIM_Handle.Init.Prescaler = 20;
    TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM_Handle.Init.Period = 50;
    TIM_Handle.Instance = TIM4;   //Same timer whose clocks we enabled
    HAL_TIM_Base_Init(&TIM_Handle4);     // Init timer
    HAL_TIM_Base_Start_IT(&TIM_Handle4); // start timer interrupts
	
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&TIM_Handle4, TIM_FLAG_UPDATE) != RESET)      //In case other interrupts are also running
    {
        if (__HAL_TIM_GET_ITSTATUS(&TIM_Handle4, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_CLEAR_FLAG(&TIM_Handle4, TIM_FLAG_UPDATE);
					
					pwm_counter++;
					limit = (value_PWM * 1666/256);
					if( pwm_counter == limit)
					{
						osSignalSet	(tid_Thread_LED,0x01); //off
					}
					else if(pwm_counter == 1666)
					{
						osSignalSet	(tid_Thread_LED,0x02); //on
						pwm_counter = 0;
					}
					

        }
    }
}

/*
The handler for the timer interrupt
no inputs or outputs required
only sets the flag at the interrupt, handle event seperately for safety
*/
void TIM3_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&TIM_Handle, TIM_FLAG_UPDATE) != RESET)      //In case other interrupts are also running
    {
        if (__HAL_TIM_GET_ITSTATUS(&TIM_Handle, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_CLEAR_FLAG(&TIM_Handle, TIM_FLAG_UPDATE);
					
					//the timer for the 7 segment display at 1000Hz (84MHz / (35*2400) = 1Khz)
					osSignalSet	(tid_Thread_Keypad,0x01);
					
					//divides the frequency by 10 for the temperature sampling rate of 100Hz
					counter = counter+1;
					if(counter>10)
					{
						osSignalSet	(tid_Thread_Temp,0x01);	//set a signal to resume code in Thread_Temp
						//osSignalSet	(tid_Thread_LED,0x01); //off
						counter = 0;
					}
        }
    }
}
