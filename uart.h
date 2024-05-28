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

// buffer
typedef struct sBufferClass
{
    unsigned char data[UART_BUF_SIZE];  // array for data
    int size;                           // size of data array
    unsigned char read;                 // location of read pointer
    unsigned char write;                // location of write pointer
    int (*empty)(struct sBufferClass *self);
    int (*full)(struct sBufferClass *self);
    int (*put)(struct sBufferClass *self, unsigned char byte);
    int (*get)(struct sBufferClass *self, unsigned char *byte);
} tBufferClass; 

// uart interrupt hardware registers
typedef struct sIntRegister
{  
    unsigned char intr_mask;   // interrupt mask
    unsigned char intr_status; // source of interrupts
    unsigned char intr_ack;    // interrupt acknowledge
} tIntRegister;

// uart hardware registers
typedef struct sRegister
{
    unsigned int  control;  // serial configuration reg
    unsigned int  status;   // status
    unsigned char tx_data;  // transmit data (with hardware fifo!)
    unsigned char rx_data;  // rcv data (with hardware fifo!)
} tRegister;

typedef struct sCommClass
{
    int (*send)(struct sCommClass *self, unsigned char *buf);
    int (*read)(struct sCommClass *self, unsigned char *buf);
    void (*tx)();
    void (*rx)();

    tBufferClass tx_buffer;
    tBufferClass rx_buffer;
    tRegister reg;
    tIntRegister int_reg;
} tCommClass;

tCommClass uart;

extern void uart_init(tCommClass *uart);
extern void fifo_init(tBufferClass *fifo);

#endif  // uart_h__

