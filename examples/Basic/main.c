#define ZENOH_ROUTER_ADDRESS "serial/UART_1#baudrate=115200"
#define ZENOH_NODE_NAME "mi_node"
#define ROS_DOMAIN_ID 0

#include "picorosso.h"
#include "ticker.h"

void app_main()
{
    picorosso_setup(ZENOH_NODE_NAME, ZENOH_ROUTER_ADDRESS, ROS_DOMAIN_ID);

    ticker_setup("tick");

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(60000));
    }
}