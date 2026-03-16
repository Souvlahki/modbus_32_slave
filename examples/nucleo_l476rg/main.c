#include "main.h"


#include "modbus.h"


UART_HandleTypeDef huart2;

int32_t modbus_serial_read(uint8_t* buf, uint16_t count, int32_t timeout) {
	if(HAL_UART_Receive(&huart2, buf, count, timeout) == HAL_OK) {
		return count;
	}

	return 0;
}

int32_t modbus_serial_write(const uint8_t* buf, uint16_t count, int32_t timeout) {
	if(HAL_UART_Transmit(&huart2, buf, count, timeout) == HAL_OK) {
		return count;
	}

	return 0;
}


int main(void)
{

  HAL_Init();

  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  slave_t slave;
  nmbs_t ctx;

  uint32_t coils = 1;
  uint32_t regs = 10;

  uint8_t coils_buf[PACK_COILS(coils)];
  uint8_t input_coils_buf[PACK_COILS(coils)];
  uint16_t input_regs_buf[regs];
  uint16_t holding_regs_buf[regs];

  slave_init(&slave, 1, coils, coils, regs, regs, coils_buf, input_coils_buf, input_regs_buf, holding_regs_buf);

  modbus_timing_conf conf_instance;
  modbus_timing_conf *conf = &conf_instance;

  conf->baudrate = 115200;
   conf->data     = 8;
   conf->parity   = 0;
   conf->stopbit  = 1;

  modbus_init(&ctx, &slave, conf);

  while (1)
  {
	  modbus_poll(&ctx);

	  if(nmbs_bitfield_read(coils_buf, 0) == 1) {
		  HAL_GPIO_WritePin(GPIOA, LD2_Pin, GPIO_PIN_SET);
	  } else {
		  HAL_GPIO_WritePin(GPIOA, LD2_Pin, GPIO_PIN_RESET);
	  }
  }
}


