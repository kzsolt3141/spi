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
    SPI_cb_ctx* t_ctx = (struct SPI_cb_ctx*)ctx;

    PORTB |= (1 << PB2);   // disable slave select

    t_ctx->data = SPDR;
    t_ctx->intr_cnt++;
}


int main(void) {
    uint8_t sts = 0;

    // UART INIT
    //-------------------------------
    const uint16_t baud_rate = 38400;

    sts = USART_init(baud_rate, 0);
    if (sts) return sts;

    printf("Init Done UART baud: %u\n", (uint16_t)baud_rate);
    //-------------------------------

    // SPI INIT
    //-------------------------------
    sts = SPI_init(SPI_PS_8, 1, 0);
    if (sts) return sts;

    printf("Init Done SPI\n");

uint8_t cnt = 0;
    while (1) {
        uint8_t d = SPI_RW_byte(cnt);
        printf("test s:%d r%d\n", cnt, d);
        _delay_ms(1000);
        cnt++;
    }
    return 0;
}
