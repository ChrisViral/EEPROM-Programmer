/* ============================== *\
 * Name:	EEPROM_Programmer.ino *
 * Created: 4/22/2020 5:58:23 PM  *
 * Author:	ChrisViral            *
\* ============================== */

#include "DecoderProgrammer.h"

/**
 * \brief One time setup of the Arduino board
 */
void setup()
{
	//Serial debugging
	Serial.begin(57600);

	//Start 7 segment decoder programmer
	DecoderProgrammer programmer;
	programmer.run();
}

/**
 * \brief Arduino loop method
 */
void loop() { }