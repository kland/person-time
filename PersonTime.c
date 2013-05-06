#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LineClipping.h"
#include "PersonTime.h"
#include "Util.h"

struct Rectangle { /*lower-left-closed upper-right-open rectangle*/
	double sigmaMin, sigmaMax;
	double tauMin, tauMax;
	double personTime; /*total person-time spent in this rectangle*/
};

static struct Rectangle **grid; /*total person-time for each rectangle*/
static int sigmaIntervalCount; /*number of intervals on the sigma axis in the grid*/
static int tauIntervalCount; /*number of intervals on the tau axis in the grid*/

void PersonTime_Init(const double sigma[], int m, const double tau[], int n)
{
	int i, j;
	
	assert(m >= 2);
	assert(n >= 2);
	
	sigmaIntervalCount = m - 1;
	tauIntervalCount = n - 1;
	NEW_N(grid, sigmaIntervalCount);
	for (i = 0; i < sigmaIntervalCount; i++) {
		NEW_N(grid[i], tauIntervalCount);
		for (j = 0; j < tauIntervalCount; j++) {
			assert(sigma[i] < sigma[i + 1]);
			grid[i][j].sigmaMin = sigma[i];
			grid[i][j].sigmaMax = sigma[i + 1];

			assert(tau[j] < tau[j + 1]);
			grid[i][j].tauMin = tau[j];
			grid[i][j].tauMax = tau[j + 1];

			grid[i][j].personTime = 0.0;
		}
	}
}


static int SigmaComparison(const void *key, const void *elem) /*returns -1/0/1 iff time `key' is less than/inside/greater than sigma interval in `elem'*/
{
	double s;
	struct Rectangle **sigmaInterval;
	int result;
	
	s = *((double *) key);
	sigmaInterval = (struct Rectangle **) elem;
	if (s < (*sigmaInterval)->sigmaMin) {
		result = -1;
	} else if ((*sigmaInterval)->sigmaMax < s) {
		result = 1;
	} else {
		result = 0;
	}
	return result;
}


static int TauComparison(const void *key, const void *elem) /*returns -1/0/1 iff time `key' is less than/inside/greater than tau interval in `elem'*/
{
	double t;
	struct Rectangle *tauInterval;
	int result;
	
	t = *((double *) key);
	tauInterval = (struct Rectangle *) elem;
	if (t < tauInterval->tauMin) {
		result = -1;
	} else if (tauInterval->tauMax < t) {
		result = 1;
	} else {
		result = 0;
	}
	return result;
}


static int InsideGrid(double s, double t) /*returns non-zero iff point (s, t) lies inside the grid*/
{
	return (grid[0][0].sigmaMin <= s)
		&& (s < grid[sigmaIntervalCount - 1][0].sigmaMax)
		&& (grid[0][0].tauMin <= t)
		&& (t < grid[0][tauIntervalCount - 1].tauMax);
}


static void GetRectangle(double s1, double t1, int *i1, int *j1) /*sets (*i1, *j1) to the grid rectangle containing point (s1, t1)*/
{
	struct Rectangle **sigmaInterval;
	struct Rectangle *tauInterval;

	assert(InsideGrid(s1, t1));

	sigmaInterval = bsearch(&s1, grid, sigmaIntervalCount, sizeof *grid, SigmaComparison);
	*i1 = sigmaInterval - grid;
	
	tauInterval = bsearch(&t1, grid[0], tauIntervalCount, sizeof *(grid[0]), TauComparison);
	*j1 = tauInterval - grid[0];
	
	assert(*i1 >= 0);
	assert(*i1 < sigmaIntervalCount);
	assert(*j1 >= 0);
	assert(*j1 < tauIntervalCount);
}


static void AddToRectangle(double s1, double t1, double s2, double t2, struct Rectangle *r) /*adds person-time spent in rectangle r for time segment with endpoints (s1, t1) and (s2, t2)*/
{
	struct LineClipping_Rectangle rectangle;
	struct LineClipping_Segment segment;
	int done;
		
	rectangle.xMin = r->sigmaMin;
	rectangle.yMin = r->tauMin;
	rectangle.xMax = r->sigmaMax;
	rectangle.yMax = r->tauMax;

	segment.x0 = s1;
	segment.y0 = t1;
	segment.x1 = s2;
	segment.y1 = t2;
	
	LineClipping_Clip(&rectangle, &segment, &done);
	if (done) {
		r->personTime += segment.x1 - segment.x0; /*x1 - x0 equals y1 - y0*/
	}
}


void PersonTime_Add(double s1, double t1, double dt)
{
	double s2, t2; /*endpoint*/
	int i1, j1; /*grid rectangle containing point (s1, t1)*/
	int i2, j2; /*grid rectangle containing point (s2, t2)*/
	int i, j;
	
	s2 = s1 + dt;
	t2 = t1 + dt;
	assert(InsideGrid(s1, t1));
	assert(InsideGrid(s2, t2));
	
	GetRectangle(s1, t1, &i1, &j1);
	GetRectangle(s2, t2, &i2, &j2);
	
	for (i = i1; i <= i2; i++) {
		for (j = j1; j <= j2; j++) {
			AddToRectangle(s1, t1, s2, t2, &(grid[i][j]));
		}
	}	
}


void PersonTime_Print(void)
{
	const int colSep = 2;
	const char sigmaMinLabel[] = "Sigma Min";
	const char sigmaMaxLabel[] = "Sigma Max";
	const char tauMinLabel[] = "Tau Min";
	const char tauMaxLabel[] = "Tau Max";	
	const char personTimeLabel[] = "Person Time";
	int i, j;

	printf("%s", sigmaMinLabel);
	printf("%*s", (int) (colSep + strlen(sigmaMaxLabel)), sigmaMaxLabel);
	printf("%*s", (int) (colSep + strlen(tauMinLabel)), tauMinLabel);
	printf("%*s", (int) (colSep + strlen(tauMaxLabel)), tauMaxLabel);
	printf("%*s", (int) (colSep + strlen(personTimeLabel)), personTimeLabel);
	putchar('\n');

	for (i = 0; i < sigmaIntervalCount; i++) {
		for (j = 0; j < tauIntervalCount; j++) {
			printf("%*.1f", (int) strlen(sigmaMinLabel), grid[i][j].sigmaMin);
			printf("%*.1f", (int) (colSep + strlen(sigmaMaxLabel)), grid[i][j].sigmaMax);
			printf("%*.1f", (int) (colSep + strlen(tauMinLabel)), grid[i][j].tauMin);
			printf("%*.1f", (int) (colSep + strlen(tauMaxLabel)), grid[i][j].tauMax);
			printf("%*.1f", (int) (colSep + strlen(personTimeLabel)), grid[i][j].personTime);
			putchar('\n');
		}
	}
}
