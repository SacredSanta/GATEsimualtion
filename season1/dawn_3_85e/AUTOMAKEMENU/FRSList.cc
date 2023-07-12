/***********************************************
 * FRSList member functions
 ***********************************************/

template <class T>
FRSList<T>::FRSList()
{

}

template <class T>
FRSList<T>::FRSList(const FRSList<T>& src)
{
	clear();
	FRSListIterator<T> i;
	for(i = src; i; i++)
		append(i.get());
}

template <class T>
void FRSList<T>::append(const T& value)
{
	FR_SListNode *newNode = new FRSListNode<T>(value);
	if(newNode == NULL)
	{
		// please implement error routine.
		return;
	}
	FR_SList::append(newNode);
}

template <class T>
FRSList<T>& FRSList<T>::operator = (const FRSList<T>& rhs)
{
	if(&rhs == this)
		return *this;
	clear();
	FRSListIterator<T> i;
	for(i = rhs; i; i++)
		append(i.get());
	return *this;
}

template <class T>
FRSList<T>& FRSList<T>::operator += (const FRSList<T>& rhs)
{
	if(&rhs == this)
	{
		FRSListIterator<T> tmp(rhs);
		FRSListIterator<T> i;
		for(i = tmp; i; i++)
			append(i.get());
	}
	else
	{
		FRSListIterator<T> i;
		for(i = rhs; i; i++)
			append(i.get());
	}
	return *this;
}

template <class T>
const T& FRSList<T>::getHead() const
{ return ((FRSListNode<T> *)getHeadNode())->value; }

template <class T>
const T& FRSList<T>::getTail() const
{ return ((FRSListNode<T> *)getTailNode())->value; }

/***********************************************
 * FRSListIterator member functions
 ***********************************************/

template <class T>
FRSListIterator<T>::FRSListIterator()
{

}

template <class T>
FRSListIterator<T>::FRSListIterator(const FRSList<T>& list)
{
	setList(list);
}

template <class T>
FRSListIterator<T> FRSListIterator<T>::operator ++ ()
{
	gotoNext();
	return *this;
}

template <class T>
FRSListIterator<T> FRSListIterator<T>::operator ++ (int)
{
	FRSListIterator<T> tmp(*this);
	gotoNext();
	return tmp;
}

template <class T>
FRSListIterator<T>::operator FRBoolean () const
{
	return valid();
}

template <class T>
const T& FRSListIterator<T>::get() const
{
	FRSListNode<T> *nodePtr = (FRSListNode<T> *)current;
	return nodePtr->value;
}

template <class T>
FRSListIterator<T>& FRSListIterator<T>::operator = (const FRSList<T>& list)
{
	setList(list);
	gotoHead();
	return *this;
}

/***********************************************
 * FRSListManipulator member functions
 ***********************************************/

template <class T>
FRSListManipulator<T>::FRSListManipulator()
{

}

template <class T>
FRSListManipulator<T>::FRSListManipulator(FRSList<T>& list)
{
	setList(list);
}

template <class T>
FRSListManipulator<T> FRSListManipulator<T>::operator ++ ()
{
	gotoNext();
	return *this;
}

template <class T>
FRSListManipulator<T> FRSListManipulator<T>::operator ++ (int)
{
	FRSListManipulator<T> tmp(*this);
	gotoNext();
	return tmp;
}

template <class T>
FRSListManipulator<T>::operator FRBoolean () const
{
	return valid();
}

template <class T>
T& FRSListManipulator<T>::get() const
{
	FRSListNode<T> *nodePtr = (FRSListNode<T> *)current;
	return nodePtr->value;
}

template <class T>
void FRSListManipulator<T>::insertBefore(const T& value)
{
	FR_SListNode *newNode;
	newNode = new FRSListNode<T>(value);
	if(newNode == NULL)
	{
		// please implement error routine.
		return;
	}
	FR_SListManipulator::insertBefore(newNode);
}

template <class T>
void FRSListManipulator<T>::insertAfter(const T& value)
{
	FR_SListNode *newNode;
	newNode = new FRSListNode<T>(value);
	if(newNode == NULL)
	{
		// please implement error routine.
		return;
	}
	FR_SListManipulator::insertAfter(newNode);
}

template <class T>
void FRSListManipulator<T>::remove()
{
	FR_SListManipulator::remove();
}

template <class T>
FRSListManipulator<T>& FRSListManipulator<T>::operator = (FRSList<T>& list)
{
	setList(list);
	gotoHead();
	return *this;
}

