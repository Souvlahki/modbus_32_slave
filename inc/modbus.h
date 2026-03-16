#ifndef MODBUS_H_
#define MODBUS_H_

#include "common.h"
#include "slave.h"
#include "modbus_port.h"



#define MAX_SILENECE_BYTE_MULT 1.5
#define MAX_SILENCE_FRAME_MUL 3.5


typedef struct modbus_timing_conf {
    uint32_t baudrate;
    bool parity;
    uint8_t stopbit;
    uint8_t data;
} modbus_timing_conf;


#ifdef __cplusplus
extern "C" {
#endif

modbus_error modbus_init(modbus_t *ctx, slave_t *slave, modbus_timing_conf *modbus_timing_conf);

void modbus_poll(modbus_t *ctx);

#ifdef __cplusplus
}
#endif

#endif /* MODBUS_H_ */
