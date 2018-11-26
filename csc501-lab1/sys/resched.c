/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <math.h>
#include <sched.h>
#include <schedfunctions.h>

#define LAMBDA 0.1
unsigned long currSP;   /* REAL sp of current process */
extern int ctxsw(int, int, int, int);
int epochduration = 0; /* stores the current epoch time */

/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:       Upon entry, currpid gives current process id.
 *              Proctab[currpid].pstate gives correct NEXT state for
 *                      current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	register struct pentry  *optr;  /* pointer to old process entry */
	register struct pentry  *nptr;  /* pointer to new process entry */
	int schedulerclass; /*scheduler policy to be used either EXPDISTSCHED or LINUX or DEFAULT*/
	int random; /* random number for exponenitial distribution scheduler */
	int nextprocid;
	schedulerclass = getschedclass();

	if(schedulerclass == EXPDISTSCHED){
		/* no switch is needed if only null process is in ready queue and the current prcoess is alive */
		if( ((optr = &proctab[currpid])->pstate == PRCURR) && (isonlynullproc())){
			return(OK);
		}
		/* force context switch */
		if (optr->pstate == PRCURR){
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}
		/* remove prcoess by using exponential scheduling policy */
		random = (int) expdev(LAMBDA);
		//		kprintf("the random valur is %d \n",random);
		nextprocid = getnextprocess(random);
		if(nextprocid == EMPTY){
			currpid = NULLPROC;
		}else{
			currpid = dequeue(nextprocid);
		}
		nptr = &proctab[currpid];
		nptr->pstate = PRCURR;          /* mark it currently running    */
#ifdef  RTCLOCK
		preempt = QUANTUM;              /* reset preemption counter     */
#endif

	}else if(schedulerclass == LINUXSCHED){
		int i = 0;
		optr= &proctab[currpid];
		int quantumused = optr->counter - preempt;
		//kprintf(" the counter %d preempt %d  quantumused %d \n",optr->counter,preempt,quantumused);
		if(quantumused >= 0){
			epochduration = epochduration - quantumused;
			if(preempt <= 0){
				optr->counter = 0;
				optr->goodness = 0;
			}else{
				optr->counter = preempt;
				optr->goodness = optr->counter + optr->epochpriority;
			}
		}
		/* force context switch */
                if (optr->pstate == PRCURR) {
                        optr->pstate = PRREADY;
                        insert(currpid,rdyhead,optr->pprio);
                }
		if(!isrunnableprocinepoch()){
			epochduration = 0;
		}

		/* New epoch starts */
		if(epochduration <= 0){
			//kprintf("epoch reset \n");
			epochduration = 0;
			for(i=0; i<NPROC; i++){
				struct pentry *procptr = &proctab[i];
				if(procptr->pstate != PRFREE){
					procptr->flagepoch = 1;
					procptr->epochpriority = procptr->pprio;
					procptr->counter = (int)(procptr->counter/2) + procptr->epochpriority;
					procptr->goodness = procptr->counter + procptr->epochpriority;
					epochduration = epochduration + procptr->counter;
				}else{
					procptr->flagepoch = 0;
				}
			}

		}
		/* select a process from ready list with max goodness value */
		nextprocid = getnextprocesslinux();
		if(nextprocid == EMPTY){
			currpid = NULLPROC;
		}else{
			currpid = dequeue(nextprocid);
		}
		nptr = &proctab[currpid];
		nptr->pstate = PRCURR;               /* mark it currently running    */
#ifdef  RTCLOCK
		preempt = nptr->counter;              /* reset preemption counter     */
#endif	
	}

	else{

		/* no switch needed if current process priority higher than next*/

		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
				(lastkey(rdytail)<optr->pprio)) {
			return(OK);
		}

		/* force context switch */

		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}

		/* remove highest priority process at end of ready list */

		nptr = &proctab[ (currpid = getlast(rdytail)) ];
		nptr->pstate = PRCURR;          /* mark it currently running    */
#ifdef  RTCLOCK
		preempt = QUANTUM;              /* reset preemption counter     */
#endif
	}
	//kprintf("\n\n changing in resched from %s to %s \n\n",optr->pname,nptr->pname);

	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);

	/* The OLD process returns here when resumed. */
	return OK;
}
