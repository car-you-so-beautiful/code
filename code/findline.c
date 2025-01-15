//-------------------------------------------------------------------------------------------------------------------
//  ���:������ͼ����
//  �ĵ����ϵĴ��룬ԽдԽʺɽ�ˡ�����
//------------------------------------------------------------------------------------------------------------------
#include "findline.h"
#include "control.h"
uint8 lose_line;
int16 qianzhan =60;
findline_TypeDef Findline = {{0}};
fp_TypeDef Findp= {{0}};
uint8 stop_flag=0;//���߱�־λ
float lastError;//ƫ��
void findline(void)  //����ͼ��Ѱ������
{

    int16 i = 0, j = 0;//i���� j����
    //������ʼ��
    lose_line = 0 ;
    Findline.midline[119] = 80;
    Findline.leftline[119] = 0;
    Findline.rightline[119] = 159;


    Findline.leftstartpoint = 0;
    Findline.rightstartpoint = 0;

    Findline.l_lose=0;
    Findline.r_lose=0;
    /*��ȡ�����߽�*/
    //��ͨѲ�� ���׿�����Ⱥ��Ҫ������
    {
        for(i = 118; i > 0; i--)
        {
            Findline.midline[i] = 0;
            Findline.leftline[i] = 0;
            Findline.rightline[i] = 159;
            Findline.leftlineflag[i] = 0;
            Findline.rightlineflag[i] = 0;
            Findline.endline = 0;
            //Ѱ���������
            if(Findline.midline[i + 1] > 80)j = Findline.midline[i + 1] > 155 ? 155 : Findline.midline[i + 1];
            else j = Findline.midline[i + 1] < 4 ? 4 : Findline.midline[i + 1];  //���ҵ��м�λ�ã����м�����߿�ʼ��
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
            //Ѱ���������
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
            //���߶�û�ҵ�
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

            //�Ҷ���
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
    //����
}

void caculate_err(void)  //����ƫ��
{
    int16 i,sum;
    //static float sum;
    lastError = Findline.err[0];
    sum = 0;
    for(i = 59; i >= 1; i--)
        Findline.err[i] = Findline.err[i - 1]; //ƫ���˲�
    Findline.err[0] = 0.0;
    //if(p_right_flag != 2)
        if(qianzhan > Findline.endline + 4)//<58
        {
            for(i = qianzhan; i > Findline.endline && i > qianzhan-3; i--) // Ŀǰ��˵������Ǽ��߲�֪����ô����
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

// ����ͣ��
void EmergencyStop(void)
{
    //˼·������ʱ�����ļ��д󲿷ֶ��Ǻ�ɫ�ģ�ֻҪ�ⲿ�ֵĺ�ɫ�����ۼƴ���һ����ֵ����Ϊ����
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
