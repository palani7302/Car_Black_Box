/*
 * File:   eeprom.c
 * Author: palan
 *
 * Created on December 27, 2023, 2:03 PM
 */


#include "eeprom.h"
#include "i2c.h"
#include "clcd.h"
#include <xc.h>

/* 
 * EEPROM Slave address
 * A0  -  Write Mode
 * A1  -  Read Mode
 */

extern unsigned char count_eve, total_events;

void write_ext_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE_EEPROM);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    
    __delay_ms(10);
}

unsigned char read_ext_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE_EEPROM);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ_EEPROM);
	data = i2c_read();
	i2c_stop();

	return data;
}

void store_event(unsigned char *data)
{   
    unsigned char address = EEPROM_ADDRESS + (16 * count_eve);
    for (unsigned char i = 0; i < 16; i++)
    {
        write_ext_eeprom(address+i, data[i]);
    }
    
    if  (count_eve++ == 9)
    {
        count_eve = 0;
        total_events = 10;
    }
    else if (total_events < 10)
        total_events++;
    
    return;
}

void read_event(unsigned char *address, unsigned char *data)
{
    for ( unsigned char i = 0; i < 16; i++ )
    {
        data[i] = read_ext_eeprom(address+i);
    }
    data[17] = '\0';
}