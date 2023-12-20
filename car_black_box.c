/*
 * File:   car_black_box.c
 * Author: palan
 *
 * Created on December 14, 2023, 1:01 PM
 */


#include <xc.h>
#include "adc.h"
#include "car_black_box.h"
#include "clcd.h"
#include "keypad.h"

unsigned char pass_flag = 0;
extern unsigned char sec;

void display_time() {
    clcd_print("12:00:00", LINE2(0));
}

void display_event(unsigned char key) {
    static unsigned char gear = 0, flag = 0;
    if (key == 1) {
        flag = 1;
        gear = 0;
    }
    if (key == 2) {
        flag = 0;
        gear++;
        if (gear == 7)
            gear = 6;
    }
    if (key == 3 && flag == 0) {
        if (--gear == 0)
            gear = 1;
    }

    if (flag == 1) {
        clcd_print(signature[7], LINE2(10));
    } else {
        clcd_print(signature[gear], LINE2(10));
    }
}

void display_speed(unsigned short speed) {

    clcd_putch((speed / 10) + 48, LINE2(14));
    clcd_putch((speed % 10) + 48, LINE2(15));

}

void enter_password() {
    unsigned char pass_key, password[5] = "1010", input_pass[5];
    unsigned char ind = 0, count = 0, attempt = 3;
    unsigned int wait = 0;

    sec = 60;
    while (pass_flag == 1) {
        pass_key = read_switches(EDGE);

        clcd_print(" Enter Password ", LINE1(0));

        //Stores the value from the key and prints '*' on the CLCD
        if (pass_key == 11) {
            sec = 60;
            input_pass[ind] = '1';
            clcd_putch('*', LINE2(ind + 5));
            ind++;
        } else if (pass_key == 12) {
            sec = 60;
            input_pass[ind] = '0';
            clcd_putch('*', LINE2(ind + 5));
            ind++;
        }
        if (sec == 55) {
            clcd_print("                ", LINE1(0));
            pass_flag = 0;
        }

        if (wait++ < 500)
            clcd_putch('_', LINE2(ind + 5));
        else if (wait > 500)
            clcd_putch(' ', LINE2(ind + 5));

        if (wait == 1000)
            wait = 0;

        //Starts checking the entered key with the default password
        if (ind == 4) {
            __delay_ms(300);
            ind = 0;
            for (int j = 0; j < 4; j++) {
                if (password[j] == input_pass[j]) {
                    count++;
                }
            }

            //If password is correct, prints the success message on the CLCD 
            //And, makes the controller to the Sleep mode
            if (count == 4) {
                clcd_print("Correct password", LINE1(0));
                clcd_print("                ", LINE2(0));
                __delay_ms(500);
                clcd_print("                ", LINE1(0));

                menu();
            }//If the password is incorrect, 5 attemts given to the user
            else if (count < 4) {
                count = 0;
                attempt--;

                //Displays the 'wrong password' message and no. of attempts left
                clcd_print(" Wrong Password ", LINE1(0));
                clcd_putch(attempt + 48, LINE2(1));
                clcd_print(" -> attempts  ", LINE2(2));
                __delay_ms(1000);
                clcd_print("                ", LINE2(0));

            }
        }

        if (attempt == 0) {
            for (sec = 59; sec != 60;) {
                clcd_print(" Attempts over  ", LINE1(0));
                clcd_print(" Wait for ", LINE2(0));
                clcd_putch(sec / 10 + 48, LINE2(10));
                clcd_putch(sec % 10 + 48, LINE2(11));
                clcd_print("sec", LINE2(13));
            }
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));

            pass_flag = 0;
        }
    }
}

void menu() {
    unsigned char menu_key, star = 0, pre_key, delay = 0;
    char line = 0;
    sec = 60;

    while (pass_flag == 1) {
        menu_key = read_switches(EDGE);
        if (menu_key == 11) {
            sec = 60;
            star = 0;
            if (pre_key == menu_key) {
                if (line-- <= 0) {
                    line = 0;
                }
            }
            pre_key = menu_key;
        } else if (menu_key == 12) {
            star = 1;
            sec = 60;
            if (pre_key == menu_key) {
                if (line++ >= 3) {
                    line = 3;
                }
            }
            pre_key = menu_key;
        }
        clcd_putch(line%10+48, LINE1(15));
        if (star == 1) {
            clcd_putch(' ', LINE1(0));
            clcd_putch('*', LINE2(0));
        } else if (star == 0) {
            clcd_putch('*', LINE1(0));
            clcd_putch(' ', LINE2(0));
        }

        clcd_print(menu_opt[line], LINE1(2));
        clcd_print(menu_opt[line + 1], LINE2(2));

        if (sec == 55) {
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));
            pass_flag = 0;
        }
    }
}