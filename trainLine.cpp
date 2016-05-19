/*-------------------------------------------------------------------------*
 *---									---*
 *---		trainLine.cpp						---*
 *---									---*
 *---	    This file defines code the implements 2 Station instances,	---*
 *---	 and some number of Train objects that transit between them.	---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 2.0		Joseph Phillips		2016 May 5	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//  Compile with: $ g++ trainLine.cpp -o trainLine -lpthread -g

//  Answers to questions:

//  They all crash (segmentation fault: 11), everyone dies - if the program is not thread safe.
//  Two trains will always be waiting on each other.
//  Minimum number of conditions needed is only 1, and is that is to let the arriving train know that no one is at the station
//  No conditions are needed for a train leaving the station
//  Noticed lastly that there are no segmentation faults or crashes...everyone lives!
//  They still talk over each other occasionally, but that's just on screen - not in the actual program

#include        <cstdlib>
#include        <string>
#include        <iostream>
#include        <unistd.h>
#include        <pthread.h>
#include        "Train.h"
#include        "Station.h"

const int	NUM_TRAINS		= 4;
const int	NUM_NORTHERNLY_TRAINS	= NUM_TRAINS / 2;
const int	NUM_SOUTHERNLY_TRAINS	= NUM_TRAINS - NUM_NORTHERNLY_TRAINS;
const int       NUM_LOOPS		= 16;
const char*	TRAIN_NAME_ARRAY[NUM_TRAINS]
= { "Thomas the Tank-Engine",
    "Engine No. 9",
    "California Zephyr",
    "Tokaido Shinkansen"
};
const int	TRAIN_TRANSIT_TIME_MSECS[NUM_TRAINS]
= { 10000,	// Thomas
    10000,	// Engine no. 9
    5000,	// CA Zephyr
    1000	// Bullet train
};

Station		northTerminal("North Station");
Station		southTerminal("South Station");

//  PURPOSE:  To make the Train object pointed to by 'vPtr' do 'NUM_LOOP' times:
//	* arrive at 'northTerminal'
//	* leave at 'northTerminal'
//	* pause
//	* arrive at 'southTerminal'
//	* leave at 'southTerminal'
//	* pause
//	Returns a pointer to the Train it used.
void*   initiallyNorthernly (void*  vPtr) {
    for (int i = 0; i < NUM_LOOPS; i++) {
        northTerminal.arrive((Train *)vPtr);
        northTerminal.leave();
        ((Train *)vPtr)->pause();
        southTerminal.arrive((Train *)vPtr);
        southTerminal.leave();
        ((Train *)vPtr)->pause();
    }
    return vPtr;
}


//  PURPOSE:  To make the Train object pointed to by 'vPtr' do 'NUM_LOOP' times:
//	* arrive at 'southTerminal'
//	* leave at 'southTerminal'
//	* pause
//	* arrive at 'northTerminal'
//	* leave at 'northTerminal'
//	* pause
//	Returns a pointer to the Train it used.
void*   initiallySouthernly (void*  vPtr) {
    for (int i = 0; i < NUM_LOOPS; i++) {
        southTerminal.arrive((Train *)vPtr);
        southTerminal.leave();
        ((Train *)vPtr)->pause();
        northTerminal.arrive((Train *)vPtr);
        northTerminal.leave();
        ((Train *)vPtr)->pause();
    }
    return vPtr;
}


int     main    (int		argc,
                 char*		argv[]
                 )
{
    if  (argc > 1)
        srand(strtol(argv[1],NULL,10));
    
    pthread_t     tidArray[NUM_TRAINS];
    Train*	trainArray[NUM_TRAINS];
    
    for  (int i = 0;  i < NUM_TRAINS;  i++)
        trainArray[i] = new Train(TRAIN_NAME_ARRAY[i],TRAIN_TRANSIT_TIME_MSECS[i]);
    
    int trainInd	  = 0;
    
    //  Please make 'NUM_NORTHERNLY_TRAINS' threads (tidArray[0] to
    //  tidArray[NUM_NORTHERNLY_TRAINS-1]) run 'initiallyNorthernly()'.
    //  Each thread should get a pointer to its own Train instance
    //  (trainArray[0] to trainArray[NUM_NORTHERNLY_TRAINS-1], this is
    //   an array of POINTERS to trains, not trains themselves).
    for (int i = 0; i < NUM_NORTHERNLY_TRAINS; i++) pthread_create(&tidArray[i], NULL, initiallyNorthernly,(void*)trainArray[i]);
    
    //  Please make 'NUM_SOUTHERNLY_TRAINS' threads
    //  (tidArray[NUM_NORTHERNLY_TRAINS] to tidArray[NUM_TRAINS-1]) run
    //   'initiallySouthernly()'.  Each thread should get a pointer to its own
    //  Train instance (trainArray[NUM_NORTHERNLY_TRAINS] to
    //   trainArray[NUM_TRAINS-1], this is an array of POINTERS to trains, not
    //   trains themselves).
    for (int i = NUM_NORTHERNLY_TRAINS; i < NUM_TRAINS; i++) pthread_create(&tidArray[i], NULL, initiallySouthernly,(void*)trainArray[i]);
    
    //  Leave this loop here.  I want to make sure you get the Train pointers
    //  back from initiallyNorthernly and initiallySouthernly().
    for  (int i = 0;  i < NUM_TRAINS;  i++) trainArray[i] = NULL;
    
    //  Wait for all Train threads.  Also, get the pointers to the Train objects
    //  and delete() them because they were created by 'new'
    for (int i = 0; i < NUM_TRAINS; i++) {
        Train *trainPtr;
        pthread_join(tidArray[i], (void**)&trainPtr);
        delete(trainPtr);
    }
    
    return(EXIT_SUCCESS);
}