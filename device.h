// addresses for uart registers
#define UART_CONTROL    0x80030000
#define UART_STATUS     0x80030002
#define UART_TX_DATA    0x80030004 
#define UART_RX_DATA    0x80030006

// addresses for uart interrupt registers
#define UART_INT_MASK   0x800a002c
#define UART_INT_STATUS 0x800a0030
#define UART_INT_ACK    0x800a0034

// mask values for uart registers (status and control)
#define DATA_READY      0x10 //data received in receive register
#define XMT_FIFO_FULL   0x40 //transmit fifo (hardware) is full
#define XMT_FIFO_EMPTY  0x80 //transmit fifo (hardware) is empty
#define UART_BUF_SIZE   1024 //size of fifo buffer (software)

// mask values for uart interrupt register (mask, status and acknowledge)
#define UART_INT_TX     0x08 //transmit interrupt
#define UART_INT_RX     0x04 //receive interrupt