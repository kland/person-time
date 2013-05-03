#include <assert.h>
#include "LineClipping.h"
#include "Util.h"

struct TestCase {
	struct LineClipping_Rectangle rectangle;
	struct LineClipping_Segment segment, expectedSegment;
	int expectedDone;	
};

static struct TestCase tests[2];

static void Init(void)
{
	struct LineClipping_Rectangle r;
	struct LineClipping_Segment s;
	
	r.xMin = 1.0;
	r.xMax = 3.0;
	r.yMin = 1.0;
	r.yMax = 2.0;

	/*test 1*/
	
	tests[0].rectangle = r;

	s.x0 = 0.0;
	s.y0 = 0.0;
	s.x1 = 4.0;
	s.y1 = 2.0;
	tests[0].segment = s;

	tests[0].expectedDone = 1;

	s.x0 = 2.0;
	s.y0 = 1.0;
	s.x1 = 3.0;
	s.y1 = 1.5;
	tests[0].expectedSegment = s;
	
	/*test 2*/
	
	tests[1].rectangle = r;

	s.x0 = 2.0;
	s.y0 = 8.0;
	s.x1 = 4.0;
	s.y1 = 1.5;
	tests[1].segment = s;

	tests[1].expectedDone = 0;	
}


int main(void)
{
	int i, done;
	struct TestCase test;
	
	Init();
	for (i = 0; i < LEN(tests); i++) {
		test = tests[i];
		LineClipping_Clip(&(test.rectangle), &(test.segment), &done);
		assert(done == test.expectedDone);
		if (done) {
			assert(test.segment.x0 == test.expectedSegment.x0);
			assert(test.segment.y0 == test.expectedSegment.y0);
			assert(test.segment.x1 == test.expectedSegment.x1);
			assert(test.segment.y1 == test.expectedSegment.y1);
		}
	}	
	return 0;
}
