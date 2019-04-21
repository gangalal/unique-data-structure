#ifndef HASH_TABLE_ENUMERATOR
#define HASH_TABLE_ENUMERATOR

#include "Enumerator.h"
#include "Exceptions.h"
#include "HashTable.h"
using namespace std;

template <typename T>
class HashTableEnumerator : public Enumerator<T>
{
private:
	unsigned long bucket = 0;								// the current bucket during the enumeration process
	OULinkedListEnumerator<T>* chainEnumerator = NULL;		// used to move through the linked list of the current bucket
	HashTable<T>* hashTable = NULL;							// pointer to the HashTable being enumerated
	// you may add additional member variables and functions here to support the operation of your code
public:
	HashTableEnumerator(HashTable<T>* hashTable);			// constructor needs a pointer to the HashTable to be enumerated
	virtual ~HashTableEnumerator();
	bool hasNext() const;									// true if there are elements that have not yet been returned via next()
	T next();												// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;											// throws ExceptionEnumerationBeyondEnd if no next item is available
};

// put implementation for HashTableEnumerator here

/**
 * hashTable Enumerator constructor sets the next data to first data recursively
 * @param  hashTable passes hastable array
 */
template <typename T>
HashTableEnumerator<T>::HashTableEnumerator(HashTable<T>* hashTable)
{
	if (hashTable->getSize() == 0 || hashTable == NULL)
	{
		throw new ExceptionHashTableAccess();
	}
	this->hashTable = hashTable;
	chainEnumerator = new OULinkedListEnumerator<T>(hashTable->table[0]->enumerator());

}

/**
 * hashtable destructor deletes new Enumeration
 */
template <typename T>
HashTableEnumerator<T>::~HashTableEnumerator()
{
	//todo: impliment 
	delete chainEnumerator;
}

/**
*returns true if any data is present in the list
*/
template <typename T>
bool HashTableEnumerator<T>::hasNext() const
{
	//return(hashTable != NULL);
	if (bucket < hashTable->getBaseCapacity())
	{
		return true;
	}
	else
	{
		return false;
	}
}
/**
*returns next item
*/
template <typename T>
T HashTableEnumerator<T>::next()
{
	if (chainEnumerator->hasNext())
	{
		return chainEnumerator->next();
	}
	else
	{
		T data;
		do
		{
			bucket++;
		} while ((bucket < (hashTable->getBaseCapacity()) && hashTable->table[bucket]->getSize() == 0));
		if (bucket == hashTable->getBaseCapacity())
		{
			throw new ExceptionEnumerationBeyondEnd();
		}
		OULinkedList<T>* temp = hashTable->table[bucket];
		delete chainEnumerator;
		chainEnumerator = new OULinkedListEnumerator<T>(temp->enumerator());
		if (chainEnumerator->hasNext())
		{
			data = chainEnumerator->next();
		}
		return data;
	}

	/*if (chainEnumerator->hasNext())
	{
		return chainEnumerator->next();
	}
	else
	{
		T data;
		do
		{
			bucket++;
		} while ((bucket < (hashTable->getBaseCapacity()) && hashTable->table[bucket]->getSize() == 0));

		if (bucket == hashTable->getBaseCapacity())
		{
			throw new ExceptionEnumerationBeyondEnd();
		}
		else
		{
			OULinkedList<T>* temp = hashTable->table[bucket];
			delete chainEnumerator;
			chainEnumerator = new OULinkedListEnumerator<T>(temp->enumerator());
			if (chainEnumerator->hasNext())
			{
				data = chainEnumerator->next();
			}
		}

		return data;
	}*/
}

/**
*returns next peek item
*/
template <typename T>
T HashTableEnumerator<T>::peek() const
{


	/*unsigned long tempBucket = bucket;
	while (hashTable->table[tempBucket]->getSize() == 0 && tempBucket < hashTable->getBaseCapacity())
	{
		tempBucket++;
	}
	OULinkedList<T>* temp = hashTable->table[tempBucket];
	return temp->enumerator().peek();*/

	unsigned long tempBucket = bucket;
	OULinkedListEnumerator<T> currentE = *chainEnumerator;
	if (currentE.hasNext())
	{
		return currentE.peek();
	}
	else
	{
		T data;
		do
		{
			tempBucket++;
		} while ((tempBucket < (hashTable->getBaseCapacity()) && hashTable->table[tempBucket]->getSize() == 0));

		if (tempBucket == hashTable->getBaseCapacity())
		{
			throw new ExceptionEnumerationBeyondEnd();
		}
		else
		{
			OULinkedList<T>* temp = hashTable->table[tempBucket];
			OULinkedListEnumerator<T> *currentEn = new OULinkedListEnumerator<T>(temp->enumerator());
			if (currentEn->hasNext())
			{
				data = currentEn->peek();
			}
		}

		return data;
	}



	/*if (bucket < hashTable->baseCapacity)
	{
		return chainEnumerator->next();
	}
	throw new ExceptionEnumerationBeyondEnd();*/
}
#endif // !HASH_TABLE_ENUMERATOR