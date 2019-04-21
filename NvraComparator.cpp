#include "NvraComparator.h"

/**
 * NvraComparator constructer with user define field
 * @author gangalal
 * @version 10/29/2018
 */
NvraComparator::NvraComparator(unsigned int column)
{
	this->column = column;
}
/**
 * @returns returns -1 is item1 < item2, 0 if item1 == item2, +1 if item1 > item2
 * @param  item1 passes item1
 * @param  item2 passes item2
 */
int NvraComparator::compare(const NvraRecord& item1, const NvraRecord& item2) const
{
	int comp = 0;
	if (column < 3)
	{
		if (item1.getNum(column) < item2.getNum(column))
		{
			comp = -1;
		}
		if (item1.getNum(column) == item2.getNum(column))

		{
			comp = 0;
		}
		if (item1.getNum(column) > item2.getNum(column))
		{
			comp = 1;
		}
	}
	if (column == 3)
	{
		if (item1.getString(0) < item2.getString(0))
		{
			comp = -1;
		}
		if (item1.getString(0) == item2.getString(0))

		{
			comp = 0;
		}
		if (item1.getString(0) > item2.getString(0))
		{
			comp = 1;
		}
	}
	if (column > 3 && column < 11)
	{
		if (item1.getNum(column - 1) < item2.getNum(column - 1))
		{
			comp = -1;
		}
		if (item1.getNum(column - 1) == item2.getNum(column - 1))

		{
			comp = 0;
		}
		if (item1.getNum(column - 1) > item2.getNum(column - 1))
		{
			comp = 1;
		}
	}
	if (column == 11)
	{
		if (item1.getString(1) < item2.getString(1))
		{
			comp = -1;
		}
		if (item1.getString(1) == item2.getString(1))

		{
			comp = 0;
		}
		if (item1.getString(1) > item2.getString(1))
		{
			comp = 1;
		}
	}
	if (column == 12)
	{
		if (item1.getString(2) < item2.getString(2))
		{
			comp = -1;
		}
		if (item1.getString(2) == item2.getString(2))

		{
			comp = 0;
		}
		if (item1.getString(2) > item2.getString(2))
		{
			comp = 1;
		}
	}

	if (column > 12 && column < MAX_NUMS + MAX_STRINGS)
	{
		if (item1.getNum(column - 3) < item2.getNum(column - 3))
		{
			comp = -1;
		}
		if (item1.getNum(column - 3) == item2.getNum(column - 3))

		{
			comp = 0;
		}
		if (item1.getNum(column - 3) > item2.getNum(column - 3))
		{
			comp = 1;
		}
	}


	return comp;
}
