#include <esp_log.h>

#include "picorosso.h"
#include "rosout.h"
#include "picoros.h"
#include "picoserdes.h"

static const char *TAG = "rosout";

static picoros_publisher_t publisher_log = {
    .topic =
        {
            .name = "rosout",
            .type = ROSTYPE_NAME(ros_Log),
            .rihs_hash = ROSTYPE_HASH(ros_Log),
        },
};

typedef struct
{
    picoros_publisher_t publisher_log;
} rosout_t;

bool rosout_setup(const char *topic_name)
{
    ESP_LOGD(TAG, "Setting up...");

    if (topic_name != NULL)
    {
        publisher_log.topic.name = topic_name;
    }

    ESP_LOGI(TAG, "Declaring publisher on [%s]\r", publisher_log.topic.name);
    picoros_res_t ret = picoros_publisher_declare(&picorosso_node, &publisher_log);

    z_sleep_ms(100);

    rosout_out("New logger active.", __FILE__, __func__, __LINE__, ROSLOG_INFO);

    ESP_LOGD(TAG, "Setting up done.");
    return true;
}

void rosout_out(const char *s,
                const char *file,
                const char *func,
                uint32_t line,
                uint8_t level)
{
    ros_Log msg_log = {
        .level = level,
        .name = (char *)picorosso_node.name,
        .msg = (char *)s,
        .file = (char *)file,
        .function = (char *)func,
        .line = line,
    };

    picorosso_set_timestamp(&msg_log.stamp);

    pr_publish(publisher_log, msg_log);
}
