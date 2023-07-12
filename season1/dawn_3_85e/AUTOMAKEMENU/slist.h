#ifndef FR_SLIST_BASE_H
#define FR_SLIST_BASE_H

#include "FRBoolean.h"
#include <stddef.h>

class FR_SListNode
{
public:
	FR_SListNode() { }

	virtual ~FR_SListNode() { }

	FR_SListNode *next;
};

class FR_SList
{
public:
	FR_SList();

	virtual ~FR_SList();

	void append(FR_SListNode*);

	void clear();

	int numberOfNodes() const;

	FR_SListNode* getHeadNode() const;
	FR_SListNode* getTailNode() const;
private:

	FR_SListNode *head, *tail;
	int numNodes;

	friend class FR_SListIterator;
	friend class FR_SListManipulator;
};

class FR_SListIterator
{
public:
	FR_SListIterator();

	virtual ~FR_SListIterator();

	void setList(const FR_SList&);

	virtual void gotoHead();
	virtual void gotoNext();

	FRBoolean valid() const;

protected:

	FR_SList *listPtr;
	FR_SListNode *current;
};

class FR_SListManipulator : public FR_SListIterator
{
public:
	FR_SListManipulator(){};

	virtual ~FR_SListManipulator(){};

	void gotoHead();
	void gotoNext();

	void insertBefore(FR_SListNode*);
	void insertAfter(FR_SListNode*);
	void remove();

protected:
	FR_SListNode *prev;
};

inline
FRBoolean FR_SListIterator::valid() const
{
	return current != NULL;
}

#endif
