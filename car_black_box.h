/* 
 * File:   car_black_box.h
 * Author: palan
 *
 * Created on December 14, 2023, 1:01 PM
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

unsigned char *signature[8] = {"ON", "GN", "G1", "G2", "G3", "G4", "GR", "C "};
unsigned char *menu_opt[5] = {"View log     ", "Set time     ", "Download log ", "Clear log    ", "Change pass  "};
unsigned char *menu_eve[5] = {"VL", "ST", "DL", "CL", "CP"};
unsigned char event[17];

void display_time();
void display_event(unsigned char );
void display_speed(unsigned short );
void enter_password(unsigned char );
void menu(unsigned char );
void store_event(unsigned char, unsigned char *);
void init_timer0();

#endif	/* CAR_BLACK_BOX_H */