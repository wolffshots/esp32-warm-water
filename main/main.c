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
#include <stdbool.h>   // booleans
#include "sdkconfig.h" // the generated configuration variables
#include "led.h"       // led module

/*
    ESP_LOGE - error (lowest)
    ESP_LOGW - warning
    ESP_LOGI - info
    ESP_LOGD - debug
    ESP_LOGV - verbose (highest)
*/

#ifdef CONFIG_ESP_ENABLE_WIFI        // check if wifi is enabled in sdkconfig
#ifdef CONFIG_ESP_ENABLE_WIFI_STA    // check if station mode is enabled
#include "wifi_sta.h"                // include station header if so
#endif                               // CONFIG_ESP_ENABLE_WIFI_STA
#ifdef CONFIG_ESP_ENABLE_WIFI_SOFTAP // check if soft ap mode is enabled
#include "wifi_sap.h"                // include soft ap header if so
#endif                               // CONFIG_ESP_ENABLE_WIFI_SOFTAP
#endif                               // CONFIG_ESP_ENABLE_WIFI
#include "utility.h"                 // used for printing chip info
#include "ds18b20_wrapper.h"         // for setting up and interfacing with the temp sensor via owb
#include "timer.h"

int num_sensors = 0;
esp_timer_handle_t periodic_temp_timer;

static const char *TAG = "esp32-warm-water";

static void poll_sensor_handler(void *arg)
{
    float results[sizeof(float) * num_sensors];    // create correctly sized float array for the sensor data
    ds18b20_wrapped_capture(results, num_sensors); // pass float array through to be captured to
    // check status and stuff here
    for (int i = 0; i < num_sensors; ++i)
    {
        ESP_LOGI(TAG, "%d: %.3f\n", i, results[i]);
    }
}
/*
free resources and ensure safe shutdown of app
*/
void app_deinit(void)
{
    ds18b20_wrapped_deinit();
    fflush(stdout);
    // turn relay off
}

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

    num_sensors = ds18b20_wrapped_init(); // capture num sensors and init
    general_timer_init(periodic_temp_timer, poll_sensor_handler, true, 5, "temp_sensor_poll_timer");

    led_on();
}
