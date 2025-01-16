
#include "zf_common_headfile.h"

uint8  mt9v03x_image_Band[MT9V03X_H][MT9V03X_W];


/*begin  ��򷨱���   begin*/
//���ٴ�򷨶�ֵ�� pixelSum = width * height/4;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ٴ��
//  @return     uint8
//  @since      v1.1
//  Sample usage:   OTSU_Threshold = otsuThreshold(mt9v03x_image_dvp[0]);//�����ֵ
//-------------------------------------------------------------------------------------------------------------------
uint8 otsuThreshold_fast(uint8 *image)   //ע�������ֵ��һ��Ҫ��ԭͼ��
{
#define GrayScale 256
    int Pixel_Max=0;
    int Pixel_Min=255;
    uint16 width = MT9V03X_W;   //��100
    uint16 height = MT9V03X_H;  //��80
    int pixelCount[GrayScale];  //������GrayScale�ĸ���pixelCount һά����
    float pixelPro[GrayScale];  //������GrayScale��ռ�ٷֱ�pixelPro һά����
    int i, j, pixelSum = width * height/4;  //pixelSum�ǻ�ȡ�ܵ�ͼ�����ظ�����1/4����Ӧ������ѯʱ�ߺͿ�����2Ϊ��λ����
    uint8 threshold = 0;
//    uint8 last_threshold = 0;
    uint8* data = image;  //ָ���������ݵ�ָ��

    //����
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    uint32 gray_sum=0;  //ÿ��ִ�е���Ὣgray_sum����
    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = 0; i < height; i+=2)   //��
    {
        for (j = 0; j < width; j+=2)    //��
        {
            pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
            gray_sum+=(int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
            if(data[i * width + j]>Pixel_Max)   Pixel_Max=data[i * width + j];
            if(data[i * width + j]<Pixel_Min)   Pixel_Min=data[i * width + j];
        }
    }

    //����ÿ������ֵ�ĵ�������ͼ���еı���
    for (i = Pixel_Min; i < Pixel_Max; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
    }

    //�����Ҷȼ�[0,255]
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;

    w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
    for (j = Pixel_Min; j < Pixel_Max; j++)
    {

        w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
        u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ

        w1=1-w0;
        u1tmp=gray_sum/pixelSum-u0tmp;

        u0 = u0tmp / w0;              //����ƽ���Ҷ�
        u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
        u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
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
/*end  ��򷨱���   end*/

/*begin  ���ѧϰ   begin*/
//------------------����ͷ����--------------//
uint8 image_threshold = 46;  //ͼ����ֵ 0~255
uint8 dis_image[60][80];

uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height)
{
    #define GrayScale 256
    int pixelCount[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ���ظ���
    float pixelPro[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ�����ر���
    int i,j;
    int Sumpix = width * height;   //�����ص�
    uint8 threshold = 0;
    uint8* data = image;  //ָ���������ݵ�ָ��


    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //������ֵ��Ϊ����������±�
          //   pixelCount[(int)image[i][j]]++;    ������ָ�������
        }
    }
    float u = 0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / Sumpix;   //����ÿ������������ͼ���еı���
        u += i * pixelPro[i];  //��ƽ���Ҷ�
    }


    float maxVariance=0.0;  //�����䷽��
    float w0 = 0, avgValue  = 0;  //w0 ǰ������ ��avgValue ǰ��ƽ���Ҷ�
    for(i = 0; i < 256; i++)     //ÿһ��ѭ������һ��������䷽����� (����for����Ϊ1��)
    {
        w0 += pixelPro[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷ�������ռ����ͼ��ı�����ǰ������
        avgValue  += i * pixelPro[i];

        float variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //��䷽��
        if(variance > maxVariance)
        {
            maxVariance = variance;
            threshold = (uint8)i;
        }
    }
    return threshold;
}

/*end  ���ѧϰ   end*/

//ͼ���ֵ��
//0 - 255
//�� - ��
void Set_image_towvalues(uint8 value)
{
    uint8 temp_valude;//�ݴ�Ҷ�ֵ
    for(uint8 i = 0;i < MT9V03X_H;i++)//��
    {
      for(uint8 j = 0;j < MT9V03X_W;j++)//��
      {
          temp_valude = mt9v03x_image[i][j];
          if(temp_valude < value)
          {
              mt9v03x_image_Band[i][j] = 0;//��
          }
          else
          {
              mt9v03x_image_Band[i][j] = 255;//��
          }
      }

    }
}

uint8 left_line_list[MT9V03X_H];    //��
uint8 right_line_list[MT9V03X_H];   //��
uint8 mid_line_list[MT9V03X_H];     //��
void Find_Mid_Line(uint8 index[MT9V03X_H][MT9V03X_W])   //indexָ��
{
    //������ߵ����ұߵ㣬������ֵ��������forѭ��û����ߵ����ұߵ�ʱ�ó�ֵ��
    uint8 left_point = 1;
    uint8 right_point = MT9V03X_W - 1;  //��

    left_line_list[0] = 1;
    right_line_list[0] = MT9V03X_W - 1;
    mid_line_list[0] = (left_point + right_point) / 2;

    //��ͼƬ�ײ�������
    for(uint8 i = MT9V03X_H - 1;i > 0;i--)  //�ߡ�����
    {
/**************���´����Ǳ���һ���������������ڰ������***************/
        //  �������֮һ���ǰ׵㣬�Ӷ���֮һ�㿪ʼ��
        if(index[i][MT9V03X_W/2] == 255)    //255 - ��
        {
            //Ѱ����ߵ�
            for(uint8 j = MT9V03X_W/2 - 1;j > 0;j--)
            {
                if(index[i][j] == 0 && index[i][j+1] == 255)    //255 - ��
                {
                    left_point = j;
                    break;
                }
            }

            //Ѱ���ұߵ�
            for(uint8 j = MT9V03X_W/2 + 1;j < MT9V03X_W;j++)
            {
                if(index[i][j] == 0 && index[i][j-1] == 255)    //255 - ��
                {
                    right_point = j;
                    break;
                }
            }
        }

        //  ����ķ�֮һ���ǰ׵㣬���ķ�֮һ�㿪ʼ��
        else if(index[i][MT9V03X_W/4] == 255)
        {
            //Ѱ����ߵ�
            for(uint8 j = MT9V03X_W/4 - 1;j > 0;j--)
            {
                if(index[i][j] == 0 && index[i][j+1] == 255)    //255 - ��
                {
                    left_point = j;
                    break;
                }
            }

            //Ѱ���ұߵ�
            for(uint8 j = MT9V03X_W/4 + 1;j < MT9V03X_W;j++)
            {
                if(index[i][j] == 0 && index[i][j-1] == 255)    //255 - ��
                {
                    right_point = j;
                    break;
                }
            }
        }

        //  ����ķ�֮�����ǰ׵㣬���ķ�֮����ʼ��
        else if(index[i][MT9V03X_W/4*3] == 255)
        {
            //Ѱ����ߵ�
            for(uint8 j = MT9V03X_W/4*3 - 1;j > 0;j--)
            {
                if(index[i][j] == 0 && index[i][j+1] == 255)    //255 - ��
                {
                    left_point = j;
                    break;
                }
            }

            //Ѱ���ұߵ�
            for(uint8 j = MT9V03X_W/4*3 + 1;j < MT9V03X_W;j++)
            {
                if(index[i][j] == 0 && index[i][j-1] == 255)    //255 - ��
                {
                    right_point = j;
                    break;
                }
            }
        }
        else    //1/2�㣬1/4�㣬3/4�㶼�Ǻ�ɫʱ
        {
            left_point = 1;
            right_point = MT9V03X_W - 1;
        }

        left_line_list[i] = left_point;
        right_line_list[i] = right_point;
        mid_line_list[i] = (left_point + right_point)/2;
    }
}

//���Ʊ��ߺ��������߼����ܶ���㣩
void Find_Mid_Line_Draw_Line(void)
{
    for(uint8 i = MT9V03X_H - 1; i > 0;i--)
    {
        //ȫ��
        draw_points(left_line_list[i],i,059);
        draw_points(right_line_list[i],i,059);
        draw_points(mid_line_list[i],i,059);
    }
}

//�Ź��񻭵㺯��
//�ھŹ����ߵ�ͬʱҪע�����鲻��Խ�磬���ж��Ƿ�Ϊ�߽�㣬����Щ���Ǳ߽��Ͳ��ܻ���
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



