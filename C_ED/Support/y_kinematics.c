#include "Variable_Includes.h"

#define pi 3.1415926f
#define cos60 0.5f

void setup_kinematics(float L0, float L1, float L2, float L3) {
	kinematics.L0 = L0;
	kinematics.L1 = L1;
	kinematics.L2 = L2;
}

void caculate(float x, float y,float z)
{
	float L0 = 0.0f; 
	float L1 = 0.0f; //杆长
	float L2 = 0.0f;
	float R = 0, D = 0;
	float aa,bb,cc,dd,ee;
	R = sqrt(x*x+y*y);
	dd = atanf(10 / (R-5));
	R = R -10;
	if(R > 245) R = 245;
	if(R < 90) R = 90;
	L0 = kinematics.L0;
	L1 = kinematics.L1;
	L2 = kinematics.L2 - Position.z;
	D = sqrt(R*R + L2*L2);
	dd = dd * 2.5;
	aa = 90.0f - acosf((L0*L0+D*D-L1*L1)/(2.0f*D*L0)) * 57.295779513- acosf((R*R+D*D-L2*L2)/(2.0f*D*R)) * 57.295779513;
	bb = 180.0f - acosf((L0*L0+L1*L1-D*D)/(2.0f*L1*L0)) * 57.295779513;
	cc = 180 - acosf((D*D+L2*L2-R*R)/(2.0f*L2*D)) * 57.295779513 - acosf((D*D+L1*L1-L0*L0)/(2.0f*L1*D)) * 57.295779513;
	 if(y) ee = atanf(x/y) * 57.295779513 + 26.8 - dd;
	else if( x>0 ) ee = 26.8 - dd + 90;
	else ee = 26.8 - dd - 90;
	kinematics.servo_angle[0] = ee;
	kinematics.servo_angle[1] = aa;
	kinematics.servo_angle[2] = bb;
	kinematics.servo_angle[3] = cc - 10;
	
//	kinematics.servo_angle[0] = 26.8 -90 ;
//	kinematics.servo_angle[1] = 0;
//	kinematics.servo_angle[2] = 0;
//	kinematics.servo_angle[3] = 0; 
	
	kinematics.servo_pwm[0] = (int)(1500 - 7.407407 * kinematics.servo_angle[0]);
	kinematics.servo_pwm[1] = (int)(1500 + 7.407407 * kinematics.servo_angle[2]);
	kinematics.servo_pwm[2] = (int)(1500 + 7.407407 * kinematics.servo_angle[1]);
	kinematics.servo_pwm[3] = (int)(1500 + 7.407407 * kinematics.servo_angle[3]);
}

