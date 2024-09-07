#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "boards.h"


// TWI instance
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

// Flag indicating transfer completion
static volatile bool m_xfer_done = false;

void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context) {
    if (p_event->type == NRF_DRV_TWI_EVT_DONE) {
        m_xfer_done = true;
    }
}

void twi_init(void) {
    nrf_drv_twi_config_t const config = {
        .scl                = I2C_SCL_PIN,   // Set SCL pin
        .sda                = I2C_SDA_PIN,   // Set SDA pin
        .frequency          = NRF_TWI_FREQ_400K,  // Set I2C frequency to 400 kHz
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,  // Set interrupt priority
        .clear_bus_init     = false   // Clear the bus initially
    };

    ret_code_t err_code = nrf_drv_twi_init(&m_twi, &config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}



void tmp117_get_device_id (void)
{
    uint8_t address = 0x1A;  // I2C address of the device
    uint8_t reg_addr = 0x01;  // Register to write/read
    uint8_t data_to_write = 0x02;  // Data to write to the device
    uint8_t data_to_read[2];  // Buffer to read data into

    // Write data to I2C device (e.g., register address and data)
    m_xfer_done = false;
    ret_code_t err_code = nrf_drv_twi_tx(&m_twi, address, &reg_addr, 1, false);  // Write the register address
    APP_ERROR_CHECK(err_code);

    // Wait for the write transfer to complete
    while (!m_xfer_done) {
        __WFE();  // Wait for an event
    }

    // Read data from the same I2C device (read 2 bytes from the register)
    m_xfer_done = false;
    err_code = nrf_drv_twi_rx(&m_twi, address, data_to_read, sizeof(data_to_read));
    APP_ERROR_CHECK(err_code);

    // Wait for the read transfer to complete
    while (!m_xfer_done) {
        __WFE();  // Wait for an event
    }

    // Now `data_to_read` contains the data read from the device

    while (true) {
        nrf_delay_ms(1000);  // Main loop doing nothing
    }
}
