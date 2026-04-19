#define ZENOH_ROUTER_ADDRESS "serial/UART_1#baudrate=115200"
#define ZENOH_NODE_NAME "mi_node"

#include "picorosso.h"
#include "ticker.h"

PicoRosso picorosso;
Ticker ticker;

void app_main()
{
    picorosso_setup(picorosso, ZENOH_NODE_NAME, ZENOH_ROUTER_ADDRESS);

    ticker_setup(ticker, "tick");

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(60000));
    }
}