#include "Encoder.h"

// Global variables to store the last count and time
static uint32_t last_count = 0;
static uint32_t last_time = 0;

void Encoder_Init(TIM_HandleTypeDef *htim) {
    // Start the encoder interface in encoder mode
    HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
}

uint32_t Encoder_GetCounter(TIM_HandleTypeDef *htim) {
    // Get the current encoder counter value
    return __HAL_TIM_GET_COUNTER(htim);
}

uint32_t Encoder_GetRPM(TIM_HandleTypeDef *htim) {
    // Get the current encoder counter value
    uint32_t current_count = Encoder_GetCounter(htim);

    // Get the current time in milliseconds
    uint32_t current_time = HAL_GetTick();

    // Calculate the time difference in milliseconds
    uint32_t time_diff = current_time - last_time;

    // Ensure time_diff is non-zero to avoid division by zero
    if (time_diff == 0) {
        return 0;
    }

    // Calculate the pulse difference
    int32_t pulse_diff = current_count - last_count;

    // Handle overflow (assuming a 16-bit timer)
    if (pulse_diff < 0) {
        pulse_diff += 0xFFFF;
    }

    // Calculate the RPM: (pulse_diff * 60000) / (PPR * time_diff)
    uint32_t rpm = (pulse_diff ) * 60000 / (PPR * time_diff);

    // Update the last count and time for the next calculation
    last_count = current_count;
    last_time = current_time;

    return rpm;
}
