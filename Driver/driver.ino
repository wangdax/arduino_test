//四路电机的测速模块使用的外部中断引脚是pin2、3、A0、8(left1,right1,left2,right2)
//2、3为外部中断int0、int1，
//A0对应PCINT8 (PCMSK1 / PCIF1 / PCIE1)
//8对应PCINT0 (PCMSK0 / PCIF0 / PCIE0)

#include"speed.h"

SPEED speed;
void Init_Interrupt()
{
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
  if(Serial.available()>0)
  {
    char cmd = Serial.read();  
    Serial.print(cmd);
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
