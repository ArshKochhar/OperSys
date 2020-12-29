//+
//    ELEC 377,  Lab 3
//
//  consumer.c
//-

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>

#include "common.h"

#define FALSE 0
#define TRUE 1

//+
// the consumer process reads data (chars in this case) from the shared buffer and puts them in the stdout
// in simple terms, consume the data from the buffer.  
//-


int main (int argc, char *argv[]){

	// initialize the shared memory, load in the initial array's, spawn the worker
	// processes.

	key_t   key;
	struct shared    *sharedPtr;
	int shmid;
	int c;
    
	/*	 Shared memory init 	*/
	key = ftok(".", 'S');
	if((shmid = shmget(key, MEMSIZE, IPC_CREAT|0666)) == -1 ){
		if( (shmid = shmget(key, MEMSIZE, 0)) == -1){
			printf("Error allocating shared memory. \n");
			exit(1);
		}
	}

	// now map the region..
	if((int)(sharedPtr = (struct shared *) shmat(shmid, 0, 0)) == -1){
	    printf("Couldn't map the memory into our process space.\n");
	    exit(1);
	}
	
    // put your code here...
	///algo from lab pdf
    getMutex(&sharedPtr->lock);
	int numProd=sharedPtr->numProducers; 
	releaseMutex(&sharedPtr->lock);
	
	int charRead= TRUE;
	
	while(numProd>0 && charRead){ //while charRead and producer exists
		charRead=FALSE;
		while (charRead == FALSE && numProd > 0){ //while charRead is false and there is a producer
			getMutex(&sharedPtr->lock);
			if(sharedPtr->count>0){ //if there is a char in the buffer
				c=sharedPtr->buffer[sharedPtr->out]; //retrieve and store in C
				sharedPtr->out=(sharedPtr->out+1)%BUFFSIZE;
				charRead=TRUE; //set to true
				sharedPtr->count--; //decremenet couunter
			}i
			else{
				numProd=sharedPtr->numProducers; // if the buffer is empty, update producers (local variable)
			}
			releaseMutex(&sharedPtr->lock); // release mutex 
		}
		putchar(c); //stdout
	}
	return 0;
    
}
