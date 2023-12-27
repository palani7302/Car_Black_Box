/*
 * File:   eeprom.c
 * Author: palan
 *
 * Created on December 27, 2023, 2:03 PM
 */


#include "eeprom.h"
#include "i2c.h"
#include <xc.h>

/* 
 * EEPROM Slave address
 * A0  -  Write Mode
 * A1  -  Read Mode
 */

void write_ext_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE_EEPROM);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
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