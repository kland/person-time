#include <float.h>
#include "PersonTime.h"
#include "Util.h"

int main(void)
{
	double sigma[] = {0.0, 60.0, 65.0, 70.0, 100}; /*partition of first time axis*/
	double tau[] = {40.0, 60.0, 100.0}; /*partition of second time axis*/
	double segments[][3] = {{65.1, 65.7, 5.0}, {58.7, 62.1, 5.0}}; /*time segments (s, t, dt)*/
	int i;
	
	PersonTime_Init(sigma, LEN(sigma), tau, LEN(tau));
	for (i = 0; i < LEN(segments); i++) {
		PersonTime_Add(segments[i][0], segments[i][1], segments[i][2]);
	}
	PersonTime_Print();
	return 0;
}
