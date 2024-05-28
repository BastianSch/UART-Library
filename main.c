#include <stdio.h>
#include "uart.h"

int main()
{
    int ret;
    unsigned char c;

    tBufferClass fifo = {
        .data={}, 
        .size=UART_BUF_SIZE, 
        .read=0, 
        .write=0
    };

    fifo_init(&fifo);
    printf("Initialized\n");

    ret = (fifo.put)(&fifo, 'c');
    printf("ret: %d\n", ret);

    ret = (fifo.get)(&fifo, &c);
    printf("ret %d %c\n", ret, c);

    tCommClass uart;

    uart_init(&uart);

    printf("Start\n");
    printf("Initialized\n");

    unsigned char tx_buf[3] = {'a', 'b', 'c'};
    printf("Write Buffer\n");
    ret = (uart.send)(&uart, tx_buf);
    printf("Sent %d\n", ret);

    ret = (uart.tx_buffer.get)(&uart.tx_buffer, &c);
    printf("tx buffer read %d %c\n", ret, c);
    ret = (uart.tx_buffer.get)(&uart.tx_buffer, &c);
    printf("tx buffer read %d %c\n", ret, c);
    ret = (uart.tx_buffer.get)(&uart.tx_buffer, &c);
    printf("tx buffer read %d %c\n", ret, c);
    
    unsigned char rv_buf[3] = {'d', 'e', 'f'};
    printf("Read Buffer\n");
    (uart.read)(&uart, rv_buf);
    printf("Read %d %s\n", ret, rv_buf);
}