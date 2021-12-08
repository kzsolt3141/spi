#include "spi.h"

#include <stddef.h>

#include <avr/io.h>
#include <avr/interrupt.h>

static ADC_isr_cb  ADC_cb_     = NULL;
static void       *ADC_cb_ctx_ = NULL;
static uint8_t     isr_set_    = 0;

void regiter_ADC_isr_cb(ADC_isr_cb cb, void* ctx) {
    if (cb) {
        ADC_cb_ = cb;
    } else {
        return;
    }

    ADC_cb_ctx_ = ctx;
    isr_set_ = 1;
}

uint8_t ADC_pin_init(
    uint8_t          pin,
    ADC_clock_source clk_src,
    uint8_t          en_free_run,
    uint8_t          en_isr
) {
    cli();

    ADMUX  |= (0 << REFS1) |
              (1 << REFS0) | // voltage ref = AVCC 
              pin;               // select pin for ADC
    ADCSRA |= (1 << ADEN) |  // enable ADC
              (1 << ADSC) |  // start conversion
              (en_free_run << ADFR) |   // free running mode
              clk_src;

    if (en_isr) {
        if (! isr_set_) return 1;
        ADCSRA |= (1     << ADIE);   // enable ADC interrup
    }

    DDRC &= ~(1 << pin);         // set ADC pin as input

    sei();

    return 0;
}

uint16_t adc_single_run() {
    ADCSRA |= (1 << ADSC);               // start a conversion
    if (ADCSRA & (1 << ADIE)) return 0;  // value will be delivered in ISR

    while (ADCSRA & (1 << ADSC));  // wait for conversion to complete
    return ADC;                    // Store ADC value
}

ISR(ADC_vect) {
    ADC_cb_(ADC_cb_ctx_);
}