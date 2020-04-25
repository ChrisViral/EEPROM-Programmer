#pragma once
#include <Arduino.h>
typedef unsigned short ushort;

class Programmer
{
public:
	#pragma region Constants
	static constexpr byte DEFAULT_VALUE = 0xFF;
	static constexpr ushort SIZE        = 2048;
	#pragma endregion

	#pragma region Constructors
	Programmer();

	Programmer(const Programmer&) = default;

	Programmer(Programmer&&) = default;

	virtual ~Programmer() = 0;
	#pragma endregion

	#pragma region Abstract methods
	/**
	 * \brief Runs the programmer routine
	 */
	virtual void run() = 0;
	#pragma endregion

	#pragma region Opertators
    Programmer& operator=(const Programmer&) = default;

	Programmer& operator=(Programmer&&) = default;
	#pragma endregion

protected:
	#pragma region Fields
	bool outputEnabled = false;
	ushort currentAddress = SIZE + 1;
	byte currentMode = INPUT;
	#pragma endregion

	#pragma region Methods
	/**
	 * \brief Sets the output enabled signal
	 * \param enabled If the output is currently enabled or not
	 */
	void setOutputEnabled(bool enabled);

	/**
	 * \brief Sets the mode of all data pins
	 * \param mode Pin mode to set
	 */
	void setDataPinModes(byte mode);

	/**
	 * \brief Sends the specified address to the shift register, in most significant bit order
	 * \param address Address to set
	 */
	void setAddress(ushort address);

	/**
	 * \brief Reads the content of the EEPROM at the specified address
	 * \param address Address to read at
	 */
	[[nodiscard]]
	byte readAtAddress(ushort address);

	/**
	 * \brief Writes the specified value in the EEPROM at the given address
	 * \param address Address to write the value to
	 * \param value Value to write
	 */
	void writeAtAddress(ushort address, byte value);

	/**
	 * \brief Writes the specified data sequentially to the EEPROM
	 * \param data Data to write
	 * \param size Size of the passed data array
	 * \param start Starting address in the EEPROM to write at
	 */
	void writeData(const byte* data, ushort size, ushort start = 0);

	/**
	 * \brief Prints the contents of the EEPROM in lines of 16 bytes
	 * \param lines How many lines of 16 bytes to print, between 1 and 128, defaults to a full print at 128 lines
	 */
	void printContents(ushort lines = 128);

	/**
	 * \brief Clears the memory of the EEPROM by resetting every address to it's default, 0xFF
	 * \param bytes Amount of bytes to clear, defaults to a full clear at 2048
	 */
	void clear(ushort bytes = SIZE);
	#pragma endregion
};
