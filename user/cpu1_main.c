/*    每日一拜 好过省赛         天天参拜 稳过国赛
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
              佛祖保佑 永无BUG 永不修改
*/
//xzy xzy xzy
/********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "isr.h"
#include "headfile_code.h"
#pragma section all "cpu1_dsram"
#define TASK_ENABLE 0
// **************************** 代码区域 ****************************
void core1_main(void)
{
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);             // 打开全局中断
    cpu_wait_event_ready();                 // 等待所有核心初始化完毕
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
              findline();  //巡线
              caculate_err(); //计算偏差
              ov7725_finish_flag = 0;
          }
      }
    }
}
#pragma section all restore
