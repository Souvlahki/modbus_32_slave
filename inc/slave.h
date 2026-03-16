#ifndef SLAVE_H_
#define SLAVE_H_

#include "common.h"

typedef struct slave_t {
    uint8_t id;
    uint32_t nb_output_coils;
    uint32_t nb_input_coils;
    uint32_t nb_input_regs;
    uint32_t nb_holding_regs;
    uint8_t *output_coils;
    uint8_t *input_coils;
    uint16_t *input_regs;
    uint16_t *holding_regs;
} slave_t;

#ifdef __cplusplus
extern "C" {
#endif
modbus_error slave_init(slave_t *self, uint8_t id,
                        uint32_t nb_output_coils, uint32_t nb_input_coils,
                        uint32_t nb_input_regs, uint32_t nb_holding_regs,
                        uint8_t *output_coils, uint8_t *input_coils
                        , uint16_t *input_regs, uint16_t *holding_regs);

#ifdef __cplusplus
}
#endif

#endif /* SLAVE_H_ */
