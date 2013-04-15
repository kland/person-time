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
		assert(partition[i] < partition[i + 1]);
		intervals[i].min = partition[i];
		intervals[i].max = partition[i + 1];
		intervals[i].personTime = 0.0;
	}
}


static int Comparison(const void *key, const void *elem) /*returns -1/0/1 iff time `key' is to the left/inside/to the right of interval `elem'*/
{
	double t;
	struct Interval *interval;
	int result;
	
	t = *((double *) key);
	interval = (struct Interval *) elem;
	if (t < interval->min) {
		result = -1;
	} else if (interval->max < t) {
		result = 1;
	} else {
		result = 0;
	}
	return result;
}


static int IntervalIndex(double t) /*returns the interval i (0 <= i < numIntervals) that t is in*/
{
	struct Interval *interval;
	int result;

	assert(t >= 0);
	assert(t < intervals[numIntervals - 1].max);

	interval = bsearch(&t, intervals, numIntervals, sizeof *intervals, Comparison);
	result = interval - intervals;
	
	assert(result >= 0);
	assert(result < numIntervals);
	return result;
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
