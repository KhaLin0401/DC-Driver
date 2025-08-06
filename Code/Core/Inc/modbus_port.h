/*
 * modbus_port.h
 *
 *  Created on: Aug 4, 2025
 *      Author: ASUS
 */

#ifndef MODBUS_PORT_H
#define MODBUS_PORT_H

#include <stdint.h>
#include <stdbool.h>

// Hàm khởi tạo UART + timer (nếu cần)
void modbus_port_init(void);

// Gửi data ra UART
void modbus_port_send(uint8_t *data, uint16_t len);

// Nhận 1 byte từ UART (được gọi từ ISR)
void modbus_port_on_byte_received(uint8_t byte);

// Thông báo timeout frame
void modbus_port_on_frame_timeout(void);

// Thời gian chờ frame timeout (3.5 char)
void modbus_port_start_timer(void);
void modbus_port_stop_timer(void);

#endif 