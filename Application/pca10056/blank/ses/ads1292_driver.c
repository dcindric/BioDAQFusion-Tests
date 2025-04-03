#include "ads1292_driver.h"

#define SPI_INSTANCE  0  // SPI instance index
const nrfx_spim_t spi = NRFX_SPIM_INSTANCE(SPI_INSTANCE);

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

    nrf_delay_ms(100);
}

void ads1292_send_command(uint8_t command) 
{
    nrf_gpio_pin_clear(ADS1292_CS_PIN);  // Select device
    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(&command, 1);
    nrfx_spim_xfer(&spi, &xfer_desc, 0);
    nrf_gpio_pin_set(ADS1292_CS_PIN);  // Deselect device
    nrf_delay_ms(1);  // Small delay to ensure command is processed
}


uint8_t ads1292_read_register(uint8_t reg_addr) {
    uint8_t tx_buf[3] = { ADS1292_CMD_RREG | reg_addr, 0x00, 0x00 }; // Send RREG command, followed by dummy byte
    uint8_t rx_buf[3] = {0};

    nrf_gpio_pin_clear(ADS1292_CS_PIN); // Select ADS1292
    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(tx_buf, sizeof(tx_buf), rx_buf, sizeof(rx_buf));
    nrfx_spim_xfer(&spi, &xfer_desc, 0);
    nrf_gpio_pin_set(ADS1292_CS_PIN); // Deselect ADS1292

    return rx_buf[2]; // Third byte contains the register value
}


void ads1292_write_register(uint8_t reg_addr, uint8_t data_to_write)
{
    uint8_t tx_buf[3] = { ADS1292_CMD_WREG | reg_addr, 0x00, data_to_write};

    nrf_gpio_pin_clear(ADS1292_CS_PIN); // Select ADS1292
    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(tx_buf, sizeof(tx_buf));
    nrfx_spim_xfer(&spi, &xfer_desc, 0);
    nrf_gpio_pin_set(ADS1292_CS_PIN); // Deselect ADS1292
}

uint8_t ads1292_read_id(void) 
{
    return ads1292_read_register(ADS1292_REG_ID);
}

uint8_t ads1292_read_config1_reg(void) 
{
    return ads1292_read_register(ADS1292_REG_CONFIG1);
}

uint8_t ads1292_read_config2_reg(void) 
{
    return ads1292_read_register(ADS1292_REG_CONFIG2);
}

uint8_t ads1292_read_loff_reg(void) 
{
    return ads1292_read_register(ADS1292_REG_LOFF);
}


uint8_t ads1292_read_ch1set_reg (void)
{
  return ads1292_read_register(ADS1292_REG_CH1SET);
}

uint8_t ads1292_read_ch2set_reg (void)
{
  return ads1292_read_register(ADS1292_REG_CH2SET);
}

uint8_t ads1292_read_rld_sens_reg (void)
{
  return ads1292_read_register(ADS1292_REG_RLD_SENS);
}

uint8_t ads1292_read_loff_sens_reg (void)
{
  return ads1292_read_register(ADS1292_REG_LOFF_SENS);
}

uint8_t ads1292_read_loff_stat_reg (void)
{
  return ads1292_read_register(ADS1292_REG_LOFF_STAT);
}

uint8_t ads1292_read_resp1_reg (void)
{
  return ads1292_read_register(ADS1292_REG_RESP1);
}

uint8_t ads1292_read_resp2_reg (void)
{
  return ads1292_read_register(ADS1292_REG_RESP2);
}

uint8_t ads1292_read_gpio_reg(void)
{
    return ads1292_read_register(ADS1292_REG_GPIO);
}


void ads1292_read_data (void)
{
    uint8_t tx_buf[10] = {0}; // Send RREG command, followed by dummy byte

    nrf_gpio_pin_clear(ADS1292_CS_PIN); // Select ADS1292
    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(tx_buf, sizeof(tx_buf));

    nrfx_spim_xfer(&spi, &xfer_desc, 0);
    nrf_gpio_pin_set(ADS1292_CS_PIN); // Deselect ADS1292

}


