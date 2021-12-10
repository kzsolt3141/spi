#include "spi.h"

#include <stddef.h>

#include <avr/io.h>
#include <avr/interrupt.h>

static SPI_isr_cb  SPI_cb_     = NULL;
static void       *SPI_cb_ctx_ = NULL;
static uint8_t     isr_set_    = 0;

void regiter_SPI_isr_cb(SPI_isr_cb cb, void* ctx) {
    if (cb) {
        SPI_cb_ = cb;
    } else {
        return;
    }

    SPI_cb_ctx_ = ctx;
    isr_set_ = 1;
}

uint8_t SPI_init(
    SPI_clock_source clk_src,
    uint8_t          master,
    uint8_t          en_isr
) {
    cli();

    DDRB |= (1 << PB3) |  // MOSI
            (1 << PB5) |  // SKC
            (1 << PB2);   // SS
    DDRB &= ~(1 << PB4);  //MISO

    SPCR = (1 << SPE) |       // enable SPI
           (master << MSTR);  // SPI mater mode

    PORTB |= (1 << PB2);  // estabilish connection with slave wit SS

    if (en_isr) {
        if (!isr_set_) return 1;
        SPCR |= (1 << SPIE);  // SPI interrupt enable
    }

    sei();

    return 0;
}

uint8_t SPI_RW_byte(uint8_t data) {
    PORTB &= ~(1 << PB2);  // slave select enable
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    PORTB |= (1 << PB2);   // disable slave select

    return SPDR;
}

ISR(SPI_STC_vect) {
    SPI_cb_(SPI_cb_ctx_);
}