#include <float.h>
#include "PersonTime.h"
#include "Util.h"

int main(void)
{
	double sigma[] = {0.0, 10.0, 20.0}; /*partition of first time axis*/
	double tau[] = {0.0, 10.0, 20.0}; /*partition of second time axis*/
	double segments[][3] = {{5.0, 5.0, 10.0}}; /*time segments (s, t, dt)*/
	int i;
	
	PersonTime_Init(sigma, LEN(sigma), tau, LEN(tau));
	for (i = 0; i < LEN(segments); i++) {
		PersonTime_Add(segments[i][0], segments[i][1], segments[i][2]);
	}
	PersonTime_Print();
	return 0;
}
