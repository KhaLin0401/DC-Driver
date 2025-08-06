#ifndef __MOTORDC_H
#define __MOTORDC_H

#include "stdint.h"




typedef struct {
	float _kp;
	float _ki;
	float _kd;
	float _accelerationLimit;       /**< Gioi han tang toc (% cua _maxDuty) */
	float _decelerationLimit;/**< He so D cua PID */
	float _targetSpee1;             /**< Toc do muc tieu (0-100%) */
	float _currentSpeed;            /**< Toc do hien tai (0-100%) */

	/* Bien noi bo cho PID */
	float _error;                   /**< Sai so hien tai */
	float _lastError;               /**< Sai so vong truoc */
	float _integral;                /**< Gia tri tich phan */
	float _output;                  /**< Gia tri dau ra dieu khien (0-100%) */

	/* Cam bien */
	float _distanceSensorValue;     /**< Gia tri khoang cach (cm), duoc cap nhat tu ben ngoai */

	/* Khoang cach an toan */
	float _safeDistance;            /**< Khoang cach an toan (cm) */

	/* Thong so PWM */
	float _maxDuty;                 /**< Gia tri PWM toi da */

	/* Trang thai he thong */
	int _direction;
	/**< Huong quay */
	int _status;
	/**< Trang thai dong co */



	void (*Update)(struct _MotorDC *motor);
} MotorControl_t;

typedef struct {
	MotorControl_t motor1;
	MotorControl_t motor2;
	uint8_t system_status;    // 0 = Idle, 1 = Running, 2 = Fault
	uint8_t error_code;       // Mã lỗi nếu có
} DriverSystem_t;

typedef struct {
    // Motor 1
    uint16_t m1_target_speed;
    uint16_t m1_actual_speed;
    uint16_t m1_direction;
    uint16_t m1_pid_kp;
    uint16_t m1_pid_ki;
    uint16_t m1_pid_kd;

    // Motor 2
    uint16_t m2_target_speed;
    uint16_t m2_actual_speed;
    uint16_t m2_direction;
    uint16_t m2_pid_kp;
    uint16_t m2_pid_ki;
    uint16_t m2_pid_kd;

    // System
    uint16_t system_status;
    uint16_t error_code;
} ModbusRegisterMap_t;

extern DriverSystem_t driver;
extern ModbusRegisterMap_t modbus_map;

void _motorInit();
void _setRuningMode(ModbusRegisterMap_t *modbus_map, DriverSystem_t *driver);
void _setEnableMotor(DriverSystem_t *driver);
void _setDisableMotor(DriverSystem_t *driver);
void _setOnOffMode(ModbusRegisterMap_t *modbus_map, DriverSystem_t *driver);
void _setLinearMode(ModbusRegisterMap_t *modbus_map, DriverSystem_t *driver);
void _setPIDMode(ModbusRegisterMap_t *modbus_map, DriverSystem_t *driver);

void _runOnOffMode(DriverSystem_t *driver);
void _runLinearMode(DriverSystem_t *driver);
void _runPIDMode(DriverSystem_t *driver);

void _updateMotor(DriverSystem_t *driver);
#endif
