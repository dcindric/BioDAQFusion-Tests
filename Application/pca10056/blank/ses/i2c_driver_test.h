#define TWI_INSTANCE_ID     1   // TWI1 (I2C1)

#define I2C_SCL_PIN         19  // SCL on P0.19
#define I2C_SDA_PIN         20  // SDA on P0.20

void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);
void twi_init(void);
void tmp117_get_device_id (void);
