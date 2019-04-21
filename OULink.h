#ifndef OU_LINK_H
#define OU_LINK_H

template <typename T>
class OULink {
	template <typename Type>
	friend class OULinkedList;
	template <typename Type>
	friend class OULinkedListEnumerator;
private:
	T* data = NULL;										// pointer to data item of any type
	OULink* next = NULL;								// pointer to next link
public:
	OULink(const T* item);
	virtual ~OULink();
};

// Implementation goes here

/**
 * OULink constructor, sets the item
 * @param  item passes item to set
 */
template <typename T>
OULink<T>::OULink(const T* item)
{
	data = new T(*item);
	next = NULL;
}

/**
 * OULink destructor
 *deletes the data item if not null
 *sets next to null
 */
template <typename T>
OULink<T>::~OULink()
{
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}
	if (next != NULL)
	{
		delete next;
		next = NULL;
	}
}

#endif // !OU_LINK_H