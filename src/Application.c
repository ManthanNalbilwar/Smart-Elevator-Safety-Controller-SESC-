//Four Floor Elevator (Main.c)
#include <lpc21xx.h>
#include "types.h"
#include "defines.h"
#include "delay.h"
#include "Elevator_Function.h"
#include "Elevator_defines.h"

typedef enum
{
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    ARRIVED,
    BUZZER,
    WAIT_FOR_DOOR_DECISION,
    DOOR_OPEN,
    DOOR_WAIT,
    DOOR_CLOSE,
    EMERGENCY          // NEW: emergency-stop state

}STATE;

int main()
{
	STATE State= IDLE;
	Init_Peripherals();     // LCD + RTC + ADC + Init_Elevator, all in one

	while(1)
	{
			// ---- safety checks run every scan, regardless of current state ----
			Check_Emergency();
			Check_Power();
			Update_Display();

			if(emergency_flag) State = EMERGENCY;

			switch(State)
			{
				case EMERGENCY: IOCLR1 = (1<<INCLED)|(1<<DECLED);   // freeze motion
												Open_Door();
												Emergency_Alarm();
												if(!emergency_flag) State = IDLE;   // button released -> resume
												break;

				case IDLE: 	Request_Floor();
										if (CFloor < RFloor )State = MOVING_UP;
										else if (CFloor > RFloor )State = MOVING_DOWN;
										
										break;
				
				case MOVING_UP: Move_Lift_up();
												if(flag==1)State=ARRIVED;
												break;
				
				case MOVING_DOWN:Move_Lift_down();
												 if(flag==1)State=ARRIVED;
												 break;
																	
				case ARRIVED: State=BUZZER;
											break;
										
				case BUZZER:Ring_Buzzer();
										State=WAIT_FOR_DOOR_DECISION;
										break;
										
				case  WAIT_FOR_DOOR_DECISION:	Wait_Door_Decision();
																			if(door_req == 1) State= DOOR_OPEN;
																			else State= DOOR_CLOSE;
																			break;
										
				case  DOOR_OPEN:	Open_Door();
													State=DOOR_WAIT;
													break;
										
				case  DOOR_WAIT: Wait_Door();
												 State=DOOR_CLOSE;
												 break;
										
				case  DOOR_CLOSE: Check_Overload();          // NEW: don't close on overload
													 if(overload_flag) State=DOOR_WAIT;   // stay open, re-check next scan
													 else
													 {
														 Close_Door();
														 State=IDLE;
													 }
													 break;
													
				default: State=IDLE;
																
			}
	}
}
