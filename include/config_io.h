//Created by Bananums: https://github.com/Bananums

#ifndef CONFIG_IO_H
#define CONFIG_IO_H

#include "driver/uart.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2

static const gpio_config_t ONBOARD_LED_GPIO_CONF = {
    .pin_bit_mask = (1ULL << LED_PIN), // Specify the LED pin
    .mode = GPIO_MODE_OUTPUT,         // Set as output
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE    // Disable interrupts
};




#endif //CONFIG_IO_H
