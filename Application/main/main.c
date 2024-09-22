#include <nrfx_spim.h>
#include <nrf_delay.h>
#include <boards.h>
#include <stdio.h>

#define SPIM_INSTANCE  2 // SPIM instance index
static const nrfx_spim_t spim = NRFX_SPIM_INSTANCE(SPIM_INSTANCE);

#define PIN_SPI_MISO   NRF_GPIO_PIN_MAP(0, 26) // P0.26
#define PIN_SPI_MOSI   NRF_GPIO_PIN_MAP(0, 27) // P0.27
#define PIN_SPI_SCLK   NRF_GPIO_PIN_MAP(0, 4)  // P0.04
#define PIN_SPI_CS     NRF_GPIO_PIN_MAP(0, 3)  // P0.03

#define BIOPOT_START   NRF_GPIO_PIN_MAP(1, 11)  //P1.11
#define BIOPOT_RESET   NRF_GPIO_PIN_MAP(1, 12)  //P1.12

#define ADS1292_REG_ID  0x00  // Device ID register address
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

void ads1292_read_device_id(void)
{
    // Activate chip select (CS)
    nrf_gpio_pin_set(BIOPOT_START);
    nrf_gpio_pin_clear(PIN_SPI_CS);

    // Prepare the command to read Device ID register (0x20 + register address)
    tx_buf[0] = 0x20 | ADS1292_REG_ID;  // READ command + register address
    tx_buf[1] = 0x00;  // Dummy byte to clock out the response

    // Perform SPIM transaction
    nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TRX(tx_buf, SPI_BUFFER_SIZE, rx_buf, SPI_BUFFER_SIZE);
    nrfx_spim_xfer(&spim, &xfer_desc, 0);

    // Wait for a short period (depends on SPI speed)
    nrf_delay_ms(100);

    // Deactivate chip select (CS)
    nrf_gpio_pin_clear(BIOPOT_START);
    nrf_gpio_pin_set(PIN_SPI_CS);
   


    // Device ID will be in rx_buf[1]
    //printf("Device ID: 0x%02X\n", rx_buf[1]);
}

int main(void)
{
    // Initialize GPIO for chip select
    nrf_gpio_cfg_output(PIN_SPI_CS);
    nrf_gpio_pin_set(PIN_SPI_CS);  // Initially de-assert CS

    //Initialize BIOPOT START pin, which is an output from MCU perspective
    nrf_gpio_cfg_output(BIOPOT_START);
    nrf_gpio_pin_clear(BIOPOT_START);

    //Initialize BIOPOT_RESET pin and set it from low to high, activating the ADS1292
    nrf_gpio_cfg_output(BIOPOT_RESET);
    nrf_gpio_pin_clear(BIOPOT_RESET);
    nrf_gpio_pin_set(BIOPOT_RESET);

    // Initialize SPIM
    spim_init();

    

    while (true)
    {
        // Main loop
        // Read Device ID from ADS1292
        ads1292_read_device_id();
        nrf_delay_ms(1000);
    }
}
