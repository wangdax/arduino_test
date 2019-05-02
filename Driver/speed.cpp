#include"speed.h"
 
#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4

void SPEED::Init_Pin()
{
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT); 
}


int SPEED::SpeedDetection()
{
  now_time = millis();//以毫秒为单位，计算当前时间 
  if(abs(now_time - old_time) >= 1000) // 如果计时时间已达1秒
  {  
    detachInterrupt(0); // 关闭外部中断0
    detachInterrupt(1); // 关闭外部中断1
    PCMSK0 &= ~(1 << PCINT0);//关闭PCI0
    PCMSK1 &= ~(1 << PCINT8);//关闭PCI8

    //把每一秒钟编码器码盘计得的脉冲数，换算为当前转速值
    //转速单位是每分钟多少转，即r/min。这个编码器码盘为20个空洞。
    left1_speed =(float)left1Counter*60/20;//小车left1车轮电机转速
    right1_speed =(float)right1Counter*60/20;//小车right1车轮电机转速
    left2_speed =(float)left2Counter*60/20;//小车left2车轮电机转速
    right2_speed =(float)right2Counter*60/20;//小车right2车轮电机转速
    
    
    Serial.println(left1_speed);//向上位机上传电机当前转速的高、低字节
    Serial.println(right1_speed);
    Serial.println(left2_speed);
    Serial.println(right2_speed);
    
    //恢复到编码器测速的初始状态
    left1Counter = 0;   //把脉冲计数值清零，以便计算下一秒的脉冲计数
    right1Counter = 0;
    left2Counter = 0;   
    right2Counter = 0;
    
    old_time=  millis();     // 记录每秒测速时的时间节点   
    return 1;
  }
  else
    return 0;
  
  
  }

void SPEED::motorRun(int cmd)
{
  switch(cmd){
    case FORWARD:
     // Serial.println("FORWARD"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
     case BACKWARD:
      //Serial.println("BACKWARD"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
     case TURNLEFT:
      //Serial.println("TURN  LEFT"); //输出状态
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
     case TURNRIGHT:
     // Serial.println("TURN  RIGHT"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
     default:
      //Serial.println("STOP"); //输出状态
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
  }
}
 

void SPEED::Right1Count_CallBack()//外部中断服务函数
{
  right1Counter++;    
}

void SPEED::Left1Count_CallBack()//外部中断服务函数
{
  left1Counter++;    
}

void SPEED::Right2Count_CallBack()//外部中断服务函数
{
  right2Counter++;    
}

void SPEED::Left2Count_CallBack()//外部中断服务函数
{
  left2Counter++;    
}
