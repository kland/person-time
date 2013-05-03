#ifndef PERSON_TIME_H
#define PERSON_TIME_H

void PersonTime_Init(const double sigma[], int m, const double tau[], int n); /*defines the grid for the person-time calculation (m and n are the respective lengths of the arrays)*/

void PersonTime_Add(double s1, double t1, double dt); /*adds person-time segment with endpoints (s1, t1) and (s1 + dt, t1 + dt)*/

void PersonTime_Print(void); /*prints total person-time for each interval to standard output*/

#endif
