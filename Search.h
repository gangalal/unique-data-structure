#ifndef SEARCH_H
#define SEARCH_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

// returns FIRST array location matching the given item (based on the comparator)
template <typename T>
long long binarySearch(const T& item, const TemplatedArray<T>& array, const Comparator<T>& comparator)
{
	// implementation goes here
	if (array.getSize() == 0)
	{
		return -1LL;
	}

	unsigned long low = 0;
	unsigned long  high = array.getSize() - 1;
	unsigned long mid = (high + low) / 2;

	while (high >= low)
	{
		if (comparator.compare(item, array.get(mid)) == 0)
		{
			while ((comparator.compare(item, array.get(mid)) == 0))
			{
				if (mid == 0)
				{
					return mid;
				}
				else
				{
					--mid;
				}
			}
			return (long long)mid + 1;
		}

		if (comparator.compare(item, array.get(mid)) == 1)
		{
			low = mid + 1;
		}
		else if (mid == 0)
		{
			return -1LL;
		}
		else
		{
			high = mid - 1;
		}
		mid = (high + low) / 2;
	}
	return -(long long)mid - 2;
}


#endif