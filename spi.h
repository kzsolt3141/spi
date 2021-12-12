#ifndef SPI_TOOLS_H_
#define SPI_TOOLS_H_

#include <stdint.h>

typedef enum SPI_clock_source_t {
    SPI_PS_4 = 0 ,
    SPI_PS_16,
    SPI_PS_64,
    SPI_PS_128,
    SPI_PS_2,
    SPI_PS_8,
    SPI_PS_32
} SPI_clock_source;

/**
 * SPI interrupt callback type
 * @param[inout] ctx user data for interrupt callback
 * When ISR occurs SPI_isr_cb will be called with ctx as parameter
 */
typedef void (*SPI_isr_cb)(void* ctx);

/**
 * Register callback and context for SPI interrupt
 * @param[in] cb   callback for isr; must not be NULL
 * @param[in] ctx  user defined callback context; must not be NULL
 * 
 */
void regiter_SPI_isr_cb(SPI_isr_cb cb, void* ctx);

/**
 * Initialize SPI
 * @param [in] clk_src  select clock prescaler, see ADC_clock_source_t
 * @param [in] master   master/slave
 * @param [in] en_isr   enable data ready interrupt
 *
 * @return 0 for success
 *         other in case of fail
 */
uint8_t SPI_init(
    SPI_clock_source clk_src,
    uint8_t          master,
    uint8_t          en_isr
);

/**
 * Send one byte over SPI and wait until transmission is finished
 * @param [in] data  data to be sent over spi
 *
 * @return received byte
 */
uint8_t SPI_RW_byte(uint8_t data);

/**
 * Send one byte over SPI and don't wait
 * @param [in] data  data to be sent over spi
 */
void SPI_W_byte_nowait(uint8_t data);

#endif /* SPI_TOOLS_H_ */