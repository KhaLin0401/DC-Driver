#include "ModbusMap.h"


// Global instance of register map
tModbusRegisters g_modbus_data = {
    // Motor 1 (0x0000 - 0x000D)
    .m1_mode = 1,
    .m1_onoff_en = 0,
    .m1_linear_en = 0,
    .m1_pid_en = 0,
    .m1_cmd_speed = 0,
    .m1_linear_input = 0,
    .m1_actual_speed = 0,
    .m1_direction = 0,
    .m1_kp = 100,
    .m1_ki = 10,
    .m1_kd = 5,
    .m1_status = 0,
    .m1_error = 0,

    // Motor 2 (0x0010 - 0x001D)
    .m2_mode = 1,
    .m2_onoff_en = 0,
    .m2_linear_en = 0,
    .m2_pid_en = 0,
    .m2_cmd_speed = 0,
    .m2_linear_input = 0,
    .m2_actual_speed = 0,
    .m2_direction = 0,
    .m2_kp = 100,
    .m2_ki = 10,
    .m2_kd = 5,
    .m2_status = 0,
    .m2_error = 0,

    // System (0x0020 - 0x0026)
    .device_id = 1,
    .firmware_ver = 0x0101,
    .system_status = 0,
    .system_error = 0,
    .reset_error_cmd = 0,
    .config_baudrate = 2,
    .config_parity = 0
};

// Register mapping constants
#define REG_START       0x0000
#define REG_END         0x0026
#define REG_SIZE        (REG_END + 1)

// Holding register callback for FreeModbus
eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                              USHORT usNRegs, eMBRegisterMode eMode) {
    USHORT iRegIndex;
    USHORT *pData = (USHORT*)&g_modbus_data;

    // Validate register range
    if ((usAddress < REG_START) || ((usAddress + usNRegs) > REG_SIZE)) {
        return MB_ENOREG;
    }

    iRegIndex = usAddress - REG_START;

    switch (eMode) {
    case MB_REG_READ:
        // Read registers - convert to big-endian format
        for (USHORT i = 0; i < usNRegs; i++) {
            pucRegBuffer[2 * i]     = (UCHAR)(pData[iRegIndex + i] >> 8);    // High byte
            pucRegBuffer[2 * i + 1] = (UCHAR)(pData[iRegIndex + i] & 0xFF); // Low byte
        }
        break;

    case MB_REG_WRITE:
        // Write registers - convert from big-endian format
        for (USHORT i = 0; i < usNRegs; i++) {
            pData[iRegIndex + i] = (USHORT)((pucRegBuffer[2 * i] << 8) |
                                            pucRegBuffer[2 * i + 1]);
        }
        break;
    }

    return MB_ENOERR;
}


