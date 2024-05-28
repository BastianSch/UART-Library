#include <stdio.h>
#include "registers.h"
#include "com.h"
#include "device.h"

reg_t uart_reg = {
    .control =  (unsigned int)  UART_CONTROL,
    .status =   (unsigned int)  UART_STATUS,
    .tx_data =  (unsigned char) UART_TX_DATA,
    .rx_data =  (unsigned char) UART_RX_DATA
};

intRegister_t uart_int_reg = {
    .intr_mask =    (unsigned char) UART_INT_MASK,
    .intr_status =  (unsigned char) UART_INT_STATUS,
    .intr_ack =     (unsigned char) UART_INT_ACK
};

//=============================================================================================
// transmit subroutine of interrupt servive
static void uart_tx(COM *com)
{
    int ret;
    unsigned char c;
    if (uart_reg.status & XMT_FIFO_EMPTY)             // check if transmit buffer empty
    {
        while (!(uart_reg.status & XMT_FIFO_FULL))    // while transmit fifo in hardware is not full
                                                      //       and software fifo is not empty
        {
            ret = com->tx_buffer.get(&com->tx_buffer, &c);             // read char from software fifo
            if (ret)
                uart_reg.tx_data = c;                 // copy char to transmit register

        }
        if (ret == 0 )                                // software transmit fifo empty?
            uart_int_reg.intr_mask &= ~UART_INT_TX;   // disable transmit interrupt

        uart_reg.control |= XMT_FIFO_EMPTY;           // enable/generate transmit empty intrerrupt
    }
}

//=============================================================================================
// receive subroutine of interrupt servive
void uart_rx(COM *uart)
{
    while ((uart_reg.status & DATA_READY) &&   //as long as hardware receive buffer is filled
            !(uart->rx_buffer.full(&uart->rx_buffer))) // and recieve fifo has space
    {
        (uart->rx_buffer.put)(&uart->rx_buffer, uart_reg.rx_data);
    }
}
COM *com;

//=============================================================================================
// interrupt service routine: will be called by receive or transmit interrupt
void uart_irq_interrupt(unsigned long vector)
{
    unsigned char status;
    status = uart_int_reg.intr_status;          // read interrupt status register
    if (status & UART_INT_RX)                 // check if recieve interrupt happened
    {
        uart_int_reg.intr_ack |= UART_INT_RX; // acknowledge receive interrupt
        uart_rx(com);                            // receive subroutine
    }
    if (status & UART_INT_TX)                 // check if transmit interrupt happened
    {
        uart_int_reg.intr_ack |= UART_INT_TX; // acknowledge transmit interrupt
        uart_tx(com);                            // transmit subroutine
    }
}

int main()
{
    COM *com = com_create(UART_BUF_SIZE, uart_tx, uart_rx, uart_int_reg, UART_INT_TX);

    int ret;
    unsigned char c;

    printf("Start\n");
    printf("Initialized\n");

    unsigned char tx_buf[3] = {'a', 'b', 'c'};
    printf("Write Buffer\n");
    ret = com->send(com, tx_buf);
    printf("Sent %d\n", ret);

    ret = com->tx_buffer.get(&com->tx_buffer, &c);
    printf("tx buffer read %d %c\n", ret, c);
    ret = com->tx_buffer.get(&com->tx_buffer, &c);
    printf("tx buffer read %d %c\n", ret, c);
    ret = com->tx_buffer.get(&com->tx_buffer, &c);
    printf("tx buffer read %d %c\n", ret, c);

    unsigned char rv_buf[3] = {'d', 'e', 'f'};
    printf("Read Buffer\n");
    com->read(com, rv_buf);
    printf("Read %d %s\n", ret, rv_buf);
}