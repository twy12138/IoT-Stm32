#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "timer.h"
#include "usart3.h"	
#include "adc.h"
#include "dht11.h"
#include "gizwits_product.h"
#include "key.h"

unsigned char wifi_sta;
 
dataPoint_t currentDataPoint; 

void Gizwits_Init(void)
{
	TIM3_Int_Init(9,7199);//1MS系统定时
	usart3_init(9600);//WIFI初始化 
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}

void Show_title()
{
		OLED_ShowCHinese(25,0,41);
		OLED_ShowCHinese(40,0,42);
		OLED_ShowCHinese(55,0,16);
		OLED_ShowCHinese(70,0,17);
		OLED_ShowCHinese(85,0,39);
		OLED_ShowCHinese(100,0,40);
}
void Show_context()
{
		OLED_ShowCHinese(5,3,43);
		OLED_ShowCHinese(20,3,45);
  	OLED_ShowChar(35,3,':',16);
	
		OLED_ShowCHinese(70,3,44);
		OLED_ShowCHinese(85,3,45);
  	OLED_ShowChar(100,3,':',16);
	
		OLED_ShowCHinese(5,6,46);
		OLED_ShowCHinese(20,6,48);
  	OLED_ShowChar(35,6,':',16);
	
		OLED_ShowString(70,6,"Wifi",16);
		OLED_ShowString(100,6,":",16);
		
}

int main(void)
{	
	int key;
	static u16 num=0;
	u8 dir =1;	 
	delay_init();	    //延时函数初始化	  
	uart_init(115200);
	LED_Init();		  	//初始化与LED连接的硬件接口
 	while(DHT11_Init());	//DHT11初始化	
	Adc_Init();
	TIM4_PWM_Init(500-1,84-1);
	OLED_Init();
	KEY_Init();
	OLED_Clear();
	Show_title();
	Show_context();
	Gizwits_Init();
	while(1)
	{

	if(dir)num=num+50;
	else num=num-50;
	if(num>=500)dir=0;
	if(num<=0)dir=1;
	TIM_SetCompare4(TIM4,num);
	TIM_SetCompare3(TIM4,num);
	TIM_SetCompare2(TIM4,num);
	TIM_SetCompare1(TIM4,num);
	delay_ms(2);
		userHandle();
		
		gizwitsHandle((dataPoint_t *)&currentDataPoint);
		
		key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键

		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位
		}
	}
	
}


