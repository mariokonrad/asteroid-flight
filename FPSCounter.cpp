#include "FPSCounter.h"

FPSCounter::FPSCounter()
	: count(0), t_start(0), t_end(0), diff(1), dcount(0)
{}

void FPSCounter::tick()
{
	if (count == 0) t_start = clock();
	if (count == 500) t_end = clock();
	if (count >= 500)
	{
		diff = t_end-t_start;
		dcount = count;
		count = 0;
	}
	else ++count;
}

double FPSCounter::fps()
{
	return (double)CLOCKS_PER_SEC / (double)diff * (double)dcount;
}

