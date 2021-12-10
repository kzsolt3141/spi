#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

int main(void) {
    uint8_t sts = 0;

    // UART INIT
    //-------------------------------
    const uint16_t baud_rate = 38400;

    sts = USART_init(baud_rate, 0);
    if (sts) return sts;

    printf("Init Done UART baud: %u\n", (uint16_t)baud_rate);
    //-------------------------------

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
        printf("r:%d, s:%d\n", PORTC, i);
    }
}
