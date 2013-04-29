#ifndef LINE_CLIPPING_H
#define LINE_CLIPPING_H

struct LineClipping_Segment {
	double x0, y0, x1, y1; /*line segment with end points (x0, y0) and (x1, y1)*/
};

struct LineClipping_Rectangle {
	double xMin, xMax, yMin, yMax;
};

void LineClipping_Clip(const struct LineClipping_Segment *s, 
	const struct LineClipping_Rectangle *r, 
	struct LineClipping_Segment *out, int *done); /*sets `out' to s clipped to r. Status flag `done' is set to non-zero iff s crosses r.*/

#endif
