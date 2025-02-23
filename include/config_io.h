//Created by Bananums: https://github.com/Bananums

#ifndef CONFIG_IO_H
#define CONFIG_IO_H

#include "driver/uart.h"
#include "driver/gpio.h"

enum : uint8_t { //Using enum in order to avoid using defines
    kLedPin = GPIO_NUM_2,
};

static const gpio_config_t ONBOARD_LED_GPIO_CONF = {
    .pin_bit_mask = (1ULL << kLedPin),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
};

static const uart_config_t UART_CONF = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
};

#endif //CONFIG_IO_H
