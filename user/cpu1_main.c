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
        if(taskNum[3] == TASK_ENABLE) // && (begin_flag == 1 || begin_flag == 2 || begin_flag == 3))
        {
          taskNum[3] = 20;
          if(ov7725_finish_flag)
          {
              fps_cnt[2]++;
              camera_binary_image_decompression(&ov7725_image_binary[0][0], &ov7725_uart_image_dec[0][0], OV7725_H*OV7725_W/8);
              Get_Attitude();
              findline();  //Ѳ��
              caculate_err(); //����ƫ��
              ov7725_finish_flag = 0;
          }
      }
    }
}
#pragma section all restore
