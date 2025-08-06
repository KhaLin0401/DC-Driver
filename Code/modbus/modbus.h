
#ifndef MODBUS_H
#define MODBUS_H

#include <stdint.h>
#include <stdbool.h>

// MODBUS RTU SLAVE IMPLEMENTATION
// Standard Modbus RTU protocol implementation for STM32F103

// Configuration
#define MODBUS_SLAVE_ADDRESS     0x01
#define MODBUS_BUFFER_SIZE       256
#define MODBUS_MAX_REGISTERS     64
#define MODBUS_FRAME_TIMEOUT_MS  4

// Function codes
#define MODBUS_FC_READ_HOLDING_REGISTERS    0x03
#define MODBUS_FC_WRITE_SINGLE_REGISTER    0x06
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS 0x10

// Exception codes
#define MODBUS_EX_ILLEGAL_FUNCTION          0x01
#define MODBUS_EX_ILLEGAL_DATA_ADDRESS      0x02
#define MODBUS_EX_ILLEGAL_DATA_VALUE        0x03
#define MODBUS_EX_SLAVE_DEVICE_FAILURE      0x04

// Status codes
typedef enum {
    MODBUS_OK = 0,
    MODBUS_ERROR_TIMEOUT,
    MODBUS_ERROR_CRC,
    MODBUS_ERROR_FRAME,
    MODBUS_ERROR_ADDRESS
} modbus_status_t;

// Initialize Modbus slave
void modbus_init(void);

// Process received byte (call from UART ISR)
void modbus_receive_byte(uint8_t byte);

// Handle frame timeout (call from timer ISR)
void modbus_handle_timeout(void);

// Register access functions
uint16_t modbus_read_register(uint16_t address);
void modbus_write_register(uint16_t address, uint16_t value);

// Get status
modbus_status_t modbus_get_status(void);

// Debug functions
uint16_t modbus_get_rx_count(void);
uint16_t modbus_get_tx_count(void);

#endif // MODBUS_H

