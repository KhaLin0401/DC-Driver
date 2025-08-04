# 📘 Modbus Register Map – Dual DC Motor Driver (STM32F103C8T6)

## 🟣 System Registers (Global)

| Address | Name                    | Type     | R/W | Description                                  | Default |
|---------|-------------------------|----------|-----|----------------------------------------------|---------|
| 0x00F0  | Device_ID               | uint16   | R/W | Modbus slave address                         | 1       |
| 0x00F1  | Firmware_Version        | uint16   | R   | Firmware version (e.g. 0x0101 = v1.01)       | 0x0101  |
| 0x00F2  | System_Status           | uint16   | R   | Bitfield: system status                      | 0x0000  |
| 0x00F3  | System_Error            | uint16   | R   | Global error code                            | 0       |
| 0x00F4  | Reset_Error_Command     | uint16   | W   | Write 1 to reset all error flags             | 0       |
| 0x00F5  | Config_Baudrate         | uint16   | R/W | 1=9600, 2=19200, 3=38400,...                  | 2       |
| 0x00F6  | Config_Parity           | uint16   | R/W | 0=None, 1=Even, 2=Odd                         | 0       |

---

## 🔵 Motor 1 Registers

| Address | Name                    | Type     | R/W | Description                                  | Default |
|---------|-------------------------|----------|-----|----------------------------------------------|---------|
| 0x0000  | M1_Control_Mode         | uint16   | R/W | 1=ONOFF, 2=LINEAR, 3=PID                      | 1       |
| 0x0001  | M1_ONOFF_Enable         | uint16   | R/W | 1=Enable ON/OFF mode                         | 0       |
| 0x0002  | M1_LINEAR_Enable        | uint16   | R/W | 1=Enable LINEAR mode                         | 0       |
| 0x0003  | M1_PID_Enable           | uint16   | R/W | 1=Enable PID mode                            | 0       |
| 0x0004  | M1_Command_Speed        | int16    | R/W | Speed setpoint                               | 0       |
| 0x0005  | M1_Linear_Input         | uint16   | R/W | Linear control input (0–1000)                | 0       |
| 0x0006  | M1_Actual_Speed         | int16    | R   | Measured speed                               | 0       |
| 0x0007  | M1_Direction            | uint16   | R/W | 0=Forward, 1=Reverse                          | 0       |
| 0x0008  | M1_PID_Kp               | uint16   | R/W | PID Kp gain (×100)                           | 100     |
| 0x0009  | M1_PID_Ki               | uint16   | R/W | PID Ki gain (×100)                           | 10      |
| 0x000A  | M1_PID_Kd               | uint16   | R/W | PID Kd gain (×100)                           | 5       |
| 0x000B  | M1_Status_Word          | uint16   | R   | Motor status flags                           | 0x0000  |
| 0x000C  | M1_Error_Code           | uint16   | R   | Error code if any                            | 0       |

---

## 🟢 Motor 2 Registers

| Address | Name                    | Type     | R/W | Description                                  | Default |
|---------|-------------------------|----------|-----|----------------------------------------------|---------|
| 0x0010  | M2_Control_Mode         | uint16   | R/W | 1=ONOFF, 2=LINEAR, 3=PID                      | 1       |
| 0x0011  | M2_ONOFF_Enable         | uint16   | R/W | 1=Enable ON/OFF mode                         | 0       |
| 0x0012  | M2_LINEAR_Enable        | uint16   | R/W | 1=Enable LINEAR mode                         | 0       |
| 0x0013  | M2_PID_Enable           | uint16   | R/W | 1=Enable PID mode                            | 0       |
| 0x0014  | M2_Command_Speed        | int16    | R/W | Speed setpoint                               | 0       |
| 0x0015  | M2_Linear_Input         | uint16   | R/W | Linear control input (0–1000)                | 0       |
| 0x0016  | M2_Actual_Speed         | int16    | R   | Measured speed                               | 0       |
| 0x0017  | M2_Direction            | uint16   | R/W | 0=Forward, 1=Reverse                          | 0       |
| 0x0018  | M2_PID_Kp               | uint16   | R/W | PID Kp gain (×100)                           | 100     |
| 0x0019  | M2_PID_Ki               | uint16   | R/W | PID Ki gain (×100)                           | 10      |
| 0x001A  | M2_PID_Kd               | uint16   | R/W | PID Kd gain (×100)                           | 5       |
| 0x001B  | M2_Status_Word          | uint16   | R   | Motor status flags                           | 0x0000  |
| 0x001C  | M2_Error_Code           | uint16   | R   | Error code if any   