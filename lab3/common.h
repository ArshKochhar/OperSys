//+
//  common.h - Common definiton of the shared
//     memory segment and prototypes for semaphore functions
// 
//  Created by Thomas Dean
//  Copyright 2005 Queen's University.
// header file 
//
//-

#define MEMSIZE 200
#define BUFFSIZE 5

struct shared {
    char buffer[BUFFSIZE];
    int numProducers;
    int lock;
    int out;
    int in;
    int count;
};


void getMutex(int * lock);
void releaseMutex(int * lock);
