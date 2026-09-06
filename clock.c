#include<reg52.h>

#define uint unsigned int
#define uchar unsigned char

//按键定义
sbit key_second = P1^5;
sbit key_minute = P1^6;
sbit key_hour = P1^7;

//0~9的十六进制数码
uchar code seg1[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};//正
uchar code seg2[]={0xc0,0xcf,0xa4,0x86,0x8b,0x92,0x90,0xc7,0x80,0x82};//反
uchar code seg3[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};//正+dp
uchar code seg4[]={0x40,0x4f,0x24,0x06,0x0b,0x12,0x10,0x47,0x00,0x02};//反+dp

//秒、分、时标志
uchar second = 0, minute = 0, hour = 0;  

//秒、分、时高位低位
uchar second_L, second_H, minute_L, minute_H, hour_L, hour_H; 

//计数
uint counter = 0; 

//延时函数
void delay(uint x)
{  
	while(x--);
}

//按键延时函数 x ms
void key_delay(int xms) 
{  
	unsigned int i, j;
	for(i=0; i<xms; ++i)
	for(j=0; j<110; ++j);
}

//定时器初始化
void T0_Init()  
{  
	AUXR |= 0x80;	 //定时器时钟1T模式
	TMOD &= 0xF0;	 //设置定时器模式
	TMOD |= 0x01;	 //设置定时器模式
	TL0 = 0x20;		 //设置定时初始值
	TH0 = 0xD1;		 //设置定时初始值
	TF0 = 0;		   //清除TF0标志
	TR0 = 1;		   //定时器0开始计时
	
	ET0 = 1;
	EA = 1;
	PT0 = 1;
}

//显示函数
void display() 
{  
	P0 = 0x20;
	P2 = seg1[second_L];  
	delay(50);

	P0 = 0x10;
	P2 = seg4[second_H]; 
	delay(50);

	P0 = 0x08;
	P2 = seg3[minute_L];  
	delay(50);

	P0 = 0x04;
	P2 = seg4[minute_H]; 
	delay(50);

	P0 = 0x02;	
	P2 = seg3[hour_L];  
	delay(50);
	
	P0 = 0x01;	
    P2 = seg2[hour_H] ;
	delay(50);

}

//按键函数
void keyscan() 
{  
	if(key_second == 0)
	{
		key_delay(20);
		if(key_second == 0)
		{
			second++;
			if(second == 60)
			{
				second = 0;
				minute++;
			}
			while(!key_second);
		}
	}
	
	if(key_minute == 0)
	{
		key_delay(20);
		if(key_minute == 0)
		{
			minute++;
			if(minute == 60)
			{
				minute = 0;
				hour++;
			}
			while(!key_minute);
		}
	}
	
	if(key_hour == 0)
	{
		key_delay(20);
		if(key_hour == 0)
		{
			hour++;
			if(hour == 24)
			hour = 0;
			while(!key_hour);
		}
	}
}

//主函数
void main()
{
	T0_Init(); 
	while(1)
	{
		display();
		keyscan();		
  }
}

//中断函数 
void timer0_Init() interrupt 1 
{  
	TH0 = (65536-50000)/256;
  TL0 = (65536-50000)%256;
	
	 counter++;
	 if(counter == 20)
   {
	   counter = 0;  
	 	 second++;	
		 if(second == 60)
		 {
		   second = 0;  
			 minute++;
			 if(minute == 60)
			 {
				 minute = 0;  
				 hour++;
				 if(hour == 24)
				 {
					 hour = 0;
					 minute = 0;
					 second = 0;
				 }
			 }
		 }
		second_L = second%10;  
		second_H = second/10; 

		minute_L = minute%10; 
		minute_H = minute/10;  

		hour_L = hour%10;  
		hour_H = hour/10;
	 }
}