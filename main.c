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

extern unsigned char pass_flag = 0, sec;

void init_config() {
    init_clcd();
    init_matrix_keypad();
    init_adc();
    init_timer0();
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