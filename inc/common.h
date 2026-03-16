#ifndef COMMON_H_
#define COMMON_H_

#include "../lib/nanomodbus/nanomodbus.h"

typedef nmbs_error modbus_error;
typedef nmbs_t modbus_t;

#define NMBS_CLIENT_DISABLED

#ifndef SLAVE_READ_FILE_RECONRD_ENABLE
#define NMBS_SERVER_READ_FILE_RECORD_DISABLED
#endif

#ifndef SLAVE_WRITE_FILE_RECORD_ENABLE
#define NMBS_SERVER_WRITE_FILE_RECORD_DISABLED
#endif

#ifndef SLAVE_READ_WRITE_REGISTERS_ENABLE
#define NMBS_SERVER_READ_WRITE_REGISTERS_DISABLED
#endif

#ifndef SLAVE_READ_DEVICE_IDENTIFICATION_ENABLE
#define NMBS_SERVER_READ_DEVICE_IDENTIFICATION_DISABLED
#endif

#ifndef SLAVE_STRERROR_ENABLE
#define NMBS_STRERROR_DISABLED
#endif

#ifndef __weak
#define __weak __attribute__((weak))
#endif

#define COIL_BUF_SIZE 1024
#define REG_BUF_SIZE 2048

#define PACK_COILS(nb_coils) ((nb_coils + 7) / 8)
#define read_coil(bf, b) ((bool) ((bf)[(b) >> 3] & (0x1 << ((b) & (8 - 1)))))
#define set_coil(bf, b) (((bf)[(b) >> 3]) = (((bf)[(b) >> 3]) | (0x1 << ((b) & (8 - 1)))))
#define unset_coil(bf, b) (((bf)[(b) >> 3]) = (((bf)[(b) >> 3]) & ~(0x1 << ((b) & (8 - 1)))))
#define write_coil(bf, b, v) ((bf)[(b) >> 3] = ((bf)[(b) >> 3] & ~(1 << ((b) & 7))) | ((v) << ((b) & 7)))
#define reset_buf(bf) memset(bf, 0, sizeof(bf))


#endif /* COMMON_H_ */
