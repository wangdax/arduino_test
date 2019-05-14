//4-7 对应left1、right1的电机驱动板1
//pin4 ---IN1
//pin5 ---IN2
//pin6 ---IN3
//pin7 ---IN4

//10-13 对应left2、right2的电机驱动板2
//pin10 ---IN1
//pin11 ---IN2
//pin12 ---IN3
//pin13 ---IN4

//L298N 
// IN1     IN2    
// 0        1     正转
// 1        0     反转
// 0        0     制动
// 1        1     制动
//四路电机的测速模块使用的外部中断引脚是pin2、3、A0、8(left1,right1,left2,right2)
//2、3为外部中断int0、int1，
//A0对应PCINT8 (PCMSK1 / PCIF1 / PCIE1)
//8对应PCINT0 (PCMSK0 / PCIF0 / PCIE0)

#include"speed.h"

const int TrigPin = A5; //发出超声波
const int EchoPin = 9; //收到反射回来的超声波
SPEED speed;
void Init_Interrupt()
{
  pinMode(TrigPin, OUTPUT); 
  pinMode(EchoPin, INPUT); 
   //外部中断 pin change interrupt 
  pinMode(0,  INPUT_PULLUP);
  pinMode(8,  INPUT_PULLUP);
   
  PCMSK0 |= bit(PCINT0); // want pin 8   right2
  PCIFR |= bit(PCIF0); // clear any outstanding interrupts
  PCICR |= bit(PCIE0); // enable pin change interrupts for D8 to D13

  PCMSK1 |= bit(PCINT8); // want pin A0  left2
  PCIFR |= bit(PCIF1); // clear any outstanding interrupts
  PCICR |= bit(PCIE1); // enable pin change interrupts for A0 to A5  

  attachInterrupt(0,Right1Count_CallBack,FALLING);//设置外部中断函数0  left1
  attachInterrupt(1,Left1Count_CallBack,FALLING);//设置外部中断函数1   right1
  }
void  Ultrasonic()
{
   digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin  
   delayMicroseconds(2);       // delayMicroseconds在更小的时间内延时准确 
   digitalWrite(TrigPin, HIGH);  
   delayMicroseconds(10);  
   digitalWrite(TrigPin, LOW); //通过这里控制超声波的发射    
   float cm = pulseIn(EchoPin, HIGH)/58.0; //将回波时间换算成cm  
   Serial.println(cm); 
   delay(1000); 
  }
void setup() 
{
  Serial.begin(115200);//串口初始化
  speed.Init_Pin();//管脚初始化
  Init_Interrupt();//中断初始化

}
////////主循环////////////////////
void loop() {
  if(speed.SpeedDetection())//到1s后关闭了中断，需要在此处重新打开
  {
     Init_Interrupt();
    
   }
  Ultrasonic();
  if(Serial.available()>0)
  {
    int cmd = Serial.read();  
    //Serial.print(cmd);
    speed.motorRun(cmd);      
  }  
}

///////服务中断函数////////////////////////
void Right1Count_CallBack()//外部中断服务函数
{
   speed.Right1Count_CallBack();    
}

void Left1Count_CallBack()//外部中断服务函数
{
  speed.Left1Count_CallBack();
}

ISR (PCINT0_vect)//pci服务函数  pin8
{
// handle pin change interrupt for pin D8 to D13 here
   speed.Right2Count_CallBack();  
}  
 
ISR (PCINT1_vect)//pci服务函数  pinA0
{
// handle pin change interrupt for pin A0 to A5 here
  speed.Left2Count_CallBack();
}  
