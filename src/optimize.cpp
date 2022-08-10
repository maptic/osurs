/**
 * @brief Optimization of space utilization on trips
 * @file optimize.c
 * @date: 2022-07-12
 * @author: Tobias Meier, Merlin Unterfinger
 */

#include <osurs/optimize.hpp>
#include <osurs/types2.hpp>
#include <stdexcept>

 // Private declarations

 // Public definitions

int spaceAvailable(
	unsigned int resArr[],
	int resCount,
	int tripCount,
	unsigned int seatCount,
	unsigned int newRes)
{
	for (int i = 0; i < tripCount; ++i)
	{
		unsigned int sum = 0;
		for (int j = 0; j < resCount; ++j)
		{
			sum += (resArr[j] & ((unsigned int)1 << i)) >> i;
		}
		sum += (newRes & ((unsigned int)1 << i)) >> i;
		if (sum > seatCount)
		{
			return 0;
		}
	}
	return 1;
}

SeatReservationCollection& optimizeReservation(unsigned int resArr[], int resArrCount, int resIds[], int maxTripLengeth, int seatIds[], int seatCount)
{
	if (resArrCount <= 0)
	{
		throw std::invalid_argument("reservation array should not be empty.");
	}

	// datatype size chack
	if (maxTripLengeth > sizeof(unsigned int) * 8)
	{
		throw std::invalid_argument("data type of resArr is to small to map all trips to it.");
	}

	SeatReservationCollection* seatCollection = new SeatReservationCollection(seatCount, seatIds);

	for (int i = 0; i < seatCount; ++i)
	{
		unsigned int currentResConfig = 0;

		for (int j = 0; j < resArrCount; ++j)
		{
			if (resArr[j] != 0)
			{
				if (currentResConfig == ((1 << maxTripLengeth) - 1))
				{
					break;
				}

				if ((currentResConfig & resArr[j]) == 0)
				{
					currentResConfig |= resArr[j];
					seatCollection->seatArr[i].AddReservation(resIds[j]);
					resArr[j] = 0;
				}
			}
		}
	}
	return *seatCollection;
}


// Private definitions
