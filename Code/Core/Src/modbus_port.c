/*
 * modbus_port.c
 *
 *  Created on: Aug 4, 2025
 *      Author: ASUS
 */

#include "modbus_port.h"
#include "modbus.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal_tim.h"

// MODIFICATION LOG
// Date: 2025-01-03 16:35
// Changed by: AI Agent
// Description: Fixed timer configuration for accurate Modbus frame timeout
// Reason: Previous timer settings were not precise for 3.5 character time
// Impact: More reliable frame detection and timeout handling
// Testing: Test with various baudrates and frame lengths

// Timer handle cho frame timeout
extern TIM_HandleTypeDef htim2;

// UART handle cho RS485 - Sử dụng USART2
extern UART_HandleTypeDef huart2;

// Flag để track timer state
static bool timer_running = false;

// Buffer cho UART receive
static uint8_t uart_rx_byte;

void modbus_port_init(void) {
    // Debug: Blink LED1 để báo modbus_port_init bắt đầu
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    
    // KHÔNG gọi lại HAL_UART_Init vì đã được gọi trong main.c
    // HAL_UART_Init(&huart2);
    
    // KHÔNG gọi lại HAL_TIM_Base_Init vì đã được gọi trong main.c  
    // HAL_TIM_Base_Init(&htim2);
    
    // Timer đã được config đúng trong main.c với Period = 4000, Prescaler = 71
    // Clear timer counter
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    
    // Debug: Blink LED2 để báo timer config OK
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    
    // Bật interrupt cho UART RX
    HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    
    // Debug: Blink LED3 để báo UART receive OK
    HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
    
    // Debug: Blink LED4 để báo modbus_port_init hoàn thành
    HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
}

void modbus_port_send(uint8_t *data, uint16_t len) {
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin); // Debug: nháy LED2 khi gửi response
    
    // Stop timer trước khi gửi để tránh timeout trong lúc transmit
    modbus_port_stop_timer();
    
    // Gửi data qua UART với timeout ngắn hơn
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, data, len, 100);
    
    // Restart UART receive sau khi gửi xong
    HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    
    // Debug: Nếu transmit thành công, blink LED3
    if (status == HAL_OK) {
        HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
        // Simple delay without FreeRTOS dependency
        for(volatile int i = 0; i < 10000; i++);
        HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
    }
}

void modbus_port_on_byte_received(uint8_t byte) {
    // Chuyển byte nhận được cho modbus core
    modbus_receive_byte(byte);
}

void modbus_port_on_frame_timeout(void) {
    // Thông báo timeout cho modbus core
    modbus_on_frame_timeout();
}

void modbus_port_start_timer(void) {
    // FIXED: Luôn reset timer counter khi nhận byte mới
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    
    if (!timer_running) {
        // Start timer interrupt nếu chưa chạy
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

// UART RX Complete Callback - STM32 HAL
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    // Debug: Toggle LED2 mỗi khi nhận 1 byte UART
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    
    if (huart->Instance == USART2) {
        // Gửi byte nhận được cho modbus
        modbus_port_on_byte_received(uart_rx_byte);
        // Tiếp tục nhận byte tiếp theo
        HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    }
}

// UART Error Callback
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        // Restart receive on error
        HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
    }
}

// Timer timeout callback - STM32 HAL
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        modbus_port_on_frame_timeout();
        modbus_port_stop_timer();
    }
}


