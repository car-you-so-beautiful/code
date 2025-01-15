#ifndef _FINDLINE_H
#define _FINDLINE_H

#include "zf_common_headfile.h"
#include "headfile_code.h"




//宏定义
#define image_h 120//图像高度
#define image_w 160//图像宽度

#define white_pixel 255
#define black_pixel 0

#define bin_jump_num    1//跳过的点数
#define border_max  image_w-2 //边界最大值
#define border_min  1   //边界最小值
extern uint8 ov7725_uart_image_dec[OV7725_H][OV7725_W];//图像数组
extern int16 qianzhan ;
extern uint8 stop_flag;



typedef struct
{

    int16 midline[120];

    int16 leftline[120];
    int16 rightline[120];
    int16 leftlineflag[120];
    int16 rightlineflag[120];

    int16 newleftline[120];
    int16 newrightline[120];

    int16 leftstartpoint;
    int16 rightstartpoint;
    int16 endline;
    int8 loseflag;
    int16 upline[160];
    int16 uplineflag[160];
    float err[60];
    float kp1;
    float kd1;
    float U;
    float U1;
    float llasterr;
    int16 r_lose;
    int16 l_lose;

} findline_TypeDef;
typedef struct
{
        int16 leftup[2];
        int16 rightup[2];
        int16 leftdown[2];
        int16 rightdown[2];
        int16 lcenter[2];
        int16 rcenter[2];
        int16 counter[1];
        float lenth;
        int16 leftdown_flag ;
        int16 leftup_flag;
        int16 rightdown_flag ;
        int16 rightup_flag ;
        int16 lcenter_flag ;
        int16 rcenter_flag;
}fp_TypeDef;

extern fp_TypeDef Findp;

extern findline_TypeDef Findline;
extern void findline(void); //直接在中断或循环里调用此程序就可以循环执行了
float Slope_Calculate(uint8 begin, uint8 end, uint8 *border);
void calculate_s_i(uint8 start, uint8 end, uint8 *border, float *slope_rate, float *intercept);
void caculate_err(void);
void EmergencyStop(void);
#endif /*_IMAGE_H*/
