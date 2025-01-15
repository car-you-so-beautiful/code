
#include "motor.h"
#include "control.h"



float Right_out = 0;   //���ֵ�����
float Left_out = 0;    //���ֵ�����
float motor_begin = 2000;




void MotorInit()
{

    //���1
    pwm_init(PWM_L_CH1, 15000, 0);//ATOM 0ģ���ͨ��0 ʹ��P02_4�������PWM  PWMƵ��10kHZ  ռ�ձȰٷ�֮0/GTM_ATOM0_PWM_DUTY_MAX*100
    pwm_init(PWM_L_CH2, 15000, 0);

    //���2
    pwm_init(PWM_R_CH1, 15000, 0);//ATOM 0ģ���ͨ��4 ʹ��P02_4�������PWM  PWMƵ��10kHZ  ռ�ձȰٷ�֮0/GTM_ATOM0_PWM_DUTY_MAX*100
    pwm_init(PWM_R_CH2, 15000, 0);
    //���
    pwm_init(SERVO_MOTOR_PWM, 50, 0);
}


int16 PWM_Limit(float  PWM,int16 max)
{
    if(PWM > -max && PWM < max)
        return (int16)PWM;     
    else if( PWM >=  max)
        PWM =max;    
    else
        PWM =-max;
    
    return (int16)PWM;
}


int preflag=1; 

void ALL_Out(void)
{


    if(CarInfo.Mode <= STOPM)
    {
        Right_out = 0;
        Left_out  = 0;
    }
    else if(CarInfo.Mode <= MOTORTEST)//�������
    {
        Right_out = 1000;
        Left_out  = 1000;
    }
    else if(CarInfo.Mode <= STAND)//ƽ��ģʽ
    {
        Right_out =  R_SpeedControl;
        Left_out  =  L_SpeedControl;
    }

    Right_out = PWM_Limit((float)Right_out,5000);
     
    Left_out = PWM_Limit((float)Left_out,5000);
//    pwm_set_duty(PWM_L_CH1, 1500);
    if(Right_out>=0)
    {
        pwm_set_duty(PWM_R_CH1,(uint32)Right_out);
        pwm_set_duty(PWM_R_CH2, 0);
    }
    else
    {
        pwm_set_duty(PWM_R_CH1, 0);
        pwm_set_duty(PWM_R_CH2, (uint32)-Right_out);
    }
    if(Left_out >=0)
    {
        pwm_set_duty(PWM_L_CH1, (uint32)Left_out);
        pwm_set_duty(PWM_L_CH2, 0);
    }
    else
    {
        pwm_set_duty(PWM_L_CH1, 0);
        pwm_set_duty(PWM_L_CH2, (uint32)-Left_out);
    }


}
