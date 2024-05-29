#include "com.h"

//=============================================================================================
// application interface function for sending len characters from buf
static int com_send(COM *com, unsigned char *buf)
{
    while (*buf)                                    // while length of buffer
    {
        com->tx_buffer.put(&com->tx_buffer, *buf++);    // write to buffer
    }
    com->_pCOM->int_reg.intr_mask |= com->_pCOM->UART_INT_TX;          // enable transmit interrupt->interrupt calls uart_irq_interrupt()
    
    return 0;
}

//=============================================================================================
// application interface function for reading characters to buf
static int com_read(COM *com, unsigned char * buf, int length)
{
    int ret;

    if(com->rx_buffer.empty(&com->rx_buffer))
        return 1;
    
    unsigned char c = '0';
    while(length--)                                  // while length of buffer
    {
        ret = com->rx_buffer.get(&com->rx_buffer, &c);  // get char from fifo
        if(ret)
            return 0;    
        *buf++ = c;                                 // write char to buffer
    }
    return 0;
}

//=============================================================================================
// application interface function: initialisiation of com
COM * com_create(int buffer_size, int(*tx)(COM *), int(*rx)(COM *), intRegister_t int_reg, unsigned char UART_INT_TX)
{
    COM * com = malloc(sizeof(com));
    com->_pCOM = malloc(sizeof(pCOM));

    com->_pCOM->tx = tx;
    com->_pCOM->rx = rx;

    Buffer *rx_buffer = buffer_create(buffer_size);
    Buffer *tx_buffer = buffer_create(buffer_size);

    com->tx_buffer = *tx_buffer;
    com->rx_buffer = *rx_buffer;

    com->_pCOM->int_reg = int_reg;

    com->_pCOM->UART_INT_TX = UART_INT_TX;

    com->send = &com_send;
    com->read = &com_read;

    return com;
}