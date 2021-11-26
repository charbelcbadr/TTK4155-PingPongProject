/*
 * Delay.c
 *
 * Created: 10.11.2021 14:29:47
 *  Author: harishnv
 */ 

/* 
Description: Delay function used to generate in ms
Input: If the value is 1, 1ms is generated approx
Output: Nothing
*/
void delay_ms(int value)
{
	for(int i=0;i<value;i++)
	{
		for(int j=0;j<25000;j++)
		{
			__asm__ __volatile__ ("nop");			
		}
	}
}
/* 
Description: Delay function used to generate in us
Input: If the value is 1, 1us is generated approx
Output: Nothing
*/
void delay_us(int value)
{
	for(int i=0;i<value;i++)
		{
			for(int j=0;j<25;j++)
			{
				__asm__ __volatile__ ("nop");			
			}
		}
}