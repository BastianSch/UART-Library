#ifndef uart_h__
#define uart_h__

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

// fifo
typedef struct 
{
    unsigned char data[UART_BUF_SIZE];  // array for data
    int size;                           // size of data array
    unsigned char read;                 // location of read pointer
    unsigned char write;                // location of write pointer
} fifo_t; 

// uart interrupt hardware registers
struct uart_int_reg
{  
    unsigned char intr_mask;   // interrupt mask
    unsigned char intr_status; // source of interrupts
    unsigned char intr_ack;    // interrupt acknowledge
} uart_int_reg;

// uart hardware registers
struct uart_reg 
{
    unsigned int  control;  // serial configuration reg
    unsigned int  status;   // status
    unsigned char tx_data;  // transmit data (with hardware fifo!)
    unsigned char rx_data;  // rcv data (with hardware fifo!)
} uart_reg;

// uart interrupt subroutines
void uart_rx(); // receive characters from uart register to fifo
void uart_tx(); // write characters from fifo to uart register

// mask values for fifo number
#define TX_FIFO 0 //first fifo is for transmit data
#define RX_FIFO 1 //second fifo is for receive data

// instances of fifo
fifo_t uart_buffers[2] =
{
    {.data={}, .size=UART_BUF_SIZE, .read=0, .write=0}, // first fifo for transmit
    {.data={}, .size=UART_BUF_SIZE, .read=0, .write=0}  // second fifo for receive
};

extern void uart_init(void);
extern int uart_send(unsigned char * buf);
extern int uart_read(unsigned char * buf);

extern int fifo_empty(fifo_t *fifo);
extern int fifo_full(fifo_t *fifo);
extern int fifo_put(fifo_t *fifo, unsigned char byte);
extern int fifo_get(fifo_t *fifo, unsigned char *byte);

#endif  // uart_h__

