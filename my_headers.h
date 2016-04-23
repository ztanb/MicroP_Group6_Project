
/*Headers for all user defined functions*/
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection

//first, the functions for the accelerometer
void Configure_Accel_GPIO 		(void);
void Configure_Accel					(void);

//the prototype for the system clock configuration
//see SystemClock_Config.c

void SystemClock_Config			(void);

void initializeLED_IO 			(void);

//the prototpyes and variables for the temperature ADC
//see Temperature.c for more
void Configure_Temp_ADC			(void);
float Temperature_Read			(void);
extern int accel_flag;

extern uint8_t value_pwm;

//functiions for I2C
void Transmit(uint8_t* data);
uint8_t* Receive(void);
void i2c_config(void);


//the variable for storing the temperature
//should be read with a MUTEX
extern float temperature;

//the protype and variable for the timer interrupt
//see Timer_Config.c for more
void Timer_Config				(void);
void Timer_Config4(void);
extern int timer_flag;

extern uint8_t *ext_value;
extern uint8_t value_PWM;
extern uint8_t s_led_mode;

//LED functions
void rotateClockwise(void);
void rotateCounterClockwise(void);
void turnLEDsOff(void);

//the prototypes for the keypad
//see keypad_config.c for more
void Configure_Key_GPIO			(void);
void read_keypad						(void);

//Settign flags to switch between modes //z:
//angle_mode_flag = 1 for pitch
//angle_mode_flag = 2 for roll
extern int angle_mode_flag;

//mode_flag = 0 for temperature
//mode_flag = 1 for accelerometer mode
extern int mode_flag;

//Alarm flag
extern int alarm_flag;

//the prototypes and variables for the 7-segment display
//many functions for setting the digits
void Display_Config 			(void);
void Set_Neg					(void);
void Set_All					(void);
void Set_Off					(void);
void Set_0						(void);
void Set_1						(void);
void Set_2						(void);
void Set_3						(void);
void Set_4						(void);
void Set_5						(void);
void Set_6						(void);
void Set_7						(void);
void Set_8						(void);
void Set_9						(void);
void Set_10						(void);

void All_Digits					(void);
void First_Digit_Set			(void);
void Second_Digit_Set			(void);
void Third_Digit_Set			(void);
void Set_Number						(int number);
void Set_Decimal					(int);
void Reset_Decimal				(void);

void normalize(float *array);
float* accelerometer_calculate_rotation(float x, float y, float z);
extern float accel_array[3];
extern float* rotation_array;

//the variables for the threads
extern osThreadId tid_Thread_Temp;
extern osThreadId tid_Thread_Accel;
extern osThreadId tid_Thread_Keypad;
extern osThreadId tid_Thread_LED;
extern osThreadId tid_Thread_I2C;

//mutex for safety
extern osMutexId temp_mutex;		
extern osMutexId accel_mutex;

//Kalman filter
float Kalmanfilter_C(float);
float Kalmanfilter_C2(float);
float Kalmanfilter_C3(float);
float Kalmanfilter_Ct(uint32_t);


