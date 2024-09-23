#include <nrfx_spim.h>
#include <nrf_delay.h>
#include <boards.h>
#include <stdio.h>

#define SPIM_INSTANCE  1 // SPIM instance index
static const nrfx_spim_t spim = NRFX_SPIM_INSTANCE(SPIM_INSTANCE);

#define PIN_SPI_MISO   NRF_GPIO_PIN_MAP(0, 15) // P0.26
#define PIN_SPI_MOSI   NRF_GPIO_PIN_MAP(0, 16) // P0.27
#define PIN_SPI_SCLK   NRF_GPIO_PIN_MAP(0, 17)  // P0.04
#define PIN_SPI_CS     NRF_GPIO_PIN_MAP(0, 8)  // P0.03, IMU1_CS

#define BIOPOT_START   NRF_GPIO_PIN_MAP(1, 11)  //P1.11
#define BIOPOT_RESET   NRF_GPIO_PIN_MAP(1, 12)  //P1.12

#define ADS1292_REG_ID  0x00  // Device ID register address

#define ADS1292_CONFIG1 0x01  //CONFIG1 register
#define ADS1292_CONFIG2 0x02  //CONFIG2 register
#define ADS1292_LOFF    0x03
#define ADS1292_GPIO    0x0B

#define SPI_BUFFER_SIZE  2

uint8_t tx_buf[SPI_BUFFER_SIZE]; // Buffer for data to send
uint8_t rx_buf[SPI_BUFFER_SIZE]; // Buffer for data to receive

void spim_event_handler(nrfx_spim_evt_t const * p_event, void * p_context)
{
    // Handle SPIM events (optional)
}

void spim_init(void)
{
    nrfx_spim_config_t spim_config = NRFX_SPIM_DEFAULT_CONFIG;
    spim_config.sck_pin  = PIN_SPI_SCLK;
    spim_config.mosi_pin = PIN_SPI_MOSI;
    spim_config.miso_pin = PIN_SPI_MISO;
    spim_config.ss_pin   = NRFX_SPIM_PIN_NOT_USED; // We'll control CS manually
    spim_config.frequency = NRF_SPIM_FREQ_250K;
    spim_config.mode = NRF_SPIM_MODE_1; // CPOL = 0, CPHA = 1
    spim_config.bit_order = NRF_SPIM_BIT_ORDER_MSB_FIRST;

    nrfx_spim_init(&spim, &spim_config, spim_event_handler, NULL);
}

void ads1292_wake_up(void)
{
    // Activate CS to wake up the device
    nrf_gpio_pin_clear(PIN_SPI_CS);
    
    // Send WAKEUP command (0x02)
    tx_buf[0] = 0x02;
    tx_buf[1] = 0x00;  // Dummy byte for the second part of the transfer

    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(tx_buf, SPI_BUFFER_SIZE, rx_buf, SPI_BUFFER_SIZE);
    nrfx_spim_xfer(&spim, &xfer_desc, 0);

    nrf_delay_us(50);  // Delay after wake-up
    nrf_gpio_pin_set(PIN_SPI_CS);  // De-assert CS

    // Wait for the ADS1292 to wake up
    nrf_delay_ms(10);
}

void ads1292_read_device_id(void)
{
    // Activate chip select (CS)
    nrf_gpio_pin_clear(PIN_SPI_CS);

    // Prepare the command to read Device ID register (0x20 + register address)
    tx_buf[0] = 0x20 | ADS1292_REG_ID;  // READ command + register address
    tx_buf[1] = 0x00;  // Dummy byte to clock out the response

    // Perform SPIM transaction
    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(tx_buf, SPI_BUFFER_SIZE, rx_buf, SPI_BUFFER_SIZE);
    nrfx_err_t spi_err_code = nrfx_spim_xfer(&spim, &xfer_desc, 0);

    if (spi_err_code != NRFX_SUCCESS)
    {
      while(true)
      {
      }
    }

    //Register value should be in rx_buf[1]!

    // Wait for a short period (depends on SPI speed)
    nrf_delay_ms(10);

    // Deactivate chip select (CS)
    nrf_gpio_pin_set(PIN_SPI_CS);
}


void bmi323_read_register(uint8_t reg_addr, uint8_t* reg_data)
{
    nrf_gpio_pin_clear(PIN_SPI_CS);
    nrf_delay_us(50);

    tx_buf[0] = reg_addr | 0x80;  // Set MSB for read operation
    tx_buf[1] = 0x00;             // Dummy byte to receive data

    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(tx_buf, SPI_BUFFER_SIZE, rx_buf, SPI_BUFFER_SIZE);
    nrfx_spim_xfer(&spim, &xfer_desc, 0);

    nrf_delay_us(500);
    nrf_gpio_pin_set(PIN_SPI_CS);

    *reg_data = rx_buf[1];
}

void lsm6dsox_read_register(uint8_t reg_addr, uint8_t* reg_data)
{
    nrf_gpio_pin_clear(PIN_SPI_CS);
    nrf_delay_us(50);

    tx_buf[0] = reg_addr | 0x80;  // Set MSB for read operation (for LSM6DSOX)
    tx_buf[1] = 0x00;             // Dummy byte to receive data

    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(tx_buf, SPI_BUFFER_SIZE, rx_buf, SPI_BUFFER_SIZE);
    nrfx_spim_xfer(&spim, &xfer_desc, 0);

    nrf_delay_ms(1);
    nrf_gpio_pin_set(PIN_SPI_CS);

    *reg_data = rx_buf[1];
}


int main(void)
{
    // Initialize GPIO for chip select
    nrf_gpio_cfg_output(PIN_SPI_CS);
    nrf_gpio_pin_set(PIN_SPI_CS);  // Initially de-assert CS

    // Initialize SPIM
    spim_init();
    nrf_delay_ms(100);

    //ads1292_wake_up();
  
    uint8_t reg_data = 0;

    while (true)
    {

        bmi323_read_register(0x00, &reg_data);  // Read ID register (0x00)
        //lsm6dsox_read_register(0x0F, &reg_data);
        nrf_delay_ms(100);
    }
}
