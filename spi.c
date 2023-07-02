#include "spi.h"

#include <stddef.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef __AVR_ATmega8__
    #define SS   PB2
    #define MOSI PB3
    #define MISO PB4
    #define SCK  PB5
#endif
#ifdef __AVR_ATmega16__
    #define SS   PB4
    #define MOSI PB5
    #define MISO PB6
    #define SCK  PB7
#endif

static SPI_isr_cb  SPI_cb_     = NULL;
static void       *SPI_cb_ctx_ = NULL;

void regiter_SPI_isr_cb(SPI_isr_cb cb, void* ctx) {
    SPI_cb_ = cb;
    SPI_cb_ctx_ = ctx;
}

void SPI_init_master(
    SPI_clock_source clk_src
) {
    cli();

    DDRB |= (1 << MOSI) |  // MOSI
            (1 << SCK) |  // SKC
            (1 << SS);   // SS
    DDRB &= ~(1 << MISO);  //MISO

    SPCR = (1 << SPE) |       // enable SPI
           (1 << MSTR) |      // SPI mater mode
           (clk_src & 0x03);  // set clock prescaler

    SPSR = (clk_src >> 2) & 0x01;  // enable double SPI clock


    PORTB |= (1 << SS);  // estabilish connection with slave wit SS

    if (SPI_cb_) {
        SPCR |= (1 << SPIE);  // SPI interrupt enable
    }

    sei();
}

void SPI_init_slave(
    SPI_clock_source clk_src
) {
    cli();

    DDRB &= ~((1 << MOSI) |  // MOSI
              (1 << SCK) |  // SKC
              (1 << SS));   // SS
    DDRB |= (1 << MISO);  //MISO

    SPCR = (1 << SPE) |       // enable SPI
           (0 << MSTR) |      // SPI slave mode
           (clk_src & 0x03);  // set clock prescaler

    SPSR = (clk_src >> 2) & 0x01;  // enable double SPI clock

    if (SPI_cb_) {
        SPCR |= (1 << SPIE);  // SPI interrupt enable
    }

    sei();
}

uint8_t SPI_RW_byte(uint8_t data) {
    PORTB &= ~(1 << SS);  // slave select enable
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    PORTB |= (1 << SS);   // disable slave select

    return SPDR;
}

void SPI_W_byte_nowait(uint8_t data) {
    PORTB &= ~(1 << SS);  // slave select enable
    SPDR = data;
}

ISR(SPI_STC_vect) {
    SPI_cb_(SPI_cb_ctx_);
}
