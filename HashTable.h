#ifndef HASH_TABLE
#define HASH_TABLE

#include "Exceptions.h"
#include "Comparator.h"
#include "OULinkedList.h"
#include "OULinkedListEnumerator.h"
#include "Hasher.h"
#include <cmath>

const unsigned int SCHEDULE_SIZE = 25;			// the number of items in the size schedule
const unsigned int SCHEDULE[SCHEDULE_SIZE] = { 1, 2, 5, 11, 23, 53, 107, 223, 449, 907, 1823, 3659, 7309, 14621, 29243, 58511, 117023, 234067, 468157, 936319, 1872667, 3745283, 7490573, 14981147, 29962343 };		// the size schedule (all primes)
const unsigned int DEFAULT_SCHEDULE_INDEX = 3;	// the default position in the size schedule
const unsigned long DEFAULT_BASE_CAPACITY = SCHEDULE[DEFAULT_SCHEDULE_INDEX]; 	// the default size of the array
const float DEFAULT_MAX_LOAD_FACTOR = 0.9f;		// the default load factor used to determine when to increase the table size
const float DEFAULT_MIN_LOAD_FACTOR = 0.4f;		// the default load factor used to determine when to decrease the table size
const unsigned long DEFAULT_TABLE_SIZE = floor(DEFAULT_BASE_CAPACITY*DEFAULT_MAX_LOAD_FACTOR); //todo: when to use?

template <typename T>
class HashTable {
	template <typename U>
	friend class HashTableEnumerator;						// necessary to allow the enumerator to access the table's private data
private:
	Comparator<T>* comparator = NULL;						// used to determine item equality
	Hasher<T>* hasher = NULL;								// used to compute hash value
	unsigned long size = 0;									// actual number of items currently in hash table
	float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR;			// the load factor used to determine when to increase the table size
	float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR;			// the load factor used to determine when to decrease the table size
	unsigned int scheduleIndex = DEFAULT_SCHEDULE_INDEX;	// the index of current location in the size schedule
	unsigned long baseCapacity = DEFAULT_BASE_CAPACITY;		// the size of the array
	unsigned long totalCapacity = baseCapacity;				// the size of the array plus chains of more than one link
	OULinkedList<T>** table = NULL;							// table will be an array of pointers to OULinkedLists of type T
	void expandTable();
	void  contractTable();
public:
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher);			// creates an empty table of DEFAULT_BASE_CAPACITY
	HashTable(Comparator<T>* comparator, Hasher<T>* hasher,
		// if size given, creates empty table with size from schedule of sufficient capacity (considering maxLoadFactor)
		unsigned long size,
		float maxLoadFactor = DEFAULT_MAX_LOAD_FACTOR,
		float minLoadFactor = DEFAULT_MIN_LOAD_FACTOR);
	virtual ~HashTable();

	// if an equivalent item is not already present, insert item at proper location and return true
	// if an equivalent item is already present, leave table unchanged and return false
	bool insert(T* item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool replace(T* item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave table unchanged and return false
	bool remove(T* item);

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionHashTableAccess
	T find(const T* item) const;

	unsigned long getSize() const;						// returns the current number of items in the table
	unsigned long getBaseCapacity() const;				// returns the current base capacity of the table
	unsigned long getTotalCapacity() const;				// returns the current total capacity of the table
	float getLoadFactor() const;						// returns the current load factor of the table size/(totalcapacity=basecapacity+overflow )//copy to new table
	unsigned long getBucketNumber(const T* item) const;	// returns the bucket number for an item using its hash function mod array size//knows the table size

};

// put implementation for HashTable here
/**
 * default constructor
 * @param  comparetor passes comparator object
 * @param  hasher passes hasher object
 */
template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher)
{
	this->comparator = comparator;
	this->hasher = hasher;
	table = new OULinkedList<T>*[DEFAULT_BASE_CAPACITY];
	for (unsigned long i = 0; i < DEFAULT_BASE_CAPACITY; i++)
	{
		table[i] = new OULinkedList<T>(this->comparator);
	}
}

/**
 * constructer with user size
 * @param  comparetor passes comparator object
 * @param  hasher passes hasher object
 * @param size passes user size
 */
template <typename T>
HashTable<T>::HashTable(Comparator<T>* comparator, Hasher<T>* hasher, unsigned long size, float maxLoadFactor, float minLoadFactor)
{
	this->comparator = comparator;
	this->hasher = hasher;
	this->minLoadFactor = minLoadFactor;
	this->maxLoadFactor = maxLoadFactor;
	unsigned long tempSize = 0;
	while (tempSize < SCHEDULE_SIZE && (SCHEDULE[tempSize] <= (unsigned int)((float)size / maxLoadFactor)))
	{
		tempSize++;
	}

	baseCapacity = SCHEDULE[tempSize];
	table = new OULinkedList<T>*[baseCapacity];
	for (unsigned long i = 0; i < baseCapacity; i++)
	{
		table[i] = new OULinkedList<T>(this->comparator);
	}
	totalCapacity = baseCapacity;
}

/**
 * hash table destructer
 */
template <typename T>
HashTable<T>::~HashTable()
{
	if (table != NULL)
	{
		delete table;
	}

}

