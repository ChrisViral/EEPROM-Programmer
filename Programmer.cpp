#include "Programmer.h"
#include "Pins.h"


Programmer::Programmer()
{
	//Setup pin modes
	pinMode(SHIFT_DATA,  OUTPUT);
	pinMode(SHIFT_CLOCK, OUTPUT);
	pinMode(LATCH_CLOCK, OUTPUT);

	//Setup flag pins
	digitalWrite(WRITE_ENABLE, HIGH);
	pinMode(WRITE_ENABLE, OUTPUT);
	setOutputEnabled(true);
	pinMode(OUTPUT_ENABLE, OUTPUT);
}

Programmer::~Programmer() = default;

void Programmer::setOutputEnabled(const bool enabled)
{
	if (outputEnabled != enabled)
	{
		outputEnabled = enabled;
		//The pin is active low, so set accordingly
		digitalWrite(OUTPUT_ENABLE, enabled ? LOW : HIGH);
	}
}

void Programmer::setDataPinModes(const byte mode)
{
	if (currentMode != mode)
	{
		currentMode = mode;
		//Set mode for all data pins
		for (byte pin = DATA0; pin <= DATA7; pin++)
		{
			pinMode(pin, mode);
		}
	}
}

void Programmer::setAddress(const ushort address)
{
	if (currentAddress != address)
	{
		currentAddress = address;
		//Put value into register
		shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, highByte(address));
		shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, lowByte(address));

		//Latch value by pulsing latch clock
		digitalWrite(LATCH_CLOCK, HIGH);
		delayMicroseconds(5);
		digitalWrite(LATCH_CLOCK, LOW);
	}
}

byte Programmer::readAtAddress(const ushort address)
{
	//Shift in address
	setAddress(address);
	setDataPinModes(INPUT);
	setOutputEnabled(true);

	//Read contents
	byte value = 0;
	for (byte pin = DATA7; pin >= DATA0; pin--)
	{
		value = (value << 1) | digitalRead(pin);
	}

	return value;
}

void Programmer::writeAtAddress(const ushort address, byte value)
{
	//Shift in address and disable output
	setAddress(address);
	setOutputEnabled(false);
	setDataPinModes(OUTPUT);

	//Get most significant bit for data polling
	int msb = value >> 7;

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
	setOutputEnabled(true);
	while (digitalRead(DATA7) != msb)
	{
		delayMicroseconds(10);
	}
	setOutputEnabled(false);
	setDataPinModes(OUTPUT);
}

void Programmer::writeData(const byte* data, const ushort size, const ushort start)
{
	//Write sequentially through array
	Serial.print("Writing...");
	for (ushort address = 0; address < size; address++)
	{
		writeAtAddress(address + start, data[address]);
		if (address % 64 == 0)
		{
			Serial.print('.');
		}
	}
	Serial.println(" complete!");
}

void Programmer::printContents(ushort lines)
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
		delete[] buffer;
	}
}

void Programmer::clear(ushort bytes)
{
	//Go through the memory and set all bytes to 0xFF
	Serial.print("Clearing ...");
	bytes = min(bytes, SIZE);
	for (ushort address = 0; address < bytes; address++)
	{
		writeAtAddress(address, DEFAULT_VALUE);
		if (address % 64 == 0)
		{
			Serial.print('.');
		}
	}
	Serial.println(" complete!");
}
