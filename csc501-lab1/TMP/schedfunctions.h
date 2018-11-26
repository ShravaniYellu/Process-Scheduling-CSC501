/* schedfunctions - Functions used to implement schedulers */

#ifndef _SCHEDFUNCTIONS_H_
#define _SCHEDFUNCTIONS_H_

/* It provides the next process to be scheduled depending on random */
int getnextprocess(int random);

/* Returns true if null process is the only process in ready list */
Bool isonlynullproc();

/* It gives next process id to be scheduled by follwing linux scheduler policy */
int getnextprocesslinux();

/* It returns true if any process in ready list has remaining quantum left in current epoch */
/* otherwise it returns false */
Bool isrunnableprocinepoch();
#endif
