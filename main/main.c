/** 
 * esp-idf warm-water project
 * 
 * employs wifi, onewire and a relay to control the temperature
 * of a body of water for the purpose of film development
 * primarily. other uses include cooking and general chemistry.
 * 
 * the main motivation to do this project was to be able to 
 * better process c41 colour film.
 */

#include <stdio.h>     // esp packaged standard io library
#include "sdkconfig.h" // the generated configuration variables
#include "led.h"       // led module

#ifdef CONFIG_ESP_ENABLE_WIFI        // check if wifi is enabled in sdkconfig
#ifdef CONFIG_ESP_ENABLE_WIFI_STA    // check if station mode is enabled
#include "wifi_sta.h"                // include station header if so
#endif                               // CONFIG_ESP_ENABLE_WIFI_STA
#ifdef CONFIG_ESP_ENABLE_WIFI_SOFTAP // check if soft ap mode is enabled
#include "wifi_sap.h"                // include soft ap header if so
#endif                               // CONFIG_ESP_ENABLE_WIFI_SOFTAP
#endif                               // CONFIG_ESP_ENABLE_WIFI
#include "utility.h"                 // used for printing chip info

/** main method that is run by default (similar to arduino style setup) */
void app_main(void)
{
#if defined(CONFIG_ESP_ENABLE_WIFI) && defined(CONFIG_ESP_ENABLE_WIFI_STA)    // check if station mode
    wifi_init_sta();                                                          // start wifi in station mode and connect to ap
#endif                                                                        // CONFIG_ESP_ENABLE_WIFI && CONFIG_ESP_ENABLE_WIFI_STA
#if defined(CONFIG_ESP_ENABLE_WIFI) && defined(CONFIG_ESP_ENABLE_WIFI_SOFTAP) // check if softap mode
    wifi_init_sap();                                                          // start wifi in soft ap mode and start broadcasting ap
#endif                                                                        // CONFIG_ESP_ENABLE_WIFI && CONFIG_ESP_ENABLE_WIFI_SOFTAP
    led_init();                                                               // setup led gpio pin
    print_chip_info();                                                        // print the chip features and details
    led_on();
    printf("led on\n");
    util_delay_seconds(1);
    led_off();
    printf("led off\n");
    util_delay_seconds(1);
    led_on();
    printf("led on\n");
    util_delay_seconds(1);

    fflush(stdout); // flush stdout stream
}
