/* 
 * File:   adc.h
 * Author: palan
 *
 * Created on December 14, 2023, 1:05 PM
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

void init_adc();
unsigned short read_adc(unsigned char channel);

#endif	/* ADC_H */