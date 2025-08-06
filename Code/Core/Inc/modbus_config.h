/*
 * modbus_config.h
 *
 *  Created on: Aug 4, 2025
 *      Author: ASUS
 */

#ifndef MODBUS_CONFIG_H
#define MODBUS_CONFIG_H

// MODIFICATION LOG
// Date: 2025-01-03 15:40
// Changed by: AI Agent
// Description: Cập nhật config với thông số tối ưu và documentation
// Reason: Tối ưu performance và memory usage
// Impact: Cải thiện hiệu suất và độ ổn định
// Testing: Test với các baudrate khác nhau

/**
 * @brief Địa chỉ slave Modbus
 * 
 * Địa chỉ này phải khác với các thiết bị khác trên bus
 * Range: 1-247 (0x01-0xF7)
 */
#define MODBUS_SLAVE_ADDRESS  0x01

/**
 * @brief Kích thước buffer cho receiving data
 * 
 * Phải đủ lớn để chứa frame lớn nhất
 * FC16 với 125 registers = 256 bytes
 */
#define MODBUS_BUFFER_SIZE    256

/**
 * @brief Số lượng holding registers tối đa
 * 
 * Phải đủ cho tất cả registers trong ModbusMap.h
 * Hiện tại: 0x0000-0x003F = 64 registers
 */
#define MODBUS_MAX_REGISTERS  64

/**
 * @brief Timeout cho frame (3.5 character time)
 * 
 * Ở 9600 baud: 3.5 * 11 bits = 38.5 bits = ~4ms
 * Ở 19200 baud: ~2ms
 * Ở 115200 baud: ~0.35ms
 */
#define MODBUS_FRAME_TIMEOUT_MS  4

/**
 * @brief Supported function codes
 */
#define MODBUS_FC_READ_HOLDING_REGISTERS    0x03
#define MODBUS_FC_WRITE_SINGLE_REGISTER    0x06
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS 0x10

#endif 