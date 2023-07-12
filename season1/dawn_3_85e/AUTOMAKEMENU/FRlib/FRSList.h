#ifndef FR_SLIST_H
#define FR_SLIST_H

#include "slist.h"

template <class T>
class FRSListNode : public FR_SListNode
{
	friend class FRSList<T>;
public:
	FRSListNode(const T& value_) : value(value_) { }

	T value;

private:
	~FRSListNode() { }
};

/**************************************************
 * FRSList
 **************************************************/

template <class T>
class FRSList : public FR_SList
{
public:
	FRSList();
	FRSList(const FRSList<T>&);

	void append(const T&);

	FRSList<T>& operator = (const FRSList<T>&);

	FRSList<T>& operator += (const FRSList<T>&);

	const T& getHead() const;
	const T& getTail() const;

	friend class FRSListIterator<T>;
	friend class FRSListManipulator<T>;
};

/**************************************************
 * FRSListIterator
 **************************************************/

template <class T>
class FRSListIterator : public FR_SListIterator
{
public:
	FRSListIterator();
	FRSListIterator(const FRSList<T>&);

	FRSListIterator<T> operator ++ ();
	FRSListIterator<T> operator ++ (int);

	operator FRBoolean () const;

	const T& get() const;

	FRSListIterator<T>& operator = (const FRSList<T>&);
};

/**************************************************
 * FRSListManipulator
 **************************************************/

template <class T>
class FRSListManipulator : public FR_SListManipulator
{
public:
	FRSListManipulator();
	FRSListManipulator(FRSList<T>&);

	FRSListManipulator<T> operator ++ ();
	FRSListManipulator<T> operator ++ (int);

	operator FRBoolean () const;

	T& get() const;

	void insertBefore(const T&);
	void insertAfter(const T&);
	void remove();

	FRSListManipulator<T>& operator = (FRSList<T>&);
};

#include "FRSList.cc"

#endif
