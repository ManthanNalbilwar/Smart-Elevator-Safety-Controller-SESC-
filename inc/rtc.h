#ifndef RTC_H
#define RTC_H
#include "types.h"

void RTC_Init(void);
void RTC_SetTime(u8 hour, u8 min, u8 sec);
void RTC_GetTime(u8 *hour, u8 *min, u8 *sec);

#endif
