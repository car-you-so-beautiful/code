/*    ÿ��һ�� �ù�ʡ��         ����ΰ� �ȹ�����
                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
              ���汣�� ����BUG �����޸�
*/
//xzy xzy xzy
/********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "isr.h"
#include "headfile_code.h"
#pragma section all "cpu1_dsram"
#define TASK_ENABLE 0
// **************************** �������� ****************************
void core1_main(void)
{
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {
        TFT180_SHOW();
        if(mt9v03x_finish_flag)     //һ��ͼ����ȫ�ɼ���Ϻ��ٽ���ͼ�����ʾ�жϺ���ʾ
                {
                    //Set_image_towvalues(150); //�̶���ֵ��ֵ��
                    int BandW_threshold = otsuThreshold_fast(mt9v03x_image[0]);//��򷨵õ���̬��ֵ
                    Set_image_towvalues(BandW_threshold); //��̬��ֵ��ֵ�����õ���ֵ����Ķ�ά����mt9v03x_image_BandW

                    Find_Mid_Line(mt9v03x_image_Band[MT9V03X_H][MT9V03X_W]);//�ұ���

                    tft180_displayimage03x(mt9v03x_image_Band[0],MT9V03X_W,MT9V03X_H);//��ʾ��ֵ�����ͼ��
                    Find_Mid_Line_Draw_Line();//����

                    mt9v03x_finish_flag = 0;//ͼ����ʾ��ɺ�ŶԱ�־λ����
                }
          }
}
#pragma section all restore
