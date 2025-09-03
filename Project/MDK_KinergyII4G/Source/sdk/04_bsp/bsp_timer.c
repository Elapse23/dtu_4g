#include "bsp_timer.h"

void bsp_timer_init(uint32_t timer_periph)
{
		/* TIMER1 configuration: generate PWM signals with different duty cycles:
			 TIMER1CLK = SystemCoreClock / 120 = 1MHz */
		timer_parameter_struct timer_initpara;
		
		if(TIMER3 == timer_periph)
		{
				rcu_periph_clock_enable(RCU_TIMER3);
				rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

				timer_deinit(TIMER3);

				/* TIMER1 configuration */
				timer_initpara.prescaler         = (uint16_t) (SystemCoreClock / 20000) - 1;		//10k Hz
				timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
				timer_initpara.counterdirection  = TIMER_COUNTER_UP;
				timer_initpara.period            = 10;
				timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
				timer_initpara.repetitioncounter = 0;
				timer_init(TIMER3,&timer_initpara);
				
				/* auto-reload preload enable */
				timer_auto_reload_shadow_enable(TIMER3);
				
				/* TIMER1 enable */
	//			timer_enable(TIMER3);
				timer_disable(TIMER3);
				
				timer_interrupt_enable(TIMER3, TIMER_INT_UP);	
				
				nvic_irq_enable(TIMER3_IRQn, 0, 0);
		}
		else if(TIMER5 == timer_periph)
		{
				rcu_periph_clock_enable(RCU_TIMER5);
				rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

				timer_deinit(TIMER5);
			 
				/* TIMER5 configuration */
				//system时钟频率200MHz
				timer_initpara.prescaler         = (uint16_t) (SystemCoreClock / 20000);		/* 定时器+1所用时间: 1/20000 = 0.00005s */
				timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
				timer_initpara.counterdirection  = TIMER_COUNTER_UP;
				timer_initpara.period            = 20000;		/* 定时器溢出时间: 0.00005*20,000 = 1s */
				timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
				timer_initpara.repetitioncounter = 0;
				timer_init(TIMER5,&timer_initpara);

				/* auto-reload preload enable */
				timer_auto_reload_shadow_enable(TIMER5);

				timer_counter_value_config(TIMER5, 0);

				/* TIMER1 enable */
				timer_enable(TIMER5);
//				timer_disable(TIMER5);

				timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
				timer_interrupt_enable(TIMER5, TIMER_INT_UP);	
				nvic_irq_enable(TIMER5_DAC_IRQn, 4, 4);
		}
}


void bsp_timer_interrupt_onoff(uint32_t timer_periph, uint8_t on_off)
{
		if(TIMER5 == timer_periph)
		{
				if(on_off)
				{
						timer_interrupt_enable(timer_periph, TIMER_INT_UP);
				}
				else
				{
						timer_interrupt_disable(timer_periph, TIMER_INT_UP);
				}
		}
}