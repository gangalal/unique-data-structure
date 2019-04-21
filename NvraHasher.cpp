#include "NvraHasher.h"
/**
* @return record Id
*/
unsigned long NvraHasher::hash(const NvraRecord& item) const
{
	return (unsigned long)item.getNum(0);
}