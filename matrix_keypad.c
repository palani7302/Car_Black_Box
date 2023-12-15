/*
 * File:   matrix_keypad.c
 * Author: palan
 *
 * Created on December 14, 2023, 1:00 PM
 */


#include <xc.h>
#include "keypad.h"

//Matrix keypad configuration
void init_matrix_keypad()
{
    TRISB = 0x1E;
    RBPU = 0;
}

//Reading the switches function definition 
unsigned char read_switches(unsigned char detection)
{
    static unsigned char once = 1, key;
    
    //Level triggering
    if ( detection == 0 )
    {
        return scan_key();
    }
    
    //Edge triggering
    else if ( detection == 1 )
    {
        key = scan_key();
        
        if ( (key != 0xFF) && once )
        {
            once = 0;
            return key;
        }
        else if ( key == 0xFF )
        {
            once = 1;
        }
        
        return 0xFF;
    }
}

//Scanning key function definition
unsigned char scan_key()
{
    //Scanning key in the Row 1
    R1 = 0;
    R2 = 1;
    R3 = 1;
    
    //Checking for keys in the respective row
    if ( C1 == 0 )
        return 1;
    else if ( C2 == 0 )
        return 4;
    else if ( C3 == 0 )
        return 7;
    else if ( C4 == 0 )
        return 10;
    
    //Scanning key in the Row 1
    R1 = 1;
    R2 = 0;
    R3 = 1;
    
    //Checking for keys in the respective row
    if ( C1 == 0 )
        return 2;
    else if ( C2 == 0 )
        return 5;
    else if ( C3 == 0 )
        return 8;
    else if ( C4 == 0 )
        return 11;
    
    //Scanning key in the Row 1
    R1 = 1;
    R2 = 1;
    R3 = 0;
    R3 = 0;
    
    //Checking for keys in the respective row
    if ( C1 == 0 )
        return 3;
    else if ( C2 == 0 )
        return 6;
    else if ( C3 == 0 )
        return 9;
    else if ( C4 == 0 )
        return 12;
    
    return 0xFF;
}