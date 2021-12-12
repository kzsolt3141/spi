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

    t_ctx->data = SPDR;
    SPDR = t_ctx->intr_cnt;

    t_ctx->intr_cnt ++;

    printf("slave s:%d r%d\n", t_ctx->data, t_ctx->intr_cnt);
    PORTC = t_ctx->data;
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
    SPI_cb_ctx spi_ctx = {};

    regiter_SPI_isr_cb(SPI_cb_handle, &spi_ctx);

    sts = SPI_init_slave(SPI_PS_8, 1);
    if (sts) return sts;

    printf("Init Done SPI\n");

    // PORTC INIT
    //-------------------------------
    DDRC = 0xFF;

    while (1) {
    };

    return 0;
}
