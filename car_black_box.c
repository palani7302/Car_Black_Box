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
#include "eeprom.h"
#include "uart.h"

extern unsigned char sec, count_eve, ev, total_events, password[4];

unsigned char gear = 0, flag = 0, repeated_event = 0;

unsigned char input_pass[4], pass_flag = 0;
unsigned char ind = 0, count = 0, attempt = 3, new_password = 0, confirm_password = 0;
unsigned char ind_1 = 0, ind_2 = 0;
unsigned int wait = 0;

unsigned char line = 0, star = 0, prekey = 0, pre_key_1 = 0, mode = 0, option = 0;

unsigned char log_index = 0, event_flag = 0, data[15], index = 0;

void display_event(unsigned char key) {
    if (key == 1 && prekey != 1) {
        event_flag = 1;
        flag = 1;
        gear = 0;
        ev = 0;
        prekey = key;
    }
    if (key == 2) {
        if (gear == 6 && prekey == 2);
        else {
            event_flag = 1;
            flag = 0;
            ev = 0;
            if (++gear >= 6) {
                gear = 6;
                prekey = key;
            }
        }
    }
    if (key == 3 && flag == 0 && gear > 1) {
        if (gear == 1 && prekey == 3);
        else {
            event_flag = 1;
            ev = 0;
            if (--gear <= 1) {
                gear = 1;
                prekey = key;
            }
        }
    }

    if (ev == 1 || option != 0) {
        clcd_print(menu_eve[option], LINE2(10));
        event[9] = menu_eve[option][0];
        event[10] = menu_eve[option][1];
    } else {
        if (flag == 1) {
            clcd_print(signature[7], LINE2(10));
            event[9] = signature[7][0];
            event[10] = signature[7][1];
        } else {
            clcd_print(signature[gear], LINE2(10));
            event[9] = signature[gear][0];
            event[10] = signature[gear][1];
        }
    }

    if (event_flag == 1) {
        event_flag = 0;
        store_event(event);
    }
}

void display_speed(unsigned short speed) {
    event[11] = ' ';
    clcd_putch((speed / 10) + 48, LINE2(14));
    clcd_putch((speed % 10) + 48, LINE2(15));
    event[12] = (speed / 10) + 48;
    event[13] = (speed % 10) + 48;
}

void store_password(unsigned char key, unsigned char *pass) {
    //Stores the value from the key and prints '*' on the CLCD
    if (key == 11) {
        sec = 60;
        pass[ind] = '1';
        clcd_putch('*', LINE2(ind + 5));
        ind++;
    } else if (key == 12) {
        sec = 60;
        pass[ind] = '0';
        clcd_putch('*', LINE2(ind + 5));
        ind++;
    }

    if (wait++ < 500)
        clcd_putch('_', LINE2(ind + 5));
    else if (wait > 500)
        clcd_putch(' ', LINE2(ind + 5));

    if (wait == 1000)
        wait = 0;
}

void enter_password(unsigned char key) {
    clcd_print(" Enter Password ", LINE1(0));

    if (sec == 55) {
        clcd_print("                ", LINE1(0));
        clcd_print("                ", LINE2(0));
        pass_flag = ind = wait = 0;
        attempt = 3;

        return;
    }

    if (ind < 4)
        store_password(key, input_pass);

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
            __delay_ms(1000);
            pass_flag = 2;
            sec = 60;
            clcd_print("                ", LINE1(0));
            ind = mode = count = wait = prekey = pre_key_1 = 0;
            attempt = 3;
        }//If the password is incorrect, 3 attemts given to the user
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

        pass_flag = ind = 0;
        attempt = 3;
        return;
    }
}

void menu(unsigned char key) {
    //Key 11
    if (key == 11) {
        prekey = 11;
        if (wait++ > 400) {
            clcd_print(" Entered into : ", LINE1(0));
            clcd_print(menu_opt[mode], LINE2(0));
            __delay_ms(1000);
            pass_flag = 3 + mode;
            if (mode == 0) {
                event_flag = ev = 0;
                read_event(EEPROM_ADDRESS + (14 * ((log_index + index) % 10)), data);
            } else {
                option = mode;
                event_flag = ev = 1;
                event[9] = menu_eve[option][0];
                event[10] = menu_eve[option][1];
            }
            
            if (ev = 1 && event_flag == 1 && mode != 3) {
                event_flag = 0;
                store_event(event);
            }

            line = star = prekey = pre_key_1 = wait = 0;
            sec = 60;
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));
            return;
        }
    } else if (wait != 0 && wait < 400 && key == 0xFF && prekey == 11) {
        sec = 60;
        star = 0;

        if (pre_key_1 == 11) {
            if (line-- <= 0)
                line = 0;
        }
        if (mode-- <= 0)
            mode = 0;

        pre_key_1 = prekey;
        wait = 0;
    }

    //Key 12
    if (key == 12) {
        prekey = 12;
        if (wait++ > 400) {
            pass_flag = line = mode = star = 0;
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));
            return;
        }
    } else if (wait != 0 && wait < 400 && key == 0xFF && prekey == 12) {
        sec = 60;
        star = 1;

        if (pre_key_1 == 12) {
            if (line++ >= 3)
                line = 3;
        }

        if (mode++ >= 4)
            mode = 4;

        pre_key_1 = prekey;
        wait = 0;
    }


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
        option = mode;
        pass_flag = line = mode = star = 0;
        return;
    }
}

