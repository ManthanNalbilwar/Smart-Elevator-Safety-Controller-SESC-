// ADC0 driver - used to read the weight/load-cell sensor for overload detection
#include <lpc21xx.h>
#include "types.h"
#include "adc.h"

void ADC_Init(void)
{
	PCONP |= (1<<12);          // Power up ADC0
	PINSEL1 |= (1<<23);        // P0.28 -> AD0.1 function (weight sensor input)
	AD0CR = (1<<21) | (25<<8); // Enable ADC0, clock divider (PCLK/26 approx)
}

u32 ADC_Read(u8 channel)
{
	u32 result;

	AD0CR &= ~0x000000FF;
	AD0CR |= (1<<channel);      // Select channel
	AD0CR |= (1<<24);           // Start conversion

	while(!(AD0DR1 & (1UL<<31)));  // Wait for DONE bit (channel 1 result reg)

	result = (AD0DR1 >> 6) & 0x3FF;  // 10-bit result
	AD0CR &= ~(1<<24);          // Stop conversion
	return result;
}
