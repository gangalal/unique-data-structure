#ifndef AVL_TREE
#define AVL_TREE

#include "Comparator.h"

template <typename T>
class AVLTree {
	template <typename U>
	friend class AVLTreeEnumerator;
private:
	Comparator<T>* comparator = NULL;				// used to determine tree order and item equality
	unsigned long size = 0;							// actual number of items currently in tree
	T* data = NULL;									// pointer to data at this node
	int diff = 0;									// height of right minus height of left
	AVLTree<T>* left = NULL;						// pointer to left subtree
	AVLTree<T>* right = NULL;						// pointer to right subtree
	void zig();										// right rotation
	void zag();										// left rotation
	void zigzag();									// left rotation on left subtree, followed by right rotation
	void zagzig();									// right rotation on right subtree, followed by left rotation
	void rebalance();								// check for and rebalance this node, if needed
	void makeEmpty(AVLTree<T>* & current) const;    // makes tree empty
public:
	AVLTree(Comparator<T>* comparator);				// creates empty linked tree with comparator
	virtual ~AVLTree();								// deletes all links and their data items

	// if an equivalent item is not already present, insert item in order and return true
	// if an equivalent item is already present, leave tree unchanged and return false
	bool insert(T* item);

	// if an equivalent item is already present, replace item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool replace(const T* item);

	// if an equivalent item is already present, remove item and return true
	// if an equivalent item is not already present, leave tree unchanged and return false
	bool remove(const T* item);

	// if an equivalent item is present, return a copy of the item
	// if an equivalent item is not present, throw a new ExceptionLinkedtreeAccess
	T find(const T* item) const;

	unsigned long getSize() const;				// returns the current number of items in the tree
};

// add public functions here
template<typename T>
void AVLTree<T>::zig()
{
	if (left == NULL) {  // no left, no way to zig
		return;
	}
	// keep track of diff of node and left for changes
	int gdiff = diff;
	int pdiff = left->diff;

	// modify the tree
	AVLTree<T>* olnr = left;  // olnr is "old left, new right"
	left = olnr->left;
	olnr->left = olnr->right;
	olnr->right = right;
	right = olnr;

	// note that the modifications kept the node itself in place, so we need to swap its data with its olnr's
	T* tempData = data;
	data = olnr->data;
	olnr->data = tempData;

	// update the diff fields for node and new right
	if (pdiff < 0) {  // if imbalance was outside left heavy (left-left violation)
		diff = gdiff + 2;
		right->diff = gdiff - pdiff + 1;
	}
	else {  // otherwise imbalance was inside left heavy (left-right violation)
		diff = pdiff + 1;
		right->diff = gdiff + 1;
	}
}

template<typename T>
void AVLTree<T>::zag()
{
	if (right == NULL) {  // no right, no way to zig
		return;
	}
	// keep track of diff of node and right for changes
	int gdiff = diff;
	int pdiff = right->diff;

	// modify the tree
	AVLTree<T>* ornl = right;  // ornl is "old right, new left"
	right = ornl->right;
	ornl->right = ornl->left;
	ornl->left = left;
	left = ornl;

	// note that the modifications kept the node itself in place, so we need to swap its data with its ornl's
	T* tempData = data;
	data = ornl->data;
	ornl->data = tempData;

	// update the diff fields for node and new left
	if (pdiff > 0) {  // if imbalance was outside right heavy (right-right violation)
		diff = gdiff - 2;
		left->diff = gdiff - pdiff - 1;
	}
	else {  // otherwise imbalance was inside right heavy (right-left violation)
		diff = pdiff - 1;
		left->diff = gdiff - 1;
	}
}

// left rotation on left subtree, followed by right rotation
template<typename T>
void AVLTree<T>::zigzag()
{
	//AVLTree<T>* temp = right;	
	left->zag();
	this->zig();
}

// right rotation on right subtree, followed by left rotation
template<typename T>
void AVLTree<T>::zagzig()
{
	//AVLTree<T>* temp = left;
	right->zig();
	this->zag();

}

// check for and rebalance this node, if needed
template<typename T>
void AVLTree<T>::rebalance()
{
	if (diff == -2)
	{
		if (left->diff <= 0)
		{
			zig();
		}
		else
		{
			zigzag();
		}
	}
	else if (diff == 2)
	{
		if (right->diff >= 0)
		{
			zag();
		}
		else
		{
			zagzig();
		}
	}
}

/**
*AVL tree constructer with comparator 
*@param comparator passes comparator object to construct AVL tree based on comparator field
*/
template<typename T>
AVLTree <T>::AVLTree(Comparator<T>* comparator)
{
	this->comparator = comparator;
	data = NULL;
	left = NULL;
	right = NULL;
}

