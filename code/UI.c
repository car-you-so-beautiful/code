#include "zf_common_headfile.h"
#include "headfile_code.h"
#include "UI.h"
#include"isr.h"
void keyScan(void);
unsigned char ui_page0[10][17] =
{
    "   Showimage    ",
    "   Speed        ",
    "   encoder      ",
    "   POUT0        ",
    "   SERVPID     ",
    "   MotorPID     ",
    "   Angle        ",
    "                ",
    "                ",
    "   Votage:      "
};

unsigned char ui_page1[10][17] =
{
    " pageChange     ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    " page2          "
};

unsigned char ui_page2[10][17] =
{
    " pageChange     ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    " page3          "
};

unsigned char ui_page3[10][17] =
{
    " pageChange     ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    " page4          "
};

unsigned char ui_Sensor[10][17] =
{
    "   L :     ",
    "   SL:     ",
    "   M :     ",
    "   SR:     ",
    "   R :     ",
    "   LL:     ",
    "   RR:     ",
    "   HW:     ",
    "   err:    ",
    "   Sensor  "
};

unsigned char ui_Speed[10][17] =
{
    "   SPEED        ",
    "                ",
    "   SPEED_SET    ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                "
};

unsigned char ui_encoder[10][17] =
{
    "   LeftCnt:     ",
    "                ",
    "   RightCnt:    ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                "
};

unsigned char ui_OUTPD[10][17] =
{
    "   POUT0:       ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                "
};
unsigned char ui_INPD[10][17] =
{
    "   INP:        ",
    "                ",
    "   IND:        ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                "
};
unsigned char ui_MotorPID[10][17] =
{
    "   MotorP:      ",
    "                ",
    "   MotorI:      ",
    "                ",
    "   MotorD:      ",
    "                ",
    "   SPEED        ",
    "                ",
    "   SPEED_SET    ",
    "                "
};
unsigned char ui_angle[10][17] =
{
    "   Angle_Y:     ",
    "   Angle_Z:     ",
    "   SPEED_Z      ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                ",
    "                "
};

UI_CLASS ui =
{
    &UI_Disp,
    {0, 0, 0, 0}, 0, -1
};


static void UI_DispUIStrings(uint8 strings[10][17])
{
	int16 i;
	for (i = 0; i < 10; i++)
	{
		if (i == ui.cursor[ui.page])
		strings[i][2] = '>';
		else
		strings[i][2] = ' ';
		ips200_show_string(3, i*22, strings[i]);
	}
}
void num_add(float* num, float step)
{
*num += step;
}

void num_sub(float* num, float step)
{
*num -= step;
if(*num<0)
    *num = 0;
}

void show_image(void)
{
    ips200_show_string(190,0,"FPS");
    ips200_show_uint(190, 20, (fps_cnt[0]), 2);
    ips200_show_gray_image(5, 5, ov7725_uart_image_dec[0], OV7725_W, OV7725_H, OV7725_W, OV7725_H, 0);
   for(int i = 119+5; i >= 5 && i > Findline.endline+5; i--)
   {
      ips200_draw_point(Findline.midline[i-5] + 5, i, RGB565_RED);
      ips200_draw_point(Findline.rightline[i-5] + 5, i, RGB565_RED);
      ips200_draw_point(Findline.leftline[i-5] + 5, i, RGB565_RED);
     }

   ips200_draw_line(0, qianzhan+5, 20, qianzhan+5, RGB565_GREEN);
   ips200_show_string(190,40,"V");
   ips200_show_float(190, 60, (float)(3.3*4*adc_mean_filter_convert(ADC0_CH8_A8, 5)/256), 2, 2);
   ips200_show_uint(190, 120, dl1a_distance_mm, 4);
   ips200_show_string(190, 100, "mm");
}
void UI_Disp(void)
{
  

	switch(ui.page)
	{
		case 0:
			if(ui.enter == -1)
			{
				UI_DispUIStrings(ui_page0);
				ips200_show_float(120, 198, (float)(3.3*4*adc_mean_filter_convert(ADC0_CH8_A8, 5)/256), 2, 2);
				ips200_show_string(190,0,"FPS");
				ips200_show_uint(190, 20, (fps_cnt[0]), 2);
			}
			else
			{
				if(ui.enter == 0)
				{
				    show_image();
				}
				else if(ui.enter == 1)
				{
					UI_DispUIStrings(ui_Speed);
					ips200_show_float(130, 0, speed_avr, 2, 2);
                    ips200_show_float(130, 44, speed.Stan, 2, 2);

				}
				else if(ui.enter == 2)
				{
					UI_DispUIStrings(ui_encoder);
//                    ips200_show_int(130, 0, Right_count, 4);
//                    ips200_show_int(130, 44, Left_count, 4);
					
				}
				else if(ui.enter == 3)
				{
					UI_DispUIStrings(ui_OUTPD);
//                    ips200_show_int(130, 0, pout0, 4);
                    //ips200_show_float(130, 44, DirOutter.BaseKD, 2, 2);
                                      

				}
				else if(ui.enter == 4)
				{
					UI_DispUIStrings(ui_INPD);
//                    ips200_show_float(130, 0,  ServPID.P, 2, 2);
//                    ips200_show_float(130, 44, ServPID.D, 2, 2);
					
				}
				else if(ui.enter == 5)
				{
					UI_DispUIStrings(ui_MotorPID);
//                    ips200_show_float(130, 0,  SpeedLoop.BaseKP, 2, 2);
//                    ips200_show_float(130, 44, SpeedLoop.BaseKI, 2, 2);
//                    ips200_show_float(130, 88, SpeedLoop.BaseKD, 2, 2);
                    ips200_show_float(130, 132, speed_avr, 2, 2);
                    ips200_show_float(130, 176, speed.Stan, 2, 2);
					
				}
				else if(ui.enter ==6)
				{
				    UI_DispUIStrings(ui_angle);
                    ips200_show_float(130, 0,  Gravity_Angle, 2, 2);
                    ips200_show_float(130, 22,  gyro.PitchAngle_Integral, 2, 2);
                    ips200_show_float(130, 44, CarAngle.Yawrate, 2, 2);
					//UI_DispUIStrings(ui_FLAG);
					
				}
				else if(ui.enter ==7)
				{

					//UI_DispUIStrings(ui_FLAG);
					
				}
			}
			break;
			

	}
}

