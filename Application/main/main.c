#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_drv_twi.h"

#define TWI_INSTANCE_ID     1   // TWI1 (I2C1)

#define I2C_SCL_PIN         19  // SCL on P0.19
#define I2C_SDA_PIN         20  // SDA on P0.20

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
        .frequency          = NRF_TWI_FREQ_100K,  // Set I2C frequency to 100 kHz
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH,  // Set interrupt priority
        .clear_bus_init     = false   // Clear the bus initially
    };

    ret_code_t err_code = nrf_drv_twi_init(&m_twi, &config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);
}


//Pin definitions on BioDAQ Fusion Platform

//GPIO PORT 0
#define CHG_MOD 2 //P0.02
#define BIOPOT_CS 3 //P0.03
#define SPI2_CLK_M  4 //P0.04
#define UART_TX 5 //P0.05
#define UART_RX 6 //P0.06
#define IMU1_INT1 7 //P0.07
#define IMU1_CS 8 //P0.08
#define LED_DBG 9 //P0.09
#define IMU1_INT2 10 //P0.10
#define IMU2_INT2 12 //P0.12
#define IMU2_INT1 13 //P0.13
#define IMU2_CS 14  //P0.14
#define SPI1_MISO 15 //P0.15
#define SPI1_MOSI_M 16 
#define SPI1_CLK_M  17

//#define I2C1_SCL  19
//#define I2C1_SDA  20
#define BIOZ_GPIO0 21
#define BIOZ_GPIO1 22
#define BIOZ_GPIO2 23
#define BIOZ_RESET  24
#define BIOZ_CS 25
#define SPI2_MISO 26
#define SPI2_MOSI_M 27


//GPIO PORT 1
#define LDO_EN  (1+32)
#define CHG_ERR (2+32)
#define CHG_SIG (3+32)
#define FLASH_MEM_HOLD (5+32)
#define FLASH_MEM_WP  (6+32)
#define FLASH_MEM_CS  (7+32)
#define GPIO_MCU_ARB_1 (8+32)
#define GPIO_MCU_ARB_2 (9+32)
#define BIOPOT_DRDY (10+32)
#define BIOPOT_START (11+32)
#define BIOPOT_RESET (12+32)
#define BIOPOT_GPIO1 (13+32)
#define BIOPOT_GPIO2 (14+32)
#define START_MEAS_BTN (15+32)



//#define PIN_P1_08  40    // P1.08 (32 + 8)
//#define PIN_P1_09  41    // P1.09 (32 + 9)

