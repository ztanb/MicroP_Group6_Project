#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "my_headers.h"

void Thread_Keypad (void const *argument);                 // thread function
//osThreadId tid_Thread_Keypad;                              // thread id
osThreadDef(Thread_Keypad, osPriorityNormal, 1, 0);

//Variables for LED display function
int update_counter = 0;
int first_decimal, second_decimal = 0;
int digit_flag = 0;
int display_value, first_digit, second_digit, third_digit = 0;
int scale = 1;

//float rotations[3];
float pitchORroll_value = 0;

float inter_value = 0.0;

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_Keypad (void) 
	{

  tid_Thread_Keypad = osThreadCreate(osThread(Thread_Keypad ), NULL); // Start LED_Thread
  if (!tid_Thread_Keypad) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  keypad
monitors the keypad for input and sets the 7-segment display
 *---------------------------------------------------------------------------*/
void Thread_Keypad (void const *argument) 
{
	while(1)
	{
		//
		//		printf("x = %f", accel_array[0]);
		//		printf("y = %f", accel_array[1]);
		//		printf("z = %f", accel_array[2]);		
		//
		
			read_keypad(); //returnd 1 if a key is pressed

			osSignalWait(0x00, osWaitForever);
			update_counter = update_counter+1;
			
			digit_flag = (digit_flag+1)%3;
			inter_value = temperature;
		
			if (angle_mode_flag == 1)
				{
					pitchORroll_value = rotation_array[0];
				}
				
				if (angle_mode_flag == 2)
				{
					pitchORroll_value = rotation_array[1];
				}
			
			if(mode_flag == 0) // display temperature
			{
				if(update_counter>100)
				{
					//handling for the decimal placement and order of values
					//if less than 10, 2 decimal places
					if(inter_value>=0 && inter_value<10)
					{
						first_decimal = 1;
						second_decimal = 0;
						scale = 100;
					}
					
					//if more than 10, but less than 100, 1 decimal place
					else if(inter_value>=10 && inter_value<100)
					{
						first_decimal = 0;
						second_decimal = 1;
						scale = 10;
					}
					
					//otherwise no decimal places
					else if(inter_value >= 100)
					{
						first_decimal = 0;
						second_decimal = 0;
						scale = 1;
					}
					
					//extract the digits
					//temperature = 6.34;
					display_value = (int)(inter_value*scale);
					first_digit = (display_value/100)%10;
					second_digit = (display_value/10)%10;
					third_digit = (display_value%10);
					update_counter = 0;
				}
				
				if((update_counter > 50) && (alarm_flag == 1))
				{
					first_digit = 10;		//10 is a case for settign all off
					second_digit = 10;
					third_digit = 10;
				}
				
				if(digit_flag == 0)
				{
					Set_Number(first_digit);
					First_Digit_Set();
					Set_Decimal(first_decimal);					
				}
				else if(digit_flag == 1)
				{
					Set_Number(second_digit);
					Second_Digit_Set();
					Set_Decimal(second_decimal);
				}
				else if(digit_flag == 2)
				{
					Set_Number(third_digit);
					Third_Digit_Set();
					Set_Decimal(0);
				}
			}
			
			if(mode_flag == 1) //display accelerometer data
			{
							//
				if(update_counter > 100)
				{
					//handling for the decimal placement and order of values
					//if less than 10, 2 decimal places
					if(pitchORroll_value>=0 && pitchORroll_value<10)
					{
						first_decimal = 1;
						second_decimal = 0;
						scale = 100;
					}
					
					//if more than 10, but less than 100, 1 decimal place
					else if(pitchORroll_value>=10 && pitchORroll_value<100)
					{
						first_decimal = 0;
						second_decimal = 1;
						scale = 10;
					}
					
					//otherwise no decimal places
					else if(pitchORroll_value >= 100)
					{
						first_decimal = 0;
						second_decimal = 0;
						scale = 1;
					}
					
					//extract the digits
					//temperature = 6.34;
					display_value = (int)(pitchORroll_value*scale);//??
					
					first_digit = (display_value/100)%10;
					second_digit = (display_value/10)%10;
					third_digit = (display_value%10);
					update_counter = 0;
							//
				}
				
				if((update_counter > 50) && (alarm_flag == 1))
				{
					first_digit = 10;		//10 is a case for settign all off
					second_digit = 10;
					third_digit = 10;
				}
				
				if(digit_flag == 0)
				{
					Set_Number(first_digit);
					First_Digit_Set();
					Set_Decimal(first_decimal);					
				}
				else if(digit_flag == 1)
				{
					Set_Number(second_digit);
					Second_Digit_Set();
					Set_Decimal(second_decimal);
				}
				else if(digit_flag == 2)
				{
					Set_Number(third_digit);
					Third_Digit_Set();
					Set_Decimal(0);
				}
				
			}
	}
}
