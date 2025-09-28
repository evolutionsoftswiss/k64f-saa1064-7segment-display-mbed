#include "mbed.h"
#include "SAA1064.h"

#define NUMBER_OF_DIGITS 4
#define TEXT_SHIFT_DELAY 1000000
#define SPACE ' '

SAA1064::SAA1064( PinName sda, PinName scl, uint8_t deviceAddress )
{
    i2c = new I2C( sda, scl );
    slaveAddress = deviceAddress;
    init();
}


SAA1064::SAA1064( I2C *i2c, uint8_t deviceAddress )
{
    this->i2c = i2c;
    slaveAddress = deviceAddress;
    init();
}

uint8_t getSegment(char c) {
    auto it = SEGMENT_MAP.find(c);
    if (it != SEGMENT_MAP.end()) return it->second;
    return 0x00; // blank for unsupported characters
}

/** Write digits
*
* @param digit1  LED segment pattern for digit1 (MSB)
* @param digit2  LED segment pattern for digit2
* @param digit3  LED segment pattern for digit3
* @param digit4  LED segment pattern for digit4 (LSB)
*/
void SAA1064::write( uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4 )
{
    digit1 = getSegment(digit1);
    digit2 = getSegment(digit2);
    digit3 = getSegment(digit3);
    digit4 = getSegment(digit4);

    data[0] = 1;
    data[1] = ((digit4<<4) & 0xF0) | (digit2 & 0x0F);
    data[2] = ((digit3<<4) & 0xF0) | (digit1 & 0x0F);
    data[3] = ((digit2>>4) & 0x0F) | (digit4 & 0xF0);
    data[4] = ((digit1>>4) & 0x0F) | (digit3 & 0xF0);
    i2c->write( slaveAddress, (char*) data, 5 );
}

void SAA1064::writeString(char const *value) {

    int textLength = strlen(value);

    if (textLength > 0 && textLength <= NUMBER_OF_DIGITS) {

        writeShortString(value);
    }
    else if (textLength > 0) {

        int n = 0;
        do {
            write(value[n], value[n + 1], value[n + 2], value[n + 3]);
            wait_us(TEXT_SHIFT_DELAY);
            n++;
        } while (n + NUMBER_OF_DIGITS <= textLength);
    }
}

  void SAA1064::writeShortString(char const *value) {
        
    int textLength = strlen(value);
    if (textLength == 1) {
        write(value[0], SPACE, SPACE, SPACE);

    } else if (textLength == 2) {
        write(value[0], value[1], SPACE, SPACE);

    } else if (textLength == 3) {
       write(value[0], value[1], value[2], SPACE);

    } else if (textLength == 4) {
       write(value[0], value[1], value[2], value[3]);
    }
}
/** Init I2C bus 
*/
void SAA1064::init( void )
{
    // init
    data[0] = 0x00;
    data[1] = 0x17;
    i2c->write( slaveAddress, (char*) data, 2 );

    // blank display
    data[0] = 1;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    i2c->write( slaveAddress, (char*) data, 5 );
}