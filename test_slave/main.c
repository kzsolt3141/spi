#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB &= ((1 << PB5) |  // MOSI
             (1 << PB7) |  // SKC
             (1 << PB4));   // SS
    DDRB |=  (1 << PB6);  //MISO

    SPCR = (1 << SPE);

    DDRC = 0xFF;
    uint8_t i = 0;

    while(1) {
        i++;
        i++;
        SPDR = i;
        while(!(SPSR & (1<<SPIF)));
        PORTC = SPDR;
    }
}
