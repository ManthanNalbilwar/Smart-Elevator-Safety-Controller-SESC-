//Elevator Functions Definitions (Elevator_Function.c)
#include <lpc21xx.h>
#include "delay.h"
#include "types.h"
#include "defines.h"
#include "Elevator_Function.h"
#include "Elevator_defines.h"
#include "lcd.h"
#include "rtc.h"
#include "adc.h"

//LOOKUP TABLE (7-segment codes for floors 0-3, index 4 = blank)
const s8 FLUT[]= {0x42, 0xF9, 0xA4, 0xB0, 0x86};

//VARIABLE INITIALIZATION
u32 RFloor=0, CFloor=0;
u8  i=0, flag=0;
u8  door_req=0;

u8  emergency_flag=0;
u8  power_fail_flag=0;
u8  overload_flag=0;
u32 weight_value=0;
u8  cur_hour=0, cur_min=0, cur_sec=0;

void Init_Elevator(void)
{
	IODIR0 |= (1<<BUZZ)|(1<<DOOR_LED)|(1<<BACKUP_RELAY)|(1<<ALARM_BUZZ)|(1<<OVERLOAD_LED);
	IODIR0 &= ~((1<<EMERGENCY_SW)|(1<<MAINS_SENSE));   // inputs
	IODIR1 |= ((0xFF<<FLRDISP)|(1<<INCLED)|(1<<DECLED));
	WRITEBYTE(IOPIN1, FLRDISP, FLUT[CFloor]);
}// IODIR setup

void Init_Peripherals(void)
{
	Init_Elevator();
	LCD_Init();
	RTC_Init();
	ADC_Init();
	RTC_SetTime(9,0,0);      // default start time, can be set by user later
	LCD_Clear();
	LCD_SetCursor(0,0);
	LCD_String("ELEVATOR READY");
}

void Request_Floor(void)
{
//REQUEST CHECKING
		if(READBIT(IOPIN0,G_FLOOR_SW)==0)
		{
			RFloor=0;
			WRITEBYTE(IOPIN1, FLRDISP, FLUT[CFloor]);
			while(READBIT(IOPIN0,G_FLOOR_SW)==0);
		}
		else if(READBIT(IOPIN0,F_FLOOR_SW)==0)
		{
			RFloor=1;
			WRITEBYTE(IOPIN1, FLRDISP, FLUT[CFloor]);
			while(READBIT(IOPIN0,F_FLOOR_SW)==0);
		}
		else if(READBIT(IOPIN0,S_FLOOR_SW)==0)
		{
			RFloor=2;
			WRITEBYTE(IOPIN1, FLRDISP, FLUT[CFloor]);
			while(READBIT(IOPIN0,S_FLOOR_SW)==0);
		}
		else if(READBIT(IOPIN0,T_FLOOR_SW)==0)
		{
			RFloor=3;
			WRITEBYTE(IOPIN1, FLRDISP, FLUT[CFloor]);
			while(READBIT(IOPIN0,T_FLOOR_SW)==0);
		}
}

void Move_Lift_up(void)
{
			delay_ms(1);
        WRITEBIT(IOSET1, INCLED, 1);
        delay_ms(100);              // Travel time
        WRITEBIT(IOCLR1, INCLED, 1);
			delay_ms(100);
        CFloor++;
        WRITEBYTE(IOPIN1, FLRDISP, FLUT[CFloor]);

        if(CFloor == RFloor)
        {
            flag = 1;
        }
}
void Move_Lift_down(void)
{
			delay_ms(1);
        WRITEBIT(IOSET1, INCLED, 1);
        delay_ms(100);              // Travel time
        WRITEBIT(IOCLR1, INCLED, 1);
			delay_ms(100);
        CFloor--;
        WRITEBYTE(IOPIN1, FLRDISP, FLUT[CFloor]);
        if(CFloor == RFloor)
        {
            flag = 1;
        }
}

