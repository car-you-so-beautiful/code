#ifndef __CONTROL_H
#define __CONTROL_H
#include "zf_common_headfile.h"
#define OutPutMinLimit -4500
#define OutPutMaxLimit 4500
#define ErrorMaxLimit 2
#define ErrorMinLimit -2
typedef enum
{
    STOPM = 1,
    MOTORTEST = 2,
    STAND = 3,
    CLOSELOOP = 4,
}CarMode;

typedef struct
{
    CarMode Mode;
    uint8 Condition;
}CarInfotypedef;

extern CarInfotypedef CarInfo;
extern uint8 stop_mode ;
extern float pout;
extern float pwmout;


typedef struct{
  float Error;
  float Stan;
  float L_ControlIntegral;
  float R_ControlIntegral;
}Speed_struct;

typedef struct
{
    float Expect;
    float KP;
    float KI;
    float KD;
    float Error;
    float ErrorFifo[5];
    float ErrorLast;
    float ErrorDtFifo[5];
    float ErrorTemp[4];
    float ErrorDtTemp[4];
    float InOut;
    float Integ;
    float OutPut;
}StandInfotypedef;


extern Speed_struct speed;

typedef struct
{
  void (*SpeedInit)();
  void (*MotorControl)(float* err);
  

} CONTROL_CLASS;
extern CONTROL_CLASS control;

typedef struct {
    float P;
    float I;
    float D;
} PID_CLASS;
extern StandInfotypedef L_SpeedLoop;
extern StandInfotypedef R_SpeedLoop;

void L_Speed_Loop(void);
void R_Speed_Loop(void);

extern float L_setSpeed;
extern float R_setSpeed;
extern float L_SpeedControl;
extern float R_SpeedControl;
extern float L_SpeedOutput;
extern float R_SpeedOutput;
void PID_init(void);
void stop(void);
extern float pout,dout;
extern float lastError;
extern float speed_diff;
#endif
