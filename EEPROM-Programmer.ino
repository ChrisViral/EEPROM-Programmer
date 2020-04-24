/* ============================== *\
 * Name:	EEPROM_Programmer.ino *
 * Created: 4/22/2020 5:58:23 PM  *
 * Author:	ChrisViral            *
\* ============================== */

#include "EEPROM-Programmer.h"

const byte displayData[16] = { 0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04, 0x08, 0x60, 0x31, 0x42, 0x30, 0x38 };

/**
 * \brief One time setup of the Arduino board
 */
void setup()
{
	//Setup pin modes
	pinMode(SHIFT_DATA,  OUTPUT);
	pinMode(SHIFT_CLOCK, OUTPUT);
	pinMode(LATCH_CLOCK, OUTPUT);

	//Setup flag pins
	digitalWrite(WRITE_ENABLE, HIGH);
	pinMode(WRITE_ENABLE, OUTPUT);
	setOutputEnable(true);
	pinMode(OUTPUT_ENABLE, OUTPUT);

	//Serial debugging
	Serial.begin(57600);

	clear();
	writeData(displayData, 16);
	printContents();
}

/**
 * \brief Arduino loop method
 */
void loop() { }

void setOutputEnable(const bool enabled)
{
	//The pin is active low, so set accordingly
	digitalWrite(OUTPUT_ENABLE, enabled ? LOW : HIGH);
}

static void setDataPinModes(const byte mode)
{
	//Set mode for all data pins
	for (byte pin = DATA0; pin <= DATA7; pin++)
	{
		pinMode(pin, mode);
	}
}

void setAddress(const ushort address)
{
	//Put value into register
	shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, highByte(address));
	shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, lowByte(address));

	//Latch value by pulsing latch clock
	digitalWrite(LATCH_CLOCK, HIGH);
	delayMicroseconds(5);
	digitalWrite(LATCH_CLOCK, LOW);
}

byte readAtAddress(const ushort address)
{
	//Shift in address
	setAddress(address);

	//Read contents
	byte value = 0;
	for (byte pin = DATA7; pin >= DATA0; pin--)
	{
		value = (value << 1) | digitalRead(pin);
	}

	return value;
}

void writeAtAddress(const ushort address, byte value)
{
	//Shift in address and disable output
	setAddress(address);
	setOutputEnable(false);
	setDataPinModes(OUTPUT);

	//Get most significant bit for data polling
	byte msb = value >> 7;

	//Write data to pins
	for (byte pin = DATA0; pin <= DATA7; pin++)
	{
		digitalWrite(pin, value & 1);
		value >>= 1;
	}

	//Pulse write signal
	digitalWrite(WRITE_ENABLE, LOW);
	delayMicroseconds(1);
	digitalWrite(WRITE_ENABLE, HIGH);

	//Tested extensively, this comes out to be the average write time and causes the less average polling cycles
	delayMicroseconds(2240);

	//Wait for write cycle end by checking with data polling
	setDataPinModes(INPUT);
	setOutputEnable(true);
	while (digitalRead(DATA7) != msb)
	{
		delayMicroseconds(10);
	}
}

void writeData(const byte data[], const int size)
{
	//Write sequentially through array
	for (ushort address = 0; address < size; address++)
	{
		writeAtAddress(address, data[address]);
	}
}

void printContents(ushort lines)
{
	//Constants
	constexpr ushort stride = 16;
	constexpr int lineSize = (3 * stride) + 7;

	//Setup buffer
	lines = constrain(lines, 1, 128);
	const ushort size = lines * stride;

	//Loop in lines
	for (ushort i = 0; i < size; i += stride)
	{
		//Line header
		char* buffer = new char[lineSize];
	    int length = sprintf(buffer, "%03X: ", i);
		for (ushort j = 0; j < stride; j++)
		{
			//Write byte
			length += sprintf(buffer + length, j == 7 ? " %02X " : " %02X", readAtAddress(i + j));
		}

		//Endline
		Serial.println(buffer);
		delete buffer;
	}
}

void clear(ushort bytes)
{
	//Go through the memory and set all bytes to 0xFF
	bytes = min(bytes, SIZE);
	for (ushort address = 0; address < bytes; address++)
	{
		writeAtAddress(address, 0xFF);
	}
}