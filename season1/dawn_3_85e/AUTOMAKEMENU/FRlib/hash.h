#ifndef FR_HASH_H
#define FR_HASH_H

/******************************************************************
 * hash.h
 *
 * 1997/1/14 (by Y.Kawabata)
 *
 *		Fix bag of deleteAllEntries().
 *		On bag-fixing, append private function clear().
 *
 *
 ******************************************************************/

#include <stddef.h>	// define 'NULL' in this header.
#include <FR/FRBoolean.h>

/*************************************************************
 *
 *		FR_HashEntry
 *
 *************************************************************/
class FR_HashEntry {
public:
	virtual ~FR_HashEntry();

	virtual const void* getkey() const = 0;

	FR_HashEntry *next;
};

inline FR_HashEntry::~FR_HashEntry()
{
}

/*************************************************************
 *
 *		FR_HashTable
 *
 *************************************************************/
class FR_HashTable {
	friend class FR_HashIterator;

public:
	FR_HashTable();
	virtual ~FR_HashTable();

	FR_HashEntry* createEntry(const void *key, FRBoolean &newflag);
	FR_HashEntry* findEntry(const void *key) const;
	void deleteEntry(const void *key);
	void deleteAllEntries();

	FRBoolean isEmpty() const;
	int numberOfEntries() const;
	int numberOfBuckets() const;

	void distribution() const;

protected:
	virtual FR_HashEntry* instanceEntry(const void *key) = 0;
	virtual int compareKey(const void *, const void *) const = 0;
	virtual unsigned int getHashValue(const void *) const = 0;

private:
	FR_HashEntry **buckets;
	int numBuckets;
	int rebuiltSize;	// if (numBuckets == rebuiltSize), rebuild buckets.
	int entries;

	FR_HashEntry* search_bucket(FR_HashEntry *bucket, const void *key) const;
	void init();
	void rebuild();
	void clear();
};

///// inline functions.

inline FR_HashTable::FR_HashTable()
{	init();
}

inline FR_HashTable::~FR_HashTable()
{	clear();
}

inline FRBoolean FR_HashTable::isEmpty() const
{	return entries == 0;
}

inline int FR_HashTable::numberOfEntries() const
{	return entries;
}

inline int FR_HashTable::numberOfBuckets() const
{	return numBuckets;
}

/*************************************************************
 *
 *		FR_HashIterator
 *
 *************************************************************/
class FR_HashIterator {
public:
	FRHashIterator();
	void searchFirstEntry();
	void searchNextEntry();
	FRBoolean valid() const;
	void setTable(const FR_HashTable &);
	FR_HashEntry* getEntry() const;

private:
	const FR_HashTable *tablePtr;
	FR_HashEntry *entryPtr;
	int index; // index of bucket.
};

///// inline functions.

inline FRBoolean FR_HashIterator::valid() const
{	return entryPtr != NULL;
}
inline FR_HashEntry* FR_HashIterator::getEntry() const
{	return entryPtr;
}

#endif
