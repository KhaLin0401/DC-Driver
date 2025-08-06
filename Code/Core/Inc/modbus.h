/*
 * modbus.h
 *
 *  Created on: Aug 4, 2025
 *      Author: ASUS
 */

#ifndef MODBUS_H
#define MODBUS_H

#include <stdint.h>

void modbus_init(void);

/**
 * @brief Nhận 1 byte từ UART (gọi từ ISR)
 * @param byte Byte nhận được từ UART
 * 
 * Hàm này được gọi từ UART RX interrupt
 * Tự động reset timeout timer
 */
void modbus_receive_byte(uint8_t byte);

/**
 * @brief Xử lý frame timeout (gọi từ timer ISR)
 * 
 * Khi timer timeout, frame được xử lý nếu đủ dữ liệu
 * Reset buffer cho frame tiếp theo
 */
void modbus_on_frame_timeout(void);

/**
 * @brief Đọc giá trị từ holding register
 * @param reg_addr Địa chỉ register (0x0000 - 0x003F)
 * @return Giá trị register (16-bit)
 */
uint16_t modbus_read_register(uint16_t reg_addr);

/**
 * @brief Ghi giá trị vào holding register
 * @param reg_addr Địa chỉ register (0x0000 - 0x003F)
 * @param value Giá trị cần ghi (16-bit)
 */
void modbus_write_register(uint16_t reg_addr, uint16_t value);

#endif 