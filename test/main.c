/*
 * adc.c
 *
 * Created: 12/17/2019 12:09:35 AM
 * Author : Zsolt
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"
#include "uart.h"


/**
 * SPI data tranfere complete interrupt
 */
typedef struct SPI_cb_ctx_t {
    uint8_t data;
    int intr_cnt;
}SPI_cb_ctx;

static void SPI_cb_handle(void* ctx) {
    SPI_cb_ctx* t_ctx = (SPI_cb_ctx*)ctx;

    PORTB |= (1 << PB2);   // disable slave select

    t_ctx->data = SPDR;
    t_ctx->intr_cnt++;
}


int main(void) {
    // UART INIT
    //-------------------------------
    const uint16_t baud_rate = 38400;

    USART_init(baud_rate);

    printf("Init Done UART baud: %u\n", (uint16_t)baud_rate);
    //-------------------------------

    // SPI INIT
    //-------------------------------
    SPI_cb_ctx spi_ctx = {};
    regiter_SPI_isr_cb(SPI_cb_handle, &spi_ctx);
    SPI_init_master(SPI_PS_4);

    printf("Init Done SPI\n");

uint8_t cnt = 0;
    while (1) {
        SPI_W_byte_nowait(cnt);
        printf("master s:%d r%d, icnt:%d\n", cnt, spi_ctx.data, spi_ctx.intr_cnt);
        _delay_ms(1000);
        cnt++;
    }

    return 0;
}
