#include "picorosso.h"
#include <esp_log.h>
#include <esp_timer.h>
#include <rom/rtc.h>

static const char *TAG = "picorosso";
static const char *zenoh_mode = "client";

picoros_node_t picorosso_node;
uint8_t picorosso_publisher_buf[PUBLISHER_BUF_SIZE];
SemaphoreHandle_t picorosso_bufSemaphore = NULL;

#if defined(USE_SYNC_TIME)
#include "sync_time.h"
#endif

#include "rosout.h"
rosout_t picorosso_rosout;

void picorosso_set_timestamp(ros_Time *stamp)
{
    z_clock_t now;
    clock_gettime(CLOCK_REALTIME, &now);
    stamp->sec = now.tv_sec;
    stamp->nanosec = now.tv_nsec;
}

void picorosso_set_timestamp_now(ros_Time *stamp, z_clock_t *now)
{
    stamp->sec = now->tv_sec;
    stamp->nanosec = now->tv_nsec;
}

static const char *reset_reason_string(const RESET_REASON reason)
{
    switch (reason)
    {
    case 1:
        return "POWERON_RESET"; /**<1, Vbat power on reset*/
    case 3:
        return "SW_RESET"; /**<3, Software reset digital core*/
    case 4:
        return "OWDT_RESET"; /**<4, Legacy watch dog reset digital core*/
    case 5:
        return "DEEPSLEEP_RESET"; /**<5, Deep Sleep reset digital core*/
    case 6:
        return "SDIO_RESET"; /**<6, Reset by SLC module, reset digital core*/
    case 7:
        return "TG0WDT_SYS_RESET"; /**<7, Timer Group0 Watch dog reset digital core*/
    case 8:
        return "TG1WDT_SYS_RESET"; /**<8, Timer Group1 Watch dog reset digital core*/
    case 9:
        return "RTCWDT_SYS_RESET"; /**<9, RTC Watch dog Reset digital core*/
    case 10:
        return "INTRUSION_RESET"; /**<10, Instrusion tested to reset CPU*/
    case 11:
        return "TGWDT_CPU_RESET"; /**<11, Time Group reset CPU*/
    case 12:
        return "SW_CPU_RESET"; /**<12, Software reset CPU*/
    case 13:
        return "RTCWDT_CPU_RESET"; /**<13, RTC Watch dog Reset CPU*/
    case 14:
        return "EXT_CPU_RESET"; /**<14, for APP CPU, reseted by PRO CPU*/
    case 15:
        return "RTCWDT_BROWN_OUT_RESET"; /**<15, Reset when the vdd voltage is not stable*/
    case 16:
        return "RTCWDT_RTC_RESET"; /**<16, RTC Watch dog reset digital core and rtc module*/
    default:
        return "";
    }
}

bool picorosso_setup(const char *node_name,
                      const char *zenoh_router_address,
                      const uint32_t domain_id)
{
    RESET_REASON reset_reason_0 = rtc_get_reset_reason(0);
    RESET_REASON reset_reason_1 = rtc_get_reset_reason(1);

    picorosso_bufSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(picorosso_bufSemaphore);

    picorosso_node = (picoros_node_t){
        .name = (char *)node_name,
        .domain_id = domain_id,
    };

    // Initialize Pico ROS interface
    picoros_interface_t ifx = {
        .mode = (char *)zenoh_mode,
        .locator = (char *)zenoh_router_address,
    };

    ESP_LOGI(TAG, "Starting pico-ros interface [%s] on [%s]", ifx.mode, ifx.locator);
    while (picoros_interface_init(&ifx) == PICOROS_NOT_READY)
    {
        z_sleep_ms(100);
    }
    ESP_LOGD(TAG, "RMW initialized.");

    ESP_LOGI(TAG, "Starting pico-ros node [%s] domain [%lu]\r", picorosso_node.name, picorosso_node.domain_id);
    picoros_node_init(&picorosso_node);

#if defined(USE_SYNC_TIME)
    // Time synchronization
    sync_time_setup(NULL, NULL, NULL);
    if (!sync_time_synchronize_clock())
    {
        ESP_LOGE(TAG, "synch time failed!");
    }
#endif

    // Initialize auxiliary modules
    rosout_setup(&picorosso_rosout, "rosout");

    char reset_reason_str[50] = {0};
    sprintf(reset_reason_str, "Coming from Reset Core0: %d %s", reset_reason_0, reset_reason_string(reset_reason_0));
    rosout_out(&picorosso_rosout, reset_reason_str, __FILE__, __func__, __LINE__, ROSLOG_INFO);
    sprintf(reset_reason_str, "Coming from Reset Core1: %d %s", reset_reason_1, reset_reason_string(reset_reason_1));
    rosout_out(&picorosso_rosout, reset_reason_str, __FILE__, __func__, __LINE__, ROSLOG_INFO);

    return true;
}
