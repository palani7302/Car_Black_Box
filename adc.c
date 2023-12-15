/*
 * File:   adc.c
 * Author: palan
 *
 * Created on December 14, 2023, 1:06 PM
 */


#include <xc.h>
#include "adc.h"

void init_adc()
{
    ADON = 1;
    VCFG1 = 0;
    VCFG0 = 0;
    PCFG3 = 1;
    PCFG2 = 0;
    PCFG1 = 1;
    PCFG0 = 0;
    ACQT2 = 1;
    ACQT1 = 0;
    ACQT0 = 0;
    ADCS2 = 0;
    ADCS1 = 1;
    ADCS0 = 0;
    ADFM = 1;
}

unsigned short read_adc(unsigned char channel)
{
    ADCON0 = (ADCON0 & 0xC3) | (channel << 2);
    
    GO = 1;
    while ( GO )
    {
        return (ADRESL | ((ADRESH & 0x03) << 8));
    }
}