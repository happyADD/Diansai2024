#include "Variable_Includes.h"

void Servo_Init()
{
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); 
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3); 
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); 

	Position.x=-100;
	Position.y=0;
	Position.z=0;
	kinematics.servo_pwm[0] = 1500;
	kinematics.servo_pwm[1] = 1500;
	kinematics.servo_pwm[2] = 1500;
	kinematics.servo_pwm[3] = 1500;
	X1 = 0;
	X2 = 0;
	Y1 = 0;
	Y2 = 0;
}

void Servo_Set()
{	
	PWM1=kinematics.servo_pwm[0];
	PWM2=kinematics.servo_pwm[1];
	PWM3=kinematics.servo_pwm[2];
	PWM4=kinematics.servo_pwm[3];
//	PWM1=1500;
//	PWM2=1500;
//	PWM3=1500;
//	PWM4=1500;
}
