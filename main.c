#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_netif.h>
#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_http_client.h"
#include "wifi_connect.h"
#include "post_img.h"
#include "driver/gpio.h"


static const char *TAG = "camera_stream";

const char* url = "http://192.168.100.100:5000/upload";


void app_main() {
    gpio_set_direction(4, GPIO_MODE_OUTPUT);
    gpio_set_level(4, 1);
    camera_fb_t *fb = NULL;
    // init wifi connection
    wifi_connection();
    //init the camera
    esp_err_t err = esp_camera_init(&camera_config);    
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed with error 0x%x", err);
        return;
    }
    printf("Camera Init succesfully\n");
    while (true)
    {
        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Camera capture failed");
        } else {
            send_pixel_data(fb ,url);
        }
        esp_camera_fb_return(fb);
        fb = NULL;
        vTaskDelay(750/portTICK_PERIOD_MS);
    }
}
