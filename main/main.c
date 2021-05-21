/*
 * MIT License
 *
 * Copyright (c) 2021 wolffshots
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * A note on the copyright line: https://www.copyright.gov/title17/92chap4.html#408
 */

/**
 * @file main.c
 * @author wolffshots
 *
 * esp-idf warm-water project. 
 * 
 * employs wifi, onewire and a relay to control the temperature
 * of a body of water for the purpose of film development
 * primarily. other uses include cooking and general chemistry.
 * 
 * the main motivation to do this project was to be able to 
 * better process c41 colour film.
 */

/*
    ESP_LOGE - error (lowest)
    ESP_LOGW - warning
    ESP_LOGI - info
    ESP_LOGD - debug
    ESP_LOGV - verbose (highest)
*/
#include <stdio.h>                   // esp packaged standard io library
#include <stdbool.h>                 // booleans
#include "sdkconfig.h"               // the generated configuration variables
#include "led.h"                     // led module
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
#include "timer.h"                   // wrappers for setting up high resolution timers

int num_sensors = 0;                    ///< the number of sensors ds18b20 init has found (initially 0)
esp_timer_handle_t periodic_temp_timer; ///< variable to control the timer associated with running the temperature polling

static const char *TAG = "esp32-warm-water";

/**
 * polls the ds18b20 sensor for it's latest temperatures
 * @param arg void pointer mainly because that's the fingerprint that the isr expects.
 * a void pointer holds some address to an unknown type. read more at: 
 * @link https://www.geeksforgeeks.org/void-pointer-c-cpp/
 */
static void poll_sensor_handler(void *arg)
{
    float results[sizeof(float) * num_sensors];    ///< create correctly sized float array for the sensor data
    ds18b20_wrapped_capture(results, num_sensors); // pass float array through to be captured to
    // check status and stuff here
    for (int i = 0; i < num_sensors; ++i)           // for each sensor
    {                                               //
        ESP_LOGI(TAG, "%d: %.3f\n", i, results[i]); // log result out
    }                                               // this will probably be phased out once relay control is implemented
}
/**
 * free resources and ensure safe shutdown of app
 */
void app_deinit(void)
{
    ds18b20_wrapped_deinit();
    fflush(stdout);
    // turn relay off
}

/** 
 * main method that is run by default (similar to arduino style setup) 
 */
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
