#ifndef _SPEED_H__
#define _SPEED_H__

//导入Arduino核心头文件
#include"Arduino.h"  
//#include"PinChangeInterrupt.h"

class SPEED
{
     private:
          int left1Counter=0,  right1Counter=0,left2Counter=0,  right2Counter=0;//四路电机码盘转数
          unsigned long now_time = 0, old_time = 0; // 时间标记
          float left1_speed,left2_speed,right1_speed,right2_speed;//四个轮子的速度
          //四路电机对应的管脚
         int input_left1_1 = 4; // 定义uno的pin 5 向 input1 输出 
         int input_left1_2 = 5; 
         int input_right1_1 = 6; 
         int input_right1_2 = 7;

         int input_left2_1 = 10; // 定义uno的pin 5 向 input1 输出 
         int input_left2_2 = 11; 
         int input_right2_1 = 12; 
         int input_right2_2 = 13;

     
     
     public:      
          void Init_Pin();//中断初始化
          int SpeedDetection();//测速函数
          void Right1Count_CallBack();
          void Left1Count_CallBack();
          void Right2Count_CallBack();
          void Left2Count_CallBack();
          void motorRun(int cmd);

};

#endif
