#include "com.h"

typedef struct pCOM
{
    void (*tx)(struct COM *self);
    void (*rx)(struct COM *self);

    intRegister_t int_reg;
    unsigned char UART_INT_TX;
} pCOM;

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
static int com_read(COM *com, unsigned char * buf)
{
    unsigned char c = '0';
    int ret = 0;
    while(*buf &&                                   // while length of buffer
        !com->rx_buffer.empty(&com->rx_buffer))         // and receive buffer not empty
    {
        ret = com->rx_buffer.get(&com->rx_buffer, &c);  // get char from fifo
        if(ret)
            *buf++ = c;                                 // write char to buffer
    }
    return 0;
}

//=============================================================================================
// application interface function: initialisiation of com
COM * com_create(int buffer_size, void(*tx)(COM *), void(*rx)(COM *), intRegister_t int_reg, unsigned char UART_INT_TX)
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