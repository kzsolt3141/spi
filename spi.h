#ifndef SPI_TOOLS_H_
#define SPI_TOOLS_H_

#include <stdint.h>

typedef enum ADC_clock_source_t {
    ADC_PS_DEFAULT_2 = 0 ,
    ADC_PS_2,
    ADC_PS_4,
    ADC_PS_8,
    ADC_PS_16,
    ADC_PS_32,
    ADC_PS_64,
    ADC_PS_128
} ADC_clock_source;

/**
 * ADC interrupt callback type
 * @param[inout] ctx user data for interrupt callback
 * When ISR occurs ADC_isr_cb will be called with ctx as parameter
 */
typedef void (*ADC_isr_cb)(void* ctx);

/**
 * Register callback and context for ADC interrupt
 * @param[in] cb   callback for isr; must not be NULL
 * @param[in] ctx  user defined callback context; must not be NULL
 * 
 */
void regiter_ADC_isr_cb(ADC_isr_cb cb, void* ctx);

/**
 * Initialize ADC for voltage ref AVCC, connect AREF to GND with C=100nF
 * this mode will use one ADC pin for single run conversion
 * @param [in] pin         select which pin of port C to be used for ADC
 * @param [in] clk_src     select clock prescaler, see ADC_clock_source_t
 * @param [in] en_free_run ADC vill measure in continious mode
 * @param [in] en_isr      enable ADC read by ISR, value is saved in rawADC
 */
uint8_t ADC_pin_init(
    uint8_t          pin,
    ADC_clock_source clk_src,
    uint8_t          en_free_run,
    uint8_t          en_isr
);

/**
 * Initialize or read one ADV value.
 * This can be used to read one adc value if free-run mode is disabled
 */
uint16_t adc_single_run();

#endif /* SPI_TOOLS_H_ */