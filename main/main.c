/* */
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
