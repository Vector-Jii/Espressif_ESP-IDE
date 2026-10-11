#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_http_server.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "lwip/inet.h"  // For IP address conversion
#include "lwip/sockets.h"

#define SERVO_GPIO_PIN 18
#define LEDC_TIMER      LEDC_TIMER_0
#define LEDC_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL    LEDC_CHANNEL_0
#define LEDC_DUTY_RES   LEDC_TIMER_13_BIT
#define LEDC_FREQUENCY  50  // Frequency in Hertz (50Hz for servo motor)

#define EXAMPLE_ESP_WIFI_SSID "servo_access"
#define EXAMPLE_ESP_WIFI_PASS "12345678"
#define EXAMPLE_ESP_WIFI_CHANNEL 1
#define EXAMPLE_MAX_STA_CONN 4

#define STATIC_IP_ADDRESS "192.168.0.1"
#define NETMASK_ADDRESS "255.255.255.0"
#define GATEWAY_ADDRESS "192.168.0.1"

// Function to set servo angle
void set_servo_angle(int angle) {
    int duty = (angle * (8192 - 4096) / 180) + 4096;  // Map 0-180 to the duty cycle
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

// HTTP GET handler to serve the webpage
esp_err_t get_handler(httpd_req_t *req) {
    const char *response =
        "<!DOCTYPE html>"
        "<html>"
        "<body>"
        "<h1>ESP32 Servo Control</h1>"
        "<input type=\"range\" min=\"0\" max=\"180\" value=\"90\" id=\"servoSlider\" oninput=\"updateServo(this.value)\">"
        "<p>Angle: <span id=\"angleValue\">90</span></p>"
        "<script>"
        "function updateServo(val) {"
        "  document.getElementById('angleValue').innerHTML = val;"
        "  var xhr = new XMLHttpRequest();"
        "  xhr.open('GET', '/servo?angle=' + val, true);"
        "  xhr.send();"
        "}"
        "</script>"
        "</body>"
        "</html>";
    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}

// HTTP GET handler to control the servo
esp_err_t servo_handler(httpd_req_t *req) {
    char param[32];
    if (httpd_req_get_url_query_str(req, param, sizeof(param)) == ESP_OK) {
        char angle_str[8];
        if (httpd_query_key_value(param, "angle", angle_str, sizeof(angle_str)) == ESP_OK) {
            int angle = atoi(angle_str);
            set_servo_angle(angle);
        }
    }
    httpd_resp_send(req, "Servo angle updated", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Function to start the web server
httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t uri_get = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &uri_get);

        httpd_uri_t uri_servo = {
            .uri       = "/servo",
            .method    = HTTP_GET,
            .handler   = servo_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(server, &uri_servo);
    }
    return server;
}

// Initialize Wi-Fi as an access point
void wifi_init_softap(void)
{
    esp_netif_init();
    esp_event_loop_create_default();
    
    esp_netif_create_default_wifi_ap(); // Create default AP interface

    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF"); // Get netif handle

    esp_netif_ip_info_t ip_info;
    inet_pton(AF_INET, STATIC_IP_ADDRESS, &ip_info.ip.addr);
    inet_pton(AF_INET, NETMASK_ADDRESS, &ip_info.netmask.addr);
    inet_pton(AF_INET, GATEWAY_ADDRESS, &ip_info.gw.addr);
    esp_netif_set_ip_info(netif, &ip_info);

    wifi_init_config_t wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_cfg);

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();

    ESP_LOGI("wifi_init_softap", "Wi-Fi AP started with SSID: %s", EXAMPLE_ESP_WIFI_SSID);
}

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize Wi-Fi in AP mode
    wifi_init_softap();

    // Setup PWM for servo motor
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel    = LEDC_CHANNEL,
        .duty       = 4096,  // 0-degree position
        .gpio_num   = SERVO_GPIO_PIN,
        .speed_mode = LEDC_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER
    };
    ledc_channel_config(&ledc_channel);

    // Start the HTTP server
    start_webserver();
}
