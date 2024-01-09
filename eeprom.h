/* 
 * File:   eeprom.h
 * Author: palan
 *
 * Created on December 27, 2023, 2:03 PM
 */

#ifndef EEPROM_H
#define EEPROM_H

#define SLAVE_READ_EEPROM		0xA1
#define SLAVE_WRITE_EEPROM		0xA0

#define EEPROM_ADDRESS 0x00
#define EEPROM_ADDRESS_1 0xAA

void write_ext_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_ext_eeprom(unsigned char address1);
void store_event(unsigned char *data);
void read_event(unsigned char *address, unsigned char *data);

#endif