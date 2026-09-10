# Enhanced Elevator Controller (LPC2148 / LPC21xx)

Ye tumhare original 4-floor (Ground, First, Second, Third) elevator project ka
enhanced version hai. Original logic (floor request, move up/down, buzzer,
door open/close, 7-segment display) same rakha gaya hai — bas 3 naye
feature-sets add kiye gaye hain.

## Naye Files
| File | Kaam |
|---|---|
| `lcd.h` / `lcd.c` | 16x2 LCD driver (4-bit mode) — floor number, time, status dikhata hai |
| `rtc.h` / `rtc.c` | LPC2148 ke built-in RTC se time track karta hai |
| `adc.h` / `adc.c` | Load sensor se weight padhta hai (overload check ke liye) |
| `main.c` | Poora program yahi se chalta hai (ye pehle zip mein tha hi nahi!) |

## Naye Features
1. **LCD + RTC** — Screen pe hamesha current floor aur time dikhta rahega,
   niche line pe status: `NORMAL`, `OVERLOAD! WAIT`, `ON BACKUP POWER`,
   ya `EMERGENCY STOP`.
2. **Emergency Alarm + Backup Power** — Emergency button dabate hi lift turant
   ruk jaati hai, door khula rehta hai, alarm bajta hai jab tak button release
   na ho. Mains power fail hone par automatically backup relay ON ho jaata
   hai (`MAINS_SENSE` pin low = power gone).
3. **Overload Sensor** — Load cell / weight sensor ADC se padha jaata hai;
   threshold cross karte hi door band nahi hoga, LED + alarm on ho jaayega,
   jab tak load kam na ho.

## Naye Pin Connections (Port0, unless noted)
```
EMERGENCY_SW   P0.10   (push button, active-low, use pull-up)
MAINS_SENSE    P0.11   (HIGH = mains OK, LOW = power failure)
BACKUP_RELAY   P0.12   (drives relay -> battery backup)
ALARM_BUZZ     P0.13   (separate buzzer for emergency/overload)
OVERLOAD_LED   P0.14   (overload indicator)
LCD_RS         P0.15
LCD_EN         P0.16
LCD_D4-D7      P0.17-20  (4-bit LCD data lines)
Weight sensor  P0.28 (AD0.1)  -- analog input, calibrate OVERLOAD_THRESHOLD
```
Purane pins (switches, buzzer, door LED, 7-seg display, up/down LED) waise
hi hai jaise pehle the.

## Build
Keil uVision mein `.uvproj` project banao (ya jo bhi tumhara existing IDE
setup hai), saari `.c` files add karo:
`main.c, delay.c, lcd.c, rtc.c, adc.c, Elevator_Function.c`
aur target LPC2148 (ya jo bhi tumhara chip hai) select karo.

## Tuning
- `OVERLOAD_THRESHOLD` (Elevator_defines.h) apne load-sensor ke hisab se
  calibrate karna — abhi ek sample value (600) rakhi hai.
- `RTC_SetTime()` main call me default 9:00:00 set kiya hai, chaho to
  real time set karne ka ek push-button menu bhi add kar sakte hain.

## Aage kya add kar sakte ho
- Zyada floors (config sirf `MAX_FLOOR` aur switch/FLUT badhana hoga)
- Multiple elevators / group control
- UART se serial monitor par logs bhejna
