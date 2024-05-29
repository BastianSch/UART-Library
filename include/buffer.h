#ifndef buffer_h__
#define buffer_h__

#include <stdlib.h>

//=============================================================================================
// application interface for buffer
typedef struct Buffer
{
    int (*empty)(struct Buffer *self);  // check if buffer is empty
    int (*full)(struct Buffer *self);   // check if buffer is full
    int (*put)(struct Buffer *self, unsigned char byte);    // put item into buffer
    int (*get)(struct Buffer *self, unsigned char *byte);   // get item from buffer

    struct pBuffer * _pBuffer;
} Buffer;

// private variables and functions
typedef struct pBuffer
{
    unsigned char *data;  // array for data
    int size;             // size of data array
    unsigned char read;   // location of read pointer
    unsigned char write;  // location of write pointer
} pBuffer;

//=============================================================================================
// application interface buffer constructor
extern Buffer * buffer_create(int size);

#endif  // buffer_h__