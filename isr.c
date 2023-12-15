/*
 * File:   isr.c
 * Author: palan
 *
 * Created on December 15, 2023, 1:16 PM
 */


#include <xc.h>

unsigned int count = 0;
unsigned char sec = 60;

void __interrupt() isr()
{
    TMR0 = TMR0 + 3038; //16-bit timer
    //TMR0 = TMR0 + 8; //8-bit timer
    
    if ( TMR0IF )
    {
        if ( count++ == 80 )
        {
            if ( sec-- == 0 )
            {
                sec = 60;
            }
            count = 0;
        }
        TMR0IF = 0;
    }
}