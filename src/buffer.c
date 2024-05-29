#include "buffer.h"

//=============================================================================================
// subroutine to check if fifo is empty
static int fifo_empty(Buffer *fifo)
{
    if (fifo->_pBuffer->read == fifo->_pBuffer->write)    // if read pointer is on the same place as write pointer
        return 1;                   // return buffer is empty
    return 0;
}

//=============================================================================================
// subroutine to check if fifo is full
static int fifo_full(Buffer *fifo)
{
    if ((fifo->_pBuffer->write + 1 == fifo->_pBuffer->read ) ||                   // if write pointer is on the place of read pointer
       (fifo->_pBuffer->read == 0 && fifo->_pBuffer->write + 1 == fifo->_pBuffer->size ))    // or space between read pointer and write pointer equals size of buffer
        return 1;                                           // return buffer is full
    return 0;
}

//=============================================================================================
// subroutine to put char into fifo
static int fifo_put(Buffer *fifo, unsigned char byte)
{
    if (fifo_full(fifo))
        return 1;

    fifo->_pBuffer->data[fifo->_pBuffer->write] = byte;   // write byte to buffer

    fifo->_pBuffer->write++;                   // increment write pointer
    if (fifo->_pBuffer->write >= fifo->_pBuffer->size)    // if write pointer place is greater than size of fifo 
        fifo->_pBuffer->write = 0;             // set to 0 to start a new round (circular buffer)

    return 0;
}

//=============================================================================================
// subroutine to get char from fifo
static int fifo_get(Buffer *fifo, unsigned char *pByte)
{
    if (fifo_empty(fifo))
        return 1;

    *pByte = fifo->_pBuffer->data[fifo->_pBuffer->read];  // get char from actual place

    fifo->_pBuffer->read++;                    // increment the read pointer
    if (fifo->_pBuffer->read >= fifo->_pBuffer->size)     // if read pointer place is greater than size of fifo
        fifo->_pBuffer->read = 0;              // set to 0 to start a new round (circular buffer)

    return 0;
}

//=============================================================================================
// application interface function: initialisiation of fifo

void fifo_init(Buffer *buffer)
{
    buffer->empty = &fifo_empty;
    buffer->full = &fifo_full;
    buffer->put = &fifo_put;
    buffer->get = &fifo_get;
}

Buffer * buffer_create(int size)
{
    Buffer * buffer = malloc(sizeof(Buffer));
    buffer->_pBuffer = malloc(sizeof(pBuffer));
    buffer->_pBuffer->data = malloc(size * sizeof(buffer->_pBuffer->data));
    buffer->_pBuffer->size = size;
    buffer->_pBuffer->write = 0;
    buffer->_pBuffer->read = 0;
    
    // init subtype fifo
    fifo_init(buffer);

    return buffer;
}