int main(void) {


    ////GPIO PORT 0 configure all as output
    
    //nrf_gpio_cfg_output(CHG_MOD);
    //nrf_gpio_cfg_output(BIOPOT_CS);
    //nrf_gpio_cfg_output(SPI2_CLK_M);
    //nrf_gpio_cfg_output(UART_TX);
    //nrf_gpio_cfg_output(UART_RX);
    //nrf_gpio_cfg_output(IMU1_INT1);
    //nrf_gpio_cfg_output(IMU1_CS);
    //nrf_gpio_cfg_output(LED_DBG);
    //nrf_gpio_cfg_output(IMU1_INT2);
    //nrf_gpio_cfg_output(IMU2_INT2);
    //nrf_gpio_cfg_output(IMU2_INT1);
    //nrf_gpio_cfg_output(IMU2_CS);
    //nrf_gpio_cfg_output(SPI1_MISO);
    //nrf_gpio_cfg_output(SPI1_MOSI_M);
    //nrf_gpio_cfg_output(SPI1_CLK_M);
    
    ////nrf_gpio_cfg_output(I2C1_SCL);
    ////nrf_gpio_cfg_output(I2C1_SDA);
    //nrf_gpio_cfg_output(BIOZ_GPIO0);
    //nrf_gpio_cfg_output(BIOZ_GPIO1);
    //nrf_gpio_cfg_output(BIOZ_GPIO2);
    //nrf_gpio_cfg_output(BIOZ_RESET);
    //nrf_gpio_cfg_output(BIOZ_CS);
    //nrf_gpio_cfg_output(SPI2_MISO);
    //nrf_gpio_cfg_output(SPI2_MOSI_M);
    



    ////GPIO PORT 0 toggle all
    //nrf_gpio_pin_set(CHG_MOD);
    //nrf_gpio_pin_clear(CHG_MOD);

    //nrf_gpio_pin_set(BIOPOT_CS);
    //nrf_gpio_pin_clear(BIOPOT_CS);

    //nrf_gpio_pin_set(SPI2_CLK_M);
    //nrf_gpio_pin_clear(SPI2_CLK_M);

    //nrf_gpio_pin_set(UART_TX);
    //nrf_gpio_pin_clear(UART_TX);

    //nrf_gpio_pin_set(UART_RX);
    //nrf_gpio_pin_clear(UART_RX);

    //nrf_gpio_pin_set(IMU1_INT1);
    //nrf_gpio_pin_clear(IMU1_INT1);

    //nrf_gpio_pin_set(IMU1_CS);
    //nrf_gpio_pin_clear(IMU1_CS);

    //nrf_gpio_pin_set(LED_DBG);
    //nrf_gpio_pin_clear(LED_DBG);

    //nrf_gpio_pin_set(IMU1_INT2);
    //nrf_gpio_pin_clear(IMU1_INT2);

    //nrf_gpio_pin_set(IMU2_INT2);
    //nrf_gpio_pin_clear(IMU2_INT2);

    //nrf_gpio_pin_set(IMU2_INT1);
    //nrf_gpio_pin_clear(IMU2_INT1);

    //nrf_gpio_pin_set(IMU2_CS);
    //nrf_gpio_pin_clear(IMU2_CS);

    //nrf_gpio_pin_set(SPI1_MISO);
    //nrf_gpio_pin_clear(SPI1_MISO);

    //nrf_gpio_pin_set(SPI1_MOSI_M);
    //nrf_gpio_pin_clear(SPI1_MOSI_M);

    //nrf_gpio_pin_set(SPI1_CLK_M);
    //nrf_gpio_pin_clear(SPI1_CLK_M);

    ////nrf_gpio_pin_set(I2C1_SCL);
    ////nrf_gpio_pin_clear(I2C1_SCL);

    ////nrf_gpio_pin_set(I2C1_SDA);
    //nrf_gpio_pin_clear(I2C1_SDA);

    //nrf_gpio_pin_set(BIOZ_GPIO0);
    //nrf_gpio_pin_clear(BIOZ_GPIO0);

    //nrf_gpio_pin_set(BIOZ_GPIO1);
    //nrf_gpio_pin_clear(BIOZ_GPIO1);

    //nrf_gpio_pin_set(BIOZ_RESET);
    //nrf_gpio_pin_clear(BIOZ_RESET);
  
    //nrf_gpio_pin_set(BIOZ_CS);
    //nrf_gpio_pin_clear(BIOZ_CS);

    //nrf_gpio_pin_set(SPI2_MISO);
    //nrf_gpio_pin_clear(SPI2_MISO);

    //nrf_gpio_pin_set(SPI2_MOSI_M);
    //nrf_gpio_pin_clear(SPI2_MOSI_M);

    //nrf_gpio_pin_set(BIOZ_GPIO2);
    //nrf_gpio_pin_clear(BIOZ_GPIO2);




    ////GPIO PORT 1 configure all as output
    ////nrf_gpio_cfg_output(LDO_EN);
    //nrf_gpio_cfg_output(CHG_ERR);
    //nrf_gpio_cfg_output(CHG_SIG);
    //nrf_gpio_cfg_output(FLASH_MEM_HOLD);
    //nrf_gpio_cfg_output(FLASH_MEM_WP);
    //nrf_gpio_cfg_output(FLASH_MEM_CS);
    //nrf_gpio_cfg_output(GPIO_MCU_ARB_1);
    //nrf_gpio_cfg_output(GPIO_MCU_ARB_2);
    ////nrf_gpio_cfg_output(BIOPOT_DRDY);
    //nrf_gpio_cfg_output(BIOPOT_START);
    //nrf_gpio_cfg_output(BIOPOT_RESET);
    //nrf_gpio_cfg_output(BIOPOT_GPIO1);
    //nrf_gpio_cfg_output(BIOPOT_GPIO2);
    //nrf_gpio_cfg_output(START_MEAS_BTN);

    ////GPIO PORT 1 toggle all
    ////nrf_gpio_pin_set(LDO_EN);
    ////nrf_gpio_pin_clear(LDO_EN);

    //nrf_gpio_pin_set(CHG_ERR);
    //nrf_gpio_pin_clear(CHG_ERR);

    //nrf_gpio_pin_set(CHG_SIG);
    //nrf_gpio_pin_clear(CHG_SIG);

    //nrf_gpio_pin_set(FLASH_MEM_HOLD);
    //nrf_gpio_pin_clear(FLASH_MEM_HOLD);

    //nrf_gpio_pin_set(FLASH_MEM_WP);
    //nrf_gpio_pin_clear(FLASH_MEM_WP);

    //nrf_gpio_pin_set(FLASH_MEM_CS);
    //nrf_gpio_pin_clear(FLASH_MEM_CS);

    //nrf_gpio_pin_set(GPIO_MCU_ARB_1);
    //nrf_gpio_pin_clear(GPIO_MCU_ARB_1);

    //nrf_gpio_pin_set(GPIO_MCU_ARB_2);
    //nrf_gpio_pin_clear(GPIO_MCU_ARB_2);

    ////nrf_gpio_pin_set(BIOPOT_DRDY);
    ////nrf_gpio_pin_clear(BIOPOT_DRDY);

    //nrf_gpio_pin_set(BIOPOT_START);
    //nrf_gpio_pin_clear(BIOPOT_START);

    //nrf_gpio_pin_set(BIOPOT_RESET);
    //nrf_gpio_pin_clear(BIOPOT_RESET);

    //nrf_gpio_pin_set(BIOPOT_GPIO1);
    //nrf_gpio_pin_clear(BIOPOT_GPIO1);

    //nrf_gpio_pin_set(BIOPOT_GPIO2);
    //nrf_gpio_pin_clear(BIOPOT_GPIO2);

    //nrf_gpio_pin_set(START_MEAS_BTN);
    //nrf_gpio_pin_clear(START_MEAS_BTN);

   twi_init();

    uint8_t address = 0x48;  // I2C address of the device, 0x48 temp sensor, 0x63 pressure sensor
    uint8_t reg_addr = 0x01;  // Register to write/read 0x0F temp sensor, 0xEF press sens
    uint8_t data_to_write = 0x00;  // Data to write to the device, 0xC8 press sens
    uint8_t data_to_read[2] = {0};  // Buffer to read data into

   

    while (true) {

         // Write data to I2C device (e.g., register address and data)
    m_xfer_done = false;
    ret_code_t err_code = nrf_drv_twi_tx(&m_twi, address, &reg_addr, 1, false);  // Write the register address
    APP_ERROR_CHECK(err_code);

     //Wait for the write transfer to complete
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

        nrf_delay_ms(1000);  // Main loop doing nothing
    }

}