/**
* helper method to expand the table
*/
template <typename T>
void HashTable<T>::expandTable()
{
	unsigned long tempSize = 0;
	while (tempSize < SCHEDULE_SIZE && (SCHEDULE[tempSize] <= (unsigned int)((float)size / maxLoadFactor)))
	{
		tempSize++;
	}

	unsigned long newBaseCapacity = SCHEDULE[tempSize];

	OULinkedList<T>** tempTable = new OULinkedList<T>*[newBaseCapacity];
	for (unsigned long i = 0; i < newBaseCapacity; i++)
	{
		tempTable[i] = new OULinkedList<T>(this->comparator);
	}
	totalCapacity = newBaseCapacity;

	for (unsigned long i = 0; i < baseCapacity; i++)
	{
		OULinkedListEnumerator<T> tempE = table[i]->enumerator();
		while (tempE.hasNext())
		{
			T item = tempE.next();
			unsigned long index = this->hasher->hash(item) % newBaseCapacity;
			OULinkedList<T>* temp = tempTable[index];
			temp->insert(&item);
			if (temp->getSize() > 1)
			{
				totalCapacity++;
			}
		}
		tempTable[i] = this->table[i];
	}
	delete[] table;
	table = tempTable;
	baseCapacity = newBaseCapacity;

}


/**
* helper method to contract the table
*/
template <typename T>
void HashTable<T>::contractTable()
{
	unsigned long tempSize = 0;
	while (tempSize < SCHEDULE_SIZE && (SCHEDULE[tempSize] <= (unsigned int)((float)size / maxLoadFactor)))
	{
		tempSize++;
	}
	unsigned long newBaseCapacity = SCHEDULE[tempSize];
	OULinkedList<T>** tempTable = new OULinkedList<T>*[newBaseCapacity];
	for (unsigned long i = 0; i < newBaseCapacity; i++)
	{
		tempTable[i] = new OULinkedList<T>(this->comparator);
	}
	totalCapacity = newBaseCapacity;
	for (unsigned long i = 0; i < baseCapacity; i++)
	{
		OULinkedListEnumerator<T> tempE = table[i]->enumerator();
		while (tempE.hasNext())
		{
			T item = tempE.next();
			unsigned long index = this->hasher->hash(item) % newBaseCapacity;
			OULinkedList<T>* temp = tempTable[index];
			temp->insert(&item);
			if (temp->getSize() > 1)
			{
				totalCapacity++;
			}
		}
		tempTable[i] = this->table[i];
	}
	delete[] table;
	table = tempTable;
	baseCapacity = newBaseCapacity;
	totalCapacity = baseCapacity;

}


/**
* @param item passes item to insert
* @retrun returns true if equivalent item is not present otherwise false
*/
template <typename T>
bool HashTable<T>::insert(T* item)
{
	if (((float)size / (float)totalCapacity) > maxLoadFactor)
	{
		expandTable();
	}
	unsigned long index = this->getBucketNumber(item);
	OULinkedList<T>* temp = table[index];
	//OULinkedList<T>* temp = new OULinkedList<T>(this->comparator);
	if (!temp->insert(item))
	{
		return false;
	}
	else
	{

		table[index] = temp;
		size++;
		if (temp->getSize() > 1)
		{
			totalCapacity++;
		}
		return true;
	}


}

/**
* @param item passes item to remove
* @retrun returns true if equivalent item is not present otherwise false
*/
template <typename T>
bool HashTable<T>::remove(T* item)
{

	unsigned long index = this->getBucketNumber(item);
	OULinkedList<T>* temp = table[index];
	//OULinkedList<T>* temp = new OULinkedList<T>(this->comparator);
	if (!temp->remove(item))
	{
		return false;
	}
	else
	{
		table[index] = temp;
		size--;
		if (temp->getSize() == 1)
		{
			totalCapacity--;
		}
		return true;
	}
	if (((float)size / (float)totalCapacity) < minLoadFactor)
	{
		contractTable();
	}
}

/**
* @param item passes item to find
* @retrun returns true if equivalent item is not present otherwise false
*/
template <typename T>
T HashTable<T>::find(const T* item) const
{

	if (table[this->getBucketNumber(item)]->getSize() == 0)
	{
		throw new ExceptionHashTableAccess();
	}
	else
	{
		//OULinkedListEnumerator<T>* chainEnumerator = new OULinkedListEnumerator<T>(table[bucket]->enumerator());
		try
		{
			return table[this->getBucketNumber(item)]->find(item);
		}
		catch (const ExceptionLinkedListAccess* e)
		{
			delete e;
			throw new ExceptionHashTableAccess();
		}

	}
}

/**
* @param item passes item to replace
* @retrun returns true if equivalent item is not present otherwise false
*/
template <typename T>
bool HashTable<T>::replace(T* item)
{

	if (table[this->getBucketNumber(item)]->replace(item))
	{
		return true;
	}
	else
	{
		return false;
	}

}


/**
* @retrun returns total number  of item
*/
template <typename T>
unsigned long HashTable<T>::getSize() const
{
	return size;
}

/**
* @retrun returns basee capacity
*/
template <typename T>
unsigned long HashTable<T>::getBaseCapacity() const
{
	return baseCapacity;
}

/**
* @retrun returns total capacity
*/
template <typename T>
unsigned long HashTable<T>::getTotalCapacity() const
{
	return totalCapacity;
}

/**
* @retrun returns load factor
*/
template <typename T>
float HashTable<T>::getLoadFactor() const // returns the current load factor of the table size/(totalcapacity=basecapacity+overflow )//copy to new table
{
	float lfactor = (float)size / (float)totalCapacity;
	return lfactor;
}

/**
* @retrun returns bucketNumber
*/
template <typename T>
unsigned long HashTable<T>::getBucketNumber(const T* item) const
{
	unsigned long bucketNum = hasher->hash(*item) % baseCapacity;
	return bucketNum;
}



#endif // !HASH_TABLE