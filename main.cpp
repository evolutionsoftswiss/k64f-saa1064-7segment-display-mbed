
#include "mbed.h"
#include "SAA1064.h"

SAA1064 display(D14, D15);

int main() {
    while (true) {
        char const *text = "HELLO. ";
        display.writeString(text);
    }
}