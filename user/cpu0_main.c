/*********************************************************************************************************************
********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "headfile_code.h"
#pragma section all "cpu0_dsram"


// **************************** �������� ****************************


#define TASK_ENABLE 0

int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    MotorInit();       //��ʼ������Ͷ������
    ips200_init(IPS200_TYPE_PARALLEL8); //��ʼ����ʾ��
    gpio_init(P21_5, GPO,0, GPO_PUSH_PULL);    //��ʼ������������˸���жϳ����Ƿ���
    ips200_show_string(0, 16, "imu660ra init.");
     imu660ra_init();  //��ʼ��������
    ips200_show_string(0, 16, "code init.");
    speedcount_init(); //��ʼ��������
    ips200_show_string(0, 16, "ov7725 init.");

    while(1)
    {
    if(ov7725_init())  //��ʼ������ͷ
    {
        ips200_show_string(0, 16, "ov7725 reinit.");
        system_delay_ms(100);
    }
    else
    {
        break;
    }
    }

    //wireless_uart_init();  ���ߴ��ڳ�ʼ��
    adc_init(ADC0_CH8_A8, ADC_8BIT); //��ʼ����ѹ�ɼ�
    bee_init(); //��ʼ��������  �÷���bee_time = 100; ���Ƿ�������100ms
    key_init(100); //������ʼ��
    Butterworth_Parameter_Init(); //��ͨ�˲�����ʼ��
    PID_init(); //���Ʋ�����ʼ��
    PIT_init();  //�жϳ�ʼ��
    dl1a_init ();
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����
    CarInfo.Mode =  STAND; //STAND MOTORTEST;
    ips200_clear();


	while (TRUE)
	{
	    if (taskNum[0] == TASK_ENABLE) //500ms
       {
           taskNum[0] = 500;
           gpio_toggle_level(P21_5);  //��ת���ŵ�ƽ
           printf("%lf\n",Right_out); //�����������Ҫ������������������115200
       }
       if (taskNum[1] == TASK_ENABLE)  //UI������ʾ
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
       if (taskNum[2] == TASK_ENABLE)  //��ʱ������
       {
           taskNum[2] = 200;


       }






        // �˴���д��Ҫѭ��ִ�еĴ���





	}
}

#pragma section all restore
