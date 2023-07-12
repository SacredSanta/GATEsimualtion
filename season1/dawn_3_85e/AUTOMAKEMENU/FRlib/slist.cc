#include "slist.h"

FR_SList::FR_SList()
{
	head = tail = NULL;
	numNodes = 0;
}

FR_SList::~FR_SList()
{
	clear();
}

void FR_SList::append(FR_SListNode *newNode)
{
	if(tail != NULL)
	{
		tail->next = newNode;
		tail = newNode;
	}
	else
	{
		tail = head = newNode;
	}
	tail->next = NULL;
	numNodes++;
}

void FR_SList::clear()
{
	FR_SListNode *nodePtr, *nextPtr;
	for(nodePtr = head; nodePtr != NULL; nodePtr = nextPtr)
	{
		nextPtr = nodePtr->next;
		delete nodePtr;
	}
	head = tail = NULL; 
	numNodes = 0;
}

int FR_SList::numberOfNodes() const
{
	return numNodes;
}

FR_SListNode* FR_SList::getHeadNode() const
{
	return head;
}
FR_SListNode* FR_SList::getTailNode() const
{
	return tail;
}

FR_SListIterator::FR_SListIterator()
{
	listPtr = NULL;
	current = NULL;
}

FR_SListIterator::~FR_SListIterator()
{

}

void FR_SListIterator::setList(const FR_SList& list)
{
	listPtr = (FR_SList *) &list;
	gotoHead();
}

void FR_SListIterator::gotoHead()
{
	if(listPtr == NULL)
	{
		// please implement erorr routine.
		return;
	}
	current = listPtr->head;
}

void FR_SListIterator::gotoNext()
{
	if(listPtr == NULL)
	{
		// please implement erorr routine.
		return;
	}
	if(current != NULL)
		current = current->next;
}

void FR_SListManipulator::gotoHead()
{
	if(listPtr == NULL)
	{
		// please implement erorr routine.
		return;
	}
	current = listPtr->head;
	prev = NULL;
}

void FR_SListManipulator::gotoNext()
{
	if(listPtr == NULL)
	{
		// please implement erorr routine.
		return;
	}
	if(current != NULL){
		prev = current;
		current = current->next;
	}
}

void FR_SListManipulator::insertBefore(FR_SListNode *newNode)
{
	if(current == NULL)
	{
		// please implement erorr routine.
		return;
	}

	if(prev == NULL)
		listPtr->head = newNode;
	else
		prev->next = newNode;

	newNode->next = current;
	prev = newNode;
	listPtr->numNodes++;
}

void FR_SListManipulator::insertAfter(FR_SListNode *newNode)
{
	if(current == NULL)
	{
		// please implement erorr routine.
		return;
	}

	if(current->next == NULL)
		listPtr->tail = newNode;

	newNode->next = current->next;
	current->next = newNode;
	listPtr->numNodes++;
}

void FR_SListManipulator::remove()
{
	if(current == NULL)
	{
		// please implement erorr routine.
		return;
	}

	FR_SListNode *oldNode = current;

	current = oldNode->next;

	if(prev == NULL)
		listPtr->head = current;
	else
		prev->next = current;

	if(current == NULL)
		listPtr->tail = prev;

	listPtr->numNodes --;

	delete oldNode;
}
