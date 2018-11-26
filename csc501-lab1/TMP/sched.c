/*sched.c - provides functions to set and get the scheduler type */

#include<stdio.h>
#include<sched.h>


int scheduler = 0;              /*scheduler stores the current scheduler class and it is intialized to default */

/* Function changes the scheduler type to either EXPDISTSCHED or LINUXSCHED. */
/* sched_class takes '1' for EXPDISTSCHED or '2' for LINUXSCHED */
void setschedclass (int sched_class){
        scheduler = sched_class;
}

/* Function returns the scheduling class which is either EXPDISTSCHED or LINUXSCHED. */
int getschedclass(){
        return scheduler;
}
