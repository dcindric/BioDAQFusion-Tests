#include "nrfx_spim.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include <stdio.h>
#include "nrf_spi.h"


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
#define ADS1292_CMD_WREG  0x40  //Write register command
#define ADS1292_CMD_SDATAC  0x11  //stop read data continously command
#define ADS1292_CMD_RDATAC 0x10 //Enable read data continous

void ads1292_init(void);

void ads1292_send_command(uint8_t command);
uint8_t ads1292_read_register(uint8_t reg_addr);
void ads1292_write_register(uint8_t reg_addr, uint8_t data_to_write);


uint8_t ads1292_read_id(void);
uint8_t ads1292_read_config1_reg(void);
uint8_t ads1292_read_config2_reg(void);
uint8_t ads1292_read_ch1set_reg (void);
uint8_t ads1292_read_ch2set_reg (void);
uint8_t ads1292_read_loff_reg (void);
uint8_t ads1292_read_rld_sens_reg (void);
uint8_t ads1292_read_loff_sens_reg (void);
uint8_t ads1292_read_loff_stat_reg (void);
uint8_t ads1292_read_resp1_reg (void);
uint8_t ads1292_read_resp2_reg (void);
uint8_t ads1292_read_gpio_reg(void);


void ads1292_read_data (void);
