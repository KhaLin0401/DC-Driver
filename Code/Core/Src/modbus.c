/*
 * modbus.c
 *
 *  Created on: Aug 4, 2025
 *      Author: ASUS
 */

#include "modbus.h"
#include "modbus_crc.h"
#include "modbus_port.h"
#include <string.h>

// MODBUS RTU SLAVE IMPLEMENTATION
// Clean, standard implementation for STM32F103

// Internal state
static uint8_t rx_buffer[MODBUS_BUFFER_SIZE];
static uint8_t tx_buffer[MODBUS_BUFFER_SIZE];
static uint16_t rx_index = 0;
static uint16_t tx_index = 0;
static uint16_t holding_registers[MODBUS_MAX_REGISTERS];
static modbus_status_t status = MODBUS_OK;
static uint16_t rx_count = 0;
static uint16_t tx_count = 0;

// Function prototypes
static void process_frame(void);
static void send_response(uint8_t *data, uint16_t len);
static void send_exception(uint8_t address, uint8_t function, uint8_t exception);
static bool validate_register_range(uint16_t start_addr, uint16_t quantity);
static void reset_rx_buffer(void);

void modbus_init(void) {
    // Initialize port layer
    modbus_port_init();
    
    // Clear all registers
    memset(holding_registers, 0, sizeof(holding_registers));
    
    // Reset state
    reset_rx_buffer();
    status = MODBUS_OK;
    rx_count = 0;
    tx_count = 0;
    
    // Set default values for testing
    holding_registers[0x0000] = 50;   // Motor 1 Target Speed
    holding_registers[0x0001] = 45;   // Motor 1 Current Speed
    holding_registers[0x0002] = 1;    // Motor 1 Direction
    holding_registers[0x0010] = 60;   // Motor 2 Target Speed
    holding_registers[0x0011] = 55;   // Motor 2 Current Speed
    holding_registers[0x0020] = 0x1234; // Device ID
    holding_registers[0x0021] = 0x0100; // Firmware Version
}

void modbus_receive_byte(uint8_t byte) {
    // Add byte to buffer
    if (rx_index < MODBUS_BUFFER_SIZE) {
        rx_buffer[rx_index++] = byte;
        rx_count++;
        
        // Start/reset timeout timer
        modbus_port_start_timer();
    }
}

void modbus_handle_timeout(void) {
    // Process frame if we have enough data
    if (rx_index >= 4) {
        process_frame();
    }
    
    // Reset for next frame
    reset_rx_buffer();
}

uint16_t modbus_read_register(uint16_t address) {
    if (address < MODBUS_MAX_REGISTERS) {
        return holding_registers[address];
    }
    return 0;
}

void modbus_write_register(uint16_t address, uint16_t value) {
    if (address < MODBUS_MAX_REGISTERS) {
        holding_registers[address] = value;
    }
}

modbus_status_t modbus_get_status(void) {
    return status;
}

uint16_t modbus_get_rx_count(void) {
    return rx_count;
}

uint16_t modbus_get_tx_count(void) {
    return tx_count;
}

// Internal functions
static void reset_rx_buffer(void) {
    rx_index = 0;
    memset(rx_buffer, 0, MODBUS_BUFFER_SIZE);
}

static bool validate_register_range(uint16_t start_addr, uint16_t quantity) {
    // Check bounds
    if (start_addr >= MODBUS_MAX_REGISTERS || 
        quantity == 0 || 
        quantity > MODBUS_MAX_REGISTERS ||
        start_addr + quantity > MODBUS_MAX_REGISTERS) {
        return false;
    }
    
    // Check valid ranges
    if (start_addr >= 0x0000 && start_addr < 0x0010) {
        return true; // Motor 1 registers
    } else if (start_addr >= 0x0010 && start_addr < 0x0020) {
        return true; // Motor 2 registers
    } else if (start_addr >= 0x0020 && start_addr < 0x0030) {
        return true; // System registers
    }
    
    return false;
}

static void send_response(uint8_t *data, uint16_t len) {
    // Calculate CRC
    uint16_t crc = modbus_crc16(data, len);
    
    // Add CRC to response
    data[len++] = crc & 0xFF;        // CRC Low byte
    data[len++] = (crc >> 8) & 0xFF; // CRC High byte
    
    // Send via port layer
    modbus_port_send(data, len);
    tx_count++;
}

static void send_exception(uint8_t address, uint8_t function, uint8_t exception) {
    uint8_t response[5];
    response[0] = address;
    response[1] = function | 0x80;  // Set MSB for exception
    response[2] = exception;
    
    send_response(response, 3);
}

