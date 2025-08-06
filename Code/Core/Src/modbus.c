/*
 * modbus.c
 *
 *  Created on: Aug 4, 2025
 *      Author: ASUS
 */

#include "modbus.h"
#include "modbus_crc.h"
#include "modbus_config.h"
#include "modbus_port.h"
#include <string.h>
#include <stdbool.h>

static uint8_t modbus_rx_buffer[MODBUS_BUFFER_SIZE];
static uint16_t modbus_rx_index = 0;

// Bộ thanh ghi ảo đơn giản
static uint16_t holding_registers[MODBUS_MAX_REGISTERS];

static void modbus_send_response(uint8_t *data, uint16_t len);
static void modbus_process_frame(void);

void modbus_init(void) {
    modbus_port_init();
    memset(holding_registers, 0, sizeof(holding_registers));
}

void modbus_receive_byte(uint8_t byte) {
    if (modbus_rx_index < MODBUS_BUFFER_SIZE) {
        modbus_rx_buffer[modbus_rx_index++] = byte;
        modbus_port_start_timer();  // reset timeout timer
    }
}

void modbus_on_frame_timeout(void) {
    if (modbus_rx_index >= 4) {
        modbus_process_frame();
    }
    modbus_rx_index = 0;  // reset for next frame
}

static void modbus_send_response(uint8_t *data, uint16_t len) {
    uint16_t crc = modbus_crc16(data, len);
    data[len++] = crc & 0xFF;        // CRC Low byte
    data[len++] = (crc >> 8) & 0xFF; // CRC High byte
    modbus_port_send(data, len);
}

static void modbus_exception_response(uint8_t address, uint8_t function, uint8_t exception_code) {
    uint8_t response[5];
    response[0] = address;
    response[1] = function | 0x80;
    response[2] = exception_code;
    modbus_send_response(response, 3);
}

uint16_t modbus_read_register(uint16_t reg_addr) {
    if (reg_addr < MODBUS_MAX_REGISTERS)
        return holding_registers[reg_addr];
    return 0;
}

void modbus_write_register(uint16_t reg_addr, uint16_t value) {
    if (reg_addr < MODBUS_MAX_REGISTERS)
        holding_registers[reg_addr] = value;
}

// Validation function cho register addresses
static bool modbus_validate_register_range(uint16_t start_addr, uint16_t quantity) {
    // Kiểm tra bounds
    if (start_addr >= MODBUS_MAX_REGISTERS || 
        quantity == 0 || 
        quantity > MODBUS_MAX_REGISTERS ||
        start_addr + quantity > MODBUS_MAX_REGISTERS) {
        return false;
    }
    
    // Kiểm tra register ranges cho motor control
    if (start_addr >= 0x0000 && start_addr < 0x0010) {
        // Motor 1 registers - OK
        return true;
    } else if (start_addr >= 0x0010 && start_addr < 0x0020) {
        // Motor 2 registers - OK  
        return true;
    } else if (start_addr >= 0x00F0 && start_addr < 0x0100) {
        // System registers - OK
        return true;
    }
    
    return false;
}

static void modbus_process_frame(void) {
    uint8_t addr = modbus_rx_buffer[0];
    if (addr != MODBUS_SLAVE_ADDRESS) return;

    uint16_t received_crc = modbus_rx_buffer[modbus_rx_index - 2] |
                            (modbus_rx_buffer[modbus_rx_index - 1] << 8);
    uint16_t calculated_crc = modbus_crc16(modbus_rx_buffer, modbus_rx_index - 2);
    if (received_crc != calculated_crc) return;

    uint8_t func = modbus_rx_buffer[1];
    switch (func) {
        case 0x03: { // Read Holding Registers
            if (modbus_rx_index < 8) return;
            uint16_t start_addr = (modbus_rx_buffer[2] << 8) | modbus_rx_buffer[3];
            uint16_t quantity   = (modbus_rx_buffer[4] << 8) | modbus_rx_buffer[5];

            if (!modbus_validate_register_range(start_addr, quantity)) {
                modbus_exception_response(addr, func, 0x02); // Illegal data address
                return;
            }

            uint8_t response[MODBUS_BUFFER_SIZE];
            response[0] = addr;
            response[1] = func;
            response[2] = quantity * 2;
            for (uint16_t i = 0; i < quantity; i++) {
                uint16_t val = modbus_read_register(start_addr + i);
                response[3 + i * 2] = val >> 8;
                response[4 + i * 2] = val & 0xFF;
            }
            modbus_send_response(response, 3 + quantity * 2);
            break;
        }

        case 0x06: { // Write Single Register
            if (modbus_rx_index < 8) return;
            uint16_t reg_addr = (modbus_rx_buffer[2] << 8) | modbus_rx_buffer[3];
            uint16_t value    = (modbus_rx_buffer[4] << 8) | modbus_rx_buffer[5];

            if (!modbus_validate_register_range(reg_addr, 1)) {
                modbus_exception_response(addr, func, 0x02);
                return;
            }

            modbus_write_register(reg_addr, value);
            modbus_send_response(modbus_rx_buffer, 6);  // Echo request
            break;
        }

        case 0x10: { // Write Multiple Registers
            if (modbus_rx_index < 9) return;
            uint16_t start_addr = (modbus_rx_buffer[2] << 8) | modbus_rx_buffer[3];
            uint16_t quantity   = (modbus_rx_buffer[4] << 8) | modbus_rx_buffer[5];
            uint8_t byte_count = modbus_rx_buffer[6];

            // Validate frame length
            if (modbus_rx_index != 9 + byte_count) {
                modbus_exception_response(addr, func, 0x03); // Illegal data value
                return;
            }

            // Validate register range
            if (!modbus_validate_register_range(start_addr, quantity)) {
                modbus_exception_response(addr, func, 0x02);
                return;
            }

            // Validate byte count
            if (byte_count != quantity * 2) {
                modbus_exception_response(addr, func, 0x03);
                return;
            }

            // Write registers
            for (uint16_t i = 0; i < quantity; i++) {
                uint16_t value = (modbus_rx_buffer[7 + i * 2] << 8) | 
                                modbus_rx_buffer[8 + i * 2];
                modbus_write_register(start_addr + i, value);
            }

            // Send response
            uint8_t response[6];
            response[0] = addr;
            response[1] = func;
            response[2] = start_addr >> 8;
            response[3] = start_addr & 0xFF;
            response[4] = quantity >> 8;
            response[5] = quantity & 0xFF;
            modbus_send_response(response, 6);
            break;
        }

        default:
            modbus_exception_response(addr, func, 0x01); // Illegal function
            break;
    }
} // end modbus_process_frame 