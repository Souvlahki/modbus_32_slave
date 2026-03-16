#include "modbus.h"
#include <string.h>


static nmbs_error slave_read_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_out, uint8_t unit_id,
                                    void* arg);
static nmbs_error slave_read_input_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_out, uint8_t unit_id,
                                    void* arg);
static nmbs_error slave_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out,
                                                uint8_t unit_id, void* arg);
static nmbs_error slave_read_input_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out,
                                                uint8_t unit_id, void* arg);
static nmbs_error slave_write_single_coil(uint16_t address, bool value, uint8_t unit_id, void* arg);
static nmbs_error slave_write_multiple_coils(uint16_t address, uint16_t quantity, const nmbs_bitfield coils,
                                              uint8_t unit_id, void* arg);
static nmbs_error slave_write_single_register(uint16_t address, uint16_t value, uint8_t unit_id, void* arg);
static nmbs_error slave_write_multiple_registers(uint16_t address, uint16_t quantity, const uint16_t* registers,
                                              uint8_t unit_id, void* arg);

static uint32_t get_char_time(modbus_timing_conf *conf);

static void modbus_conf_init(nmbs_platform_conf *conf);
static void modbus_callbacks_init(nmbs_callbacks *cb);

static slave_t *slave;


modbus_error modbus_init(modbus_t *ctx, slave_t *_slave, modbus_timing_conf *modbus_timing_conf) {
	nmbs_platform_conf conf;
	nmbs_callbacks cb;

	modbus_conf_init(&conf);


	slave = _slave;

	modbus_callbacks_init(&cb);

	nmbs_error status = nmbs_server_create(ctx, slave->id, &conf, &cb);
	    if (status != NMBS_ERROR_NONE) {
	        return status;
	    }

	nmbs_set_byte_timeout(ctx, (uint32_t)(get_char_time(modbus_timing_conf) * MAX_SILENECE_BYTE_MULT));
	nmbs_set_read_timeout(ctx, (uint32_t)(get_char_time(modbus_timing_conf) * MAX_SILENCE_FRAME_MUL));

	return NMBS_ERROR_NONE;
}

void modbus_poll(modbus_t *ctx) {
	nmbs_server_poll(ctx);
}


static void modbus_conf_init(nmbs_platform_conf *conf) {
	nmbs_platform_conf_create(conf);


	conf->transport = NMBS_TRANSPORT_RTU;
	conf->read = modbus_serial_read;
	conf->write = modbus_serial_write;
}

static void modbus_callbacks_init(nmbs_callbacks *cb) {
	nmbs_callbacks_create(cb);

		cb->read_discrete_inputs = slave_read_input_coils;
		cb->read_coils = slave_read_coils;
		cb->read_input_registers = slave_read_input_registers;
		cb->read_holding_registers = slave_read_holding_registers;
		cb->write_single_coil = slave_write_single_coil;
		cb->write_multiple_coils = slave_write_multiple_coils;
		cb->write_single_register = slave_write_single_register;
		cb->write_multiple_registers = slave_write_multiple_registers;
}

static uint32_t get_char_time(modbus_timing_conf *conf) {
    return (((conf->data + conf->parity + conf->stopbit + 1) * 1000000UL) / conf->baudrate);
}

static slave_t* get_slave(uint8_t id) {
    if (id == slave->id) {
        return slave;
    }
    else {
        return NULL;
    }
}

static nmbs_error slave_read_input_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_in, uint8_t unit_id,
                                    void* arg) {
    slave_t* slave = get_slave(unit_id);

    if(address + quantity > slave->nb_input_coils) {
    	return NMBS_ERROR_INVALID_REQUEST;
    }

    for (size_t i = 0; i < quantity; i++) {
        nmbs_bitfield_write(coils_in, address, nmbs_bitfield_read(slave->input_coils, address));
        address++;
    }
    return NMBS_ERROR_NONE;
}

static nmbs_error slave_read_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_out, uint8_t unit_id,
                                    void* arg) {
    slave_t* slave = get_slave(unit_id);

    if(address + quantity > slave->nb_output_coils) {
    	return NMBS_ERROR_INVALID_REQUEST;
    }

    for (size_t i = 0; i < quantity; i++) {
        nmbs_bitfield_write(coils_out, address, nmbs_bitfield_read(slave->output_coils, address));
        address++;
    }
    return NMBS_ERROR_NONE;
}


static nmbs_error slave_read_input_registers(uint16_t address, uint16_t quantity, uint16_t* registers_in,
                                                uint8_t unit_id, void* arg) {
    slave_t* slave = get_slave(unit_id);

    if(address + quantity > slave->nb_input_regs) {
    	return NMBS_ERROR_INVALID_REQUEST;
    }

    for (size_t i = 0; i < quantity; i++) {
        registers_in[i] = slave->input_regs[address++];
    }
    return NMBS_ERROR_NONE;
}

static nmbs_error slave_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out,
                                                uint8_t unit_id, void* arg) {
    slave_t* slave = get_slave(unit_id);

    if(address + quantity > slave->nb_holding_regs) {
       	return NMBS_ERROR_INVALID_REQUEST;
       }

    for (size_t i = 0; i < quantity; i++) {
        registers_out[i] = slave->holding_regs[address++];
    }
    return NMBS_ERROR_NONE;
}

static nmbs_error slave_write_single_coil(uint16_t address, bool value, uint8_t unit_id, void* arg) {
    uint8_t coil = 0;
    if (value) {
        coil |= 0x01;
    }
    return slave_write_multiple_coils(address, 1, &coil, unit_id, arg);
}

static nmbs_error slave_write_multiple_coils(uint16_t address, uint16_t quantity, const nmbs_bitfield coils,
                                              uint8_t unit_id, void* arg) {
    slave_t* slave = get_slave(unit_id);

    if(address + quantity > slave->nb_output_coils) {
       	return NMBS_ERROR_INVALID_REQUEST;
       }

    for (size_t i = 0; i < quantity; i++) {
        nmbs_bitfield_write(slave->output_coils, address, nmbs_bitfield_read(coils, i));
        address++;
    }
    return NMBS_ERROR_NONE;
}

static nmbs_error slave_write_single_register(uint16_t address, uint16_t value, uint8_t unit_id, void* arg) {
    uint16_t reg = value;
    return slave_write_multiple_registers(address, 1, &reg, unit_id, arg);
}

static nmbs_error slave_write_multiple_registers(uint16_t address, uint16_t quantity, const uint16_t* registers,
                                                  uint8_t unit_id, void* arg) {
    slave_t* slave = get_slave(unit_id);

    if (address + quantity > slave->nb_holding_regs) {
    	return NMBS_ERROR_INVALID_REQUEST;
    }

    for (size_t i = 0; i < quantity; i++) {
        slave->holding_regs[address++] = registers[i];
    }
    return NMBS_ERROR_NONE;
}