static void process_frame(void) {
    // Check minimum frame length
    if (rx_index < 4) {
        status = MODBUS_ERROR_FRAME;
        return;
    }
    
    // Extract address and function
    uint8_t address = rx_buffer[0];
    uint8_t function = rx_buffer[1];
    
    // Check slave address
    if (address != MODBUS_SLAVE_ADDRESS) {
        status = MODBUS_ERROR_ADDRESS;
        return;
    }
    
    // Verify CRC
    uint16_t received_crc = rx_buffer[rx_index - 2] | (rx_buffer[rx_index - 1] << 8);
    uint16_t calculated_crc = modbus_crc16(rx_buffer, rx_index - 2);
    
    if (received_crc != calculated_crc) {
        status = MODBUS_ERROR_CRC;
        return;
    }
    
    // Process by function code
    switch (function) {
        case MODBUS_FC_READ_HOLDING_REGISTERS: {
            if (rx_index < 8) {
                send_exception(address, function, MODBUS_EX_ILLEGAL_DATA_VALUE);
                return;
            }
            
            uint16_t start_addr = (rx_buffer[2] << 8) | rx_buffer[3];
            uint16_t quantity = (rx_buffer[4] << 8) | rx_buffer[5];
            
            if (!validate_register_range(start_addr, quantity)) {
                send_exception(address, function, MODBUS_EX_ILLEGAL_DATA_ADDRESS);
                return;
            }
            
            // Prepare response
            uint8_t response[MODBUS_BUFFER_SIZE];
            response[0] = address;
            response[1] = function;
            response[2] = quantity * 2;  // Byte count
            
            // Add register values
            for (uint16_t i = 0; i < quantity; i++) {
                uint16_t value = modbus_read_register(start_addr + i);
                response[3 + i * 2] = value >> 8;     // High byte
                response[4 + i * 2] = value & 0xFF;   // Low byte
            }
            
            send_response(response, 3 + quantity * 2);
            status = MODBUS_OK;
            break;
        }
        
        case MODBUS_FC_WRITE_SINGLE_REGISTER: {
            if (rx_index < 8) {
                send_exception(address, function, MODBUS_EX_ILLEGAL_DATA_VALUE);
                return;
            }
            
            uint16_t reg_addr = (rx_buffer[2] << 8) | rx_buffer[3];
            uint16_t value = (rx_buffer[4] << 8) | rx_buffer[5];
            
            if (!validate_register_range(reg_addr, 1)) {
                send_exception(address, function, MODBUS_EX_ILLEGAL_DATA_ADDRESS);
                return;
            }
            
            // Write register
            modbus_write_register(reg_addr, value);
            
            // Echo the request as response
            send_response(rx_buffer, 6);
            status = MODBUS_OK;
            break;
        }
        
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS: {
            if (rx_index < 9) {
                send_exception(address, function, MODBUS_EX_ILLEGAL_DATA_VALUE);
                return;
            }
            
            uint16_t start_addr = (rx_buffer[2] << 8) | rx_buffer[3];
            uint16_t quantity = (rx_buffer[4] << 8) | rx_buffer[5];
            uint8_t byte_count = rx_buffer[6];
            
            // Validate frame length
            if (rx_index != 9 + byte_count) {
                send_exception(address, function, MODBUS_EX_ILLEGAL_DATA_VALUE);
                return;
            }
            
            // Validate register range
            if (!validate_register_range(start_addr, quantity)) {
                send_exception(address, function, MODBUS_EX_ILLEGAL_DATA_ADDRESS);
                return;
            }
            
            // Validate byte count
            if (byte_count != quantity * 2) {
                send_exception(address, function, MODBUS_EX_ILLEGAL_DATA_VALUE);
                return;
            }
            
            // Write registers
            for (uint16_t i = 0; i < quantity; i++) {
                uint16_t value = (rx_buffer[7 + i * 2] << 8) | rx_buffer[8 + i * 2];
                modbus_write_register(start_addr + i, value);
            }
            
            // Send response
            uint8_t response[6];
            response[0] = address;
            response[1] = function;
            response[2] = start_addr >> 8;
            response[3] = start_addr & 0xFF;
            response[4] = quantity >> 8;
            response[5] = quantity & 0xFF;
            
            send_response(response, 6);
            status = MODBUS_OK;
            break;
        }
        
        default:
            send_exception(address, function, MODBUS_EX_ILLEGAL_FUNCTION);
            status = MODBUS_ERROR_FRAME;
            break;
    }
} 