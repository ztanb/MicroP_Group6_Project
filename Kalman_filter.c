//
//Function for the kalman filter
//
#include "stm32f4xx_hal.h"
#include "my_headers.h"									//our headers
#include "stdio.h"


//The parameter values
float q = 1.1;
float r = 1.1;
float x = 9000;
float p = 0.1;
float k = 0.00;

float q2 = 1.1;
float r2 = 1.1;
float x2 = 9000;
float p2 = 0.1;
float k2 = 0.00;

float q3 = 1.1;
float r3 = 1.1;
float x3 = 9000;
float p3 = 0.1;
float k3 = 0.00;

float qt = 1.1;
float rt = 1.1;
float xt = 9000;
float pt = 0.1;
float kt = 0.00;

float OutputValue;

float Kalmanfilter_C(float InputValue)
{
	p = p + q;				// the kalman filter implementation
	k = p / (p + r);
	x = x + k * (InputValue - x);
	p = (1 - k) *p;
		
	OutputValue = x;			// returns the filtered value
	
return OutputValue;	
}

float Kalmanfilter_C2(float InputValue)
{
	p2 = p2 + q2;				// the kalman filter implementation
	k2 = p2 / (p2 + r2);
	x2 = x2 + k2 * (InputValue - x2);
	p2 = (1 - k2) *p2;
		
	OutputValue = x2;			// returns the filtered value
	
return OutputValue;	
}

float Kalmanfilter_C3(float InputValue)
{
	p3 = p3 + q3;				// the kalman filter implementation
	k3 = p3 / (p3 + r3);
	x3 = x3 + k3 * (InputValue - x3);
	p3 = (1 - k3) *p3;
		
	OutputValue = x3;			// returns the filtered value
	
return OutputValue;	
}

float Kalmanfilter_Ct(uint32_t InputValue)
{
	pt = pt + qt;				// the kalman filter implementation
	kt = pt / (pt + rt);
	xt = xt + kt * ((float)InputValue - xt);
	pt = (1 - kt) *pt;
		
	OutputValue = xt;			// returns the filtered value
	
return OutputValue;	
}
