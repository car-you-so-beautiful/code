//-------------------------------------------------------------------------------------------------------------------
//  简介:八邻域图像处理
//  改的网上的代码，越写越屎山了。。。
//------------------------------------------------------------------------------------------------------------------
#include "findline.h"
#include "control.h"
uint8 lose_line;
int16 qianzhan =60;
findline_TypeDef Findline = {{0}};
fp_TypeDef Findp= {{0}};
uint8 stop_flag=0;//丢线标志位
float lastError;//偏差
void findline(void)  //根据图像寻找中线
{

    int16 i = 0, j = 0;//i是行 j是列
    //参数初始化
    lose_line = 0 ;
    Findline.midline[119] = 80;
    Findline.leftline[119] = 0;
    Findline.rightline[119] = 159;


    Findline.leftstartpoint = 0;
    Findline.rightstartpoint = 0;

    Findline.l_lose=0;
    Findline.r_lose=0;
    /*提取赛道边界*/
    //普通巡线 进阶可以找群主要八邻域
    {
        for(i = 118; i > 0; i--)
        {
            Findline.midline[i] = 0;
            Findline.leftline[i] = 0;
            Findline.rightline[i] = 159;
            Findline.leftlineflag[i] = 0;
            Findline.rightlineflag[i] = 0;
            Findline.endline = 0;
            //寻找左跳变点
            if(Findline.midline[i + 1] > 80)j = Findline.midline[i + 1] > 155 ? 155 : Findline.midline[i + 1];
            else j = Findline.midline[i + 1] < 4 ? 4 : Findline.midline[i + 1];  //先找到中间位置，从中间向左边开始找
            for(; j > 3; j--)
            {
                if(ov7725_uart_image_dec[i][j] == 255 && ov7725_uart_image_dec[i][j - 1] == 255 && ov7725_uart_image_dec[i][j - 2] == 0 && ov7725_uart_image_dec[i][j - 3] == 0)
                {
                    if(Findline.leftstartpoint == 0)
                        Findline.leftstartpoint = i;
                    Findline.leftline[i] = j - 1;
                    Findline.leftlineflag[i] = 1;
                    break;
                }
            }
            //寻找右跳变点
            if(Findline.midline[i + 1] > 80)j = Findline.midline[i + 1] > 155 ? 155 : Findline.midline[i + 1];
            else j = Findline.midline[i + 1] < 4 ? 4 : Findline.midline[i + 1];
            for(j = Findline.midline[i + 1]; j < 156; j++)
            {
                if(ov7725_uart_image_dec[i][j] == 255 && ov7725_uart_image_dec[i][j + 1] == 255 && ov7725_uart_image_dec[i][j + 2] == 0 && ov7725_uart_image_dec[i][j + 3] == 0)
                {
                    if(Findline.rightstartpoint == 0)
                        Findline.rightstartpoint = i;
                    Findline.rightline[i] = j + 1;
                    Findline.rightlineflag[i] = 1;
                    break;
                }
            }


            Findline.midline[i] = (Findline.leftline[i] + Findline.rightline[i]) / 2;
            //边线都没找到
            if((Findline.leftlineflag[i] == 0) && (Findline.rightlineflag[i] == 0))
            {
                lose_line++;
                Findline.midline[i] = Findline.midline[i + 1];

            }
            else if(lose_line > 0)
                lose_line--;

            if(Findline.leftlineflag[i] == 0)
                Findline.r_lose++;

            if(Findline.rightlineflag[i] == 0)
                Findline.l_lose++;

            //找顶点
            if(Findline.midline[i + 1] > 80)j = Findline.midline[i + 1] > 155 ? 155 : Findline.midline[i + 1];
            else j = Findline.midline[i + 1] < 4 ? 4 : Findline.midline[i + 1];

            if( ov7725_uart_image_dec[i - 1][j] == 0 && (ov7725_uart_image_dec[i - 1][j - 2] == 0 && ov7725_uart_image_dec[i - 1][j + 2] == 0)  &&  ov7725_uart_image_dec[i - 2][j] == 0 && (ov7725_uart_image_dec[i - 2][j - 2] == 0 && ov7725_uart_image_dec[i - 2][j + 2] == 0))
            {
                Findline.endline = i;
                break;
            }

        }
    }
    //EmergencyStop();
    //划线
}

void caculate_err(void)  //计算偏差
{
    int16 i,sum;
    //static float sum;
    lastError = Findline.err[0];
    sum = 0;
    for(i = 59; i >= 1; i--)
        Findline.err[i] = Findline.err[i - 1]; //偏差滤波
    Findline.err[0] = 0.0;
    //if(p_right_flag != 2)
        if(qianzhan > Findline.endline + 4)//<58
        {
            for(i = qianzhan; i > Findline.endline && i > qianzhan-3; i--) // 目前来说这个还是鸡肋不知道怎么砍掉
            {
                sum++;
                Findline.err[0] += (float)(Findline.midline[i] - 80);
            }
            Findline.err[0] = Findline.err[0] / sum;
        }
        else
            Findline.err[0] = (float)(Findline.midline[Findline.endline + 2] - 80);

        if (lastError * Findline.err[0] < 0 && fabs(lastError - Findline.err[0]) > 40)
        {
            Findline.err[0] = lastError;
        }

}

// 丢线停车
void EmergencyStop(void)
{
    //思路：丢线时近处的几行大部分都是黑色的，只要这部分的黑色像素累计大于一个阈值就认为丢线
    int16 i = 0, j = 0, count_black = 0 ;

    for(i = 118; i > 110; i--)
    {
        for(j = 130; j >= 30; j--)
        {
            if(ov7725_uart_image_dec[i][j] == 0)
            {
                count_black = count_black + 1;
            }
        }
    }
    if(count_black > 300&&stop_flag==0)  //300
    {
        Findline.loseflag = 1;
        stop();

        //bee_time = 200;
    }

}
