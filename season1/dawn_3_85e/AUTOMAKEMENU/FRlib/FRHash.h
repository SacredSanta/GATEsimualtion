#ifndef FR_THASH_H
#define FR_THASH_H

/******************************************************************
 * FRHash.h
 *
 * 1997/1/14 (by Y.Kawabata)
 *
 *		Change FRHashTable<K, T>'s inheritance. (private -> public)
 *
 *
 ******************************************************************/

#include "hash.h"

/*******************************************************************
 * FRHashEntry
 *
 *******************************************************************/
template <class K, class T>
class FRHashEntry : private FR_HashEntry {
	friend class FRHashTable<K, T>;

public:
	const K&	getKey() const { return key; }
	const T&	getValue() const { return value; }
	void		setValue(const T &value_) { value = value_; }

private:
	// destructor is private.
	// FRHashEntry can not be created in freestore.
	~FRHashEntry() { }

	// getkey() is inherit from FR_HashEntry
	const void* getkey() const { return (void *) &key; }

	K key;
	T value;
};

/*******************************************************************
 * FRHashTable
 *
 *******************************************************************/
template <class K, class T>
class FRHashTable : public FR_HashTable {
public:
	FRHashTable();
	FRHashTable(const FRHashTable<K, T> &);
	FRHashTable(unsigned (*)(const K &));
	~FRHashTable();

	FRHashEntry<K, T>* create(const K &);
	FRHashEntry<K, T>* create(const K &, FRBoolean &newflag);
	const FRHashEntry<K, T>* find(const K &) const;
	void remove(const K &);

	void setFunction(unsigned (*)(const K &));

	FRHashTable<K, T>& operator = (const FRHashTable<K, T> &);

	FR_HashTable::isEmpty;
	FR_HashTable::numberOfEntries;
	FR_HashTable::numberOfBuckets;

private:
	// Functions 
	// inherit from FR_HashTable
	//
	FR_HashEntry* instanceEntry(const void *key);
	int compareKey(const void *, const void *) const;
	unsigned getHashValue(const void *) const;

	// Members
	unsigned (*hashFuncPtr)(const K &); // pointer to hash-function.
	
};

/*******************************************************************
 * FRHashIterator
 *
 *******************************************************************/
template <class K, class T>
class FRHashIterator : private FR_HashIterator {

public:
	FRHashIterator();
	FRHashIterator(const FRHashTable<K, T> &);

	FRHashIterator<K, T>& operator = (const FRHashTable<K, T> &);
	FRHashIterator<K, T> operator ++ ();
	FRHashIterator<K, T> operator ++ (int);
	operator FRBoolean () const;
	FRHashEntry<K, T>* get() const;
};

/************************************************
 * FRHashTable's member functions.
 *
 ************************************************/

template <class K, class T>
FRHashTable<K, T>::FRHashTable() : hashFuncPtr(NULL)
{
}

template <class K, class T>
FRHashTable<K, T>::FRHashTable(const FRHashTable<K, T> &rhs)
{
	FRHashIterator<K, T> itr;
	FRHashEntry<K, T> *entryPtr;

	for (itr = rhs; itr; itr++) {
		entryPtr = itr.get();
		create(entryPtr->getKey())->setValue(entryPtr->getValue());
	}
}

template <class K, class T>
FRHashTable<K, T>::FRHashTable(unsigned (*func)(const K &)) : hashFuncPtr(func)
{
}

template <class K, class T>
FRHashTable<K, T>::~FRHashTable()
{
}

template <class K, class T>
FRHashEntry<K, T>* FRHashTable<K, T>::create(const K &key)
{
	FRBoolean dummy;
	return (FRHashEntry<K, T> *) createEntry((const void *) &key, dummy);
}

template <class K, class T>
FRHashEntry<K, T>* FRHashTable<K, T>::create(const K &key, FRBoolean &newflag)
{
	return (FRHashEntry<K, T> *) createEntry((const void *) &key, newflag);
}

template <class K, class T>
const FRHashEntry<K, T>* FRHashTable<K, T>::find(const K &key) const
{
	return (FRHashEntry<K, T> *) findEntry((const void *) &key);
}

template <class K, class T>
void FRHashTable<K, T>::remove(const K &key)
{
	deleteEntry((const void *) &key);
}

template <class K, class T>
void FRHashTable<K, T>::setFunction(unsigned (*func)(const K &))
{	hashFuncPtr = func;
}

template <class K, class T>
FRHashTable<K, T>& FRHashTable<K, T>::operator = (const FRHashTable<K, T> &rhs)
{
	if (&rhs == this)
		return *this;

	deleteAllEntries();

	FRHashIterator<K, T> itr;
	FRHashEntry<K, T> *entryPtr;

	for (itr = rhs; itr; itr++) {
		entryPtr = itr.get();
		create(entryPtr->getKey())->setValue(entryPtr->getValue());
	}
	return *this;
}

template <class K, class T>
FR_HashEntry* FRHashTable<K, T>::instanceEntry(const void *key)
{
	FRHashEntry<K, T> *entry = new FRHashEntry<K, T>;

	entry->key = *((const K *) key);
	return entry;
}

template <class K, class T>
int FRHashTable<K, T>::compareKey(const void *key1, const void *key2) const
{
	const K *k1, *k2;

	k1 = (const K *) key1;
	k2 = (const K *) key2;
	return *k1 == *k2;
}

template <class K, class T>
unsigned FRHashTable<K, T>::getHashValue(const void *key) const
{
	return hashFuncPtr(*((const K *) key));
}

/************************************************
 * FRHashIterator's member functions.
 *
 ************************************************/

template <class K, class T>
FRHashIterator<K, T>::FRHashIterator()
{
}

template <class K, class T>
FRHashIterator<K, T>::FRHashIterator(const FRHashTable<K, T> &table)
{	setTable(table);
}

template <class K, class T>
FRHashIterator<K, T>& FRHashIterator<K, T>::operator = (
	const FRHashTable<K, T> &table)
{	setTable(table);
	return *this;
}

template <class K, class T>
FRHashIterator<K, T>::operator FRBoolean () const
{	return valid();
}

template <class K, class T>
FRHashIterator<K, T> FRHashIterator<K, T>::operator ++ ()
{	searchNextEntry();
	return *this;
}

template <class K, class T>
FRHashIterator<K, T> FRHashIterator<K, T>::operator ++ (int)
{	FRHashIterator<K, T> tmp(*this);
	searchNextEntry();
	return tmp;
}

template <class K, class T>
FRHashEntry<K, T>* FRHashIterator<K, T>::get() const
{	return (FRHashEntry<K, T> *) getEntry();
}

#endif
