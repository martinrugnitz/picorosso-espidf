#include <esp_log.h>

#include "picorosso.h"
#include "rosout.h"
#include "picoros.h"
#include "picoserdes.h"

static const char *TAG = "picorosso";

void rosout_init(rosout_t *r, const char *topic_name)
{
    r->publisher_log.topic.name = topic_name;
    r->publisher_log.topic.type = ROSTYPE_NAME(ros_Log);
    r->publisher_log.topic.rihs_hash = ROSTYPE_HASH(ros_Log);
}

bool rosout_setup(rosout_t *r, const char *topic_name)
{
    ESP_LOGD(TAG, "Setting up...");

    r->publisher_log.topic.name = topic_name;

    picoros_publisher_declare(&picorosso_node, &r->publisher_log);

    ESP_LOGD(TAG, "Setting up done.");

    z_sleep_ms(100);

    rosout_out(r, "New logger active.", __FILE__, __func__, __LINE__, ROSLOG_INFO);

    return true;
}

void rosout_out(rosout_t *r,
                const char *s,
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

    set_timestamp(msg_log.stamp);

    pr_publish(r->publisher_log, msg_log);
}
