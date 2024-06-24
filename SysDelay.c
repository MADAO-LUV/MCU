#include "stm32f10x.h"                  // Device header

/*
*static修饰全局变量，该变量只在本文件被访问，不能被外部文件访问
*/
static u32 fac_us;  //定义 1 微秒的值
static u32 fac_ms;  //定义 1  毫秒的值

/*************************************
*
* 初始化系统的嘀嗒定时器
*

*解释:
对于时延的初始化，直接让 Systick 使用系统时钟的 8 分频，
使其达到 9MHz 的频率，产生 1ms时间基准 
此处的校准值为9000 则 9000 * 1 / (9 * 10**6) = 1 * 10**-3 s == 1 ms
**************************************/
void Sysdelay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	//选择外部时钟，将 72MHz 的频率 8 分频，把系统嘀嗒定时器的变化定位在 1s
	
	fac_us = SystemCoreClock / 8000000;  //此处的SystemCoreClock---72MHZ
	fac_ms = fac_us * 1000;   //1 毫秒的时间	
}


/**************************************
* 微秒延时计数
*
*@para-> u32 nus    你所需延时的微妙时间
*
 无返回值
* temp & 0x01 && !(temp & (1 << 16)) 详解：
* temp & 0x01 通过定时器状态值的最小位（1位）判断定时器是否处于使能状态
* !(temp & (1 << 16)) 通过定时器状态值的最大位（16位）判断定时器是否处于归零状态
***************************************/
void Sysdelay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD = nus * fac_us;   //设置重载值  当到达溢出后，计数器回到此值
	SysTick->VAL |= 0x00;    //将定时器归零
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //开启定时器
	
	//这里是 do{}while(); 先执行{}里的代码 后进行判断
	// 通过循环判定定时器的状态位值来确定定时器是否归零
	do
	{
		temp = SysTick->CTRL; //获取定时器状态值
	}while(temp & 0x01 && !(temp & (1 << 16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭定时器
	SysTick->VAL |= 0x00;  //定时器归零
	
}



/**************************************
* 毫秒延时计数
*
*@para-> u32 nms    你所需延时的毫秒时间
*
 无返回值
* temp & 0x01 && !(temp & (1 << 16)) 详解：
* temp & 0x01 通过定时器状态值的最小位（1位）判断定时器是否处于使能状态
* !(temp & (1 << 16)) 通过定时器状态值的最大位（16位）判断定时器是否处于归零状态
***************************************/
void Sysdelay_ms(u32 nms)
{
	u32 temp;
	SysTick->LOAD = nms * fac_ms;   //设置重载值  当到达溢出后，计数器回到此值
	SysTick->VAL |= 0x00;    //将定时器归零
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //开启定时器
	
	//这里是 do{}while(); 先执行{}里的代码 后进行判断
	// 通过循环判定定时器的状态位值来确定定时器是否归零
	do
	{
		temp = SysTick->CTRL; //获取定时器状态值
	}while(temp & 0x01 && !(temp & (1 << 16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭定时器
	SysTick->VAL |= 0x00;  //定时器归零
	
}




