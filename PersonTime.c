#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "PersonTime.h"
#include "Util.h"

struct Interval { /*left-closed right-open interval*/
	double min;
	double max;
	double personTime; /*total person-time spent in this interval*/
};

static struct Interval *intervals; /*person-time in interval order*/
static int numIntervals; /*number of intervals*/

void PersonTime_Init(double partition[], int n)
{
	int i;
	
	assert(n >= 2);
	numIntervals = n - 1;
	NEW_N(intervals, numIntervals);
	for (i = 0; i < numIntervals; i++) {
		intervals[i].min = partition[i];
		intervals[i].max = partition[i + 1];
		intervals[i].personTime = 0.0;
	}
}


static int IntervalCmp(double t, int i) /*returns -1/0/1 iff t is to the left/inside/to the right of interval i*/
{
	int result;
	
	if ((i > 0) && (t < intervals[i].min)) {
		result = -1;
	} else if (t >= intervals[i].max) {
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
	assert(t < intervals[numIntervals - 1].max);

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
	assert(t2 < intervals[numIntervals - 1].max);
	
	fromInterval = IntervalIndex(t1); 
	toInterval = IntervalIndex(t2);
	
	/*add interval fractions (from the ends of the time segment)*/
	if (fromInterval < toInterval) {
		intervals[fromInterval].personTime += intervals[fromInterval].max - t1;
		assert(toInterval > 0);
		intervals[toInterval].personTime += t2 - intervals[toInterval].min;
	} else {
		intervals[fromInterval].personTime += t2 - t1;
	}

	/*add whole intervals*/
	for (i = fromInterval + 1; i < toInterval; i++) {
		intervals[i].personTime += intervals[i].max - intervals[i].min;
	}	
}


void PersonTime_Print(void)
{
	int i;

	puts("Time interval  Person-time");
	for (i = 0; i < numIntervals; i++) {
		if (i > 0) {
			printf("%5.1f -", intervals[i].min);		
		} else {
			printf("  0.0 -");
		}
		if (intervals[i].max < DBL_MAX) {
			printf("%5.1f", intervals[i].max);
		} else {
			printf("     ");
		}
		printf("   %.2f\n", intervals[i].personTime);
	}
}
