#include "ads1292_driver.h"


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

    id = ads1292_read_id();
    nrf_delay_ms(1);

    config1_reg_def = ads1292_read_config1_reg();
    nrf_delay_ms(1);

    config2_reg_def = ads1292_read_config2_reg();
    nrf_delay_ms(1);

    nrf_delay_ms(1000);
  
    //nrf_gpio_pin_set(ADS1292_START_PIN);
    //ads1292_send_command(ADS1292_CMD_RDATAC);

    //WRITE REGISTER TEST

    //Read default values of CH1SET and CH2SET registers
    uint8_t ch1set_reg_default = ads1292_read_ch1set_reg();
    uint8_t ch2set_reg_default = ads1292_read_ch2set_reg();

    if ((ch1set_reg_default != 0x00) || (ch2set_reg_default != 0x00))
    {
      while(1)
      {
        nrf_delay_ms(1);
      }
    }

    //Write 0xFF in the CH1SET register
    ads1292_write_register(ADS1292_REG_CH1SET, 0xFF);

    //Write 0xDC in the CH2SET register
    ads1292_write_register(ADS1292_REG_CH2SET, 0xDC);


    //Read both register after writing
    uint8_t ch1set_reg_updated = ads1292_read_ch1set_reg();
    nrf_delay_ms(1);

    uint8_t ch2set_reg_updated = ads1292_read_ch2set_reg();
    nrf_delay_ms(1);

     if ((ch1set_reg_updated != 0xFF) || (ch2set_reg_updated != 0xDC))
    {
      while(1)
      {
        nrf_delay_ms(1);
      }
    }


    while(1)
    {
      ads1292_read_data();
      nrf_delay_ms(1000);
    }

    return 0;
}
