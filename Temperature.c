#define AVERAGE_SLOPE      2.5
#define V_25               760
#define OFFSET             329
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "my_headers.h"
#include "cmsis_os.h"    

ADC_HandleTypeDef ADC_Handler;
ADC_InitTypeDef ADC_Init;
ADC_ChannelConfTypeDef ADC_Channel;

uint32_t ADC_Value;
float filtered_ADC_Value;

float value;
float noise_compensation= 0;
int alarm_flag = 0;

//int temp_thresh = 25.0;	//for testing
int temp_thresh = 56.0;

/**Configure ADC
   * @brief 	Takes no arguments and configures the ADC for simple operation
		This function simply configures the ADC with the simplest useable values for our application
		No inputs are required otherwise, no outputs are passed
   * @retval None
   */
void Configure_Temp_ADC()
{
	__ADC1_CLK_ENABLE();
	
	ADC_Handler.Instance = ADC1;
	
	//first set the initializer
	//config values chosen from studyign the datasheets, as well as help from the class discussion board.
	
	ADC_Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
	ADC_Init.Resolution = ADC_RESOLUTION_12B;
	ADC_Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_Init.ContinuousConvMode = DISABLE;
	ADC_Init.DiscontinuousConvMode = DISABLE;
	ADC_Init.NbrOfDiscConversion = 0;
	ADC_Init.ScanConvMode = DISABLE;
	ADC_Init.NbrOfConversion = 1;
	ADC_Init.DMAContinuousRequests = DISABLE;
	ADC_Init.EOCSelection = DISABLE;
	
	ADC_Init.ExternalTrigConv = ADC_SOFTWARE_START;
	ADC_Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;
	
	//now set it to the handler
	ADC_Handler.Init = ADC_Init;
	
	//now initialize it
	HAL_ADC_Init(&ADC_Handler);
	
	//now set the channel info
	ADC_Channel.Channel = ADC_CHANNEL_16;
	ADC_Channel.Rank = 1;
	ADC_Channel.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	ADC_Channel.Offset = 0;
	
	
}

/*A wrapper for reading the temperature
no inputs, outputs a unsigned int
simply reads the temperature ADC value
*/
float Temperature_Read(void)
{
	alarm_flag = 0;
	
	HAL_ADC_Start(&ADC_Handler);
	
	ADC_Value = HAL_ADC_GetValue(&ADC_Handler);
	
	filtered_ADC_Value = Kalmanfilter_Ct(ADC_Value);
	
	value = ((((filtered_ADC_Value)*700.0)/4096.0) -V_25)/AVERAGE_SLOPE +OFFSET;
	
	HAL_ADC_Stop(&ADC_Handler);
	
	
	if(value >= temp_thresh)			// this is our threshhold temp for an overheat alarm
	{		
		alarm_flag = 1;
	}	
	
	return value;
} 

