#ifndef LED_H
#define LED_H

#include "gpio_wrapper.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

    void led_init(void);
    void led_on(int pin);
    void led_off(int pin);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LED_H