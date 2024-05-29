#ifndef com_h__
#define com_h__

#include "buffer.h"
#include "registers.h"
//=============================================================================================
// application interface for communication
typedef struct COM
{
    int (*send)(struct COM *self, unsigned char *buf);
    int (*read)(struct COM *self, unsigned char *buf, int length);

    Buffer tx_buffer;
    Buffer rx_buffer;
    
    struct pCOM * _pCOM;
} COM;

// private variables and functions
typedef struct pCOM
{
    int (*tx)(struct COM *self);
    int (*rx)(struct COM *self);

    intRegister_t int_reg;
    unsigned char UART_INT_TX;
} pCOM;

//=============================================================================================
// application interface com constructor
extern COM * com_create(int buffer_size, int(*tx)(COM*), int(*rx)(COM*), intRegister_t int_reg, unsigned char UART_INT_TX);

#endif  // com_h__

