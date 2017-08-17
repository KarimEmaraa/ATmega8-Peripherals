/*
 * EEPROM.c
 *
 * Created: 8/9/2017 7:30:56 PM
 *  Author: karim
 */ 

#include "EEPROM.h"

void EE_Write(uint16_t addr, const uint8_t data)
{
	//Wait if there is a write operation
	while(EECR & (1<<EEPE));
	//check for self programming bit
	#ifdef SPMEN_CHECK_ENABLE
		check();
	#endif
	//Set up address and ensure no interrupts occur during assignment and write operation
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		EEAR = addr;
		//1 byte operation no problem
		NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE)
		{
			//set the data
			EEDR = data;
			//set mode to write only
			EECR |= ((1<< EEPM1) | (1<<EEMPE)) & (~(1<<EEPM0)); 
		}
		//Start write operation
		EECR |= (1<<EEPE);
		//Wait for access time to be elapsed
		do 
		{
			
		}while(EECR & (1<<EEPE));
		
	} 
}

char EE_Read(uint16_t addr)
{
	//Wait if there is a write operation
	do
	{
				
	}while(EECR & (1<<EEPE));
	//check for self programming bit
	#ifdef SPMEN_CHECK_ENABLE
	check();
	#endif
	//Set up address register and restore state of Global interrupt
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		EEAR = addr;
		//start reading
		EECR |= (1<<EERE);
		 _delay_ms(3);//cpu is halted for 4 clocks
	}
	return EEDR;
}
void EE_ReadBlock(uint16_t startAddr, char * const Arr, const uint8_t size)
{
	for (uint8_t i = 0; i < size; i++, startAddr++)
	{
		Arr[i] = EE_Read(startAddr);
	}
}

void EE_WriteBlock(uint16_t startAddr, const char * const Arr, const uint8_t size)
{
	uint8_t i = 0;
	do
	{
		EE_Write(startAddr, Arr[i]);
		i++;
		startAddr++;
	} while (i < size);
}