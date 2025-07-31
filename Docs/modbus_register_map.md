# 📘 Modbus RTU Register Mapping - STM32 Power Management

This document defines the Modbus RTU register map used for communication between STM32 and an external master device (e.g. Radxa) via RS485. The values are cached from BMS and SK60X modules and stored in EEPROM (AT24C256).

## ⚙️ Modbus Configuration

| Address   | Name | Description | Data Type | Values / Notes | Default | Access |
|-----------|------|-------------|-----------|----------------|---------|--------|
| `0x0300`  | `slave_id` | Modbus slave address | `uint8` | `1–247` | `0x01` | Read/Write |
| `0x0101`  | `baudrate_code`| UART baudrate setting (encoded) | `uint8` | `1=9600`, `2=19200`, `3=38400`, `4=57600`, `5=115200` | `1` | Read/Write |
| `0x0102`  | `parity`| UART parity | `uint8` | `0=None`, `1=Even`, `2=Odd` | `0` | Read/Write |
| `0x0103`  | `stop_bits` | UART stop bits | `uint8` | `1` or `2` | `1` | Read/Write |
| `0x0104`  | `fc_mask` | Supported function codes (bitmask) | `uint8` | Bitmask: `0x01=FC3`, `0x02=FC6`, `0x04=FC16` | `0x07` | Read/Write |
| `0x0105`  | `apply_config` | Apply config flag (write = 1 to apply)| `uint8` | `1 = Apply config` | `0` | Write only |
| `0x0106`  | `config_crc` | Optional CRC for config validation | `uint16` | CRC16 over above config | `0x0000`| Optional |

---


## 📑 Register Map
### 0x0000
| Address | Name | Unit | Description | Data Type | Access |
|---------|------|------|-------------|-----------|--------|
| `0x0000`| M1_TargetSpeed | % |  Target speed of motor1| `uint16` | R/W |
| `0x0001`| M1_CurentSpeed | % | Curent speed of motor1 | `uint16` | RO  |
| `0x0002`| M1_Direction   | - | Direction of motor1    | `uint16` | R/W |
| `0x0003`| M1_Kp | - | Kp motor1 | `uint16_t (×100)` | R/W |
| `0x0004`| M1_Ki | - | Ki motor1 | `uint16_t (×100)` | R/W |
| `0x0005`| M1_Kd | - | Kd motor1 | `uint16_t (×100)` | R/W |
| `0x0010`| M2_TargetSpeed | % |  Target speed of motor2| `uint16` | R/W |
| `0x0011`| M2_CurentSpeed | % | Curent speed of motor2 | `uint16` | RO  |
| `0x0012`| M2_Direction   | - | Direction of motor2    | `uint16` | R/W |
| `0x0013`| M2_Kp | - | Kp motor2 | `uint16_t (×100)` | R/W |
| `0x0014`| M2_Ki | - | Ki motor2 | `uint16_t (×100)` | R/W |
| `0x0015`| M2_Kd | - | Kd motor2 | `uint16_t (×100)` | R/W |
| `0x0020`| System_Status | - | System status | `uint16` | RO |
| `0x0021`| Error_Code    | - | Error code    | `uint16` | RO |


### 0x0030 - SK60X Data
| Address | Name | Unit | Description | Data Type | Access |
|---------|------|------|-------------|-----------|--------|
| `0x0030–31`| v_set | V | Voltage setpoint | `float` | Read/Write |
| `0x0032-33`| i_set | A | Current setpoint | `float` | Read/Write |
| `0x0034–35`| v_out | V | Output voltage | `float` | Read Only |
| `0x0036–37`| i_out | A | Output current | `float` | Read Only |
| `0x0038–39`| p_out | W | Output power   | `float` | Read Only |
| `0x003A–3B`| v_in  | V | Input voltage  | `float` | Read Only |
| `0x003C–3D`| i_in  | A | Input current  | `float` | Read Only |
| `0x003E–3F`| temp | °C | Temperature of SK60X | `float` | Read Only |
| `0x0040` | h_use | h | Time used – hours | `uint8` | Read Only |
| `0x0041` | m_use | m | Time used – minutes | `uint8` | Read Only |
| `0x0042` | s_use | s | Time used – seconds | `uint8` | Read Only |
| `0x0043` | status | | Operational status | `bool` | Read Only |
| `0x0044` | on_off | | Output ON/OFF state | `bool` | Read/Write |
| `0x0045` | charge_relay | | 1 = Enable , 0 = Disable | `bool` | Read/Write |
| `0x0046` | charge_state | | 0 = NOT, 1 = PREPARE, 2 = READY | `bool` | Read/Write |



### 0x005 - INA219 Sensor Values 

| Address | Name | Unit | Description | Data Type | Access    |
|---------|------|------|-------------|-----------|-----------|
| `0x0050` | 12V Voltage Output | mV | Voltage of 12V output | `uint8` | Read Only |
| `0x0051` | 12V Current Output | mA | Current of 12V output | `uint8` | Read Only |
| `0x0052` | 5V Voltage Output | mV | Voltage of 5V output | `uint8` | Read Only |
| `0x0053` | 5V Current Output | mA | Current of 5V output | `uint16`  | Read Only |
| `0x0054` | 3.3V Voltage Output | mV | Voltage of 3.3V output | `uint8`  | Read Only |
| `0x0055` | 3.3V Current Output | mA | Current of 3.3V output  | `uint8`  | Read Only |

---

## 📝 Notes

- **Data Type:**
  - `float`: 2 registers (4 bytes), IEEE 754 format.
  - `uint8`: 1 register (1 byte, upper or lower byte used).
  - `bool`: 1 bit, stored in 1 register.

- **Access:**
  - `Read Only`: Chỉ đọc, không ghi được qua Modbus RTU.
  - `Read/Write`: Cho phép đọc và ghi qua Modbus RTU.

- **Note:** Tất cả địa chỉ được viết ở định dạng Hex (`0x`), theo chuẩn của Modbus.


---

## 🔁 Example Queries

| Action                 | Function Code | Start Addr | Quantity | Notes                      |
|------------------------|----------------|------------|----------|----------------------------|
| Read Battery Voltage   | `0x03`         | `0x00`     | 1        | Read DalyBMS voltage       |
| Write Relay 12V ON     | `0x06`         | `0x11`     | 1        | Payload: `0x0001`          |
| Read All INA219 Data   | `0x03`         | `0x20`     | 6        | Read all 3 channels (V, I) |

---

