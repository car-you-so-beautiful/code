/*********************************************************************************************************************
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "headfile_code.h"
#pragma section all "cpu0_dsram"


// **************************** 代码区域 ****************************


#define TASK_ENABLE 0

int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    MotorInit();       //初始化电机和舵机控制
    ips200_init(IPS200_TYPE_PARALLEL8); //初始化显示屏
    gpio_init(P21_5, GPO,0, GPO_PUSH_PULL);    //初始化引脚用于闪烁，判断程序是否卡死
    ips200_show_string(0, 16, "imu660ra init.");
     imu660ra_init();  //初始化陀螺仪
    ips200_show_string(0, 16, "code init.");
    speedcount_init(); //初始化编码器
    ips200_show_string(0, 16, "ov7725 init.");

    while(1)
    {
    if(ov7725_init())  //初始化摄像头
    {
        ips200_show_string(0, 16, "ov7725 reinit.");
        system_delay_ms(100);
    }
    else
    {
        break;
    }
    }

    //wireless_uart_init();  无线串口初始化
    adc_init(ADC0_CH8_A8, ADC_8BIT); //初始化电压采集
    bee_init(); //初始化蜂鸣器  用法，bee_time = 100; 就是蜂鸣器响100ms
    key_init(100); //按键初始化
    Butterworth_Parameter_Init(); //低通滤波器初始化
    PID_init(); //控制参数初始化
    PIT_init();  //中断初始化
    dl1a_init ();
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    CarInfo.Mode =  STAND; //STAND MOTORTEST;
    ips200_clear();


	while (TRUE)
	{
	    if (taskNum[0] == TASK_ENABLE) //500ms
       {
           taskNum[0] = 500;
           gpio_toggle_level(P21_5);  //反转引脚电平
           printf("%lf\n",Right_out); //串口输出，需要连接下载器，波特率115200
       }
       if (taskNum[1] == TASK_ENABLE)  //UI部分显示
       {
           taskNum[1] = 200;
           if(gpio_get_level(P33_13))
           {
               UI_Disp();
               keyScan();
           }
           else
               show_image();
       }
//
       if (taskNum[2] == TASK_ENABLE)  //暂时无作用
       {
           taskNum[2] = 200;


       }






        // 此处编写需要循环执行的代码





	}
}

#pragma section all restore
