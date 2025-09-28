#include <unordered_map>
#include <cstdint>

#ifndef _SAA1064_H
#define _SAA1064_H

//Base address and others for SAA1064
#define SAA1064_SA0 0x70

const std::unordered_map<char, uint8_t> SEGMENT_MAP = {
    // Digits
    {'0', 0xE7}, {'1', 0x84}, {'2', 0xD3}, {'3', 0xD6}, {'4', 0xB4},
    {'5', 0x76}, {'6', 0x77}, {'7', 0xC4}, {'8', 0xF7}, {'9', 0xF6},

    // Uppercase letters
    {'A', 0xF5}, {'B', 0xF7}, {'C', 0x63}, {'D', 0x97}, {'E', 0x73},
    {'F', 0x71}, {'G', 0x75}, {'H', 0xB5}, {'I', 0x84}, {'J', 0x86},
    {'K', 0xB5}, {'L', 0x23}, {'M', 0x95}, {'N', 0x15}, {'O', 0xE7},
    {'P', 0xF1}, {'Q', 0xF9}, {'R', 0xF5}, {'S', 0x76}, {'T', 0x33},
    {'U', 0xA7}, {'V', 0xA7}, {'W', 0xB6}, {'X', 0x15}, {'Y', 0xB6},
    {'Z', 0xD3},

    // Lowercase letters
    {'a', 0xF5}, {'b', 0x37}, {'c', 0x13}, {'d', 0x97}, {'e', 0x73},
    {'f', 0x71}, {'g', 0x75}, {'h', 0x35}, {'i', 0x04}, {'j', 0x86},
    {'k', 0x34}, {'l', 0x03}, {'m', 0x15}, {'n', 0x15}, {'o', 0x17},
    {'p', 0xF1}, {'q', 0xF9}, {'r', 0x11}, {'s', 0x76}, {'t', 0x33},
    {'u', 0x07}, {'v', 0x07}, {'w', 0xB6}, {'x', 0x15}, {'y', 0xB6},
    {'z', 0xD3},

    // Special chars
    {'-', 0x10}, {'_', 0x02}, {'.', 0x08}, {' ', 0x00}
}; 

/** Create an SAA1064 object connected to the specified I2C bus and deviceAddress
*/
class SAA1064
{
public:

    /** Create a SAA1064 LED displaydriver object using a specified I2C bus and slaveaddress
     *
     * @param sda Pin
     * @param scl Pin
     * @param char deviceAddress the address of the SAA1064
    */
    SAA1064( PinName sda = D14, PinName scl = D15, uint8_t deviceAddress = SAA1064_SA0 );

    SAA1064( I2C* i2c, uint8_t deviceAddress = SAA1064_SA0 );

    /** Write digits
    *
    * @param digit1  LED segment pattern for digit1 (MSB)
    * @param digit2  LED segment pattern for digit2
    * @param digit3  LED segment pattern for digit3
    * @param digit4  LED segment pattern for digit4 (LSB)
    */
    void write(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4);

    void writeString (char const *value);

protected:
    void init( void );
    void writeShortString(char const *value);

    I2C *i2c;                    //I2C bus reference
    uint8_t slaveAddress;        //I2C Slave address of device
    uint8_t data[6];

};

#endif