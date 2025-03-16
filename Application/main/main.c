#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include <stdio.h>

#define I2C_INSTANCE_ID 0
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(I2C_INSTANCE_ID);

#define TMP117_I2C_ADDR      0x48  // TMP117 I2C address (default)
#define TMP117_REG_DEVICE_ID 0x0F  // Device ID register

/**
 * @brief Initialize I2C (TWI) on P0.19 (SCL) and P0.20 (SDA)
 */
void i2c_init(void) {
    nrf_drv_twi_config_t twi_config = {
        .scl                = 19,  // P0.19 as SCL
        .sda                = 20,  // P0.20 as SDA
        .frequency          = NRF_DRV_TWI_FREQ_100K, // 100 kHz
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
        .clear_bus_init     = false
    };

    // Initialize the I2C driver
    ret_code_t err_code = nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    // Enable the I2C instance
    nrf_drv_twi_enable(&m_twi);
}

/**
 * @brief Read the ID register from TMP117
 * @param id Pointer to store the read ID
 * @return NRF_SUCCESS on success, otherwise error code
 */
ret_code_t tmp117_read_id(uint16_t *id) {
    ret_code_t err_code;
    uint8_t reg_addr = TMP117_REG_DEVICE_ID;
    uint8_t id_data[2];

    // Write register address
    err_code = nrf_drv_twi_tx(&m_twi, TMP117_I2C_ADDR, &reg_addr, 1, true);
    if (err_code != NRF_SUCCESS) return err_code;

    // Read 2 bytes from the ID register
    err_code = nrf_drv_twi_rx(&m_twi, TMP117_I2C_ADDR, id_data, 2);
    if (err_code != NRF_SUCCESS) return err_code;

    // Convert to 16-bit value (TMP117 uses big-endian format)
    *id = ((uint16_t)id_data[0] << 8) | id_data[1];

    return NRF_SUCCESS;
}


/**
 * @brief Read the temperature from TMP117
 * @param temperature Pointer to store the temperature value in °C
 * @return NRF_SUCCESS on success, otherwise error code
 */
ret_code_t tmp117_read_temperature(float *temperature) {
    ret_code_t err_code;
    uint8_t reg_addr = 0x00;  // Temperature register
    uint8_t temp_data[2];
    int16_t raw_temp;

    // Write register address
    err_code = nrf_drv_twi_tx(&m_twi, TMP117_I2C_ADDR, &reg_addr, 1, true);
    if (err_code != NRF_SUCCESS) return err_code;

    // Read 2 bytes from the temperature register
    err_code = nrf_drv_twi_rx(&m_twi, TMP117_I2C_ADDR, temp_data, 2);
    if (err_code != NRF_SUCCESS) return err_code;

    // Convert big-endian data to int16_t
    raw_temp = ((int16_t)temp_data[0] << 8) | temp_data[1];

    // Convert raw data to Celsius (TMP117 scale factor is 0.0078125 °C/LSB)
    *temperature = raw_temp * 0.0078125f;

    return NRF_SUCCESS;
}


/**
 * @brief Main function
 */
int main(void) {
    uint16_t device_id = 0;

    // Initialize I2C
    i2c_init();

    // Small delay to allow the sensor to power up
    nrf_delay_ms(10);

    // Read TMP117 ID register
    ret_code_t err = tmp117_read_id(&device_id);

    float temp_c = 0;

    while (1) {
        // Main loop (can add further functionality here)
        tmp117_read_temperature(&temp_c);
        nrf_delay_ms(1000);
    }
}
