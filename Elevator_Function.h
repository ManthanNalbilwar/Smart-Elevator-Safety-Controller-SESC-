//Elevator Functions Declarations (Elevator_Function.h)
#ifndef ELEVATOR_FUNCTION_H
#define ELEVATOR_FUNCTION_H
#include "types.h"

extern u32 CFloor;
extern u32 RFloor;
extern u8  flag;
extern u8  door_req;

// --- new safety/feature state, shared with main.c ---
extern u8  emergency_flag;   // 1 = emergency stop button pressed
extern u8  power_fail_flag;  // 1 = mains lost, running on backup
extern u8  overload_flag;    // 1 = cabin overloaded
extern u32 weight_value;     // last raw ADC reading from load sensor
extern u8  cur_hour, cur_min, cur_sec;

// --- core elevator control (existing) ---
void Init_Elevator(void);
void Request_Floor(void);
void Move_Lift_up(void);
void Move_Lift_down(void);
void Ring_Buzzer(void);
void Open_Door(void);          // DOOR_OPEN state
void Wait_Door(void);          // DOOR_WAIT state
void Close_Door(void);         // DOOR_CLOSE state
void Wait_Door_Decision(void);

// --- new safety / feature functions ---
void Init_Peripherals(void);   // sets up LCD, RTC, ADC in addition to Init_Elevator
void Check_Emergency(void);    // polls emergency stop button
void Check_Power(void);        // polls mains sense line, switches backup relay
void Check_Overload(void);     // reads load sensor, flags overload
void Emergency_Alarm(void);    // sounds alarm pattern while emergency_flag is set
void Update_Display(void);     // refreshes LCD with floor/time/status

#endif
