#include <assert.h>
#include "FRString.h"

char FRString::mDummy = '\0';

FRString::FRString()
{
	mLock = FRFalse; mSize = 0; mString = 0;
	copy( '\0' );
}

FRString::FRString(const FRString& str)
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!str.mLock);
	
	mLock = FRFalse; mSize = 0; mString = 0;
	copy(str.mString);
}

FRString::FRString(const char* str)
{
	mLock = FRFalse; mSize = 0; mString = 0;
	copy(str);
}

FRString::FRString(char c)
{
	mLock = FRFalse; mSize = 0; mString = 0;
	copy(c);
}

FRBoolean FRString::resize(size_t newSize, FRBoolean keepflag = FRTrue)
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!mLock);

	if (newSize == 0){
		assert(0);//	Please implement error routine.
		//	cout << "Failed size\'0\' memory allocation!" << endl;
		return FRFalse;
	}
	if (mSize == newSize) return FRTrue;
	
	char* newString = new char [newSize];
	if (newString == 0){
		assert(0);//	Please implement error routine.
		//	cout << "Failed memory allocation!" << endl;
		return FRFalse;
	}

	if (mString && keepflag){
		if (mSize > newSize) mLength = newSize - 1;
		strncpy(newString, mString, mLength);
		newString[mLength] = '\0';
		delete [] mString;
	} else {
		delete [] mString;
		newString[0] = '\0';
		mLength = 0;
	}
	mString = newString;
	mSize = newSize;
	
	return FRTrue;
}

char* FRString::GetBuffer(size_t newSize, FRBoolean keepflag = FRTrue)
{
	if (resize( newSize, keepflag)){
		mLock = FRTrue;
		return mString;
	} else {
		// Error!  Failed memory allocation.
		assert(0);//	Please implement error routine.
		return 0;
	}
}

FRBoolean FRString::ReleaseBuffer() // trimming buffer and release
{
	mLock = FRFalse; // release.
	size_t len = strlen(mString);
	
	if (len >= mSize) {
		// Error!  Because using buffer is longer than allocate memories.
		assert(0);//	Please implement error routine.
		return FRFalse;
	}
	mLength = len;
	return resize(mLength + 1, FRTrue);
}

// copy
FRString& FRString::copy(const FRString& str, size_t len = NPOS)
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!str.mLock);
	assert(!mLock);

	return copy(str.mString, len);
}

FRString& FRString::copy(const char* str, size_t len = NPOS)
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!mLock);

	if (!str){
		mLength = len;
		resize(0);
	} else {
		size_t slen = strlen(str);
		if (len > slen) len = slen;
		if (len + 1 > mSize)
			if (!resize(len + 1, FRFalse)) return *this; // return failed.

		strncpy(mString, str, len);
		mString[len] = '\0';
		mLength = len;
	}
	return *this;
}

FRString& FRString::copy(char c)
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!mLock);

	if (c == '\0'){
		if (mSize < 1) resize(1, FRFalse);
		mLength = 0;
		mString[0] = '\0';
	} else {
		if (mSize < 2) resize(2, FRFalse);
		mLength = 1;
		mString[0] = c;
		mString[1] = '\0';
	}
	return *this;
}

// compare
int FRString::compare(const FRString& str, size_t len) const
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!str.mLock);
	assert(!mLock);

	return comparefunc(str.mString, len);
}

int FRString::compare(const char* str, size_t len) const
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!mLock);

	return comparefunc(str, len);
}

int FRString::compare(char c) const
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!mLock);

	char tmp[2];
	tmp[0] = c;
	tmp[1] = '\0';
	return comparefunc(tmp, NPOS);
}

FRString& FRString::cat(const FRString& str)
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!str.mLock);
	assert(!mLock);

	return cat( str.mString );
}

FRString& FRString::cat(const char* str)
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!mLock);

	size_t slen;

	if (!str) return *this;
	
	slen = strlen(str);

	if (mLength + slen + 1 > mSize)
		if (!resize(mLength + slen + 1, FRTrue)) return *this;	// return failed

	mLength += slen;
	strcat(mString, str);

	return *this;
}

FRString& FRString::cat(char c)
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!mLock);

	if (mLength + 2 > mSize){
		if (!resize( mLength + 2 )) return *this;	// return failed
	}
	
	mString[mLength] = c;
	mLength++;
	mString[mLength] = '\0';

	return *this;
}

int FRString::find(const FRString& str, size_t n = 0) const
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!str.mLock);
	assert(!mLock);

	return find(str.mString, n);
}

int FRString::find(const char* str, size_t n = 0) const
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!mLock);

	size_t co;

	if (!str || str[0] == '\0') return -1;
	
	for (co = 0; mString[co]; co++){
		if (strncmp(str, &mString[co], strlen(str)) == 0){
			if (n-- <= 0) return co;
		}
	}
	return -1; // not found.
}

int FRString::find(const char c, size_t n = 0) const
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!mLock);

	size_t i;
	
	for (i = 0; i < mLength; i++){
		if (mString[ i ] == c){
			if (n-- <= 0) return i;
		}
	}
	return -1; // not found.
}

FRString FRString::extract(size_t start, size_t len = NPOS) const
{
	//   If you called GetBuffer(), don't call this function
	// until you call ReleaseBuffer().
	assert(!mLock);
	
	if (start >= mLength) return FRString();
	if (len + start > mLength)
		len = mLength > start ? mLength - start : 0 ;

	FRString tmp;

	return tmp.copy(&mString[start], len);
}
