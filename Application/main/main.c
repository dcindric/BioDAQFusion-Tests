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

#define LED_DEBUG NRF_GPIO_PIN_MAP(0,9)


//Registers address definition

#define ADS1292_REG_ID  0x00  // ID Register Address
#define ADS1292_REG_CONFIG1 0x01  //CONFIG1 register address
#define ADS1292_REG_CONFIG2 0x02  //CONFIG2 register address
#define ADS1292_REG_LOFF  0x03
#define ADS1292_REG_CH1SET  0x04
#define ADS1292_REG_CH2SET  0x05
#define ADS1292_REG_RLD_SENS 0x06
#define ADS1292_REG_LOFF_SENS 0x07
#define ADS1292_REG_LOFF_STAT 0x08
#define ADS1292_REG_RESP1 0x09
#define ADS1292_REG_RESP2 0x0A
#define ADS1292_REG_GPIO  0x0B  //GPIO register address

//SPI commands for ADS1292 control and configuration
#define ADS1292_CMD_RREG  0x20  // Read Register Command
#define ADS1292_CMD_SDATAC  0x11  //stop read data continously command
#define ADS1292_CMD_RDATAC 0x10 //Enable read data continous

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

    nrf_delay_ms(100);
}

void ads1292_send_command(uint8_t command) {
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

uint8_t ads1292_read_id(void) {
    return ads1292_read_register(ADS1292_REG_ID);
}

uint8_t ads1292_read_config1_reg(void) {
    return ads1292_read_register(ADS1292_REG_CONFIG1);
}

uint8_t ads1292_read_config2_reg(void) {
    return ads1292_read_register(ADS1292_REG_CONFIG2);
}

uint8_t ads1292_read_loff_reg (void)
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

uint8_t ads1292_read_gpio_reg(void) {
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


int main(void) {

    nrf_gpio_cfg_output(BIOZ_CS);
    nrf_gpio_cfg_output(LED_DEBUG);
    nrf_gpio_cfg_output(ADS1292_START_PIN);

    //BIOZ_CS should be held high for the whole duration of SPI transaction with ADS1292!
    nrf_gpio_pin_set(BIOZ_CS);
    nrf_gpio_pin_set(LED_DEBUG);
    nrf_gpio_pin_clear(ADS1292_START_PIN);
    
    ads1292_init();

    uint8_t id = 0;
    uint8_t config1_reg_def = 0;
    uint8_t config2_reg_def = 0;
    uint8_t gpio_reg_def = 0;
    uint8_t loff_reg_def = 0;
    uint8_t ch1set_reg_def = 0;
    uint8_t ch2set_reg_def = 0;
    uint8_t rld_sens_reg_def = 0;
    uint8_t loff_sens_reg_def = 0;
    uint8_t loff_stat_reg_def = 0;
    uint8_t resp1_reg_def = 0;
    uint8_t resp2_reg_def = 0;

    id = ads1292_read_id();
    nrf_delay_ms(1);

    config1_reg_def = ads1292_read_config1_reg();
    nrf_delay_ms(1);

    config2_reg_def = ads1292_read_config2_reg();
    nrf_delay_ms(1);

    loff_reg_def = ads1292_read_loff_reg();
    nrf_delay_ms(1);

    ch1set_reg_def = ads1292_read_ch1set_reg();
    nrf_delay_ms(1);

    ch2set_reg_def = ads1292_read_ch2set_reg();
    nrf_delay_ms(1);

    rld_sens_reg_def = ads1292_read_rld_sens_reg();
    nrf_delay_ms(1);

    loff_sens_reg_def = ads1292_read_loff_sens_reg();
    nrf_delay_ms(1);

    loff_stat_reg_def = ads1292_read_loff_stat_reg();
    nrf_delay_ms(1);

    resp1_reg_def = ads1292_read_resp1_reg();
    nrf_delay_ms(1);

    resp2_reg_def = ads1292_read_resp2_reg();
    nrf_delay_ms(1);

    gpio_reg_def = ads1292_read_gpio_reg();
    nrf_delay_ms(1);

    nrf_delay_ms(1000);

    
    nrf_gpio_pin_set(ADS1292_START_PIN);
    ads1292_send_command(ADS1292_CMD_RDATAC);

    while(1)
    {
     ads1292_read_data();
     nrf_delay_ms(1000);

    }

    return 0;
}
