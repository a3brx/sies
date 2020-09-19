#pragma once

#define RTC_IRQ 8

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define SECONDS_REG_ADDR 0
#define SECONDS_ALARM_REG_ADDR 1
#define MINUTES_REG_ADDR 2
#define MINUTES_ALARM_REG_ADDR 3
#define HOUR_REG_ADDR 4
#define HOUR_ALARM_REG_ADDR 5
#define WEEKDAY_REG_ADDR 6
#define MONTHDAY_REG_ADDR 7
#define MONTH_REG_ADDR 8
#define YEAR_REG_ADDR 9
#define A_REG_ADDR 10
#define B_REG_ADDR 11
#define C_REG_ADDR 12
#define D_REG_ADDR 13

#define UIP_REG_A BIT(7)

#define SET_REG_B BIT(7)
#define UNSET_REG_B 0x7F
#define ALARM_INT_REG_B BIT(5)
#define UPDATE_INT_REG_B BIT(4)
#define PERIODIC_INT_REG_B BIT(6)
#define NO_INT_MASK BIT(0) | BIT(1) | BIT(2) | BIT(3) | BIT(7)

#define DONT_CARE_ALARM 0xc0

