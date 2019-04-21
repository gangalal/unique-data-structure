#pragma once

#ifndef OU_LINKED_LIST
#define OU_LINKED_LIST
#include "OULink.h"
#include "Comparator.h"
#include "OULinkedListEnumerator.h"

/**
 * creates the Ordered, Unique Linked List
 * @author gangalal
 * @version 10/29/2018
 */

 // OULinkedList stands for Ordered, Unique Linked List. It is a linked list that is always maintained in
 // order (based on the comparator provided to it when the list is created) and that only contains unique
 // items (that is, duplicates are not allowed)
template <typename T>
class OULinkedList {
	template <typename Type>
	friend class OULinkedListEnumerator;
private:
	Comparator<T>* comparator = NULL;				// used to determine list order and item equality
	unsigned long size = 0;							// actual number of items currently in list
	OULink<T>* first = NULL;						// pointer to first link in list
	OULink<T>* last = NULL;							// pointer to last link in list
public:
	OULinkedList(Comparator<T>* comparator);		// creates empty linked list with comparator
	virtual ~OULinkedList();						// deletes all links and their data items

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave list unchanged and return false
	bool insert(const T* item);

	// if item is greater than item at last, append item at end and return true
	// if item is less than or equal to item at last, leave list unchanged and return false
	bool append(const T* item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool replace(T* item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave list unchanged and return false
	bool remove(T* item);

	// if any items are present, return a copy of the first item
	// if no items are present, throw new ExceptionLinkedListAccess
	T get() const;

	// if an equivalent item is present, return a copy of the first such item
	// if an equivalent item is not present, throw a new ExceptionLinkedListAccess
	T find(const T* item) const;

	unsigned long getSize() const;					// returns the current number of items in the list

	OULinkedListEnumerator<T> enumerator() const;	// create an enumerator for this linked list
};

// Implementation goes here

/**
 * OULinkedList constructor
 * @param  comparator passes comparator object to order on user define field
 */
template <typename T>
OULinkedList <T> ::OULinkedList(Comparator<T>* comparator)
{
	this->comparator = comparator;
	this->size = 0;
	first = NULL;
	last = NULL;
}

/**
* OULinkedList destructor which deletes first item and recursively deletes the next until NULL
*/
template <typename T>
OULinkedList <T> :: ~OULinkedList()
{
	if (first != NULL)
	{
		delete first;
		first = NULL;
	}

}

/**
 * inserts the item in sorted order
 * @param  item passes item to insert
 * @return if an equivalent item is not already present, return true else false
 */
template <typename T>
bool OULinkedList <T> ::insert(const T* item)
{
	if (first == NULL)//checking first item
	{
		OULink<T>* newData = new OULink<T>(item);
		first = newData;
		last = newData;
		size++;
		return true;
	}
	if ((comparator->compare(*first->data, *item) > 0))//checking first item
	{
		OULink<T>* newLink = new OULink<T>(item);
		newLink->next = first;
		first = newLink;
		size++;
		return true;
	}
	if ((comparator->compare(*first->data, *item) == 0))//checking first item
	{
		return false;
	}

	OULink<T>* current = first;
	OULink<T>* prev = NULL;
	while (current != NULL && (comparator->compare(*current->data, *item) < 0))//now checks next item recursively
	{
		prev = current;
		current = current->next;

	}
	if (current == NULL)//if current pointer moves out of last item 
	{
		if ((comparator->compare(*prev->data, *item) > 0))
		{
			OULink<T>* newLink = new OULink<T>(item);
			newLink->next = prev;
			prev = newLink;
			size++;
			return true;
		}
		else if ((comparator->compare(*prev->data, *item) < 0))
		{
			OULink<T>* newLink = new OULink<T>(item);
			prev->next = newLink;
			last = newLink;
			size++;
			return true;
		}
		else if ((comparator->compare(*prev->data, *item) == 0))
		{
			return false;
		}

	}
	else if ((comparator->compare(*prev->data, *item) < 0) && (comparator->compare(*current->data, *item) > 0))// if current item is greater than previous item
	{
		OULink<T>* newLink = new OULink<T>(item);
		prev->next = newLink;
		newLink->next = current;
		size++;
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

/**
 *  f item is greater than item at last, append item at end
 * @param  item passes item to append
 * @return if item is greater than item at last return true else false
 */
template <typename T>
bool OULinkedList <T> ::append(const T* item)
{
	if (first == NULL)//checking first item
	{
		OULink<T>* newData = new OULink<T>(item);
		first = newData;
		last = newData;
		size++;
		return true;
	}
	else
	{
		if ((comparator->compare(*last->data, *item) == -1))//now checking next item recursivly
		{
			OULink<T>* newLink = new OULink<T>(item);
			last->next = newLink;
			last = last->next;
			size++;
			return true;
		}
		else
		{
			return false;
		}
	}
}

/**
 * if an equivalent item is already present, replace item else remain unchange
 * @param  item passes item to replace
 * @return if an equivalent item is already present, return true else false
 */
template <typename T>
bool OULinkedList <T> ::replace(T* item)
{
	if (first == NULL)//checking first item
	{
		throw new  ExceptionMemoryNotAvailable();
	}
	else//now checking next item recursivly
	{

		OULink<T>* current = first;
		while (current != NULL)
		{
			if (current == NULL)
			{
				return false;
			}
			else if ((comparator->compare(*current->data, *item) == 0))
			{
				current->data = item;
				return true;
			}
			else
			{
				return false;
			}
			current = current->next;
		}
		return false;
	}
}

/**
 * if an equivalent item is already present, removes item else remain unchange
 * @param  item passes item to removes
 * @return if an equivalent item is already present, return true else false
 */
template <typename T>
bool OULinkedList <T> ::remove(T* item)
{
	if (first == NULL)//checking first item
	{
		return false;
	}
	OULink<T>* temp = first;
	OULink<T>* prev = NULL;
	if (temp != NULL && (comparator->compare(*temp->data, *item) == 0)) //checking first item
	{
		first = temp->next;
		temp->next = NULL;
		delete temp;
		temp = first;
		size--;
		return true;
	}
	else//now checking next items recursivly
	{
		while (temp != NULL)
		{

			prev = temp;
			temp = temp->next;
			if (temp != NULL)
			{
				if ((comparator->compare(*temp->data, *item) == 0))
				{
					prev->next = temp->next;
					temp->next = NULL;
					delete temp;
					size--;
					return true;
				}
			}
		}
		return false;
	}
}

/**
 * f an equivalent item is present, return a copy of the first such item else hrow new  ExceptionLinkedListAccess
 * @param  item passes item to find
 * @return if an equivalent item is present, return a copy of the first such item
 */
template <typename T>
T OULinkedList <T> ::find(const T* item) const
{
	if (first == NULL)//checking first item
	{
		throw new  ExceptionLinkedListAccess();
	}
	else//now checking next items recursivly
	{
		OULink<T>* current = first;
		while (current != NULL && (comparator->compare(*current->data, *item) != 0))
		{
			current = current->next;
		}
		if (current == NULL)
		{
			throw new  ExceptionLinkedListAccess();
		}
		else
		{
			return *current->data;
		}
	}

}

/*
*if any items are present, return a copy of the first item else throw new ExceptionLinkedListAccess
*/
template <typename T>
T OULinkedList <T> ::get() const
{
	if (first == NULL)//checking first item
	{
		throw new ExceptionLinkedListAccess();
	}
	else
	{
		return *first->data;
	}
}

/*
*returns the current number of items in the list
*/
template <typename T>
unsigned long OULinkedList <T> ::getSize() const
{
	/*unsigned long c = 0;
	OULink<T>* current = first;
	while (current != NULL)
	{
		current = current->next;
		c++;
	}*/
	return size;
}

/*
*create an enumerator for this linked list
*/
template <typename T>
OULinkedListEnumerator<T> OULinkedList <T>::enumerator() const
{
	OULinkedListEnumerator<T>* enumerator = new OULinkedListEnumerator<T>(first);//create an enumerator for this linked list
	return *enumerator;
}



#endif // !OU_LINKED_LIST