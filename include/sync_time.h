#ifndef __SYNC_TIME_H
#define __SYNC_TIME_H

//    CTYPE(ros_TimeReference,
//        FIELD(ros_Header, header)
//        FIELD(ros_Time, time_ref)
//        FIELD(rstring, source)
//    )
//    CTYPE(ros_Header,
//        FIELD(ros_Time, stamp)
//        FIELD(rstring, frame_id)
//    )

#define WAIT_FOR_SERVER_US 20 * 1000 * 1000 // Time for watiing until first response (in case we connect on a fresh router)
#define MESSAGE_TIMEOUT_US 200 * 1000       // time to wait for a response
#define MESSAGE_COUNT 20                    // measures averaged during estimate_offset

typedef enum
{
  SYNC_TIME_UNSYNCED,
  SYNC_TIME_SYNCING,
  SYNC_TIME_SYNCED
} sync_time_state_t;

extern sync_time_state_t sync_time_state;

bool sync_time_setup(const char *topic_request,
                     const char *topic_response,
                     const char *source);
bool sync_time_synchronize_clock(void);
void sync_time_trigger(void);

#endif // __SYNC_TIME_H
