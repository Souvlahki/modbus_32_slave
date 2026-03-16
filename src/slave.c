#include "slave.h"

nmbs_error slave_init(slave_t *self, uint8_t id, uint32_t nb_output_coils,
		uint32_t nb_input_coils, uint32_t nb_input_regs, uint32_t nb_holding_regs,
		uint8_t *output_coils, uint8_t *input_coils, uint16_t *input_regs, uint16_t *holding_regs) {

	if(nb_output_coils * 8 > COIL_BUF_SIZE || nb_input_coils * 8 > COIL_BUF_SIZE ||
			nb_input_regs > REG_BUF_SIZE || nb_holding_regs > REG_BUF_SIZE) {
		return NMBS_ERROR_INVALID_REQUEST;
	}

	memset(output_coils, 0, PACK_COILS(nb_output_coils) * sizeof(uint8_t));
	memset(input_coils, 0, PACK_COILS(nb_input_coils) * sizeof(uint8_t));
	memset(input_regs, 0, nb_input_regs * sizeof(uint16_t));
	memset(holding_regs, 0, nb_holding_regs * sizeof(uint16_t));


	self->id = id;
	self->nb_output_coils = nb_output_coils;
	self->nb_input_coils = nb_input_coils;
	self->nb_input_regs = nb_input_regs;
	self->nb_holding_regs = nb_holding_regs;
	self->output_coils = output_coils;
	self->input_coils = input_coils;
	self->input_regs = input_regs;
	self->holding_regs = holding_regs;

	return NMBS_ERROR_NONE;
}