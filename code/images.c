
#include "zf_common_headfile.h"

uint8  mt9v03x_image_Band[MT9V03X_H][MT9V03X_W];


/*begin  大津法比赛   begin*/
//快速大津法二值化 pixelSum = width * height/4;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      快速大津
//  @return     uint8
//  @since      v1.1
//  Sample usage:   OTSU_Threshold = otsuThreshold(mt9v03x_image_dvp[0]);//大津法阈值
//-------------------------------------------------------------------------------------------------------------------
uint8 otsuThreshold_fast(uint8 *image)   //注意计算阈值的一定要是原图像
{
#define GrayScale 256
    int Pixel_Max=0;
    int Pixel_Min=255;
    uint16 width = MT9V03X_W;   //宽100
    uint16 height = MT9V03X_H;  //高80
    int pixelCount[GrayScale];  //各像素GrayScale的个数pixelCount 一维数组
    float pixelPro[GrayScale];  //各像素GrayScale所占百分比pixelPro 一维数组
    int i, j, pixelSum = width * height/4;  //pixelSum是获取总的图像像素个数的1/4，相应下面轮询时高和宽都是以2为单位自增
    uint8 threshold = 0;
//    uint8 last_threshold = 0;
    uint8* data = image;  //指向像素数据的指针

    //清零
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint32 gray_sum=0;  //每次执行到这会将gray_sum清零
    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i+=2)   //高
    {
        for (j = 0; j < width; j+=2)    //宽
        {
            pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
            gray_sum+=(int)data[i * width + j];       //灰度值总和
            if(data[i * width + j]>Pixel_Max)   Pixel_Max=data[i * width + j];
            if(data[i * width + j]<Pixel_Min)   Pixel_Min=data[i * width + j];
        }
    }

    //计算每个像素值的点在整幅图像中的比例
    for (i = Pixel_Min; i < Pixel_Max; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }

    //遍历灰度级[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;

    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = Pixel_Min; j < Pixel_Max; j++)
    {

        w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
        u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //背景平均灰度
        u1 = u1tmp / w1;              //前景平均灰度
        u = u0tmp + u1tmp;            //全局平均灰度
        deltaTmp = (float)(w0 *w1* (u0 - u1)* (u0 - u1)) ;
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = (uint8)j;
        }
        if (deltaTmp < deltaMax)
        {
            break;
        }

    }

    return threshold;
}
/*end  大津法比赛   end*/

/*begin  大津法学习   begin*/
//------------------摄像头参数--------------//
uint8 image_threshold = 46;  //图像阈值 0~255
uint8 dis_image[60][80];

uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height)
{
    #define GrayScale 256
    int pixelCount[GrayScale] = {0};//每个灰度值所占像素个数
    float pixelPro[GrayScale] = {0};//每个灰度值所占总像素比例
    int i,j;
    int Sumpix = width * height;   //总像素点
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针


    //统计灰度级中每个像素在整幅图像中的个数
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
          //   pixelCount[(int)image[i][j]]++;    若不用指针用这个
        }
    }
    float u = 0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / Sumpix;   //计算每个像素在整幅图像中的比例
        u += i * pixelPro[i];  //总平均灰度
    }


    float maxVariance=0.0;  //最大类间方差
    float w0 = 0, avgValue  = 0;  //w0 前景比例 ，avgValue 前景平均灰度
    for(i = 0; i < 256; i++)     //每一次循环都是一次完整类间方差计算 (两个for叠加为1个)
    {
        w0 += pixelPro[i];  //假设当前灰度i为阈值, 0~i 灰度像素所占整幅图像的比例即前景比例
        avgValue  += i * pixelPro[i];

        float variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //类间方差
        if(variance > maxVariance)
        {
            maxVariance = variance;
            threshold = (uint8)i;
        }
    }
    return threshold;
}

/*end  大津法学习   end*/

//图像二值化
//0 - 255
//黑 - 白
void Set_image_towvalues(uint8 value)
{
    uint8 temp_valude;//暂存灰度值
    for(uint8 i = 0;i < MT9V03X_H;i++)//高
    {
      for(uint8 j = 0;j < MT9V03X_W;j++)//宽
      {
          temp_valude = mt9v03x_image[i][j];
          if(temp_valude < value)
          {
              mt9v03x_image_Band[i][j] = 0;//黑
          }
          else
          {
              mt9v03x_image_Band[i][j] = 255;//白
          }
      }

    }
}

