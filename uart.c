#include "uart.h"

//=============================================================================================
// application interface function: initialisiation of uart
void uart_init(void)
{
    uart_reg.control = (unsigned int )  UART_CONTROL;
    uart_reg.status  = (unsigned int )  UART_STATUS;
    uart_reg.tx_data = (unsigned char ) UART_TX_DATA; 
    uart_reg.rx_data = (unsigned char ) UART_RX_DATA;

    uart_int_reg.intr_mask      = (unsigned char) UART_INT_MASK;
    uart_int_reg.intr_status    = (unsigned char) UART_INT_STATUS;
    uart_int_reg.intr_ack       = (unsigned char) UART_INT_ACK;
}

//=============================================================================================
// application interface function for sending len characters from buf
int uart_send(unsigned char * buf)
{
    while (*buf)                                    // while length of buffer
    {
        fifo_put(&uart_buffers[TX_FIFO], *buf++);    // write to buffer
    }
    uart_int_reg.intr_mask |= UART_INT_TX;          // enable transmit interrupt->interrupt calls uart_irq_interrupt()
    
    return(0);
}

//=============================================================================================
// application interface function for reading characters to buf
int uart_read(unsigned char * buf)
{
    unsigned char c = '0';
    //int ret = 0;
    while(*buf /*&&                                   // while length of buffer
        !fifo_empty(uart_buffers[RX_FIFO])*/)         // and receive buffer not empty
    {
        /*ret = */fifo_get(&uart_buffers[RX_FIFO], &c);  // get char from fifo
        /*if(ret)*/
            *buf++ = c;                                 // write char to buffer
    }
    return 0;
}

//=============================================================================================
// interrupt service routine: will be called by receive or transmit interrupt
void uart_irq_interrupt(unsigned long vector)
{
    unsigned char status;
    status = uart_int_reg.intr_status;        // read interrupt status register
    if (status & UART_INT_RX)                 // check if recieve interrupt happened
    {
        uart_int_reg.intr_ack |= UART_INT_RX; // acknowledge receive interrupt
        uart_rx();                            // receive subroutine
    }
    if (status & UART_INT_TX)                 // check if transmit interrupt happened
    {
        uart_int_reg.intr_ack |= UART_INT_TX; // acknowledge transmit interrupt
        uart_tx();                            // transmit subroutine
    }
}

//=============================================================================================
// transmit subroutine of interrupt servive
void uart_tx(void)
{
    int ret;
    unsigned char c;
    if (uart_reg.status & XMT_FIFO_EMPTY)             // check if transmit buffer empty
    {
        while (!(uart_reg.status & XMT_FIFO_FULL))    // while transmit fifo in hardware is not full
                                                      //       and software fifo is not empty
        {
            ret = fifo_get(&uart_buffers[TX_FIFO], &c);// read char from software fifo
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
void uart_rx(void)
{
    while ((uart_reg.status & DATA_READY) &&   //as long as hardware receive buffer is filled
            !fifo_full(&uart_buffers[RX_FIFO])) // and recieve fifo has space
    {
        fifo_put(&uart_buffers[RX_FIFO], uart_reg.rx_data);
    }
}

//=============================================================================================
// subroutine to check if fifo is empty
int fifo_empty(fifo_t *fifo)
{
    if (fifo->read == fifo->write)    // if read pointer is on the same place as write pointer
        return 1;                   // return buffer is empty
    return 0;
}

//=============================================================================================
// subroutine to check if fifo is full
int fifo_full(fifo_t *fifo)
{
    if ((fifo->write + 1 == fifo->read ) ||                   // if write pointer is on the place of read pointer
       (fifo->read == 0 && fifo->write + 1 == fifo->size ))    // or space between read pointer and write pointer equals size of buffer
        return 1;                                           // return buffer is full
    return 0;
}

//=============================================================================================
// subroutine to put char into fifo
int fifo_put(fifo_t *fifo, unsigned char byte)
{
    if (fifo_full(fifo))
        return 1;

    fifo->data[fifo->write] = byte;   // write byte to buffer

    fifo->write++;                   // increment write pointer
    if (fifo->write >= fifo->size)    // if write pointer place is greater than size of fifo 
        fifo->write = 0;             // set to 0 to start a new round (circular buffer)

    return 0;
}

//=============================================================================================
// subroutine to get char from fifo
int fifo_get(fifo_t *fifo, unsigned char *pByte)
{
    if (fifo_empty(fifo))
        return 1;

    *pByte = fifo->data[fifo->read];  // get char from actual place

    fifo->read++;                    // increment the read pointer
    if (fifo->read >= fifo->size)     // if read pointer place is greater than size of fifo
        fifo->read = 0;              // set to 0 to start a new round (circular buffer)

    return 0;
}