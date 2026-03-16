#include <Arduino.h>
#include "modbus.h"

int32_t modbus_serial_read(uint8_t* buf, uint16_t count, int32_t timeout)
{
    uint16_t i = 0;

    while (i < count)
    {
        if (Serial.available() > 0)
        {
            buf[i++] = Serial.read();
        }

       
    }

    return i;
}

int32_t modbus_serial_write(const uint8_t* buf, uint16_t count, int32_t timeout)
{
    size_t written = Serial.write(buf, count);

    Serial.flush(); // wait for TX complete (important for Modbus timing)

    return written;
}

nmbs_t ctx;
slave_t slave;
modbus_timing_conf conf_instance;
modbus_timing_conf *conf = &conf_instance;

void setup() {
  uint32_t coils = 1;
  uint32_t regs = 1;
  uint8_t coils_buf[PACK_COILS(coils)];
  uint8_t input_coils_buf[PACK_COILS(coils)];
  uint16_t input_regs_buf[regs];
  uint16_t holding_regs_buf[regs];


  conf->baudrate = 115200;
  conf->data     = 8;
  conf->parity   = 0; 
  conf->stopbit  = 1;

  slave_init(&slave, 1, coils, coils, regs, regs, coils_buf, input_coils_buf, input_regs_buf, holding_regs_buf);
  modbus_init(&ctx, &slave, conf);
  Serial.begin(115200);
  
}

void loop() {
  modbus_poll(&ctx);
}

