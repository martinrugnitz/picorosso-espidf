#ifndef __ROSOUT_H
#define __ROSOUT_H

#define ROSLOG_DEBUG 10
#define ROSLOG_INFO 20
#define ROSLOG_WARN 30
#define ROSLOG_ERROR 40
#define ROSLOG_FATAL 50

bool rosout_setup(const char *topic_name);

void rosout_out(const char *s,
                const char *file,
                const char *func,
                uint32_t line,
                uint8_t level);

#endif // __ROSOUT_H
