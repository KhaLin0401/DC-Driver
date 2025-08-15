/*
 * modbus_port.c
 * MODBUS PORT LAYER FOR STM32F103
 * Hardware abstraction layer for UART and Timer
 */

#include "modbus_port.h"
#include "modbus.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal_tim.h"

// External handles
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;

// Internal state
static bool timer_running = false;
static uint8_t uart_rx_byte = 0;

// MODBUS PORT LAYER IMPLEMENTATION

void modbus_port_init(void) {
    // Clear timer counter
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    
    // Start UART receive interrupt
    HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    
    // Debug: Blink LED1 to show initialization
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    for(volatile int i = 0; i < 10000; i++);
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

void modbus_port_send(uint8_t *data, uint16_t len) {
    // Stop timer during transmission
    modbus_port_stop_timer();
    
    // Send data via UART
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, data, len, 50);
    
    // Restart UART receive
    HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    
    // Debug: Blink LED2 on successful transmission
    if (status == HAL_OK) {
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
        for(volatile int i = 0; i < 5000; i++);
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    }
}

void modbus_port_start_timer(void) {
    // Always reset timer counter
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    
    // Start timer if not running
    if (!timer_running) {
        HAL_TIM_Base_Start_IT(&htim2);
        timer_running = true;
    }
}

void modbus_port_stop_timer(void) {
    if (timer_running) {
        HAL_TIM_Base_Stop_IT(&htim2);
        timer_running = false;
    }
}

void modbus_port_on_byte_received(uint8_t byte) {
    // Forward byte to modbus core
    modbus_receive_byte(byte);
}

void modbus_port_on_frame_timeout(void) {
    // Forward timeout to modbus core
    modbus_handle_timeout();
}

// STM32 HAL CALLBACKS

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        // Debug: Blink LED3 for each received byte
        HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
        
        // Process received byte
        modbus_port_on_byte_received(uart_rx_byte);
        
        // Continue receiving
        HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        // Restart receive on error
        HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // Debug: Blink LED4 on timeout
        HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
        
        // Handle frame timeout
        modbus_port_on_frame_timeout();
        
        // Stop timer
        modbus_port_stop_timer();
    }
}


