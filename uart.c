#include "uart.h"

static int fifo_empty(tBufferClass *fifo);
static int fifo_full(tBufferClass *fifo);
static int fifo_put(tBufferClass *fifo, unsigned char byte);
static int fifo_get(tBufferClass *fifo, unsigned char *byte);

static int uart_send(tCommClass *uart, unsigned char * buf);
static int uart_read(tCommClass *uart, unsigned char * buf);
static void uart_tx();
static void uart_rx();

//=============================================================================================
// application interface function: initialisiation of fifo
void fifo_init(tBufferClass *fifo)
{
    fifo->empty = &fifo_empty;
    fifo->full = &fifo_full;
    fifo->put = &fifo_put;
    fifo->get = &fifo_get;
}

//=============================================================================================
// application interface function: initialisiation of uart
void uart_init(tCommClass *uart)
{
    tRegister reg = {
        .control =  (unsigned int)  UART_CONTROL,
        .status =   (unsigned int)  UART_STATUS,
        .tx_data =  (unsigned char) UART_TX_DATA,
        .rx_data =  (unsigned char) UART_RX_DATA
    };
    uart->reg = reg;

    tIntRegister int_reg = {
        .intr_mask =    (unsigned char) UART_INT_MASK,
        .intr_status =  (unsigned char) UART_INT_STATUS,
        .intr_ack =     (unsigned char) UART_INT_ACK
    };
    uart->int_reg = int_reg; 

    tBufferClass tx_buffer = {.data={}, .size=UART_BUF_SIZE, .read=0, .write=0};
    fifo_init(&tx_buffer);

    tBufferClass rx_buffer = {.data={}, .size=UART_BUF_SIZE, .read=0, .write=0};
    fifo_init(&rx_buffer);
    
    uart->tx_buffer = tx_buffer;
    uart->rx_buffer = rx_buffer;

    uart->send = &uart_send;
    uart->read = &uart_read;

    uart->tx = &uart_tx;
    uart->rx = &uart_rx;
}

//=============================================================================================
// application interface function for sending len characters from buf
int uart_send(tCommClass *uart, unsigned char * buf)
{
    while (*buf)                                    // while length of buffer
    {
        (uart->tx_buffer.put)(&uart->tx_buffer, *buf++);    // write to buffer
    }
    uart->int_reg.intr_mask |= UART_INT_TX;          // enable transmit interrupt->interrupt calls uart_irq_interrupt()
    
    return 0;
}

//=============================================================================================
// application interface function for reading characters to buf
int uart_read(tCommClass *uart, unsigned char * buf)
{
    unsigned char c = '0';
    //int ret = 0;
    while(*buf /*&&                                   // while length of buffer
        !fifo_empty(uart_buffers[RX_FIFO])*/)         // and receive buffer not empty
    {
        /*ret = */(uart->rx_buffer.get)(&uart->rx_buffer, &c);  // get char from fifo
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
    status = uart.int_reg.intr_status;          // read interrupt status register
    if (status & UART_INT_RX)                 // check if recieve interrupt happened
    {
        uart.int_reg.intr_ack |= UART_INT_RX; // acknowledge receive interrupt
        uart.rx();                            // receive subroutine
    }
    if (status & UART_INT_TX)                 // check if transmit interrupt happened
    {
        uart.int_reg.intr_ack |= UART_INT_TX; // acknowledge transmit interrupt
        uart.tx();                            // transmit subroutine
    }
}

//=============================================================================================
// transmit subroutine of interrupt servive
void uart_tx(void)
{
    int ret;
    unsigned char c;
    if (uart.reg.status & XMT_FIFO_EMPTY)             // check if transmit buffer empty
    {
        while (!(uart.reg.status & XMT_FIFO_FULL))    // while transmit fifo in hardware is not full
                                                      //       and software fifo is not empty
        {
            ret = (uart.tx_buffer.get)(&uart.tx_buffer, &c);// read char from software fifo
            if (ret)
                uart.reg.tx_data = c;                 // copy char to transmit register

        }
        if (ret == 0 )                                // software transmit fifo empty?
            uart.int_reg.intr_mask &= ~UART_INT_TX;   // disable transmit interrupt

        uart.reg.control |= XMT_FIFO_EMPTY;           // enable/generate transmit empty intrerrupt
    }
}

//=============================================================================================
// receive subroutine of interrupt servive
void uart_rx(void)
{
    while ((uart.reg.status & DATA_READY) &&   //as long as hardware receive buffer is filled
            !((uart.rx_buffer.full)(&uart.rx_buffer))) // and recieve fifo has space
    {
        (uart.rx_buffer.put)(&uart.rx_buffer, uart.reg.rx_data);
    }
}

//=============================================================================================
// subroutine to check if fifo is empty
int fifo_empty(tBufferClass *fifo)
{
    if (fifo->read == fifo->write)    // if read pointer is on the same place as write pointer
        return 1;                   // return buffer is empty
    return 0;
}

//=============================================================================================
// subroutine to check if fifo is full
int fifo_full(tBufferClass *fifo)
{
    if ((fifo->write + 1 == fifo->read ) ||                   // if write pointer is on the place of read pointer
       (fifo->read == 0 && fifo->write + 1 == fifo->size ))    // or space between read pointer and write pointer equals size of buffer
        return 1;                                           // return buffer is full
    return 0;
}

//=============================================================================================
// subroutine to put char into fifo
int fifo_put(tBufferClass *fifo, unsigned char byte)
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
int fifo_get(tBufferClass *fifo, unsigned char *pByte)
{
    if (fifo_empty(fifo))
        return 1;

    *pByte = fifo->data[fifo->read];  // get char from actual place

    fifo->read++;                    // increment the read pointer
    if (fifo->read >= fifo->size)     // if read pointer place is greater than size of fifo
        fifo->read = 0;              // set to 0 to start a new round (circular buffer)

    return 0;
}