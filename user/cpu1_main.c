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
        TFT180_SHOW();
        if(mt9v03x_finish_flag)     //一幅图像完全采集完毕后，再进行图像的显示判断和显示
                {
                    //Set_image_towvalues(150); //固定阈值二值化
                    int BandW_threshold = otsuThreshold_fast(mt9v03x_image[0]);//大津法得到动态阈值
                    Set_image_towvalues(BandW_threshold); //动态阈值二值化，得到二值化后的二维数组mt9v03x_image_BandW

                    Find_Mid_Line(mt9v03x_image_Band[MT9V03X_H][MT9V03X_W]);//找边线

                    tft180_displayimage03x(mt9v03x_image_Band[0],MT9V03X_W,MT9V03X_H);//显示二值化后的图像
                    Find_Mid_Line_Draw_Line();//画线

                    mt9v03x_finish_flag = 0;//图像显示完成后才对标志位清零
                }
          }
}
#pragma section all restore
