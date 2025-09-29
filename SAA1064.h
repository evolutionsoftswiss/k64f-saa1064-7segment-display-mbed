#include <string>
#include <unordered_map>

#ifndef _SAA1064_H
#define _SAA1064_H

// Base address for SAA1064
#define SAA1064_SA0 0x70
#define NUMBER_OF_DIGITS 4
// Two bytes 
#define I2C_CONTROL_COMMAND_SIZE 2
// Five bytes
#define I2C_SEGMENT_WRITE_SIZE 5
#define SPACE ' '
#define ENCODED_SPACE 0x00
#define POINT '.'
// One second in microseconds
#define TEXT_SHIFT_DELAY 1000000

const std::unordered_map<char, uint8_t> SEGMENT_MAP = {
    // Digits
    {'0', 0xE7}, {'1', 0x84}, {'2', 0xD3}, {'3', 0xD6}, {'4', 0xB4},
    {'5', 0x76}, {'6', 0x77}, {'7', 0xC4}, {'8', 0xF7}, {'9', 0xF6},

    // Uppercase letters
    {'A', 0xF5}, {'B', 0xF7}, {'C', 0x63}, {'D', 0x97}, {'E', 0x73},
    {'F', 0x71}, {'G', 0x75}, {'H', 0xB5}, {'I', 0x84}, {'J', 0x86},
    {'K', 0xB5}, {'L', 0x23}, {'M', 0x95}, {'N', 0x15}, {'O', 0xE7},
    {'P', 0xF1}, {'Q', 0xF9}, {'R', 0xF5}, {'S', 0x76}, {'T', 0x33},
    {'U', 0xA7}, {'V', 0xA7}, {'W', 0xB6}, {'X', 0xB5}, {'Y', 0xB6},
    {'Z', 0xD3},

    // Lowercase letters
    {'a', 0xF5}, {'b', 0x37}, {'c', 0x13}, {'d', 0x97}, {'e', 0x73},
    {'f', 0x71}, {'g', 0x75}, {'h', 0x35}, {'i', 0x04}, {'j', 0x86},
    {'k', 0x34}, {'l', 0x03}, {'m', 0x15}, {'n', 0x15}, {'o', 0x17},
    {'p', 0xF1}, {'q', 0xF9}, {'r', 0x11}, {'s', 0x76}, {'t', 0x33},
    {'u', 0x07}, {'v', 0x07}, {'w', 0xB6}, {'x', 0xB5}, {'y', 0xB6},
    {'z', 0xD3},

    // Special chars
    {'-', 0x10}, {'_', 0x02}, {POINT, 0x08}, {SPACE, ENCODED_SPACE}
};

class SAA1064
{
public:

    /** Constructs a SAA1064 LED display driver using specified SDA and SCL pins. 
     *
     * @param sda Pin
     * @param scl Pin
     * @param deviceAddress the address of the SAA1064
    */
    SAA1064( PinName sda = D14, PinName scl = D15, uint8_t deviceAddress = SAA1064_SA0 );
 
    /** Creates a SAA1064 LED display driver object using a specified I2C bus and device address.
     *
     * @param sda Pin
     * @param scl Pin
     * @param deviceAddress the address of the SAA1064
    */
    SAA1064( I2C* i2c, uint8_t deviceAddress = SAA1064_SA0 );
    
    /**
    * Writes string value to the display using scrolling to display more than 4 characters.
    *
    * @param value string to print on display
    */
    void writeString (std::string value);

    /** Writes exactly 4 encoded chars to display. Shorter strings are filled with spaces.
    *
    * @param encodedChar1  LED segment mask for char1 (MSB)
    * @param encodedChar2  LED segment mask for char2
    * @param encodedChar3  LED segment mask for char3
    * @param encodedChar4  LED segment mask for char4 (LSB)
    */
    void write(uint8_t encodedChar1, uint8_t encodedChar2, uint8_t encodedChar3, uint8_t encodedChar4);

protected:

    /** 
    * Initialize I2C bus with brightness 0x10 and multiplex for 4 digits with 0x07 resulting in 0x17.
    */
    void init( void );

    /**
    * Special handling of point character to support output strings of decimal point numbers 
    * and point character in text without additional space. 
    * 
    * @param value
    * @param encodedCharacters
    * @param textLength
    * @param positionInText
    */
    int handlePointCharacters(std::string value, char encodedCharacters[], int textLength, int n);

    /**
    * Split each character's segment byte into upper and lower nibbles to conform i2c packaging.
    * data[0] Command/register byte (here = 1)
    * data[1] Upper half of digit4 | lower half of digit2
    * data[2] Upper half of digit3 | lower half of digit1
    * data[3] Lower half of digit2 | upper half of digit4
    * data[4] Lower half of digit1 | upper half of digit3
    *
    * @param encodedChar1  LED segment pattern for digit1 (MSB)
    * @param encodedChar2  LED segment pattern for digit2
    * @param encodedChar3  LED segment pattern for digit3
    * @param encodedChar4  LED segment pattern for digit4 (LSB)
    */
    void arrangeSegmentBitsForI2C(uint8_t encodedChar1, uint8_t encodedChar2, uint8_t encodedChar3, uint8_t encodedChar4);

    I2C *i2c;                    //I2C bus reference
    uint8_t slaveAddress;        //I2C slave address of device
    uint8_t data[I2C_SEGMENT_WRITE_SIZE];

};

#endif