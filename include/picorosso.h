#ifndef __PICOROSSO_H
#define __PICOROSSO_H

#include "picorosso_config.h"

#include "picoros.h"
#include "picoserdes.h"

#include "rosout.h"

extern picoros_node_t picorosso_node;
extern rosout_t picorosso_rosout;

extern uint8_t picorosso_publisher_buf[PUBLISHER_BUF_SIZE];
extern SemaphoreHandle_t picorosso_bufSemaphore;

/*
Publishes using a provided buffer for serialization. Buffer
should not shared between threads.
*/
#define pr_publish_buf(publisher, msg, buf, buf_size)        \
  ({                                                         \
    size_t len_ = ps_serialize(buf, &msg, buf_size);         \
    if (len_ > 0)                                            \
    {                                                        \
      picoros_publish(&publisher, buf, len_);                \
    }                                                        \
    else                                                     \
    {                                                        \
      ESP_LOGE("picorosso", "Message serialization error."); \
    }                                                        \
  })

/*
Publishes using an internal buffer of size PUBLISHER_BUF_SIZE
for serialization. Thread safe. Good for easy, sporadic publishing.
*/
#define pr_publish(publisher, msg)                                      \
  ({                                                                    \
    xSemaphoreTake(picorosso_bufSemaphore, portMAX_DELAY);             \
    size_t len_ = ps_serialize(picorosso_publisher_buf,                \
                               &msg, sizeof(picorosso_publisher_buf)); \
    if (len_ > 0)                                                       \
    {                                                                   \
      picoros_publish(&publisher, picorosso_publisher_buf, len_);      \
    }                                                                   \
    else                                                                \
    {                                                                   \
      ESP_LOGE("picorosso", "Message serialization error.");            \
    }                                                                   \
    xSemaphoreGive(picorosso_bufSemaphore);                            \
  })

bool picorosso_setup(const char *node_name,
                     const char *zenoh_router_address,
                     uint32_t domain_id);
void picorosso_set_timestamp(ros_Time *stamp);
void picorosso_set_timestamp_now(ros_Time *stamp, z_clock_t *now);

#endif // __PICOROSSO_H
