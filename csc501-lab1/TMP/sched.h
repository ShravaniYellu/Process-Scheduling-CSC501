/*sched.h - provides functions to set and get the scheduler type */

#ifndef _SCHED_H_
#define _SCHED_H_

#define EXPDISTSCHED 1
#define LINUXSCHED 2

/* Function changes the scheduler type to either EXPDISTSCHED or LINUXSCHED. */
void setschedclass (int sched_class);

/* Function returns the scheduling class which is either EXPDISTSCHED or LINUXSCHED. */
int getschedclass();

#endif
