/* 
 * File:   keypad.h
 * Author: palan
 *
 * Created on December 14, 2023, 12:59 PM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

//Macro definition for mode of the key
#define LEVEL 0
#define EDGE 1

//Macro definition for Row keys
#define R1 RB5
#define R2 RB6
#define R3 RB7

//Macro definition for Column keys
#define C1 RB1
#define C2 RB2
#define C3 RB3
#define C4 RB4

//Function declarations for matrix keypad
void init_matrix_keypad();
unsigned char read_switches(unsigned char );
unsigned char scan_key();

#endif	/* KEYPAD_H */