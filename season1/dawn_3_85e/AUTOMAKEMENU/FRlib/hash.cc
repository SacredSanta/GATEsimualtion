#include "hash.h"
#include <stdio.h>

// bucket size at initializing.
const int FRHASH_INITSIZE = 4;

void FR_HashTable::init()
{
	int i;

	entries = 0;
	buckets = new FR_HashEntry* [FRHASH_INITSIZE];
	numBuckets = FRHASH_INITSIZE;
	rebuiltSize = FRHASH_INITSIZE;
	for (i = 0; i < numBuckets; i++)
		buckets[i] = NULL;
}

void FR_HashTable::clear()
{
	int i;
	for (i = 0; i < numBuckets; i++) {
		FR_HashEntry *hp, *next;

		for (hp = buckets[i]; hp != NULL; hp = next) {
			next = hp->next;
			delete hp;
		}
	}
	delete [] buckets;
}

void FR_HashTable::deleteAllEntries()
{
	clear();
	init();
}

FR_HashEntry* FR_HashTable::createEntry(const void *key, FRBoolean &newflag)
{
	FR_HashEntry *hp;
	unsigned int index;

	index  = getHashValue(key) % numBuckets;
	if ((hp = search_bucket(buckets[index], key)) != NULL) {
		newflag = FRFalse;
		return hp;
	}

	//
	// if not found, instance a new entry
	// and append entry to chain.
	//
	hp = instanceEntry(key);
	hp->next = buckets[index];
	buckets[index] = hp;
	newflag = FRTrue;;

	entries++;
	if (entries >= rebuiltSize)
		rebuild();
	return hp;
}

FR_HashEntry* FR_HashTable::findEntry(const void *key) const
{
	unsigned int index;

	index = getHashValue(key) % numBuckets;
	return search_bucket(buckets[index], key);
}

void FR_HashTable::deleteEntry(const void *key)
{
	FR_HashEntry *hp, *prev;
	FR_HashEntry **bucketPtr;
	unsigned int index;

	index = getHashValue(key) % numBuckets;

	bucketPtr = &buckets[index];
	hp = search_bucket(*bucketPtr, key);
	if (*bucketPtr == hp) {
		*bucketPtr = hp->next;
	} else {
		for (prev = *bucketPtr; ; prev = prev->next) {
			if (prev->next == hp) {
				prev->next = hp->next;
				break;
			}
		}
	}
	entries--;
	delete hp;
}

void FR_HashTable::distribution() const
{
	printf("===============================\n");
	printf("FR_HashTable::distribution\n");
	printf("-------------------------------\n");
	int i;
	for (i = 0; i < numBuckets; i++) {
		FR_HashEntry *hp;
		int sum;

		sum = 0;
		for (hp = buckets[i]; hp != NULL; hp = hp->next)
			sum ++;
		printf("%6d total: %6d\n", i, sum);
	}
	printf("===============================\n");
}

FR_HashEntry* FR_HashTable::search_bucket(
	FR_HashEntry *bucket, const void *key) const
{
	FR_HashEntry *hp;

	for (hp = bucket; hp != NULL; hp = hp->next)
		if (compareKey(key, hp->getkey()))
			return hp;
	return NULL; // not found.
}

void FR_HashTable::rebuild()
{
	int oldsize;
	FR_HashEntry **oldhp, **oldbuckets ;

	oldsize = numBuckets;
	oldbuckets = buckets;

	numBuckets *= 4;
	rebuiltSize *= 4;
	buckets = new (FR_HashEntry *) [numBuckets];

	int i;
	for (i = 0; i < numBuckets; i++) {
		buckets[i] = NULL;
	}
	for (oldhp = oldbuckets; oldsize > 0; --oldsize, oldhp++) {
		FR_HashEntry* hp;

		for (hp = *oldhp; hp != NULL; hp = *oldhp) {
			unsigned int index;

			*oldhp = hp->next;
			index = getHashValue(hp->getkey()) % numBuckets;
			hp->next = buckets[index];
			buckets[index] = hp;
		}
	}
	delete [] oldbuckets;
}


FR_HashIterator::FR_HashIterator()
{
	tablePtr = NULL;
	entryPtr = NULL;
	index = 0;
}

void FR_HashIterator::setTable(const FR_HashTable &table)
{
	tablePtr = &table;
	searchFirstEntry();
}

void FR_HashIterator::searchFirstEntry()
{
	index = 0;
	entryPtr = *tablePtr->buckets;
	while (entryPtr == NULL) {
		if (index == (tablePtr->numBuckets - 1))
			return;
		index++;
		entryPtr = tablePtr->buckets[index];
	}
}

void FR_HashIterator::searchNextEntry()
{
	if (entryPtr != NULL)
		entryPtr = entryPtr->next;
	while (entryPtr == NULL) {
		if (index == (tablePtr->numBuckets - 1))
			return;
		index++;
		entryPtr = tablePtr->buckets[index];
	}
}
