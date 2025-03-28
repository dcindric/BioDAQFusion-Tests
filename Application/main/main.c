#include "nrfx_spim.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include <stdio.h>
#include "nrf_spi.h"

#define SPI_INSTANCE  0  // SPI instance index
static const nrfx_spim_t spi = NRFX_SPIM_INSTANCE(SPI_INSTANCE);

#define ADS1292_CS_PIN  NRF_GPIO_PIN_MAP(0,3)   // Chip Select
#define ADS1292_SCK_PIN NRF_GPIO_PIN_MAP(0,4)   // SPI Clock
#define ADS1292_MOSI_PIN NRF_GPIO_PIN_MAP(0,27) // MOSI
#define ADS1292_MISO_PIN NRF_GPIO_PIN_MAP(0,26) // MISO
#define ADS1292_RESET_PIN NRF_GPIO_PIN_MAP(1,12) // RESET
#define ADS1292_START_PIN NRF_GPIO_PIN_MAP(1,11) // START

#define BIOZ_CS        NRF_GPIO_PIN_MAP(0,25)   //P0.25

#define ADS1292_REG_ID  0x00  // ID Register Address
#define ADS1292_CMD_RREG  0x20  // Read Register Command


void ads1292_init(void);
void ads1292_send_command(uint8_t command);


void ads1292_init(void) {
    nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG;
    spi_config.sck_pin = ADS1292_SCK_PIN;
    spi_config.mosi_pin = ADS1292_MOSI_PIN;
    spi_config.miso_pin = ADS1292_MISO_PIN;
    spi_config.ss_pin = NRFX_SPIM_PIN_NOT_USED; // Manually control CS
    spi_config.frequency = NRF_SPIM_FREQ_125K;
    spi_config.mode = NRF_SPIM_MODE_1;  // CPOL=0, CPHA=1
    spi_config.bit_order = NRF_SPIM_BIT_ORDER_MSB_FIRST;

    nrfx_spim_init(&spi, &spi_config, NULL, NULL);

    // Configure control pins
    nrf_gpio_cfg_output(ADS1292_CS_PIN);
    nrf_gpio_cfg_output(ADS1292_RESET_PIN);
    nrf_gpio_cfg_output(ADS1292_START_PIN);

    // Reset sequence
    nrf_gpio_pin_clear(ADS1292_RESET_PIN);
    nrf_delay_ms(10);
    nrf_gpio_pin_set(ADS1292_RESET_PIN);
    nrf_delay_ms(100); // Increased delay after reset

    // Start operation
    nrf_gpio_pin_set(ADS1292_START_PIN);
    nrf_delay_ms(10);

    // Stop continuous data mode
    ads1292_send_command(0x11);  // SDATAC command
}

void ads1292_send_command(uint8_t command) {
    nrf_gpio_pin_clear(ADS1292_CS_PIN);  // Select device
    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(&command, 1);
    nrfx_spim_xfer(&spi, &xfer_desc, 0);
    nrf_gpio_pin_set(ADS1292_CS_PIN);  // Deselect device
    nrf_delay_ms(1);  // Small delay to ensure command is processed
}


uint8_t ads1292_read_id(void) {

    uint8_t tx_buf[2] = { ADS1292_CMD_RREG | ADS1292_REG_ID, 0x00 }; // RREG command
    uint8_t rx_buf[2] = {0};
    
    nrf_gpio_pin_clear(ADS1292_CS_PIN); // Select ADS1292
    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(tx_buf, sizeof(tx_buf), rx_buf, sizeof(rx_buf));
    nrfx_spim_xfer(&spi, &xfer_desc, 0);
    nrf_gpio_pin_set(ADS1292_CS_PIN); // Deselect ADS1292
    
    return rx_buf[0]; // First byte contains the register value

    //Alternative approach
    //uint8_t tx_buffer[3];
    //uint8_t rx_buffer[3] = {0};

    //// Construct Read Register Command
    //// Format: 001r rrrr (read command + number of registers to read)
    //tx_buffer[0] = ADS1292_CMD_RREG | 0x01;  // Read 1 register
    //tx_buffer[1] = ADS1292_REG_ID;           // Start at ID register
    //tx_buffer[2] = 0x00;                     // Dummy byte for reading

    //// Assert Chip Select
    //nrf_gpio_pin_clear(ADS1292_CS_PIN);

    //nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(tx_buffer, sizeof(tx_buffer), rx_buffer, sizeof(rx_buffer));
    //nrfx_spim_xfer(&spi, &xfer_desc, 0);

    //// Deassert Chip Select
    //nrf_gpio_pin_set(ADS1292_CS_PIN);

    //// Return the ID register value (3rd byte in rx_buffer)
    //return rx_buffer[2];

}

int main(void) {

    nrf_gpio_cfg_output(BIOZ_CS);

    //BIOZ_CS should be held high for the whole duration of SPI transaction with ADS1292!
    nrf_gpio_pin_set(BIOZ_CS);
    
    ads1292_init();

    uint8_t id = 0;
    uint8_t id_v2 = 0;

    while(1)
    {
      id = ads1292_read_id();

      nrf_delay_ms(1000);
    }
    
    while (1);
}
