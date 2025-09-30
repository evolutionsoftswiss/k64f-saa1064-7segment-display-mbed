#include <string>
#include "mbed.h"
#include "SAA1064.h"

SAA1064::SAA1064(PinName sda, PinName scl, uint8_t deviceAddress)
{
    i2c = new I2C(sda, scl);
    slaveAddress = deviceAddress;
    init();
}

SAA1064::SAA1064(I2C *i2c, uint8_t deviceAddress)
{
    this->i2c = i2c;
    slaveAddress = deviceAddress;
    init();
}

uint8_t getSegment(char c) {
    auto it = SEGMENT_MAP.find(c);
    if (it != SEGMENT_MAP.end()) return it->second;
    return ENCODED_SPACE; // blank for unsupported characters
}

void SAA1064::writeString(string value) {

    int textLength = value.length();

    if (textLength > 0) {

        int scrollPosition = 0;
        int totalCollapsedPoints = 0;
        int currentPoints = 0;
        do {
            char encodedCharacters[] = { getSegment(value.at(scrollPosition)),
                                         static_cast<char>(scrollPosition + 1 < textLength ? getSegment(value.at(scrollPosition + 1)) : ENCODED_SPACE),
                                         static_cast<char>(scrollPosition + 2 < textLength ? getSegment(value.at(scrollPosition + 2)) : ENCODED_SPACE),
                                         static_cast<char>(scrollPosition + 3 < textLength ? getSegment(value.at(scrollPosition + 3)) : ENCODED_SPACE) };
            currentPoints = handlePointCharacters(value, encodedCharacters, textLength, scrollPosition);
            
            write(encodedCharacters[0], encodedCharacters[1], encodedCharacters[2], encodedCharacters[3]);
            wait_us(TEXT_SHIFT_DELAY);
            
            scrollPosition++;
            if (scrollPosition + NUMBER_OF_DIGITS + totalCollapsedPoints + currentPoints - 1 > textLength) {

                break;
            }
            if (value[scrollPosition] == POINT && value[scrollPosition - 1] != POINT) {
  
                scrollPosition++;
                totalCollapsedPoints++;
            } 

        } while (scrollPosition + NUMBER_OF_DIGITS <= textLength);
    }
}

void SAA1064::write(uint8_t encodedChar1, uint8_t encodedChar2, uint8_t encodedChar3, uint8_t encodedChar4)
{
    arrangeSegmentBitsForI2C(encodedChar1, encodedChar2, encodedChar3, encodedChar4);
    i2c -> write(slaveAddress, (char*) data, I2C_SEGMENT_WRITE_SIZE);
}

int SAA1064::handlePointCharacters(string value, char *encodedCharacters, int textLength, int positionInText) {
          
    int collapsedPoints = 0;
    if (value[positionInText + 1] == POINT && 
        value[positionInText] != POINT) {

        encodedCharacters[0] |= getSegment(POINT);
        encodedCharacters[1] = encodedCharacters[2];
        encodedCharacters[2] = encodedCharacters[3];
        encodedCharacters[3] = positionInText + NUMBER_OF_DIGITS < textLength ?
                                 getSegment(value.at(positionInText + NUMBER_OF_DIGITS)) :
                                 ENCODED_SPACE; 
        collapsedPoints++;
     }

    if (value[positionInText + collapsedPoints + 2] == POINT && 
        value[positionInText + collapsedPoints + 1] != POINT) {

        encodedCharacters[1] |= getSegment(POINT);
        encodedCharacters[2] = encodedCharacters[3];
        encodedCharacters[3] = positionInText + collapsedPoints + NUMBER_OF_DIGITS < textLength ?
                                 getSegment(value.at(positionInText + collapsedPoints + NUMBER_OF_DIGITS)) :
                                 ENCODED_SPACE;
        collapsedPoints++;
    }

    if (value[positionInText + collapsedPoints + 3] == POINT && 
        value[positionInText + collapsedPoints + 2] != POINT) {

        encodedCharacters[2] |= getSegment(POINT);
        encodedCharacters[3] = positionInText + collapsedPoints + NUMBER_OF_DIGITS < textLength ?
                                getSegment(value.at(positionInText + collapsedPoints + NUMBER_OF_DIGITS)) :
                                ENCODED_SPACE;
        collapsedPoints++;
    }

    if (positionInText + collapsedPoints + NUMBER_OF_DIGITS < textLength && 
        value[positionInText + collapsedPoints + NUMBER_OF_DIGITS] == POINT && 
        value[positionInText + collapsedPoints + 3] != POINT) {
 
        encodedCharacters[3] = encodedCharacters[3] | getSegment(POINT);
        collapsedPoints++;
    }

    return collapsedPoints;
}

void SAA1064::arrangeSegmentBitsForI2C(uint8_t encodedChar1, uint8_t encodedChar2, uint8_t encodedChar3, uint8_t encodedChar4) {

    data[0] = 1;
    data[1] = ((encodedChar4 << 4) & 0xF0) | (encodedChar2 & 0x0F);
    data[2] = ((encodedChar3 << 4) & 0xF0) | (encodedChar1 & 0x0F);
    data[3] = ((encodedChar2 >> 4) & 0x0F) | (encodedChar4 & 0xF0);
    data[4] = ((encodedChar1 >> 4) & 0x0F) | (encodedChar3 & 0xF0);
}

void SAA1064::init( void )
{
    // init
    data[0] = 0x00;
    data[1] = 0x17;
    i2c -> write(slaveAddress, (char*) data, I2C_CONTROL_COMMAND_SIZE);

    // full display
    data[0] = 1;
    data[1] = 0xFF;
    data[2] = 0xFF;
    data[3] = 0xFF;
    data[4] = 0xFF;
    i2c -> write(slaveAddress, (char*) data, I2C_SEGMENT_WRITE_SIZE);
}