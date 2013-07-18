#ifndef __FPSCOUNTER_H__
#define __FPSCOUNTER_H__

#include <time.h>

class FPSCounter
{
	private:
		long count;
		clock_t t_start;
		clock_t t_end;
		clock_t diff;
		long dcount;
	public:
		FPSCounter();
		void tick();
		double fps();
};

#endif
