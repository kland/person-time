#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "PersonTime.h"
#include "Util.h"

typedef struct {
	double tsup; /*supremum of the interval (minimum equals supremum for the previous interval or zero)*/
	double personTime; /*total person-time spent in this interval*/
} Interval;

static Interval *intervals; /*person-time in interval order*/
static int numIntervals; /*number of intervals*/

void PersonTime_Init(double endpoints[], int n)
{
	int i;
	
	assert(n > 0);
	NEW_N(intervals, n);
	numIntervals = n;
	for (i = 0; i < n; i++) {
		intervals[i].tsup = endpoints[i];
		intervals[i].personTime = 0.0;
	}
}


static int IntervalCmp(double t, int i) /*returns -1/0/1 iff t is to the left/inside/to the right of interval i*/
{
	int result;
	
	if ((i > 0) && (t < intervals[i - 1].tsup)) {
		result = -1;
	} else if (t >= intervals[i].tsup) {
		result = 1;
	} else {
		result = 0;
	}
	return result;
}


static int IntervalIndex(double t) /*returns the interval i (0 <= i < numIntervals) that t is in*/
{
	int low, mid, high, sign;

	assert(t >= 0);
	assert(t < intervals[numIntervals - 1].tsup);

	/*binary search for interval containing t*/
	low = 0;
	high = numIntervals - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		sign = IntervalCmp(t, mid);
		if (sign == 0) {
			high = low - 1; /*exit the loop*/
		} else if (sign < 0) {
			high = mid - 1;
		} else {
			low = mid + 1;
		}
	}
	
	assert(mid >= 0);
	assert(mid < numIntervals);
	assert(IntervalCmp(t, mid) == 0);
	
	return mid;
}


void PersonTime_Add(double t1, double t2)
{
	int fromInterval, toInterval, i;
	
	assert(t1 >= 0);
	assert(t1 < t2);
	assert(t2 < intervals[numIntervals - 1].tsup);
	
	fromInterval = IntervalIndex(t1); 
	toInterval = IntervalIndex(t2);
	
	/*add interval fractions (from the ends of the time segment)*/
	if (fromInterval < toInterval) {
		intervals[fromInterval].personTime += intervals[fromInterval].tsup - t1;
		assert(toInterval > 0);
		intervals[toInterval].personTime += t2 - intervals[toInterval - 1].tsup;
	} else {
		intervals[fromInterval].personTime += t2 - t1;
	}

	/*add whole intervals*/
	for (i = fromInterval + 1; i < toInterval; i++) {
		intervals[i].personTime += intervals[i].tsup - intervals[i - 1].tsup;
	}	
}


void PersonTime_Print(void)
{
	int i;

	puts("Time interval  Person-time");
	for (i = 0; i < numIntervals; i++) {
		if (i > 0) {
			printf("%5.1f -", intervals[i - 1].tsup);		
		} else {
			printf("  0.0 -");
		}
		if (intervals[i].tsup < DBL_MAX) {
			printf("%5.1f", intervals[i].tsup);
		} else {
			printf("     ");
		}
		printf("   %.2f\n", intervals[i].personTime);
	}
}
