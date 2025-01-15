#include "control.h"
#include "gyroscope.h"
#include "findline.h"
#define ki_max  2.0
StandInfotypedef L_SpeedLoop;
StandInfotypedef R_SpeedLoop;
float L_SpeedControl;
float R_SpeedControl;
float L_SpeedOutput;
float R_SpeedOutput;
float L_setSpeed;
float R_setSpeed;
Speed_struct speed;
CarInfotypedef CarInfo;
float speed_diff=0.025;//后轮差速系数  需要自己调整
/***********************************************************************************************/
//串级pid 模糊pid等其他，控制方案可私聊 ——py


void stop(void)
{
    speed.Stan = 0;
    CarInfo.Mode = STAND;//STOPM;

}
void PID_init(void)
{
    //每辆车pid都需要自行调整
    speed.Stan = 1;

    L_SpeedLoop.KP = 0.5;
    L_SpeedLoop.KI = 0.1;
    L_SpeedLoop.KD = 0;

    R_SpeedLoop.KP = 0.5;//
    R_SpeedLoop.KI = 0.1;//
    R_SpeedLoop.KD = 0;
}

void L_Speed_Loop(void)
{
    L_setSpeed = speed.Stan/*(1-speed_diff*(Findline.err[0]))*/;

    L_SpeedLoop.ErrorFifo[2] = L_SpeedLoop.ErrorFifo[1];
    L_SpeedLoop.ErrorFifo[1] = L_SpeedLoop.ErrorFifo[0];
    L_SpeedLoop.ErrorFifo[0] = L_SpeedLoop.Error;

    L_SpeedLoop.ErrorDtFifo[2] = L_SpeedLoop.ErrorDtFifo[1];
    L_SpeedLoop.ErrorDtFifo[1] = L_SpeedLoop.ErrorDtFifo[0];
    L_SpeedLoop.ErrorDtFifo[0] = L_SpeedLoop.ErrorFifo[0] - L_SpeedLoop.ErrorFifo[2];

    L_SpeedLoop.Error = (L_setSpeed - L_CarSpeed+((float)(Findline.err[0])*0.014)) ;

    if(L_SpeedLoop.Error>=ErrorMaxLimit) L_SpeedLoop.Error = ErrorMaxLimit;
    if(L_SpeedLoop.Error<=ErrorMinLimit) L_SpeedLoop.Error = ErrorMinLimit;

    if(fabs(L_SpeedLoop.Error)<1)
        L_SpeedLoop.Integ += L_SpeedLoop.Error;

    if(L_SpeedLoop.Integ> ki_max) L_SpeedLoop.Integ = ki_max;
    if(L_SpeedLoop.Integ<-ki_max) L_SpeedLoop.Integ = -ki_max;

    L_SpeedLoop.OutPut = L_SpeedLoop.KP * L_SpeedLoop.Error + L_SpeedLoop.KI * L_SpeedLoop.Integ + L_SpeedLoop.KD * (L_SpeedLoop.ErrorDtFifo[0] * 0.6 + L_SpeedLoop.ErrorDtFifo[1] * 0.4) - speed_diff*(Findline.err[0]) ;
    L_SpeedLoop.OutPut =  L_SpeedLoop.OutPut * 5000;
    if(L_SpeedLoop.OutPut>=OutPutMaxLimit) L_SpeedLoop.OutPut = OutPutMaxLimit;
    if(L_SpeedLoop.OutPut<=OutPutMinLimit) L_SpeedLoop.OutPut = OutPutMinLimit;
    L_SpeedControl = ( 0.2 * L_SpeedLoop.OutPut + 0.8 * L_SpeedControl);
}

void R_Speed_Loop(void)
{
    R_setSpeed = speed.Stan/*(1+speed_diff*(Findline.err[0]))*/;

    R_SpeedLoop.ErrorFifo[2] = R_SpeedLoop.ErrorFifo[1];
    R_SpeedLoop.ErrorFifo[1] = R_SpeedLoop.ErrorFifo[0];
    R_SpeedLoop.ErrorFifo[0] = R_SpeedLoop.Error;

    R_SpeedLoop.ErrorDtFifo[2] = R_SpeedLoop.ErrorDtFifo[1];
    R_SpeedLoop.ErrorDtFifo[1] = R_SpeedLoop.ErrorDtFifo[0];
    R_SpeedLoop.ErrorDtFifo[0] = R_SpeedLoop.ErrorFifo[0] - R_SpeedLoop.ErrorFifo[2];

    R_SpeedLoop.Error = (R_setSpeed - R_CarSpeed+((float)(Findline.err[0])*0.014)) ;

    if(R_SpeedLoop.Error>=ErrorMaxLimit) R_SpeedLoop.Error = ErrorMaxLimit;
    if(R_SpeedLoop.Error<=ErrorMinLimit) R_SpeedLoop.Error = ErrorMinLimit;

    if(fabs(R_SpeedLoop.Error)<1)
        R_SpeedLoop.Integ += R_SpeedLoop.Error;

    if(R_SpeedLoop.Integ> ki_max) R_SpeedLoop.Integ = ki_max;
    if(R_SpeedLoop.Integ<-ki_max) R_SpeedLoop.Integ = -ki_max;

    R_SpeedLoop.OutPut = R_SpeedLoop.KP * R_SpeedLoop.Error + R_SpeedLoop.KI * R_SpeedLoop.Integ + R_SpeedLoop.KD * (R_SpeedLoop.ErrorDtFifo[0] * 0.6 + R_SpeedLoop.ErrorDtFifo[1] * 0.4) +speed_diff*(Findline.err[0]);

    R_SpeedLoop.OutPut =  R_SpeedLoop.OutPut * 5000;
    if(R_SpeedLoop.OutPut>=OutPutMaxLimit) R_SpeedLoop.OutPut = OutPutMaxLimit;
    if(R_SpeedLoop.OutPut<=OutPutMinLimit) R_SpeedLoop.OutPut = OutPutMinLimit;
    R_SpeedControl = ( 0.2 * R_SpeedLoop.OutPut + 0.8 * R_SpeedControl);

}




