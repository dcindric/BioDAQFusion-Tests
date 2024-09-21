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
#include "nrf_gpio.h"


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




void gpio_test (void);

