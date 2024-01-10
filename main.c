/*
 * File:   main.c
 * Author: palan
 *
 * Created on December 14, 2023, 1:00 PM
 */

#include <xc.h>
#include "car_black_box.h"
#include "clcd.h"
#include "keypad.h"
#include "adc.h"
#include "RTC.h"
#include "i2c.h"
#include "eeprom.h"

extern unsigned char pass_flag = 0, sec;

unsigned char clock_reg[3], time[9], password[4] = {'1', '0', '1', '0'};
unsigned char count_eve = 0, ev = 0, total_events = 0;

static void get_time(void) {
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40) {
        time[0] = event[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
        time[1] = event[1] = '0' + (clock_reg[0] & 0x0F);
    } else {
        time[0] = event[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
        time[1] = event[1] = '0' + (clock_reg[0] & 0x0F);
    }
    time[2] = event[2] = ':';
    time[3] = event[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = event[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = event[5] = ':';
    time[6] = event[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = event[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';
    event[8] = ' ';
}

void display_time() {
    get_time();
    clcd_print(time, LINE2(0));
}

void init_config() {
    init_clcd();
    init_adc();
    init_matrix_keypad();
    init_timer0();
    init_i2c();
    init_ds1307();
}

void main(void) {
    init_config();
    unsigned short adc_val = 0;
    unsigned char key;

    event[9] = 'O';
    event[10] = 'N';
    adc_val = read_adc(4) / 10.23;
    display_time();
    display_speed(adc_val);
    store_event(event);
    
    for(unsigned char i = 0; i < 4; i++)
    {
        password[i] = read_ext_eeprom(EEPROM_ADDRESS_1 + i);
    }

    while (1) {
        adc_val = read_adc(4) / 10.23;

        if (key == 10 && pass_flag == 0) {
            pass_flag = 1;
            sec = 60;
            clcd_print("                ", LINE2(0));
        }

        if (pass_flag == 0 || pass_flag == 1 || pass_flag == 7)
            key = read_switches(EDGE);
        else
            key = read_switches(LEVEL);

        switch (pass_flag) {
            case 0:
                clcd_print("TIME", LINE1(2));
                clcd_print("EV", LINE1(10));
                clcd_print("SP", LINE1(14));

                display_time();
                display_event(key);
                display_speed(adc_val);
                break;

            case 1:
                enter_password(key);
                break;

            case 2:
                menu(key);
                break;

            case 3:
                view_log(key);
                break;

            case 4:
                set_time(key);
                break;

            case 5:
                download_log();
                break;

            case 6:
                clear_log();
                break;

            case 7:
                change_password(key);
                break;
        }

    }
}