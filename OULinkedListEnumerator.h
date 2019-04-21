#ifndef OU_LINKED_LIST_ENUMERATOR
#define OU_LINKED_LIST_ENUMERATOR

#include "Enumerator.h"
#include "OULink.h"

template <typename T>
class OULinkedListEnumerator : public Enumerator<T>
{
private:
	OULink<T>* current;
public:
	OULinkedListEnumerator(OULink<T>* first);
	virtual ~OULinkedListEnumerator();
	bool hasNext() const;
	T next();
	T peek() const;
};

// Implementation goes here

/**
 * OULinkedListEnumerator constructor sets the next data to first data recursively
 * @param  first passes next  data to set first
 */
template <typename T>
OULinkedListEnumerator <T> ::OULinkedListEnumerator(OULink<T>* first)
{
	current = first;
}

/**
 * OULinkedListEnumerator destructor deletes new Enumeration
 */
template <typename T>
OULinkedListEnumerator <T> ::~OULinkedListEnumerator()
{

}

/**
*returns true if any data is present in the list
*/
template <typename T>
bool OULinkedListEnumerator <T> ::hasNext() const
{
	return (current != NULL);
}

/**
*returns copy of next element and advances to next position
*/
template <typename T>
T OULinkedListEnumerator <T> ::next()
{
	if (current == NULL)
	{
		throw new ExceptionEnumerationBeyondEnd();
	}
	else
	{
		OULink<T>* newElement = current;
		current = current->next;
		return *newElement->data;
	}

}

/**
* returns copy of next element without advancing position
*/
template <typename T>
T OULinkedListEnumerator <T> ::peek() const
{
	if ((current == NULL))
	{
		throw new ExceptionEnumerationBeyondEnd();
	}
	return *current->data;
}






#endif // !OU_LINKED_LIST_ENUMERATOR