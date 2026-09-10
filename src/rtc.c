// Real-Time Clock driver using LPC2148's on-chip RTC peripheral
#include <lpc21xx.h>
#include "types.h"
#include "rtc.h"

void RTC_Init(void)
{
	PCONP |= (1<<9);     // Power up RTC block
	CCR   = 0x01;        // Enable RTC clock, use external 32.768kHz clock
}

void RTC_SetTime(u8 hour, u8 min, u8 sec)
{
	CCR &= ~0x01;        // Stop clock while setting
	HOUR = hour;
	MIN  = min;
	SEC  = sec;
	CCR |= 0x01;         // Resume counting
}

void RTC_GetTime(u8 *hour, u8 *min, u8 *sec)
{
	*hour = (u8)HOUR;
	*min  = (u8)MIN;
	*sec  = (u8)SEC;
}
