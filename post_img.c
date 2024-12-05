#include <esp_log.h>
#include "esp_http_server.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_http_client.h"
#include "esp_camera.h"
#include "post_img.h"

static const char *TAG = "camera_stream";

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGW(TAG, "HTTP_EVENT_ERROR\n");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGW(TAG, "HTTP_EVENT_ON_CONNECTED\n");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGW(TAG, "HTTP_EVENT_HEADER_SENT\n");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGW(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s \n", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGW(TAG, "HTTP_EVENT_ON_DATA, len=%d \n", evt->data_len);
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGW(TAG, "HTTP_EVENT_ON_FINISH\n");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "HTTP_EVENT_DISCONNECTED\n");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGW(TAG, "HTTP_EVENT_REDIRECT\n");
            break;
    }
    return ESP_OK;
}

void send_pixel_data(camera_fb_t *fb ,const char* url) {
    esp_http_client_config_t config_http = {
        .url = url,
        .method = HTTP_METHOD_POST,
        .cert_pem = NULL,
        .event_handler = _http_event_handler
    };
    
    esp_http_client_handle_t client = esp_http_client_init(&config_http);
    esp_http_client_set_header(client, "Content-Type", "application/octet-stream");
    esp_http_client_set_post_field(client, (const char*)fb->buf, fb->len);
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %lld", esp_http_client_get_status_code(client), esp_http_client_get_content_length(client));
    } else {
        ESP_LOGW(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
}
