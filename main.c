#include <stdio.h>
#include "uart.h"

int main()
{
    printf("Start\n");
    uart_init();
    printf("Initialized\n");

    unsigned char tx_buf[3] = {'a', 'b', 'c'};
    printf("Write Buffer\n");
    uart_send(tx_buf);
    printf("Sent\n");

    unsigned char rv_buf[3] = {'d', 'e', 'f'};
    printf("Read Buffer\n");
    uart_read(rv_buf);
    printf(rv_buf);
    printf("\n");
}