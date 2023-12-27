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
unsigned char clock_reg[3], time[9];
unsigned char count_eve = 0, ev = 0;

static void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);
    event[0] = ' ';
    event[1] = ' ';

	if (clock_reg[0] & 0x40)
	{
		time[0] = event[2] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = event[3] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = event[2] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = event[3] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = event[4] = ':';
	time[3] = event[5] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = event[6] = '0' + (clock_reg[1] & 0x0F);
	time[5] = event[7] = ':';
	time[6] = event[8] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = event[9] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
    event[10] = ' ';
}

void display_time() {
    get_time();
    clcd_print(time, LINE2(0));
}

void init_config() {
    init_clcd();
    init_matrix_keypad();
    init_adc();
    init_timer0();
    init_i2c();
	init_ds1307();
}

void main(void) {
    init_config();
    unsigned short adc_val;
    unsigned char key;

    while (1) {
        adc_val = read_adc(4) / 10.23;
        
        if (pass_flag == 2)
            key = read_switches(LEVEL);
        else
            key = read_switches(EDGE);

        if (key == 10) {
            pass_flag = 1;
            sec = 60;
            clcd_print("                ", LINE2(0));
        }

        if (pass_flag == 1) {
            enter_password(key);
        } 
        else if (pass_flag == 2) {
            menu(key);
        } 
        else {
            clcd_print("TIME", LINE1(2));
            clcd_print("EV", LINE1(10));
            clcd_print("SP", LINE1(14));

            display_time();
            display_event(key);
            display_speed(adc_val);
        }
    }
}