void keyScan(void)
{
    static unsigned char temp_page;
    if(key_get_state(KEY_2) == KEY_LONG_PRESS && ui.enter == -1) //key1_flag==0)
    {
        ui.enter = ui.cursor[ui.page];
        ips200_clear();
        temp_page = ui.cursor[ui.page];
        ui.cursor[ui.page] = 0;
        bee_time=50;
    }

    if(key_get_state(KEY_2) == KEY_SHORT_PRESS && ui.enter != -1)
    {
        ui.enter = -1;
        ips200_clear();
        ui.cursor[ui.page] = temp_page;
        temp_page = 0;
        bee_time=50;
    }

    if(key_get_state(KEY_1) == KEY_SHORT_PRESS)
    {
        ui.cursor[ui.page]++;
        bee_time=50;
       // ips200_clear();
        if (ui.cursor[ui.page] > 4 && ui.enter != -1)
        {
            ui.cursor[ui.page] = 0;
        }
        else if(ui.cursor[ui.page] > 6 && ui.enter == -1)
        {
            ui.cursor[ui.page] = 0;
        }

    }
    if(key_get_state(KEY_3) == KEY_SHORT_PRESS) //key3_flag==0&&switch1_state==1)
     {
        bee_time=50;
        if(ui.enter == 1 && ui.cursor[ui.page] == 2)
        {
            num_add(&speed.Stan, 0.1);
        }
//        if(ui.enter == 3 && ui.cursor[ui.page] == 0)
//        {
//            num_add(&pout0, 10);
//        }
//        if(ui.enter == 3 && ui.cursor[ui.page] == 2)
//        {
//            //num_add(&DirOutter.BaseKD, 0.1);
//        }
//        if(ui.enter == 4 && ui.cursor[ui.page] == 0)
//        {
//            num_add(&ServPID.P, 0.1);
//        }
//        if(ui.enter == 4 && ui.cursor[ui.page] == 2)
//        {
//            num_add(&ServPID.D, 0.1);
//        }
//
//        if(ui.enter == 5 && ui.cursor[ui.page] == 0)
//        {
//            num_add(&SpeedLoop.BaseKP, 0.1);
//        }
//        if(ui.enter == 5 && ui.cursor[ui.page] == 2)
//        {
//            num_add(&SpeedLoop.BaseKI, 0.1);
//        }
//        if(ui.enter == 5 && ui.cursor[ui.page] == 4)
//        {
//            num_add(&SpeedLoop.BaseKD, 0.1);
//        }



     }

    if(key_get_state(KEY_4) == KEY_SHORT_PRESS)
     {
        if(ui.enter == 1 && ui.cursor[ui.page] == 2)
        {
            num_sub(&speed.Stan, 0.1);
        }
//        if(ui.enter == 3 && ui.cursor[ui.page] == 0)
//        {
//            num_sub(&pout0, 10);
//        }
//        if(ui.enter == 3 && ui.cursor[ui.page] == 2)
//        {
//            //num_sub(&DirOutter.BaseKD, 0.1);
//        }
//        if(ui.enter == 4 && ui.cursor[ui.page] == 0)
//        {
//            num_sub(&ServPID.P, 0.1);
//        }
//        if(ui.enter == 4 && ui.cursor[ui.page] == 2)
//        {
//            num_sub(&ServPID.D, 0.1);
//        }
//
//        if(ui.enter == 5 && ui.cursor[ui.page] == 0)
//        {
//            num_sub(&SpeedLoop.BaseKP, 0.1);
//        }
//        if(ui.enter == 5 && ui.cursor[ui.page] == 2)
//        {
//            num_sub(&SpeedLoop.BaseKI, 0.1);
//        }
//        if(ui.enter == 5 && ui.cursor[ui.page] == 4)
//        {
//            num_add(&SpeedLoop.BaseKD, 0.1);
//        }


     }
}


