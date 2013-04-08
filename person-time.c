#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

typedef struct {
	double tsup; /*supremum of the interval (minimum equals supremum for the previous interval or zero)*/
	double personTime; /*total person-time spent in this interval*/
} Interval;

Interval intervals[] = { /*person-time in interval order*/
	{60.0, 0.0},
	{65.0, 0.0},
	{70.0, 0.0},
	{DBL_MAX, 0.0}};


int IntervalCmp(double t, int i) /*returns -1/0/1 iff t is to the left/inside/to the right of interval i*/
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


int IntervalIndex(double t) /*returns the interval i (0 <= i < LEN(intervals)) that t is in*/
{
	int low, mid, high, sign;

	assert(t >= 0);
	assert(t < intervals[LEN(intervals) - 1].tsup);

	/*binary search for interval containing t*/
	low = 0;
	high = LEN(intervals) - 1;
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
	assert(mid < LEN(intervals));
	assert(IntervalCmp(t, mid) == 0);
	
	return mid;
}


void AddPersonTime(double t1, double t2) /*adds time segment [t1, t2] to intervals*/
{
	int fromInterval, toInterval, i;
	
	assert(t1 >= 0);
	assert(t1 < t2);
	assert(t2 < intervals[LEN(intervals) - 1].tsup);
	
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


void PrintPersonTime(void) /*prints array intervals*/
{
	int i;

	puts("Time interval  Person-time");
	for (i = 0; i < LEN(intervals); i++) {
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


int main(void)
{
	int i;
	double segments[2][2] = {{65.1, 65.7}, {58.7, 62.1}};
	
	for (i = 0; i < LEN(segments); i++) {
		AddPersonTime(segments[i][0], segments[i][1]);
	}
	PrintPersonTime();
	return 0;
}
