#include <float.h>
#include "PersonTime.h"
#include "Util.h"

int main(void)
{
	int i;
	double partition[] = {0, 60, 65, 70, DBL_MAX};
	double segments[][2] = {{65.1, 65.7}, {58.7, 62.1}};
	
	PersonTime_Init(partition, LEN(partition));
	for (i = 0; i < LEN(segments); i++) {
		PersonTime_Add(segments[i][0], segments[i][1]);
	}
	PersonTime_Print();
	return 0;
}
