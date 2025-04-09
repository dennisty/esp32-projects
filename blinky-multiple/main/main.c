
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define RED_LED_GPIO_NUM GPIO_NUM_12
#define RED_LED_ON_DURATION_MS 1000
#define RED_LED_OFF_DURATION_MS 1000
#define BLUE_LED_GPIO_NUM GPIO_NUM_26
#define BLUE_LED_ON_DURATION_MS 500
#define BLUE_LED_OFF_DURATION_MS 500
#define WHITE_LED_GPIO_NUM GPIO_NUM_25
#define WHITE_LED_ON_DURATION_MS 100
#define WHITE_LED_OFF_DURATION_MS 100

#define LED_ON 1
#define LED_OFF 0

// Logging tag constants
static const char LED_GPIO_CONFIG_TAG[] = "LED_GPIO_CONFIG";
static const char RED_LED_TAG[] = "RED_LED";
static const char BLINK_RED_LED_TASK_TAG[] = "BLINK_RED_LED_TASK";
static const char BLUE_LED_TAG[] = "BLUE_LED";
static const char BLINK_BLUE_LED_TASK_TAG[] = "BLINK_BLUE_LED_TASK";
static const char WHITE_LED_TAG[] = "WHITE_LED";
static const char BLINK_WHITE_LED_TASK_TAG[] = "BLINK_WHITE_LED_TASK";

// Parameters to pass into LED blink tasks
typedef struct
{
    gpio_num_t gpio_num;
    uint16_t led_on_duration_ms;
    uint16_t led_off_duration_ms;
    const char *blink_logging_tag;
    const char *task_logging_tag;
} led_task_params_t;

static esp_err_t config_led_gpios(void)
// Configures LED GPIO ports for output
{
    gpio_config_t io_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (((1ULL << RED_LED_GPIO_NUM) | (1ULL << BLUE_LED_GPIO_NUM)) | (1ULL << WHITE_LED_GPIO_NUM)),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    esp_err_t res = gpio_config(&io_config);

    if (res != ESP_OK)
    {
        ESP_LOGE(LED_GPIO_CONFIG_TAG, "Failed to configure LED GPIO with error: %d", res);
    }
    else
    {
        ESP_LOGI(LED_GPIO_CONFIG_TAG, "Successfully configured LED GPIO!");
    }

    return res;
}

void blink_led_task(void *pvParameters)
{
    led_task_params_t *params = (led_task_params_t *)pvParameters;

    ESP_LOGI(params->task_logging_tag, "Starting task...");

    while (1)
    {
        // Set LED pin to high
        ESP_LOGI(params->blink_logging_tag, "LED ON");
        gpio_set_level(params->gpio_num, LED_ON);
        vTaskDelay(pdMS_TO_TICKS(params->led_on_duration_ms));

        // Set LED pin to low
        ESP_LOGI(params->blink_logging_tag, "LED OFF");
        gpio_set_level(params->gpio_num, LED_OFF);
        vTaskDelay(pdMS_TO_TICKS(params->led_off_duration_ms));
    }
}

void app_main(void)
{
    // TODO: Add error handling to config_led_gpios and task create
    config_led_gpios();

    static led_task_params_t red_led_params = {
        .gpio_num = RED_LED_GPIO_NUM,
        .led_on_duration_ms = RED_LED_ON_DURATION_MS,
        .led_off_duration_ms = RED_LED_OFF_DURATION_MS,
        .blink_logging_tag = RED_LED_TAG,
        .task_logging_tag = BLINK_RED_LED_TASK_TAG,
    };

    static led_task_params_t blue_led_params = {
        .gpio_num = BLUE_LED_GPIO_NUM,
        .led_on_duration_ms = BLUE_LED_ON_DURATION_MS,
        .led_off_duration_ms = BLUE_LED_OFF_DURATION_MS,
        .blink_logging_tag = BLUE_LED_TAG,
        .task_logging_tag = BLINK_BLUE_LED_TASK_TAG,
    };

    static led_task_params_t white_led_params = {
        .gpio_num = WHITE_LED_GPIO_NUM,
        .led_on_duration_ms = WHITE_LED_ON_DURATION_MS,
        .led_off_duration_ms = WHITE_LED_OFF_DURATION_MS,
        .blink_logging_tag = WHITE_LED_TAG,
        .task_logging_tag = BLINK_WHITE_LED_TASK_TAG,
    };

    // TODO: Learn about task priority
    xTaskCreate(blink_led_task, "blink_red_led_task", 2048, &red_led_params, 5, NULL);
    xTaskCreate(blink_led_task, "blink_blue_led_task", 2048, &blue_led_params, 5, NULL);
    xTaskCreate(blink_led_task, "blink_white_led_task", 2048, &white_led_params, 5, NULL);
}