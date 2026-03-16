#include "modbus_port.h"

__weak int32_t modbus_serial_read(uint8_t* buf, uint16_t count, int32_t timeout) {
	return 0;
}

__weak int32_t modbus_serial_write(const uint8_t* buf, uint16_t count, int32_t timeout) {
	return 0;
}