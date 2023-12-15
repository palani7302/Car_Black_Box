/*
 * File:   timer0.c
 * Author: palan
 *
 * Created on December 15, 2023, 1:16 PM
 */


#include <xc.h>

void init_timer0() 
{
    //interrupt
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;
    TMR0IF = 0;

    //timer
    TMR0 = 3036; //16-bit
    //TMR0 = 6;//8-bit
    TMR0ON = 1;
    T08BIT = 0; //16-bit
    //T08BIT = 1;//8-bit
    T0CS = 0;
    PSA = 1;
}