uint8 left_line_list[MT9V03X_H];    //高
uint8 right_line_list[MT9V03X_H];   //高
uint8 mid_line_list[MT9V03X_H];     //高
void Find_Mid_Line(uint8 index[MT9V03X_H][MT9V03X_W])   //index指数
{
    //定义左边点与右边点，并赋初值（当下面for循环没有左边点与右边点时用初值）
    uint8 left_point = 1;
    uint8 right_point = MT9V03X_W - 1;  //宽

    left_line_list[0] = 1;
    right_line_list[0] = MT9V03X_W - 1;
    mid_line_list[0] = (left_point + right_point) / 2;

    //从图片底部往上找
    for(uint8 i = MT9V03X_H - 1;i > 0;i--)  //高——行
    {
/**************以下代码是遍历一整个横行找两个黑白跳变点***************/
        //  如果二分之一点是白点，从二分之一点开始找
        if(index[i][MT9V03X_W/2] == 255)    //255 - 白
        {
            //寻找左边点
            for(uint8 j = MT9V03X_W/2 - 1;j > 0;j--)
            {
                if(index[i][j] == 0 && index[i][j+1] == 255)    //255 - 白
                {
                    left_point = j;
                    break;
                }
            }

            //寻找右边点
            for(uint8 j = MT9V03X_W/2 + 1;j < MT9V03X_W;j++)
            {
                if(index[i][j] == 0 && index[i][j-1] == 255)    //255 - 白
                {
                    right_point = j;
                    break;
                }
            }
        }

        //  如果四分之一点是白点，从四分之一点开始找
        else if(index[i][MT9V03X_W/4] == 255)
        {
            //寻找左边点
            for(uint8 j = MT9V03X_W/4 - 1;j > 0;j--)
            {
                if(index[i][j] == 0 && index[i][j+1] == 255)    //255 - 白
                {
                    left_point = j;
                    break;
                }
            }

            //寻找右边点
            for(uint8 j = MT9V03X_W/4 + 1;j < MT9V03X_W;j++)
            {
                if(index[i][j] == 0 && index[i][j-1] == 255)    //255 - 白
                {
                    right_point = j;
                    break;
                }
            }
        }

        //  如果四分之三点是白点，从四分之三开始找
        else if(index[i][MT9V03X_W/4*3] == 255)
        {
            //寻找左边点
            for(uint8 j = MT9V03X_W/4*3 - 1;j > 0;j--)
            {
                if(index[i][j] == 0 && index[i][j+1] == 255)    //255 - 白
                {
                    left_point = j;
                    break;
                }
            }

            //寻找右边点
            for(uint8 j = MT9V03X_W/4*3 + 1;j < MT9V03X_W;j++)
            {
                if(index[i][j] == 0 && index[i][j-1] == 255)    //255 - 白
                {
                    right_point = j;
                    break;
                }
            }
        }
        else    //1/2点，1/4点，3/4点都是黑色时
        {
            left_point = 1;
            right_point = MT9V03X_W - 1;
        }

        left_line_list[i] = left_point;
        right_line_list[i] = right_point;
        mid_line_list[i] = (left_point + right_point)/2;
    }
}

//绘制边线函数（画线即画很多个点）
void Find_Mid_Line_Draw_Line(void)
{
    for(uint8 i = MT9V03X_H - 1; i > 0;i--)
    {
        //全画
        draw_points(left_line_list[i],i,059);
        draw_points(right_line_list[i],i,059);
        draw_points(mid_line_list[i],i,059);
    }
}

//九宫格画点函数
//在九宫格画线的同时要注意数组不能越界，即判断是否为边界点，若有些点是边界点就不能画点
void draw_points(uint8 x,uint8 y,uint16 color)
{
    if(x != MT9V03X_W - 1)
    {
        tft180_draw_point(x+1,y,color);
        if(y != MT9V03X_H - 1)
        {
            tft180_draw_point(x,y+1,color);
            tft180_draw_point(x+1,y+1,color);
        }
        if(y != 0)
        {
            tft180_draw_point(x,y-1,color);
            tft180_draw_point(x+1,y-1,color);
        }
    }
    if(x != 0)
    {
        tft180_draw_point(x-1,y,color);
        if(y != MT9V03X_H - 1)
        {
            tft180_draw_point(x,y+1,color);
            tft180_draw_point(x-1,y+1,color);
        }
        if(y != 0)
        {
            tft180_draw_point(x,y-1,color);
            tft180_draw_point(x-1,y-1,color);
        }
    }
    tft180_draw_point(x,y,color);
}



