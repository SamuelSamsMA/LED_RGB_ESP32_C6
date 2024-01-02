#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "led_strip.h"


led_strip_handle_t configure_led(void);


void app_main(void)
{
    // gpio_set_direction(GPIO_NUM_3, GPIO_MODE_OUTPUT);
    
    led_strip_handle_t led_rgb = configure_led();
    uint32_t counter = 1;

    while (1)
    {
        uint32_t color_red, color_green, color_blue;

        (counter & 1) ? (color_blue = 28) : (color_blue = 0);
        (counter & 2) ? (color_green = 28) : (color_green = 0);
        (counter & 4) ? (color_red = 28) : (color_red = 0);

        led_strip_set_pixel(led_rgb, 0, color_red, color_green, color_blue);
        led_strip_refresh(led_rgb);

        (counter == 7) ? (counter = 1) : (counter++);

        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // gpio_set_level(GPIO_NUM_3, 1);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // gpio_set_level(GPIO_NUM_3, 0);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    
}


led_strip_handle_t configure_led(void)
{
    led_strip_config_t led_config = {
        .strip_gpio_num = 8,
        .max_leds = 1,                          // Led único de la placa
        .led_pixel_format = LED_PIXEL_FORMAT_GRB,
        .led_model = LED_MODEL_WS2812
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,      // 10 MHz
        .flags.with_dma = false
    };

    led_strip_handle_t led_handle;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&led_config, &rmt_config, &led_handle));
    return led_handle;
}
