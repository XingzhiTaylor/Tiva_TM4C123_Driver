#include "TM4C123GH6PM.h"
#include "i2c_driver.h"
#include "gpio_driver.h"

#define SLAVE_OWN_ADDRESS      (uint8_t) 0x53
#define I2C0_IRQ_NUM					  8

void delay(unsigned long halfsecs){
  unsigned long count;
  
  while(halfsecs > 0 ) { // repeat while there are still halfsecs to delay
    count = 1538460; // 400000*0.5/0.13 that it takes 0.13 sec to count down to zero
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    halfsecs--;
  }
}

int main(void){
	RCC_I2C0_CLK_ENABLE();
	
	i2c_init_t i2cx_init;
	i2cx_init.Clock_period = 2;
	i2cx_init.Loopback = I2C_LOOPBACK;
	i2cx_init.Glitch_filter = 0;
	i2cx_init.High_speed = 0;
	i2cx_init.Master = I2C_MASTER;
	i2c_handle_t i2cx;
	i2cx.Instance = I2C0;
	i2cx.Init = i2cx_init;
	i2c_init(&i2cx);
	
	//i2cx.Init.Master = I2C_SLAVE;
	//i2cx.Init.Slave_own_address = SLAVE_OWN_ADDRESS;
	//i2cx.Init.Dual_address_en = 0;
	//i2c_init(&i2cx);
	
	NVIC_enable_interrupt(I2C0_IRQ_NUM);
	i2c_slave_int_enable(&i2cx);
	
	while(1){
		uint8_t test_arr[3] = {'A', 'B', 'C'};
		uint8_t *tsm_ptr = test_arr;
		
		i2cx.Slave_addr = SLAVE_OWN_ADDRESS;
		i2cx.pBuffPtr = tsm_ptr;
		i2cx.XferSize = 3;
		i2cx.XferCount = 0;
		
		i2c_master_tx(&i2cx);

		delay(2);
	
		uint8_t *rsv_ptr;
		i2cx.pBuffPtr = rsv_ptr;
		i2cx.XferSize = 3;
		i2cx.XferCount = 0;
		
		i2c_slave_rx(&i2cx);
	}
}

void I2C0_Handler(i2c_handle_t *i2cx){
	i2c_irq_handler(i2cx);
	NVIC_clear_interrupt(I2C0_IRQ_NUM);
}
