//DEFINITIONS (Elevator_defines.h)
#ifndef ELEVATOR_DEFINES_H
#define ELEVATOR_DEFINES_H

//--------------- EXISTING PORT0 PINS ---------------
#define DOPEN_SW    2
#define DCLSE_SW    3
#define G_FLOOR_SW  4
#define F_FLOOR_SW  5
#define S_FLOOR_SW  6
#define T_FLOOR_SW  7
#define BUZZ        8
#define DOOR_LED    9

//--------------- NEW PORT0 PINS --------------------
#define EMERGENCY_SW    10   // Emergency stop push button (active low)
#define MAINS_SENSE     11   // HIGH = mains present, LOW = power failure
#define BACKUP_RELAY    12   // Drives relay to switch to battery backup
#define ALARM_BUZZ      13   // Separate alarm buzzer (emergency / overload)
#define OVERLOAD_LED    14   // Overload indicator LED

//--------------- LCD PINS (Port0, 4-bit mode) ------
#define LCD_RS      15
#define LCD_EN      16
#define LCD_D4      17
#define LCD_D5      18
#define LCD_D6      19
#define LCD_D7      20

//--------------- PORT1 PINS (unchanged) ------------
#define FLRDISP     16
#define INCLED      24
#define DECLED      25

//--------------- OVERLOAD SENSOR (ADC0) -------------
#define OVERLOAD_ADC_CHANNEL   1     // AD0.1 -> P0.28
#define OVERLOAD_THRESHOLD     600   // 10-bit ADC value; tune to load cell/sensor

//--------------- MISC -------------------------------
#define MAX_FLOOR   3   // 0=Ground,1=First,2=Second,3=Third
#define DOOR_WAIT_LOOP_COUNT 4000

#endif
