#ifndef _VARIABLE_INCLUDES_H_
#define _VARIABLE_INCLUDES_H_

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "Servo.h"
#include "string.h"
#include "y_kinematics.h"
#include "math.h"

//=======================================================================

typedef struct
{
	int x;
	int y;
	int z;
}Position_t;

typedef struct {
	float L0;
	float L1;
	float L2;
	
	float servo_angle[6];	//0号到4号舵机的角度
	float servo_pwm[6];		//0号到4号舵机的角度
}kinematics_t;


extern kinematics_t kinematics;
extern int Offset_X;
extern int Offset_Y;
extern Position_t Position;
extern int PWM1,PWM2;
extern int PWM3,PWM4,PWM5;
extern float Angle1,Angle2;
extern float Angle3,Angle4;
extern float W1,W2;
extern uint8_t rx_data[10];
extern uint8_t tx_data[1];
extern uint8_t tt1;
extern uint8_t tt2;
extern int Error;
extern float x,y,z;
extern float X1,X2,Y1,Y2;
extern int t;

#endif // !_VARIABLE_INCLUDES_H_
