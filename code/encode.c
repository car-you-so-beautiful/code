#include "encode.h"

#include "math.h"


int16 Left_count;
int16 Right_count;



float speed_avr;  //左右轮平均速度
float L_CarSpeed;   //左轮速度
float R_CarSpeed;   //右轮速度

void speedcount_init(void)
{

    encoder_quad_init(ENCODER1_GPT,count1_pin,dir1_pin);
    encoder_quad_init(ENCODER2_GPT,count2_pin,dir2_pin);
}

void GetSpeed()
{
    static int16 l_filter[4],r_filter[4];

   
    Left_count= -encoder_get_count(ENCODER2_GPT);
    encoder_clear_count(ENCODER2_GPT);
    Right_count= encoder_get_count(ENCODER1_GPT);
    encoder_clear_count(ENCODER1_GPT);


    r_filter[0]=Right_count;
    for (int i = 3; i > 0; i--)
    {
        r_filter[i] = r_filter[i - 1];
    }
    l_filter[0]=Left_count;
    for (int i = 3; i > 0; i--)
    {
        l_filter[i] = l_filter[i - 1];
    }
     //	//修改频率，编码器单位
    L_CarSpeed = (L_CarSpeed*0.15 + 0.85*(0.4*l_filter[0]+0.3*l_filter[1]+0.2*l_filter[2]+0.1*l_filter[3]) * SPEED_F / L_QD_UNIT);    //（计数值*计数频率/一米计数值）求出车速转换为M/S
    R_CarSpeed = (R_CarSpeed*0.15 + 0.85*(0.4*r_filter[0]+0.3*r_filter[1]+0.2*r_filter[2]+0.1*r_filter[3]) * SPEED_F / R_QD_UNIT);    //求出车速转换为M/S
    if (L_CarSpeed > 8)L_CarSpeed = 8;//限速
    if (R_CarSpeed > 8)R_CarSpeed = 8;



    speed_avr= (L_CarSpeed+R_CarSpeed)/2;





}
