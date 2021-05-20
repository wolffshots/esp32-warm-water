#include "led.h"

/** set up the led as an output */
void led_init(void)
{
    gpio_output(CONFIG_ESP_BUILTIN_LED);
}
/** turn the led on (gpio low) */
void led_on(void)
{
    gpio_set_level(CONFIG_ESP_BUILTIN_LED, 0);
}
/** turn the led off (gpio high) */
void led_off(void)
{
    gpio_set_level(CONFIG_ESP_BUILTIN_LED, 1);
}