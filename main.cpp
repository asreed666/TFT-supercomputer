#include "mbed.h"
#include "TextLCD.h"

TextLCD lcd(p9, p10);

int main() {
    lcd.printf("Hi mbed World!\n");
}
