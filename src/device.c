#include "device.h"

//=============================================================================================
// transmit subroutine of interrupt servive
int uart_tx(COM *com)
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
        return 0;
    }
    return 1;
}

//=============================================================================================
// receive subroutine of interrupt servive
int uart_rx(COM *com)
{
    while ((uart_reg.status & DATA_READY) &&   //as long as hardware receive buffer is filled
            !(com->rx_buffer.full(&com->rx_buffer))) // and recieve fifo has space
    {
        com->rx_buffer.put(&com->rx_buffer, uart_reg.rx_data);
    }
    return 0;
}

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