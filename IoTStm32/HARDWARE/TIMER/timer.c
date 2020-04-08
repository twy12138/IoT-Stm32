#include "timer.h"
#include "gizwits_product.h"



//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		gizTimerMs();
	}
}


void TIM4_PWM_Init(unsigned short arr, unsigned short psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);//使能B和复用时钟
	
	 //设置该引脚为复用输出功能,输出TIM4 CH4的PWM脉冲波形	GPIOB.9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//灯灭
	GPIO_SetBits(GPIOB, GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);
	
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStruct.TIM_Period = arr;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc;//设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM4 Channel4 PWM模式	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;//选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM4, &TIM_OCInitStruct);//根据T指定的参数初始化外设TIM4 OC4
	
	//使能TIM4在CCR4上的预装载寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	
		//初始化TIM4 Channel4 PWM模式	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;//选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM4, &TIM_OCInitStruct);//根据T指定的参数初始化外设TIM4 OC4
	
	//使能TIM4在CCR4上的预装载寄存器
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	
		//初始化TIM4 Channel4 PWM模式	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;//选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM4, &TIM_OCInitStruct);//根据T指定的参数初始化外设TIM4 OC4
	
	//使能TIM4在CCR4上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 	
		//初始化TIM4 Channel4 PWM模式	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2;//选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStruct);//根据T指定的参数初始化外设TIM4 OC4
	
	//使能TIM4在CCR4上的预装载寄存器
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 
  //使能TIM4
	TIM_Cmd(TIM4, ENABLE);

}









