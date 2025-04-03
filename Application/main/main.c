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

    id = ads1292_read_id();
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
