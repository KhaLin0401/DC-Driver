#ifndef MODBUS_CRC_H
#define MODBUS_CRC_H

#include <stdint.h>

// Tính CRC-16 theo chuẩn Modbus RTU
uint16_t modbus_crc16(const uint8_t *data, uint16_t length);

#endif // MODBUS_CRC_H
