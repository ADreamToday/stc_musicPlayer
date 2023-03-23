#include<STC32G.h>
#define u8 unsigned char
#define u16 unsigned int

const int code soundTrack1[] = {784 ,784 ,1046 ,1046 ,1046 ,988 ,1174 ,1046 ,784 ,659 ,880 ,880 ,880 ,880 ,698 ,698 ,0 ,880 ,1174 ,1174 ,1174 ,1046 ,988 ,880 ,784 ,698 ,659 ,659 ,659 ,659 ,659 ,659 ,784 ,784 ,1046 ,1046 ,1046 ,988 ,1174 ,1046 ,784 ,659 ,880 ,880 ,880 ,880 ,698 ,880 ,1174 ,1046 ,988 ,988 ,1174 ,1174 ,1396 ,1396 ,988 ,988 ,1046 ,1046 ,1046 ,1046 ,1046 ,0 ,1318 ,1174 ,988 ,988 ,988 ,988 ,880 ,988 ,1046 ,880 ,988 ,988 ,988 ,988 ,784 ,784 ,698 ,784 ,880 ,880 ,880 ,880 ,1174 ,1174 ,1174 ,1046 ,988 ,988 ,988 ,988 ,988 ,0 ,1174 ,1174 };


u16 rupt_ = 1;
u16 rupt_pre = 1;
	
void Timer0() interrupt 1
{
	rupt_  = rupt_ + 1;
}
	
void main()
{
	u16 len = 98;		//曲谱长度
	int speed = 480; // 120个音符代码每分钟
	
	int temp = 0;  //用于循环等工作的临时变量
	
	int time1 = 0;
	u16 local_time1 = 0;

	
	//20MHz = 20*10^6hz
	EAXFR = 1;
	WTST = 0x00;
	
	P0M0 = 0xff;
	P0M1 = 0x00;
	P1M0 = 0xff;
	P1M1 = 0x00;
	P2M0 = 0xff;
	P2M1 = 0x00;
	P3M0 = 0xff;
	P3M1 = 0x00;
	P4M0 = 0xff;
	P4M1 = 0x00;
	P5M0 = 0xff;
	P5M1 = 0x00;
	P6M0 = 0xff;
	P6M1 = 0x00;
	P7M0 = 0xff;
	P7M1 = 0x00;
	
	TMOD = 0x00;
	
	TL0 = 0xeb;	//16位自动重载 10微妙终断一次
	TH0 = 0xff;		
	TR0 = 1;				//start timer0
	ET0 = 0;				//先关闭中断
	EA = 1;
	
	
	
	//=========================
	P10 = 1;
	//主循环
	ET0 = 1;  //允许中断
	while(1)
	{
		// 播放
		for(temp = 0;temp<len;temp++)
		{
			local_time1 = (int)(200000.0 / (double)soundTrack1[temp]) ; 	//一半的周期 前一半是1 后一半是0 ,  这个代表 中断 times次后，蜂鸣器的输出端口的值应该改变( 有数学简化运算，所以式子看起来会有点怪)
			
			
			time1 = local_time1; //times 作为自减用的变量
			
			rupt_ = 1;
			rupt_pre = 1;
			while(rupt_ <= (6000000/speed))  // 一次音符的时间到后，退出循环
			{
				if(rupt_pre  != rupt_)	// 如果发生了中断
				{time1 = time1 - (rupt_ - rupt_pre); rupt_pre = rupt_;  } // 记录中断 times自减	
				
				if(time1 <= 0)	//如果times减到0了 端口值变换
				{ 
					ET0 = 0;
					time1 = local_time1; 
					P10 = !P10;
					ET0 = 1;
				}
			}
			rupt_ = 1;
			rupt_pre = 1;
			
			
			
		}
		while(1)
		{}
		
	}
}