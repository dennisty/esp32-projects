// Blinks a single LED using a GPIO pin on an ESP32

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO_NUM GPIO_NUM_4
#define LED_ON_DURATION_MS 1000
#define LED_OFF_DURATION_MS 1000
#define LED_ON 1
#define LED_OFF 0

static const char *LED_LOGGING_TAG = "LED";

void configure_led_gpio()
{
    gpio_reset_pin(LED_GPIO_NUM);
    gpio_config_t io_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << LED_GPIO_NUM),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    gpio_config(&io_config);
}

void app_main(void)
{
    // Reset LED pin and enable output
    configure_led_gpio();

    while (1)
    {
        // Set LED pin to high
        ESP_LOGI(LED_LOGGING_TAG, "LED ON");
        gpio_set_level(LED_GPIO_NUM, LED_ON);
        vTaskDelay(LED_ON_DURATION_MS / portTICK_PERIOD_MS);

        // Set LED pin to low
        ESP_LOGI(LED_LOGGING_TAG, "LED OFF");
        gpio_set_level(LED_GPIO_NUM, LED_OFF);
        vTaskDelay(LED_OFF_DURATION_MS / portTICK_PERIOD_MS);
    }
}
