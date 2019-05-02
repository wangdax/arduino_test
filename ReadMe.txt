2019-5-2
1、使用arduino uno 来充当驱动板。
2、四轮电机、四路测速数据输入，使用的是外部中断，
      因 uno只有两路中断，故使用pci进行扩展。
      ISR (PCINT2_vect) 处理 Pin D0 to D7
      ISR (PCINT0_vect) 处理 Pin D8 to D13
      ISR (PCINT1_vect) 处理 Pin A0 to A5
3、还有超声波模块
4、四路电机驱动，使用的是两个L298N模块。