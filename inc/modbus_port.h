#ifndef MODBUS_PORT_H_
#define MODBUS_PORT_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif
int32_t modbus_serial_read(uint8_t* buf, uint16_t count, int32_t timeout);

int32_t modbus_serial_write(const uint8_t* buf, uint16_t count, int32_t timeout);
#ifdef __cplusplus
}
#endif

#endif /* MODBUS_PORT_H_ */
