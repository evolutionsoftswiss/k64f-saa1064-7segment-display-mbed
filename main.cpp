#include "mbed.h"
#include "SAA1064.h"

SAA1064 display;

int main() {
 
    while (true) {
        wait_us(TEXT_SHIFT_DELAY);
        display.writeString(" .HELLO. ");
        display.writeString("123456");
        display.writeString("-0.9791");
        display.writeString("-_-");
    }
}