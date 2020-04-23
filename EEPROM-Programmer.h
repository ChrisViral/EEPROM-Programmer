#pragma once

#pragma region Type definitions
//Only to calm Intellisense
using uint8_t = unsigned char;

/**
 * \brief Unsigned 16bit integer type
 */
typedef unsigned short ushort;
#pragma endregion

#pragma region Constants
/** \brief Shift register data pin */
constexpr byte SHIFT_DATA   = 2;
/** \brief Shift register clock pin */
constexpr byte SHIFT_CLOCK  = 3;
/** \brief Shift register latch clock pin */
constexpr byte LATCH_CLOCK  = 4;
/** \brief Data pin 0 */
constexpr byte DATA0        = 5;
/** \brief Data pin 1 */
constexpr byte DATA1        = 6;
/** \brief Data pin 2 */
constexpr byte DATA2        = 7;
/** \brief Data pin 3 */
constexpr byte DATA3        = 8;
/** \brief Data pin 4 */
constexpr byte DATA4        = 9;
/** \brief Data pin 5 */
constexpr byte DATA5        = 10;
/** \brief Data pin 6 */
constexpr byte DATA6        = 11;
/** \brief Data pin 7 */
constexpr byte DATA7        = 12;
/** \brief Write enable pin */
constexpr byte WRITE_ENABLE = 13;
/** \brief Output enable pin */
constexpr byte OUTPUT_ENABLE = A0;
/** \brief Size of the EEPROM's memory */
constexpr ushort SIZE = 2048;
#pragma endregion

#pragma region Static methods
/**
 * \brief Sets the output enabled signal
 * \param enabled If the output is currently enabled or not
 */
static void setOutputEnable(bool enabled);

/**
 * \brief Sets the mode of all data pins
 * \param mode Pin mode to set
 */
static void setDataPinModes(byte mode);

/**
 * \brief Sends the specified address to the shift register, in most significant bit order
 * \param address Address to set
 */
static void setAddress(ushort address);

/**
 * \brief Reads the content of the EEPROM at the specified address
 * \param address Address to read at
 */
static byte readAtAddress(ushort address);

/**
 * \brief Writes the specified value in the EEPROM at the given address
 * \param address Address to write the value to
 * \param value Value to write
 */
static void writeAtAddress(ushort address, byte value);

/**
 * \brief Writes the specified data sequentially to the EEPROM
 * \param data Data to write
 * \param size Size of the passed data array
 */
static void writeData(byte data[], int size);

/**
 * \brief Prints the contents of the EEPROM in lines of 16 bytes
 * \param lines How many lines of 16 bytes to print, between 1 and 128, defaults to 16
 */
static void printContents(ushort lines = 16);
#pragma endregion