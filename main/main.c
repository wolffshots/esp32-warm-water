/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef CONFIG_ESP_ENABLE_WIFI
#ifdef CONFIG_ESP_ENABLE_WIFI_STA
#include "wifi_sta.h"
#endif
#ifdef CONFIG_ESP_ENABLE_WIFI_SOFTAP
#include "wifi_sap.h"
#endif
#endif

#include "utility.h"

void app_main(void)
{
#if defined(CONFIG_ESP_ENABLE_WIFI) && defined(CONFIG_ESP_ENABLE_WIFI_STA)
    wifi_init_sta();
#endif
#if defined(CONFIG_ESP_ENABLE_WIFI) && defined(CONFIG_ESP_ENABLE_WIFI_SOFTAP)
    wifi_init_sap();
#endif
    print_chip_info();

    fflush(stdout);
}
