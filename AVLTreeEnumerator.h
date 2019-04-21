#ifndef AVL_TREE_ENUMERATOR
#define AVL_TREE_ENUMERATOR
using namespace std;
#include <stack>
#include "Enumerator.h"
#include "AVLTreeOrder.h"
#include "AVLTree.h"

template <typename T>
class AVLTreeEnumerator : public Enumerator<T>
{
	template <typename U>
	friend class OULinkedList;
private:
	AVLTreeOrder order;
	stack<const AVLTree<T>*> traversalStack;
	void buildTraversalStack(const AVLTree<T>* current);
public:
	AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order);
	virtual ~AVLTreeEnumerator();
	bool hasNext() const;
	T next();						// throws ExceptionEnumerationBeyondEnd if no next item is available
	T peek() const;					// throws ExceptionEnumerationBeyondEnd if no next item is available
	AVLTreeOrder getOrder();		// returns the order of this enumerator (preorder, inorder, or postorder)
};

/**
*building traversal stack
*if postorder builds stack in order self, right, left
*if preorder builds stack in order right, left,self
*if inorder builds stack in order right, self, left
*@param current passes root of tree
*/
template <typename T>
void AVLTreeEnumerator<T>::buildTraversalStack(const AVLTree<T>* current)
{
	if (order == AVLTreeOrder::postorder)
	{
		traversalStack.push(current);
		if (current->right != NULL)
		{
			buildTraversalStack(current->right);
		}
		if (current->left != NULL)
		{
			buildTraversalStack(current->left);
		}
	}

	if (order == AVLTreeOrder::preorder)
	{
		if (current->right != NULL)
		{
			buildTraversalStack(current->right);
		}
		if (current->left != NULL)
		{
			buildTraversalStack(current->left);
		}
		traversalStack.push(current);
	}
	
	if (order == AVLTreeOrder::inorder)
	{
		if (current->right != NULL)
		{
			buildTraversalStack(current->right);
		}
		traversalStack.push(current);

		if (current->left != NULL)
		{
			buildTraversalStack(current->left);
		}
	}
}

/**
*AVLTreeEnumerator constructor
*/
template <typename T>
AVLTreeEnumerator<T>::AVLTreeEnumerator(const AVLTree<T>* root, AVLTreeOrder order)
{
	if (root->getSize() == 0)
	{
		throw new ExceptionAVLTreeAccess();
	}
	this->order = order;
	buildTraversalStack(root);
}

/**
*AVLTreeEnumerator destructor
*/
template <typename T>
AVLTreeEnumerator<T>::~AVLTreeEnumerator()
{
}

/**
*@return returns true if next item is found
*/
template <typename T>
bool AVLTreeEnumerator<T>::hasNext()const
{
	return (!traversalStack.empty());
}

/**
*@return returns next item
*/
template <typename T>
T AVLTreeEnumerator<T>::next()
{
	T* record = traversalStack.top()->data;
	traversalStack.pop();
	return *record;
}

/**
*@return returns peek item
*/
template <typename T>
T AVLTreeEnumerator<T>::peek()const
{
	return *traversalStack.top()->data;
}

/**
*@return returns order
*/
template <typename T>
AVLTreeOrder AVLTreeEnumerator<T>::getOrder()
{
	return order;
}

#endif // !AVL_TREE_ENUMERATOR