// Signature must be retained. Implementation must be added.
#ifndef SORT_H
#define SORT_H

#include "Exceptions.h"
#include "TemplatedArray.h"
#include "Comparator.h"

template <typename T>
class Sorter {
private:
	// additional member functions (methods) and variables (fields) may be added
	static void quickSort(TemplatedArray<T>& list, unsigned long  left, unsigned long right, const Comparator<T>& comparator);
public:
	static void sort(TemplatedArray<T>& array, const Comparator<T>& comparator);
};

template <typename T>
//long long binarySearch(const T& item, const TemplatedArray<T>& array, const Comparator<T>& comparator)
void Sorter <T> ::quickSort(TemplatedArray<T>& list, unsigned long left, unsigned long right, const Comparator<T>& comparator)
{
	if (left < right)
	{
		// Pivot Value Choice Step
		T pivot = list.get(left);

		// Partition Step
		unsigned long i = left;
		unsigned long j = right + 1;
		do
		{
			do
			{
				i = i + 1;
			} while ((i < list.getSize()) && (comparator.compare(list.get(i), pivot) < 0));
			do
			{
				j = j - 1;
			} while ((comparator.compare(list.get(j), pivot) > 0));
			if (i < j)
			{
				T iValue = list.get(i);
				T jValue = list.get(j);
				list.replaceAt(&jValue, i);
				list.replaceAt(&iValue, j);
			}
		} while (i < j);
		T leftValue = list.get(left);
		T jValue = list.get(j);
		list.replaceAt(&jValue, left);
		list.replaceAt(&leftValue, j);

		// Recursive Step	
		if (j > 0)
		{
			quickSort(list, left, j - 1, comparator);
		}
		quickSort(list, j + 1, right, comparator);
	}
}


template <typename T>
void Sorter<T>::sort(TemplatedArray<T>& array, const Comparator<T>& comparator)
{
	quickSort(array, 0UL, array.getSize() - 1, comparator);
}
#endif