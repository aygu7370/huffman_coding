 /********************************************************************************
 * @file fifo.h
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
#ifndef SOURCE_FIFO_H_
#define SOURCE_FIFO_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define FIFO_CAPACITY                                                     (512)

/*****************************************************************************
 *  Structure has the metadata for circular buffer.
 *  It has an array to store the data, rpt & wptr to track the position
 *  for enqueue and dequeue, nElements to keep track of number of elements
 *  enqueued and bufferFullFlag boolean to track the buffer full condition.
 * ***************************************************************************/
typedef struct cbfifo{
    uint8_t data[FIFO_CAPACITY];
    uint16_t rptr;
    uint16_t wptr;
    uint16_t nElements;
    bool bufferFullFlag;
} cbfifo_t;

/*******************************************************************************
 *  @name fifoInit
 *
 *  @param in
 *      fifo: ptr to fifo in question
 *
 *  @description: fifo initialization.
 *****************************************************************************/
void fifoInit(cbfifo_t* fifo);

/*******************************************************************************
 * @name isEmpty
 *
 * @param in
 *  cbfifo - fifo in question
 *
 * @description - returns a 0 if the fifo is not empty otherwise returns -1.
 ******************************************************************************/
int QIsEmpty(cbfifo_t* cbfifo);

/******************************************************************************
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   buf      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns (size_t) -1.
  ******************************************************************************/
size_t cbfifo_enqueue(void *buf, size_t nbyte, cbfifo_t* cbfifo);


/******************************************************************************
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buf      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 ******************************************************************************/
size_t cbfifo_dequeue(void *buf, size_t nbyte, cbfifo_t* cbfifo);


/******************************************************************************
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 ******************************************************************************/
size_t cbfifo_length(cbfifo_t* cbfifo);

#endif /* SOURCE_FIFO_H_ */
