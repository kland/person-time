#ifndef PERSON_TIME_H
#define PERSON_TIME_H

typedef struct PersonTime_GridDesc *PersonTime_Grid;

PersonTime_Grid PersonTime_New(const double sigma[], int m, const double tau[], int n); /*returns a grid for the person-time calculation (m and n are the respective lengths of the arrays)*/

void PersonTime_Add(double s1, double t1, double dt, PersonTime_Grid grid); /*adds person-time segment with endpoints (s1, t1) and (s1 + dt, t1 + dt) to the grid*/

void PersonTime_Print(PersonTime_Grid grid); /*prints total person-time for each rectangle in the grid to standard output*/

#endif
