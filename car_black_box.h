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
unsigned char *menu[5] = {"View log", "Down - log", "Clear log", "Set time", "Change pass"};

void display_time();
void display_event(unsigned char );
void display_speed(unsigned short );
void enter_password();
void init_timer0();

#endif	/* CAR_BLACK_BOX_H */