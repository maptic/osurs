#include <osurs/reservationTypes.hpp>
#include <stdexcept>

SeatReservationCollection::SeatReservationCollection(int count) :seatCount(count) {
	seatArr = new SeatReservation[seatCount];
}

SeatReservationCollection::SeatReservationCollection(int count, int ids[]) :seatCount(count) {
	seatArr = new SeatReservation[seatCount];
	for (int i = 0; i < seatCount; ++i) {
		seatArr[i].SetSeatId(ids[i]);
	}
}

SeatReservationCollection::~SeatReservationCollection()
{
	delete[] seatArr;
	seatArr = nullptr;
}

SeatReservation::SeatReservation()
{
	resCount = 0;
	seatId = 0;
	resIdArr = new int[sizeof(unsigned int) * 8];
}

SeatReservation::SeatReservation(int sId) : seatId(sId)
{
	resCount = 0;
	resIdArr = new int[sizeof(unsigned int) * 8];
}

SeatReservation::~SeatReservation()
{
	delete[] resIdArr;
	resIdArr = nullptr;
}

void SeatReservation::AddReservation(int rId)
{
	resIdArr[resCount] = rId;
	++resCount;
}

int SeatReservation::GetReservation(int pos)
{
	if (pos < resCount)
	{
		return resIdArr[pos];
	}
	else {
		throw std::out_of_range("Index out of range");
	}
}

void SeatReservation::SetSeatId(int sId)
{
	seatId = sId;
}
