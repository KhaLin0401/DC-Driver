/*
 * ModbusMap.h
 *
 *  Created on: Aug 4, 2025
 *      Author: ASUS
 */

#ifndef INC_MODBUSMAP_H_
#define INC_MODBUSMAP_H_

#include <stdint.h>

// FreeModbus type definitions for compatibility
#ifndef UCHAR
#define UCHAR uint8_t
#endif

#ifndef USHORT
#define USHORT uint16_t
#endif

#ifndef ULONG
#define ULONG uint32_t
#endif

// FreeModbus error codes
typedef enum {
    MB_ENOERR = 0x00,
    MB_ENOREG = 0x02,
    MB_EINVAL = 0x04,
    MB_EPORTERR = 0x06,
    MB_ENORES = 0x08,
    MB_EIO = 0x0A,
    MB_EILLSTATE = 0x0B,
    MB_ETIMEDOUT = 0x0E
} eMBErrorCode;

// FreeModbus register modes
typedef enum {
    MB_REG_READ = 0,
    MB_REG_WRITE = 1
} eMBRegisterMode;

typedef enum {
    // Motor 1 Registers (0x0000 - 0x000D)
    REG_M1_CONTROL_MODE = 0x0000,
    REG_M1_ONOFF_ENABLE,
    REG_M1_LINEAR_ENABLE,
    REG_M1_PID_ENABLE,
    REG_M1_COMMAND_SPEED,
    REG_M1_LINEAR_INPUT,
    REG_M1_ACTUAL_SPEED,
    REG_M1_DIRECTION,
    REG_M1_PID_KP,
    REG_M1_PID_KI,
    REG_M1_PID_KD,
    REG_M1_STATUS_WORD,
    REG_M1_ERROR_CODE,

    // Motor 2 Registers (0x0010 - 0x001D)
    REG_M2_CONTROL_MODE = 0x0010,
    REG_M2_ONOFF_ENABLE,
    REG_M2_LINEAR_ENABLE,
    REG_M2_PID_ENABLE,
    REG_M2_COMMAND_SPEED,
    REG_M2_LINEAR_INPUT,
    REG_M2_ACTUAL_SPEED,
    REG_M2_DIRECTION,
    REG_M2_PID_KP,
    REG_M2_PID_KI,
    REG_M2_PID_KD,
    REG_M2_STATUS_WORD,
    REG_M2_ERROR_CODE,

    // System Registers (0x0020 - 0x0026) - FIXED: Moved to avoid conflicts
    REG_DEVICE_ID = 0x0020,
    REG_FIRMWARE_VERSION,
    REG_SYSTEM_STATUS,
    REG_SYSTEM_ERROR,
    REG_RESET_ERROR_COMMAND,
    REG_CONFIG_BAUDRATE,
    REG_CONFIG_PARITY,

    TOTAL_REG_COUNT  // Use this to size the holding register array
} ModbusRegisterMap_t;

// Struct for holding register values - FreeModbus compatible
typedef struct {
    // Motor 1 (0x0000 - 0x000D)
    uint16_t m1_mode;
    uint16_t m1_onoff_en;
    uint16_t m1_linear_en;
    uint16_t m1_pid_en;
    int16_t  m1_cmd_speed;
    uint16_t m1_linear_input;
    int16_t  m1_actual_speed;
    uint16_t m1_direction;
    uint16_t m1_kp;
    uint16_t m1_ki;
    uint16_t m1_kd;
    uint16_t m1_status;
    uint16_t m1_error;

    // Motor 2 (0x0010 - 0x001D)
    uint16_t m2_mode;
    uint16_t m2_onoff_en;
    uint16_t m2_linear_en;
    uint16_t m2_pid_en;
    int16_t  m2_cmd_speed;
    uint16_t m2_linear_input;
    int16_t  m2_actual_speed;
    uint16_t m2_direction;
    uint16_t m2_kp;
    uint16_t m2_ki;
    uint16_t m2_kd;
    uint16_t m2_status;
    uint16_t m2_error;

    // System (0x0020 - 0x0026)
    uint16_t device_id;
    uint16_t firmware_ver;
    uint16_t system_status;
    uint16_t system_error;
    uint16_t reset_error_cmd;
    uint16_t config_baudrate;
    uint16_t config_parity;
} tModbusRegisters;

// Global instance
extern tModbusRegisters g_modbus_data;

// FreeModbus callback function declaration
eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                              USHORT usNRegs, eMBRegisterMode eMode);

#endif /* INC_MODBUSMAP_H_ */
