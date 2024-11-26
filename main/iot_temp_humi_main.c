
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "mqtt_client.h"

#include "protocol_examples_common.h"
#include "wifi.h"
#include "mqtt.h"
#include "dht11.h"

#define LED_GPIO GPIO_NUM_2  // Definindo o pino do LED interno
#define CONFIG_DHT11_PIN GPIO_NUM_4
//#define CONFIG_CONNECTION_TIMEOUT 5

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    esp_mqtt_client_handle_t client = mqtt_app_start();

    DHT11_init(CONFIG_DHT11_PIN);

    while(1) {
        if(DHT11_read().status == 0){
    
            char temperatura[10];
            sprintf(temperatura, "%d", DHT11_read().temperature);
            char umidade[10];
            sprintf(umidade, "%d", DHT11_read().humidity);
        
            printf("Temperatura:%s \n", temperatura);
            printf("Umidade: %s\n", umidade);

            esp_mqtt_client_publish(client, "mestrado/iot/aluno/eduardo/temperatura", temperatura, 0, 1, 0);
            esp_mqtt_client_publish(client, "mestrado/iot/aluno/eduardo/umidade", umidade, 0, 1, 0);
        }
        vTaskDelay(10000/portTICK_PERIOD_MS);
    }
}
