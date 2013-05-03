#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "LineClipping.h"

/*Outcode set elements*/
#define LEFT 0
#define RIGHT 1
#define BOTTOM 2
#define TOP 3

static int In(int x, int A) /*returns non-zero iff x is a member of set A (bit x in A is set)*/
{
	return ((1 << x) & A) != 0;
}


static void Include(int x, int *A) /*includes element x in set *A*/
{
	*A |= (1 << x);
}


static int Outcode(double x, double y, const struct LineClipping_Rectangle *r) /*returns the outcode for point (x, y) in relation to rectangle r*/
{
	int result;
	
	result = 0; /*empty set*/
	if (x < r->xMin) {
		Include(LEFT, &result);
	} else if (x > r->xMax) {
		Include(RIGHT, &result);
	}
	if (y < r->yMin) {
		Include(BOTTOM, &result);
	} else if (y > r->yMax) {
		Include(TOP, &result);
 	}
	return result;
}


static int SameSide(int outcode0, int outcode1) /*returns non-zero iff both outcodes lie on the same side outside of the clip rectangle*/
{
	return (outcode0 & outcode1) != 0;
}


static int Inside(int outcode0, int outcode1) /*returns non-zero iff both outcodes lie inside of the clip rectangle*/
{
	return (outcode0 == 0) && (outcode1 == 0);
}


static void GetIntersection(const struct LineClipping_Rectangle *r,
	const struct LineClipping_Segment *s,
	int outcode,
	double *x,
	double *y)
{
	if (In(TOP, outcode)) {
		*x = s->x0 + (s->x1 - s->x0) * (r->yMax - s->y0) / (s->y1 - s->y0);
		*y = r->yMax;
	} else if (In(BOTTOM, outcode)) {
		*x = s->x0 + (s->x1 - s->x0) * (r->yMin - s->y0) / (s->y1 - s->y0);
		*y = r->yMin;
	} else if (In(RIGHT, outcode)) {
		*y = s->y0 + (s->y1 - s->y0) * (r->xMax - s->x0) / (s->x1 - s->x0);
		*x = r->xMax;
	} else if (In(LEFT, outcode)) {
		*y = s->y0 + (s->y1 - s->y0) * (r->xMin - s->x0) / (s->x1 - s->x0);
		*x = r->xMin;
	}
}


void LineClipping_Clip(const struct LineClipping_Rectangle *r,
	struct LineClipping_Segment *s, 
	int *done)
{
	int outcode0, outcode1, i;
	double x, y;
	
	/*Cohen-Sutherland line clipping algorithm*/
	outcode0 = Outcode(s->x0, s->y0, r);
	outcode1 = Outcode(s->x1, s->y1, r);
	i = 0;
	while (! Inside(outcode0, outcode1) && ! SameSide(outcode0, outcode1) && (i < 4)) { /*counter i guarantees loop termination*/
		if (outcode0 != 0) {
			GetIntersection(r, s, outcode0, &x, &y);
			s->x0 = x;
			s->y0 = y;
			outcode0 = Outcode(x, y, r);
		} else {
			GetIntersection(r, s, outcode1, &x, &y);
			s->x1 = x;
			s->y1 = y;
			outcode1 = Outcode(x, y, r);
		}
		i++;
	}
	assert(i < 4);
	*done = Inside(outcode0, outcode1);
}

