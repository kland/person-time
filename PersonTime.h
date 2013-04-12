#ifndef PERSON_TIME_H
#define PERSON_TIME_H

void PersonTime_Init(double endpoints[], int n); /*defines the intervals for the person-time calculation. For example with n = 3 and endpoints = (60, 65, 70) the intervals are [0, 60), [60, 65) and [65, 70).*/

void PersonTime_Add(double t1, double t2); /*adds time segment [t1, t2] to intervals*/

void PersonTime_Print(void); /*prints array intervals*/

#endif
