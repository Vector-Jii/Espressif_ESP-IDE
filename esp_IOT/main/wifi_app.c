/*
 * wifi_app.c
 *
 *  Created on: 06-Jan-2025
 *      Author: Tejas Sawant
 */

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"
#include "lwip/netdb.h"

#include "rgb_led.h"
#include "tasks_common.h"
#include "wifi_app.h"


