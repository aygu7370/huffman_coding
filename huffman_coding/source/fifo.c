 /********************************************************************************
 * @file fifo.c
 ********************************************************************************
 *
 *
 * Date:        03-21-2022
 * Author:      Ayush Gupta
 * Description: PES - Assignment 6. This file handles all the operations related to
 * 				Circular Buffer.
 *              It is to be used for ECEN 5813 "Principles of Embedded Software"
 *              Course Assignment.
 ********************************************************************************/

#include "fifo.h"
#include "MKL25Z4.h"

/**************************************************************
 *  @name incrementPtr
 *
 *  @param ptr
 *      interger which represents the write and read pointers
 *
 *  Returns ptr value between 0 to Buffer Maximum size by
 *  incrementing the ptr value by 1 and roll over to 0 when
 *  ptr value exceeds Maximum buffer size.
 * ************************************************************/
static uint8_t incrementPtr(uint8_t ptr){
    if ((ptr + 1) == FIFO_CAPACITY)
        return 0;
    else
        ptr++;
    return ptr;
}

/**************************************************************
 *  @name fifoInit
 *
 *  @param in
 *      fifo: ptr to fifo in question
 *
 *  Refer fifo.h for more details.
 * ************************************************************/
void fifoInit(cbfifo_t* fifo){
	fifo->nElements = 0;
	fifo->rptr = 0;
	fifo->wptr = 0;
	fifo->bufferFullFlag = false;
	for(int i = 0; i<FIFO_CAPACITY; i++){
		(fifo->data)[i] = 0;
	}

}


/**************************************************************
 *  @name QIsEmpty
 *
 *  @param in
 *      fifo: ptr to fifo in question
 *
 *  Check if the buffer is empty or not based on the value
 *  of wptr, rptr and bufferFullFlag. Returns a -1 if the buffer
 *  is empty and 0 otherwise.
 * ************************************************************/
int QIsEmpty(cbfifo_t* cbfifo){
    if ((cbfifo->wptr == cbfifo->rptr) && (cbfifo->bufferFullFlag == 0))
        return -1;
    else
        return 0;
}

/**************************************************************
 *  @name QWriteByte
 *
 *  @param value
 *      data to be stored in the FIFO - one byte
 *
 *  @param cbfifo
 *      ptr to the FIFO.
 *
 *  Check if the buffer is full or not, and if it not full add
 *  the new element in the queue and return the number of elements
 *  added.
 * ************************************************************/
int QWriteByte(uint8_t value, cbfifo_t* cbfifo){
    if (cbfifo == NULL){
        return -1;
    }

    //check if buffer is full or not
    if (!(cbfifo->bufferFullFlag)){
        //if not full add the value in the buffer and return success as status code and increment wptr
    	cbfifo->data[cbfifo->wptr] = value;
    	NVIC_DisableIRQ(UART0_IRQn);
    	cbfifo->wptr = incrementPtr(cbfifo->wptr);
    	NVIC_EnableIRQ(UART0_IRQn);
    	cbfifo->nElements++;


    }
    else{
        //printf("Buffer is full\r\n");
        return -1;
    }

    //check if the buffer is full and if it is full assign the bufferFullFlag as true
    if((cbfifo->wptr == cbfifo->rptr) && (cbfifo->rptr != 0 && cbfifo->wptr!=0) ) {
    	NVIC_DisableIRQ(UART0_IRQn);
        cbfifo->bufferFullFlag = true;
        NVIC_EnableIRQ(UART0_IRQn);
    }
    return 0;
}

/**************************************************************
 *  @name QReadByte
 *
 *  @param cbfifo
 *      ptr to the FIFO.
 *
 *  Check if the buffer is empty or not, and if it not empty,
 *  remove the element from the FIFO and return its value.
 * ************************************************************/
uint8_t QReadByte(cbfifo_t* cbfifo){
    uint8_t value;
    //if not empty return the value pointed by rptr and increment it


    NVIC_DisableIRQ(UART0_IRQn);
    value = cbfifo->data[cbfifo->rptr];
    cbfifo->rptr = incrementPtr(cbfifo->rptr);
    cbfifo->bufferFullFlag = false;
    NVIC_EnableIRQ(UART0_IRQn);
    cbfifo->nElements--;
    return value;
}

/***************************************************************************************
 *  Dequeue an element from Circular Buffer. Returns the number of bytes that's
 *  been removed from the buffer.
 *
 *  See fifo.h for more detail.
 * *************************************************************************************/
size_t cbfifo_dequeue(void *buf, size_t nbyte, cbfifo_t* cbfifo){

    int nCopiedElements = 0;
    //check if buf points to the valid position in the memory
    if (buf == NULL){
        return (size_t) -1;
    }

    //check if nbyte is more than 0
    if (nbyte <= 0){
        return (size_t)0;
    }

    while(nbyte > 0){
        //check if buffer is empty or not
        if (QIsEmpty(cbfifo) != 0){
            //printf("FIFO is empty\n");
            break;
        }
        else{
            *((uint8_t *)buf + nCopiedElements) = QReadByte(cbfifo);
            nCopiedElements++;
        }
        nbyte--;
    }
    return (size_t)nCopiedElements;
}

/***************************************************************************************
 *  Enqueue an element from Circular Buffer. Returns the number of bytes that's
 *  been added to the buffer.
 *
 *  See cbfifo.h for more detail.
 * *************************************************************************************/
size_t cbfifo_enqueue(void *buf, size_t nbyte, cbfifo_t* cbfifo){
    int nPushedBytes = 0;
    uint8_t* temp = (uint8_t*)buf;
    //check if buf points to the valid position in the memory
    if (buf == NULL){
        return (size_t) -1;
    }

    //check if nbyte is less than or equal to 0
    if (nbyte <= 0){
        return (size_t)0;
    }

    while(nbyte > 0){
        if (QWriteByte(*(temp+nPushedBytes), cbfifo) == 0){
            nPushedBytes++;
            nbyte--;
        }
        else{
        	break;
        }
    }

    return (size_t)nPushedBytes;
}


/*******************************************************
 *  Returns the number of elements in the Circular Buffer
 *
 *  See cbfifo.h for more detail.
 * ****************************************************/
size_t cbfifo_length(cbfifo_t* cbfifo){
    return (size_t)cbfifo->nElements;
}


