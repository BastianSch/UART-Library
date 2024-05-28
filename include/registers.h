#ifndef device_h__
#define device_h__

// hardware registers
typedef struct s_Register
{
    unsigned int  control;  // serial configuration reg
    unsigned int  status;   // status
    unsigned char tx_data;  // transmit data (with hardware fifo!)
    unsigned char rx_data;  // rcv data (with hardware fifo!)
} reg_t;

// interrupt hardware registers
typedef struct sIntRegister
{  
    unsigned char intr_mask;   // interrupt mask
    unsigned char intr_status; // source of interrupts
    unsigned char intr_ack;    // interrupt acknowledge
} intRegister_t; 

#endif  // device_h__