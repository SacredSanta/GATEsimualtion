////////////////////
//// FRString.h ////
////////////////////

#if !defined FR_STRING_H
#define FR_STRING_H

#include <std/stddef.h>
#include <string.h>
#include <iostream.h>
#include "FRBoolean.h"

class FRString
{
	protected:
		static char mDummy;
		
		char* mString;
		size_t mSize;
		size_t mLength;
		FRBoolean mLock;

		FRBoolean resize(size_t newSize, FRBoolean keepflag = FRTrue);

		inline int comparefunc(const char *str, size_t len) const {
			if(len == NPOS)
				return strcmp(mString, str);
			else
				return strncmp(mString, str, len);
		}

	public:
		FRString();

		FRString(const FRString& str);

		FRString(const char* str);

		FRString(char c);

		virtual ~FRString()
		{	delete [] mString; }

		size_t length() const { return mLength;	}

		FRString& copy(const FRString& str, size_t n = NPOS);
		FRString& copy(const char* str, size_t n = NPOS);
		FRString& copy(char c);
		
		int compare(const FRString& str, size_t n = NPOS) const;
		int compare(const char* str, size_t n = NPOS) const;
		int compare(char str) const;
		
		FRString& cat(const FRString& str);
		FRString& cat(const char* str);
		FRString& cat(char c);

		int find(const FRString& str, size_t n = 0) const;
		int find(const char* str, size_t n = 0) const;
		int find(char c, size_t n = 0) const;

		operator const char*() const { return mString; }

		char * GetBuffer(size_t newSize, FRBoolean keepflag = FRTrue);
		FRBoolean ReleaseBuffer();
		size_t size() const { return mSize; }

		FRString extract(size_t start_index, size_t len = NPOS) const;

		char operator[](size_t index) const
		{ return index >= mLength ? '\0' : mString[index] ; }
		char& operator[](size_t index)
		{ return index >= mLength ? mDummy : mString[index] ; }

		FRString& operator=(const FRString& str){	return copy(str.mString); }
		FRString& operator=(const char* str){	return copy(str); }
		FRString& operator=(char c){	return copy(c); }
		FRString& operator+=(const FRString& str) {	return cat(str);	}
		FRString& operator+=(const char* str) {	return cat(str);	}
		FRString& operator+=(char c) {	return cat(c);	}
};

inline FRString operator+(const FRString& L, const FRString& R)
{	return FRString(L)+=R; }

inline FRString operator+(const char* L, const FRString& R)
{	return FRString(L)+=R; }

inline FRString operator+(const FRString& L, const char* R)
{	return FRString(L)+=R; }

inline FRString operator+(const FRString& L, char R)
{	return FRString(L)+=R; }

inline FRString operator+(char L, const FRString& R)
{	return FRString(L)+=R; }

// operator ==
inline FRBoolean operator==(const FRString& L, const FRString& R)
{	return L.compare(R) == 0; }

inline FRBoolean operator==(const char* L, const FRString& R)
{	return R.compare(L) == 0; }

inline FRBoolean operator==(const FRString& L, const char* R)
{	return L.compare(R) == 0; }

inline FRBoolean operator==(char L, const FRString& R)
{	return R.compare(L) == 0; }

inline FRBoolean operator==(const FRString& L, char R)
{	return L.compare(R) == 0; }


// operator !=
inline FRBoolean operator!=(const FRString& L, const FRString& R)
{	return L.compare(R) != 0; }

inline FRBoolean operator!=(const char* L, const FRString& R)
{	return R.compare(L) != 0; }

inline FRBoolean operator!=(const FRString& L, const char* R)
{	return L.compare(R) != 0; }

inline FRBoolean operator!=(char L, const FRString& R)
{	return R.compare(L) != 0; }

inline FRBoolean operator!=(const FRString& L, char R)
{	return L.compare(R) != 0; }


// operator >
inline FRBoolean operator>(const FRString& L, const FRString& R)
{	return L.compare(R) > 0; }

inline FRBoolean operator>(const char* L, const FRString& R)
{	return R.compare(L) < 0; }

inline FRBoolean operator>(const FRString& L, const char* R)
{	return L.compare(R) > 0; }

inline FRBoolean operator>(char L, const FRString& R)
{	return R.compare(L) < 0; }

inline FRBoolean operator>(const FRString& L, char R)
{	return L.compare(R) > 0; }

// operator <
inline FRBoolean operator<(const FRString& L, const FRString& R)
{	return L.compare(R) < 0; }

inline FRBoolean operator<(const char* L, const FRString& R)
{	return R.compare(L) > 0; }

inline FRBoolean operator<(const FRString& L, const char* R)
{	return L.compare(R) < 0; }

inline FRBoolean operator<(char L, const FRString& R)
{	return R.compare(L) > 0; }

inline FRBoolean operator<(const FRString& L, char R)
{	return L.compare(R) < 0; }

// operator >=
inline FRBoolean operator>=(const FRString& L, const FRString& R)
{	return L.compare(R) >= 0; }

inline FRBoolean operator>=(const char* L, const FRString& R)
{	return R.compare(L) <= 0; }

inline FRBoolean operator>=(const FRString& L, const char* R)
{	return L.compare(R) >= 0; }

inline FRBoolean operator>=(char L, const FRString& R)
{	return R.compare(L) <= 0; }

inline FRBoolean operator>=(const FRString& L, char R)
{	return L.compare(R) >= 0; }

// operator <=
inline FRBoolean operator<=(const FRString& L, const FRString& R)
{	return L.compare(R) <= 0; }

inline FRBoolean operator<=(const char* L, const FRString& R)
{	return R.compare(L) >= 0; }

inline FRBoolean operator<=(const FRString& L, const char* R)
{	return L.compare(R) <= 0; }

inline FRBoolean operator<=(char L, const FRString& R)
{	return R.compare(L) >= 0; }

inline FRBoolean operator<=(const FRString& L, char R)
{	return L.compare(R) <= 0; }

#endif
