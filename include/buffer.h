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

//=============================================================================================
// application interface buffer constructor
extern Buffer * buffer_create(int size);

#endif  // buffer_h__