void Ring_Buzzer()
{
	//RINGING BUZZER
	if(flag==1)
	{
			for(i=0;i<3;i++)
			{
					WRITEBIT(IOSET0,BUZZ,1);
					delay_ms(100);
					WRITEBIT(IOCLR0,BUZZ,1);
					delay_ms(100);
			}
			WRITEBIT(IOCLR0,BUZZ,1);
	}
	else IOCLR0=(1<<BUZZ);
}
void Wait_Door(void)
{
	//DOOR CHECK SECTION
			while(READBIT(IOPIN0, DCLSE_SW) != 0)
				{
					// bail out immediately if an emergency or overload occurs while waiting
					if(emergency_flag || overload_flag) break;
					WRITEBYTE(IOPIN1,FLRDISP,FLUT[CFloor]);
					delay_ms(500);
					WRITEBYTE(IOPIN1,FLRDISP,FLUT[4]);
					delay_ms(500);
				}
				WRITEBYTE(IOPIN1,FLRDISP,FLUT[CFloor]);
				flag=0;
}

void Wait_Door_Decision(void)
{
	u32 door_cnt=0;
    for(door_cnt=0 ; door_cnt<DOOR_WAIT_LOOP_COUNT ; door_cnt++)
    {
        delay_ms(1);
        if(READBIT(IOPIN0, DOPEN_SW) == 0)
        {
            door_req=1;
            return;
        }
        if(READBIT(IOPIN0, DCLSE_SW) == 0)
        {
            door_req=0;
            return;
				}
    }
		door_req=1;
}

void Open_Door(void)
{
	IOSET0=(1<<DOOR_LED);
}
void Close_Door(void)
{
	// Never let the door close on an overloaded cabin
	if(overload_flag)
	{
		IOSET0=(1<<DOOR_LED);
		return;
	}
	IOCLR0=(1<<DOOR_LED);
	flag=0;
	door_req=0;
}

void Check_Emergency(void)
{
	if(READBIT(IOPIN0, EMERGENCY_SW) == 0)   // active low push button
	{
		emergency_flag = 1;
	}
}

void Check_Power(void)
{
	if(READBIT(IOPIN0, MAINS_SENSE) == 0)    // mains lost
	{
		power_fail_flag = 1;
		WRITEBIT(IOSET0, BACKUP_RELAY, 1);   // switch to battery backup
	}
	else
	{
		power_fail_flag = 0;
		WRITEBIT(IOCLR0, BACKUP_RELAY, 1);   // back on mains, drop relay
	}
}

void Check_Overload(void)
{
	weight_value = ADC_Read(OVERLOAD_ADC_CHANNEL);
	if(weight_value > OVERLOAD_THRESHOLD)
	{
		overload_flag = 1;
		WRITEBIT(IOSET0, OVERLOAD_LED, 1);
	}
	else
	{
		overload_flag = 0;
		WRITEBIT(IOCLR0, OVERLOAD_LED, 1);
	}
}

void Emergency_Alarm(void)
{
	// Distinct fast-beep pattern, different from the normal arrival buzzer
	for(i=0;i<5;i++)
	{
		WRITEBIT(IOSET0, ALARM_BUZZ, 1);
		delay_ms(50);
		WRITEBIT(IOCLR0, ALARM_BUZZ, 1);
		delay_ms(50);
	}
}

void Update_Display(void)
{
	RTC_GetTime(&cur_hour, &cur_min, &cur_sec);

	LCD_SetCursor(0,0);
	LCD_String("Floor:");
	LCD_Char('0' + CFloor);
	LCD_String(" ");
	LCD_Char('0' + (cur_hour/10));
	LCD_Char('0' + (cur_hour%10));
	LCD_Char(':');
	LCD_Char('0' + (cur_min/10));
	LCD_Char('0' + (cur_min%10));

	LCD_SetCursor(1,0);
	if(emergency_flag)      LCD_String("EMERGENCY STOP  ");
	else if(overload_flag)  LCD_String("OVERLOAD! WAIT  ");
	else if(power_fail_flag)LCD_String("ON BACKUP POWER ");
	else                     LCD_String("STATUS: NORMAL  ");
}
