#include "DecoderProgrammer.h"

/*
 * Pin layout:
 *    ___
 *   | 4 |
 *  5|___|3
 *   | 6 |
 *  0|___|2
 *     1
 *
 * Pin 7 is unused
 */

/** \brief Maximum value of the unsigned byte range */
constexpr ushort MAX_VALUE = 256;
/** \brief Maximum value of the signed byte range */
constexpr short SMAX_VALUE = 128;
/** \brief Seven Segment decoder data */
constexpr byte DIGITS_DATA[10] =
{
		0b0111111, //0
		0b0001100, //1
		0b1011011, //2
		0b1011110, //3
		0b1101100, //4
		0b1110110, //5
		0b1110111, //6
		0b0011100, //7
		0b1111111, //8
		0b1111110  //9
};
/** \brief Blank display value */
constexpr byte BLANK = 0b0000000;    //Empty
/** \brief Negative display value */
constexpr byte NEG   = 0b1000000;    //-

void DecoderProgrammer::run()
{
	//Setup data array
	byte* data = new byte[MAX_VALUE];

	short pow = 1;
	ushort digit = 0;
	for (/*digit = 0*/; digit < 4; digit++)
	{
		Serial.println("Writing data for digit " + String(digit));

		for (ushort num = 0; num < MAX_VALUE; num++)
		{
			//Write the value of the specified digit, blank for sign position
			data[num] = digit == 3 ? BLANK : DIGITS_DATA[(num / pow) % 10];
		}
		pow *= 10;

		//Write all the data to the EEPROM
		writeData(data, MAX_VALUE, digit << 8);
	}

	pow = 1;
	for (/*digit = 4*/; digit < 8; digit++)
	{
		Serial.println("Writing signed data for digit " + String(digit - 4));

		for (short num = -128; num < SMAX_VALUE; num++)
		{
			//Write the value of the specified digit, or the sign if needed
			data[static_cast<byte>(num)] = digit == 7 ? (num < 0 ? NEG : BLANK) : DIGITS_DATA[abs(num / pow) % 10];
		}
		pow *= 10;

		//Write all the data to the EEPROM
		writeData(data, MAX_VALUE, digit << 8);
	}

	//Clear memory and print
	delete[] data;
	printContents();
}
