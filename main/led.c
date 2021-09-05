#include "led.h"

/** set up the led as an output */
void led_init(void)
{
    gpio_output(CONFIG_STATUS_ONE_PIN);
    gpio_output(CONFIG_STATUS_TWO_PIN);
}
/** turn the led on */
void led_on(int pin)
{
    gpio_set_level(pin, 1);
}
/** turn the led off */
void led_off(int pin)
{
    gpio_set_level(pin, 0);
}