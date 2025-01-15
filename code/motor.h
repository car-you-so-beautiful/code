#ifndef __MOTOR_H
#define __MOTOR_H

/****Includes*************************************************/

#include "zf_common_headfile.h"

/****Definitions**********************************************/
#define PWM_R_CH1           ATOM0_CH5_P02_5
#define PWM_R_CH2           ATOM0_CH7_P02_7
#define PWM_L_CH1           ATOM0_CH4_P02_4
#define PWM_L_CH2           ATOM0_CH6_P02_6

#define SERVO_MOTOR_PWM     ATOM1_CH1_P33_9   // 定义主板上舵机对应引脚

void ALL_Out(void);
void MotorInit(void);
extern float Right_out ;
extern float Left_out;

#endif