/**
*AVL tree distructor
*/
template<typename T>
 AVLTree <T>::~AVLTree()
{
	 makeEmpty(left);
	 makeEmpty(right);
}

 /**
*AVL tree insert fuction
*@param item passes templated item to insert in AVL tree
*@return returns true if no duplicate found
*/
 template<typename T>
 bool AVLTree <T>::insert(T* item)
 {
	 bool check = false;
	 if (data == NULL)
	 {
		 data = new T(*item);
		 size++;
		 return true;
	 }
	 if (comparator->compare(*item, *data) < 0)
	 {
		 if (left == NULL)
		 {
			 left = new AVLTree<T>(comparator);
			 left->data = new T(*item);
			 //left->insert(item);
			 --diff;
			 size++;
			 check = true;
		 }
		 else
		 {
			 int olddiff = left->diff;
			 if (!left->insert(item))
			 {
				 return false;
			 }
			 else
			 {
				 size++;
				 check = true;
			 }
			 if ((olddiff != left->diff) && (left->diff != 0))
			 {
				 --diff;
			 }
		 }

	 }
		 if (comparator->compare(*item, *data) > 0)
		 {
			 if (right == NULL)
			 {
				 right = new AVLTree<T>(comparator);
				 right->data = new T(*item);
				 ++diff;
				 size++;
				 check = true;
			 }
			 else
			 {
				 int olddiff = right->diff;
				 if (!right->insert(item))
				 {
					 return false;
				 }
				 else
				 {
					 size++;
					 check = true;
				 }
				 if ((olddiff != right->diff) && (right->diff != 0))
				 {
					 ++diff;

				 }
			 }
		 }
		 rebalance();
		 return check;
 }

 /**
 *AVL tree find fuction
 *@param item passes templated item to insert in AVL tree
 *@ return equivalent item
 */
template<typename T>
T AVLTree <T>::find(const T* item) const
{
	if (data == NULL||this->size==0)
	{
		throw new ExceptionAVLTreeAccess();
	}
	if (comparator->compare(*data, *item) == 0)
	{
		return *data;
	}

	
	else if (comparator->compare(*item, *data) == -1)
	{
		if (left == NULL)
		{
			throw new ExceptionAVLTreeAccess();
		}
		else
		{
			return left->find(item);
		}
	}
	else
	{
		if (right == NULL)
		{
			throw new ExceptionAVLTreeAccess();
		}
		else
		{
			return right->find(item);
		}
	}
	throw new ExceptionAVLTreeAccess();
}

/**
*AVL tree replace fuction
*@param item passes templated item to replace in AVL tree
*@return return true if equivalent items found
*/
template<typename T>
bool AVLTree <T>::replace(const T* item)
{
	if (data == NULL)
	{
		return false;
	}
	else
	{
		T* currentData = data;
		T* tempData = new T(*item);
		if (comparator->compare(*tempData, *currentData) == 0)
		{
			data = tempData;
			delete currentData;
			return true;
		}
		else if (comparator->compare(*item, *currentData) < 0)
		{
			if (left == NULL)
			{
				return false;
			}
			else
			{
				left->replace(item);
			}
		}
		else
		{
			if (right == NULL)
			{
				return false;
			}
			else
			{
				right->replace(item);
			}
		}
		return true;
	}
	return false;
}

/**
*AVL tree remove fuction
*@param item passes templated item to remove in AVL tree
*@return return true if equivalent items found
*/
template<typename T>
bool AVLTree <T>::remove(const T* item)
{
	T* temp = new T(*item);
	AVLTree<T>* oldRight = right;

	if (data == NULL)
	{
		return false;
	}
	else
	{
		if (comparator->compare(*temp, *data) == 0)
		{
			delete data;
			data = NULL;
			if (right != NULL) 
			{
				if (right->left == NULL)
				{
					data = right->data;
					oldRight->data = NULL;
					oldRight = right;
					right = right->right;
					oldRight->right = NULL;
					delete oldRight;
					diff--;

				}
				else
				{
					AVLTree<T>* tree;
					tree = right;
					while (tree->left != NULL)
					{
						tree = tree->left;
					}
					data = new T(*tree->data);
					int oldDiff = right->diff;
					right->remove(tree->data);
					if (oldDiff != right->diff && right->diff == 0)
					{
						diff--;
					}
				}

			}
			else if (left != NULL)
			{
				data = left->data; 
				left->data = NULL;
				delete left;
				left = NULL;
				diff++;
			}
		}
		else if (comparator->compare(*item, *data) < 0)
		{
			if (left == NULL)
			{
				return false;
			}
			else
			{
				int oldDiff = left->diff;
				if (!left->remove(item))
				{
					return false;
				}
				else
				{
					if (left != NULL && left->data == NULL)
					{
						delete left;
						left = NULL;
						diff++;
					}
					else if (left == NULL || (oldDiff != left->diff && left->diff == 0))
					{
						diff++;
					}
				}

			}
		}
		else
		{
			if (right == NULL)
			{
				return false;
			}
			else
			{
				int oldDiff = right->diff;
				if (!right->remove(item))
				{
					return false;
				}
				else
				{
					if (right != NULL && right->data == NULL)
					{
						delete right;
						right = NULL;
						diff--;
					}
					else if (right == NULL || (oldDiff != right->diff && right->diff == 0))
					{
						diff--;
					}
				}

			}
		}
		size--;
		rebalance();
		return true;
	}
}

/**
*AVL tree getSize fuction
*@return return size of tree
*/
template<typename T>
unsigned long AVLTree <T>::getSize() const
{
	return size;
}


// add additional private functions here
/**
*makes tree empty
*/
template <typename T>
void AVLTree<T>::makeEmpty(AVLTree<T>* & current) const
{
	if (current != NULL)
	{
		makeEmpty(current->left);
		makeEmpty(current->right);
		delete current->data;
		delete current;
	}
	current = NULL;
}

#endif // !AVL_TREE


