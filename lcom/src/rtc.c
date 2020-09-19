#include <lcom/lcf.h>
#include "rtc.h"
#include "macros/error.h"
#include "game.h"
#include "utils.h"
#include "draw.h"
#include "timer.h"
#include <string.h>

extern char last_rtc_time[6];
char first_rtc_time[6];
bool passed_a_minute = false;
extern uint8_t *board_video_mem;

int rtc_hook_id = RTC_IRQ;

int rtc_subscribe_int(uint16_t *bit_no) {
    *bit_no = BIT(rtc_hook_id);
    error(sys_irqsetpolicy(RTC_IRQ , IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id),
          "Error on RTC interruptions subscription");
    return 0;
}

int rtc_unsubscribe_int() {
    error(sys_irqrmpolicy(&rtc_hook_id), "Error on RTC interruptions unsubscription");
    return 0;
}

char last_rtc_time[6];

void rtc_enable_alarm_int() {
    uint8_t cur_reg;
    sys_outb(RTC_ADDR_REG, B_REG_ADDR);
    util_sys_inb(RTC_DATA_REG, &cur_reg);
    cur_reg |= ALARM_INT_REG_B;
    sys_outb(RTC_ADDR_REG, B_REG_ADDR);
    sys_outb(RTC_DATA_REG, cur_reg);

}

void rtc_disable_int() {
    uint8_t cur_reg;
    sys_outb(RTC_ADDR_REG, B_REG_ADDR);
    util_sys_inb(RTC_DATA_REG, &cur_reg);
    cur_reg &= NO_INT_MASK;
    sys_outb(RTC_ADDR_REG, B_REG_ADDR);
    sys_outb(RTC_DATA_REG, cur_reg);
}

void rtc_set_alarm_every_minute() {
    uint8_t cur_reg;
    sys_outb(RTC_ADDR_REG, B_REG_ADDR);
    util_sys_inb(RTC_DATA_REG, &cur_reg);
    cur_reg |= SET_REG_B;
    sys_outb(RTC_ADDR_REG, B_REG_ADDR);
    sys_outb(RTC_DATA_REG, cur_reg);

    uint8_t min = DONT_CARE_ALARM, hour = DONT_CARE_ALARM, sec = DONT_CARE_ALARM;
    sys_outb(RTC_ADDR_REG, SECONDS_ALARM_REG_ADDR);
    sys_outb(RTC_DATA_REG, sec);
    sys_outb(RTC_ADDR_REG, MINUTES_ALARM_REG_ADDR);
    sys_outb(RTC_DATA_REG, min);
    sys_outb(RTC_ADDR_REG, HOUR_ALARM_REG_ADDR);
    sys_outb(RTC_DATA_REG, hour);

    sys_outb(RTC_ADDR_REG, B_REG_ADDR);
    util_sys_inb(RTC_DATA_REG, &cur_reg);
    cur_reg &= UNSET_REG_B;
    sys_outb(RTC_ADDR_REG, B_REG_ADDR);
    sys_outb(RTC_DATA_REG, cur_reg); 

    sys_outb(RTC_ADDR_REG, B_REG_ADDR);
    util_sys_inb(RTC_DATA_REG, &cur_reg);   
}

int rtc_read_time() {
    rtc_disable_int();
    uint8_t reg_a = 0;
    do {
        sys_outb(RTC_ADDR_REG, A_REG_ADDR);
        util_sys_inb(RTC_DATA_REG, &reg_a);
    } while (reg_a & UIP_REG_A);
    uint8_t min, hour;
    sys_outb(RTC_ADDR_REG, MINUTES_REG_ADDR);
    util_sys_inb(RTC_DATA_REG, &min);
    sys_outb(RTC_ADDR_REG, HOUR_REG_ADDR);
    util_sys_inb(RTC_DATA_REG, &hour);

    hour = bcd_to_decimal(hour);
    min = bcd_to_decimal(min);

    strcpy(last_rtc_time,"\0");
    char aux_dp[2], *aux;
    aux_dp[0] = ':'; aux_dp[1] = 0;
    aux = util_itoa(hour);
    strcat(last_rtc_time, aux);
    strcat(last_rtc_time, aux_dp);
    aux = util_itoa(min);
    strcat(last_rtc_time, aux);
    free(aux);
    rtc_enable_alarm_int();
    return 0;
}

extern uint8_t *moves_video_mem;

void rtc_int_handler() {
    rtc_read_time();
    write_string(last_rtc_time, 512 - 50, 30,5, board_video_mem);
}

void rtc_passed_a_minute() {
    rtc_read_time();
    if (strcmp(last_rtc_time, first_rtc_time)) {
        passed_a_minute = true;
        timer_reset_counter();
    }
}

void rtc_set_first_rtc_time() {
    strcpy(first_rtc_time, last_rtc_time);
}