void view_log(unsigned char key) {
    //Key 11
    if (key == 11) {
        prekey = 11;
        wait++;
    } else if (wait != 0 && wait < 400 && key == 0xFF && prekey == 11) {
        if (log_index-- == 0)
            log_index = (total_events - 1);
        
        wait = 0;
    }

    //Key 12
    if (key == 12) {
        prekey = 12;
        if (wait++ > 500) {
            log_index = wait = 0;
            sec = 60;
            pass_flag = 2;
            clcd_print("                ", LINE1(0));
            clcd_print("                ", LINE2(0));
            return;
        }
    } else if (wait != 0 && wait < 500 && key == 0xFF && prekey == 12) {
        if (++log_index > (total_events - 1))
            log_index = 0;

        wait = 0;
    }

    clcd_print("Logs :-         ", LINE1(0));
    clcd_putch(log_index + 48, LINE2(0));
    read_event(EEPROM_ADDRESS + (14 * ((log_index + index) % 10)), data);
    clcd_print(data, LINE2(2));
}

void set_time(unsigned char key) {
    clcd_putch('2', LINE2(15));
    __delay_ms(1000);

    mode = wait = 0;
    pass_flag = 2;
    sec = 60;
}

void download_log() {
    clcd_print("Downloading...  ", LINE1(0));
    clcd_putch(total_events/10 + 48, LINE2(5));
    clcd_putch(total_events%10 + 48, LINE2(6));
    clcd_putch('3', LINE2(15));
    __delay_ms(1000);

    //interrupt
    GIE = 0;
    PEIE = 0;
    TMR0IE = 0;
    TMR0IF = 0;
    TMR0ON = 0;
    
    init_uart();
    
    puts("#   TIME   EV SP\n\r");
    for ( unsigned char i = 0; i < total_events; i++ )
    {
        putch(((log_index+index)%10)/10+48);
        puts("\n\r");
    }
    
    //interrupt
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;
    TMR0IF = 0;
    TMR0ON = 1;
    
    pass_flag = 2;
    mode = wait = 0;
    sec = 60;
    return;
}

void clear_log() {
    clcd_putch('4', LINE2(15));
    count_eve = total_events = log_index = index = mode = wait = 0;
    store_event(event);
    event_flag = 0;
    clcd_print("  Logs cleared  ", LINE1(0));
    __delay_ms(1000);
    clcd_putch(count_eve + 48, LINE2(10));
    __delay_ms(1000);

    pass_flag = 2;
    sec = 60;
}

void change_password(unsigned char key) {
    if (sec == 55) {
        clcd_print("                ", LINE1(0));
        clcd_print("                ", LINE2(0));
        ind = mode = wait = 0;
        pass_flag = 2;
        sec = 60;
        return;
    }

    if (ind < 4 && new_password == 0 && confirm_password == 0) {
        store_password(key, input_pass);
        clcd_print("Current Password", LINE1(0));
    }

    //Starts checking the entered key with the default password
    if (ind == 4 && new_password == 0 && confirm_password == 0) {
        __delay_ms(100);
        for (int j = 0; j < 4; j++) {
            if (password[j] == input_pass[j]) {
                count++;
            }
        }

        if (count == 4) {
            clcd_print("Password Matched", LINE1(0));
            clcd_print("                ", LINE2(0));
            __delay_ms(1000);
            count = ind = wait = key = 0;
            sec = 60;
            new_password = 1;
            attempt = 3;
        } else if (count < 4) {
            count = ind = wait = 0;
            attempt--;

            clcd_print(" Wrong Password ", LINE1(0));
            clcd_putch(attempt + 48, LINE2(1));
            clcd_print(" -> attempts  ", LINE2(2));
            __delay_ms(1000);
            clcd_print("                ", LINE2(0));
        }
    }
    if (attempt == 0) {
        clcd_print(" Attempts over  ", LINE2(0));
        __delay_ms(1000);
        ind = mode = wait = 0;
        pass_flag = 2;
        sec = 60;
        return;
    }

    if (new_password == 1) {
        clcd_print("  New Password ", LINE1(0));
        if (ind < 4) {
            store_password(key, password);
        } else {
            __delay_ms(100);
            new_password = ind = key = 0;
            confirm_password = 1;
            clcd_print("                ", LINE2(0));
        }
    }

    if (confirm_password == 1) {
        clcd_print("Confirm Password", LINE1(0));
        if (ind < 4)
            store_password(key, input_pass);

        if (ind == 4) {
            __delay_ms(100);

            for (int j = 0; j < 4; j++) {
                if (password[j] == input_pass[j]) {
                    count++;
                }
            }

            if (count == 4) {
                clcd_print("Password Matched", LINE1(0));
                clcd_print("                ", LINE2(0));
                __delay_ms(1000);
                clcd_print("Password changed", LINE1(0));
                clcd_print("  Successfully  ", LINE2(0));
                
                for ( unsigned char i = 0; i < 4; i++ )
                {
                    write_ext_eeprom(EEPROM_ADDRESS_1 + i, password[i]);
                }
                
                __delay_ms(1000);
                count = ind = new_password = confirm_password = mode = wait = 0;
                attempt = 3;
                pass_flag = 2;
                sec = 60;
                return;
            } else if (count < 4) {
                clcd_print(" Wrong Password ", LINE1(0));
                clcd_print("                ", LINE2(0));
                __delay_ms(1000);
                clcd_print("  Password Not  ", LINE1(0));
                clcd_print("     Changed    ", LINE2(0));
                __delay_ms(1000);
                count = ind = new_password = confirm_password = mode = wait = 0;
                attempt = 3;
                pass_flag = 2;
                sec = 60;
                return;
            }
        }
    }
}