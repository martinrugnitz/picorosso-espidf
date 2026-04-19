//#include <stdint.h>
#ifndef __ROSOUT_H
#define __ROSOUT_H

#define ROSLOG_DEBUG 10
#define ROSLOG_INFO 20
#define ROSLOG_WARN 30
#define ROSLOG_ERROR 40
#define ROSLOG_FATAL 50

static picoros_publisher_t publisher_log = {
    .topic =
        {
            .name = "rosout",
            .type = ROSTYPE_NAME(ros_Log),
            .rihs_hash = ROSTYPE_HASH(ros_Log),
        },
};

typedef struct {
    picoros_publisher_t publisher_log;
} rosout_t;

#endif // __ROSOUT_H
