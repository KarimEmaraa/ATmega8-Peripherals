/*
 * EEPROM.h
 *
 * Created: 8/9/2017 7:26:02 PM
 *  Author: karim
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_


#include <stdint.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>

//SET it to one 
#ifdef SPMEN_CHECK_ENABLE
	#define check()\
		do {} while( SPMCSR & (1<<SPMEN) )
#endif

// These EEPROM bits have different names on different devices. 
#ifndef EEPE
	#define EEPE  EEWE  //!< EEPROM write enable.
	#define EEMPE EEMWE //!< EEPROM master write enable.
#endif
//set to one to enable Self programming bit check



void EE_Write( uint16_t addr, const uint8_t data);
char EE_Read( uint16_t addr);
void EE_ReadBlock(uint16_t  startAddr, char * const Arr, const uint8_t size);
void EE_WriteBlock(uint16_t  startAddr, const char * const Arr, const uint8_t size);

#endif /* EEPROM_H_ */

