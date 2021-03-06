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
 * @brief main entrypoint for the warm-water project
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

#include <stdio.h>     // esp packaged standard io library
#include <stdbool.h>   // booleans
#include "sdkconfig.h" // the generated configuration variables
#include "led.h"       // led module
char ip_address[16];
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
#include "ssd1306.h"
#include "symbols.h"
#include "esp_log.h"
#include "control.h"
#include "spiffs.h"
volatile float goal = CONFIG_INITIAL_GOAL_TEMP; ///< goal temp for the system to aim for
volatile float temp = 0.0f;                     ///< current temp
volatile float under = 0.5f;                    ///< margin below goal temp at which to turn relay on
volatile float over = 0.5f;                     ///< margin above goal temp at which to turn relay off
volatile bool heating = false;                  ///< whether or not the system should be outputing a signal to enable the relay (note: could be used to control pump as well)
#include "webserver.h"                          // webserver import must come after volatiles because it interacts with them

int num_sensors = 0;                     ///< the number of sensors ds18b20 init has found (initially 0)
esp_timer_handle_t periodic_check_timer; ///< variable to control the timer associated with running the temperature polling to check system

SSD1306_t dev; ///< device for oled
// char lineChar[20];

static const char *TAG = "esp32-warm-water";

void update_display(void)
{
    char line[20];
    char position = 0;
    sprintf(line, " goal: %0.1f", goal);
    position = strlen(line);
    sprintf(line, " goal: %0.1f C", goal);
    ssd1306_wrapped_display_text(&dev, 2, line);
    ssd1306_display_image(&dev, 2, position * 8, degree_symbol, 8);
    sprintf(line, "  %1.1f      %1.1f  ", under, over);
    ssd1306_wrapped_display_text(&dev, 3, line);
    ssd1306_display_image(&dev, 3, 6 * 8, down_arrow, 8);
    ssd1306_display_image(&dev, 3, 9 * 8, up_arrow, 8);
    sprintf(line, " temp: %0.3f C", temp);
    ssd1306_wrapped_display_text(&dev, 5, line);
    sprintf(line, " temp: %0.3f", goal);
    position = strlen(line);
    ssd1306_display_image(&dev, 5, position * 8, degree_symbol, 8);
    sprintf(line, " %s", ip_address);
    ssd1306_wrapped_display_text(&dev, 6, line);
    ssd1306_wrapped_display_text(&dev, 7, " mode:");
    position = strlen(" mode: ");
    ssd1306_display_image(&dev, 7, position * 8, heating ? fire_symbol : snowflake_symbol, 8);
}

/** turn the relay on */
void relay_on(void)
{
    gpio_set_level(CONFIG_RELAY_ONE_PIN, 0);
    gpio_set_level(CONFIG_RELAY_TWO_PIN, 0);
    ESP_LOGI(TAG, "relay on: setting to %d", 0);
}
/** turn the relay off */
void relay_off(void)
{
    gpio_set_level(CONFIG_RELAY_ONE_PIN, 1);
    gpio_set_level(CONFIG_RELAY_TWO_PIN, 1);
    ESP_LOGI(TAG, "relay off: setting to %d", 1);
}

/**
 * polls the ds18b20 sensor for it's latest temperatures and 
 * checks what range we are in with respect to the goal and
 * sets the output to the relay accordingly
 * @param arg void pointer mainly because that's the fingerprint that the isr expects.
 * a void pointer holds some address to an unknown type. read more at: 
 * @link https://www.geeksforgeeks.org/void-pointer-c-cpp/
 */
void check_system_handler(void *arg)
{
    float results[sizeof(float) * num_sensors];                                   ///< create correctly sized float array for the sensor data
    ds18b20_wrapped_capture(results, num_sensors);                                // pass float array through to be captured to
    check_relay(results[0]);                                                      ///< average temp across sensors (for now =results[0] until we have averaging is implemented)
    for (int i = 0; i < num_sensors; ++i)                                         // for each sensor
    {                                                                             //
        ESP_LOGI(TAG, "%d: %.3f - %lld us", i, results[i], esp_timer_get_time()); // log result out
    }                                                                             // this will probably be phased out once relay control is implemented
    temp = results[0];                                                            // this should be the average
    update_display();
}
/**
 * check if a change in the relay's operation is necessary and perform it if so
 */
void check_relay(float average_temp)
{
    /// @todo average results/check difference between sensors if multiple here

    if ((average_temp > goal + over) && heating) // check status and stuff here
    {
        // turn off relay
        heating = false;
        relay_off();
        led_off(CONFIG_STATUS_ONE_PIN);
    }
    else if ((average_temp < goal - under) && !heating)
    {
        // turn on relay
        heating = true;
        relay_on();
        led_on(CONFIG_STATUS_ONE_PIN);
    }
    else
    {
        // continue what it's currently doing
        // heating = heating
    }
}

/**
 * free resources and ensure safe shutdown of app
 */
void app_deinit(void)
{
    general_timer_deinit(periodic_check_timer); ///< cleanup timer associated with checking temp and relay
    relay_deinit();                             ///< relay off and deinit gpio
    ds18b20_wrapped_deinit();                   ///< deallocate owb and ds18b20
    ssd1306_deinit(&dev);                       ///< deallocate i2c and ssd1306
    wifi_shared_deinit();                       ///< wifi off and deinit
    spiffs_deinit();                            ///< disable spiffs
    fflush(stdout);
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
    print_chip_info();                                                        // print the chip features and details

    relay_off();
    gpio_output(CONFIG_RELAY_ONE_PIN);
    gpio_output(CONFIG_RELAY_TWO_PIN);
    led_init();

    num_sensors = ds18b20_wrapped_init(); // capture num sensors and init

    general_timer_init(periodic_check_timer, check_system_handler, true, CONFIG_CONTROL_INTERVAL, "check_system_handler");

    spiffs_init();

    ssd1306_init(&dev);

    ssd1306_clear_screen(&dev, false);
    ssd1306_wrapped_display_text(&dev, 0, " warm water :)");

    ESP_ERROR_CHECK(start_file_server("/spiffs"));
}
