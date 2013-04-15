#ifndef PERSON_TIME_H
#define PERSON_TIME_H

void PersonTime_Init(double partition[], int n); /*defines the time interval partition for the person-time calculation. For example with n = 4 and partition = (0, 60, 65, 70) the intervals are [0, 60), [60, 65) and [65, 70).*/

void PersonTime_Add(double t1, double t2); /*adds person-time segment [t1, t2]*/

void PersonTime_Print(void); /*prints total person-time for each interval to standard output*/

#endif
