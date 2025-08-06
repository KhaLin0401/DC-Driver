#ifndef MODBUS_PORT_H
#define MODBUS_PORT_H

#include <stdint.h>
#include <stdbool.h>

// MODBUS PORT LAYER FOR STM32F103
// Hardware abstraction layer for UART and Timer

// Initialize hardware (UART and Timer)
void modbus_port_init(void);

// Send data via UART
void modbus_port_send(uint8_t *data, uint16_t len);

// Timer control functions
void modbus_port_start_timer(void);
void modbus_port_stop_timer(void);

// Callback functions (implemented in modbus_port.c)
void modbus_port_on_byte_received(uint8_t byte);
void modbus_port_on_frame_timeout(void);

#endif // MODBUS_PORT_H
