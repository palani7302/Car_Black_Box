/* 
 * File:   uart.h
 * Author: palan
 *
 * Created on January 8, 2024, 5:29 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

void init_uart();
void putch(unsigned char data);
void puts(unsigned char *data);
unsigned char getch();
unsigned char getche();

#endif	/* UART_H */