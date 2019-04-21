#include "NvraRecord.h"
#include "string"
/**
 * stores the single Nvra data record
 * @author gangalal
 * @version 10/29/2018
 */

 /**
  *
  * @param num passes integer num
  *
  */
void NvraRecord::addNum(unsigned int num)
{

	nums[numCtr] = num;						// holds the numeric data, in column order
	++numCtr;
}

/**
 *
 * @param num passes integer num
 *
 */
void NvraRecord::addString(std::string string)
{
	strings[strCtr] = string;		// holds the string data, in column order
	++strCtr;
}


/**
 *returns the number of defined index
 * @param index passes inndex
 *
 */
unsigned int NvraRecord::getNum(unsigned int index) const
{
	if (index > MAX_NUMS)
	{
		throw new ExceptionIndexOutOfRange();
	}
	return nums[index];
}


/**
 *returns the string of defined index
 * @param index passes inndex
 *
 */
std::string NvraRecord::getString(unsigned int index) const
{
	if (index > MAX_STRINGS)
	{
		throw new ExceptionIndexOutOfRange();
	}
	return strings[index];
}


/**
 *sets nums[index] to num, doesn't change counter
 * @param num passes unsigned intger
 * @param index passes inndex
 *
 */
void NvraRecord::setNum(unsigned int num, unsigned int index)
{
	if (index > MAX_NUMS)
	{
		throw new ExceptionIndexOutOfRange();
	}
	else
	{
		nums[index] = num;
	}
}


/**
 *sets strings[index] to string, doesn't change counter
 * @param num passes unsigned intger
 * @param index passes inndex
 *
 */
void NvraRecord::setString(std::string string, unsigned int index)
{
	if (index > MAX_STRINGS)
	{
		throw new ExceptionIndexOutOfRange();
	}
	else
	{
		strings[index] = string;
	}
}



/**
 *returns the string as formated
 * @param record passes record
 * @param os passes ostream
 *
 */
std::ostream& operator<<(std::ostream& os, const NvraRecord& record) //const can't use accesser and mutator methods
{
	std::string str = "";
	int numflag = 0;
	int strflag = 0;
	for (int i = 0; i < MAX_NUMS + MAX_STRINGS - 1; i++)
	{
		if (i != 3 && i != 11 && i != 12) {
			str += std::to_string(record.nums[numflag++]) + ';';
		}
		else
			str += record.strings[strflag++] + ';';
	}
	str += std::to_string(record.nums[numflag++]);
	return os << str;
}
