/*
 * util.cpp
 *
 *  Created on: Feb 28, 2011
 *      Author: fqhuy
 */

#include <ships/core/core.hpp>

namespace Sp {
//static double Timer::deltaT(int iCounterID);

template<> int Random::Next(int min, int max) {
	return rand() % (max - min) + min;
}

template<> void Random::NextN(int* array, const uint32_t& size, const int& min,
		const int& max, const bool& duplicate) {
	int i, j;
	if (duplicate) {
		for (i = 0; i < size; i++) {
			array[i] = rand() % (max - min) + min;
		}
	} else {
		for (i = 0; i < size; i++) {
			array[i] = rand() % (max - min) + min;
			for (j = 0; j < i; j++) {
				if (array[i] == array[j]) {
					i--;
					break;
				}
			}
		}
	}
}

double Timer::deltaT(int iCounterID) {
	//TODO: this file need modification to support Windows.
	double DeltaT;

	static struct timeval _NewTime; // new wall clock time (struct representation in seconds and microseconds)
	static struct timeval _OldTime0; // old wall clock time 0(struct representation in seconds and microseconds)
	static struct timeval _OldTime1; // old wall clock time 1(struct representation in seconds and microseconds)
	static struct timeval _OldTime2; // old wall clock time 2(struct representation in seconds and microseconds)

	// Get new counter reading
	gettimeofday(&_NewTime, NULL);

	switch (iCounterID) {
	case 0: {
		// Calculate time difference for timer 0.  (zero when called the first time)
		DeltaT
				= ((double) _NewTime.tv_sec + 1.0e-6
						* (double) _NewTime.tv_usec)
						- ((double) _OldTime0.tv_sec + 1.0e-6
								* (double) _OldTime0.tv_usec);

		// Reset old time 0 to new
		_OldTime0.tv_sec = _NewTime.tv_sec;
		_OldTime0.tv_usec = _NewTime.tv_usec;

		break;
	}
	case 1: {
		// Calculate time difference for timer 1.  (zero when called the first time)
		DeltaT
				= ((double) _NewTime.tv_sec + 1.0e-6
						* (double) _NewTime.tv_usec)
						- ((double) _OldTime1.tv_sec + 1.0e-6
								* (double) _OldTime1.tv_usec);

		// Reset old time 1 to new
		_OldTime1.tv_sec = _NewTime.tv_sec;
		_OldTime1.tv_usec = _NewTime.tv_usec;

		break;
	}
	case 2: {
		// Calculate time difference for timer 2.  (zero when called the first time)
		DeltaT
				= ((double) _NewTime.tv_sec + 1.0e-6
						* (double) _NewTime.tv_usec)
						- ((double) _OldTime2.tv_sec + 1.0e-6
								* (double) _OldTime2.tv_usec);

		// Reset old time 2 to new
		_OldTime2.tv_sec = _NewTime.tv_sec;
		_OldTime2.tv_usec = _NewTime.tv_usec;

		break;
	}
	default: {
		// Requested counter ID out of range
		return -9999.0;
	}
	}

	// Returns time difference in seconds sunce the last call
	return DeltaT;
}

}
