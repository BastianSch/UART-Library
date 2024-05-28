#ifndef com_h__
#define com_h__

#include "buffer.h"
#include "registers.h"
//=============================================================================================
// application interface for communication
typedef struct COM
{
    int (*send)(struct COM *self, unsigned char *buf);
    int (*read)(struct COM *self, unsigned char *buf);

    Buffer tx_buffer;
    Buffer rx_buffer;
    
    struct pCOM * _pCOM;
} COM;

//=============================================================================================
// application interface com constructor
extern COM * com_create(int buffer_size, void(*tx)(COM*), void(*rx)(COM*), intRegister_t int_reg, unsigned char UART_INT_TX);


#endif  // com_h__

