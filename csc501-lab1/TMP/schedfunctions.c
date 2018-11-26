/* schedfunctions - Implements functions used by scheduler */

#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <schedfunctions.h>


/* Returns true if null process is the only process in ready list */
Bool isonlynullproc(){
	int curr = q[rdyhead].qnext;
	if(curr == rdytail){
		return FALSE;
	}
	if(curr == 0 && q[curr].qnext == rdytail){
		return TRUE;
	}
	return FALSE;
}

/* Functions used to implement scheduler */
int getnextprocess(int random){
	int curr = q[rdyhead].qnext;
	int roundrobin;
	int roundrobinkey;
	// if queue is empty return Empty (-1) 
	if(curr == rdytail){
		return EMPTY;
	}

	while(curr!=rdytail && q[curr].qkey <= random){
		curr = q[curr].qnext;
	}

	// If random number is greater then the largest priority process then largest priority process is scheduled 
	if(curr == rdytail){
		return q[rdytail].qprev;
	}

	// To schedule process in round robin when many processes have same priority which has to be selected 
	roundrobin = q[curr].qnext;
	roundrobinkey = q[curr].qkey;

	while(roundrobin!= rdytail && q[roundrobin].qkey == roundrobinkey){
		roundrobin = q[roundrobin].qnext;
	}
	return q[roundrobin].qprev;
}

/* select a process from ready list with max goodness value */
int getnextprocesslinux(){
	int curr = q[rdyhead].qnext;
	int currpid = 0;
	int maxgoodness = 0;
	if(curr == rdytail){
		return EMPTY;
	}
	while(curr != rdytail){
		struct pentry *procptr = &proctab[curr];
		if(procptr->flagepoch == 1 && procptr->goodness > 0 && procptr->goodness >= maxgoodness){	
			maxgoodness = procptr->goodness;
			currpid = curr;
		}
		curr = q[curr].qnext;

	}
	return currpid;
}

/* It returns true is any process in ready list has remaining quantum left in current epoch */
/* otherwise it returns false */
Bool isrunnableprocinepoch(){
	int curr = q[rdyhead].qnext;
	while(curr != rdytail){
 		struct pentry *procptr = &proctab[curr];
		if(procptr->flagepoch == 1 && procptr->counter > 0){
			return TRUE;
		}
		curr = q[curr].qnext;
	}
	return FALSE;
}
