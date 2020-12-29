//+
//	ELEC 377,  Lab 3
//
//  producer.c
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
//  The job of the producer is to initialize shared memory, generate data (chars) from stdout , put it into the shared buffer and generate more data.
//-

int main (int argc, char *argv[]){

	// initialize the shared memory, load in the initial array's, spawn the worker
	// processes.

	key_t   key;
	struct shared    *sharedPtr;
	int shmid;
	int c,stored;
    
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
	
    // put your code here..
	//algo from lab pdf 
    getMutex(&sharedPtr->lock);
	sharedPtr->numProducers++;
	releaseMutex(&sharedPtr->lock);
	
	while ((c = getchar())!=EOF){ //getChar allows to check till the end of file
		stored=FALSE;
		while(stored==FALSE){ //checking to see if there is room in the queue 
			getMutex(&sharedPtr->lock);
				if(sharedPtr->count<BUFFSIZE){
					sharedPtr->buffer[sharedPtr->in]= c; //add character to the queue
					sharedPtr->in = (sharedPtr->in + 1)%BUFFSIZE; 
					sharedPtr->count++;  
					stored=TRUE;
				}
				releaseMutex(&sharedPtr->lock); //releases the mutex
			}
		}
		
		getMutex(&sharedPtr->lock);
		sharedPtr->numProducers--;
		releaseMutex(&sharedPtr->lock);
		
		return 0;

	
}

