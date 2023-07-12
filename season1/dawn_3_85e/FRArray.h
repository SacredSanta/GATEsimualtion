#ifndef	FRARRAY_H
#define	FRARRAY_H

#include "FRBoolean.h"
#include <stddef.h>

template <class T>
class FRArray
{
public:
	FRArray();
	FRArray(size_t);
	FRArray(const T&, size_t);
	FRArray(const T*, size_t);
	FRArray(const FRArray<T>&);
	virtual ~FRArray();

	FRArray<T>& assign(const T&, size_t = 1);
	FRArray<T>& assign(const T*, size_t = 1);
	FRArray<T>& assign(const FRArray<T>& );

	size_t size() const	{ return mSize; }
	void resize(size_t n) { mAlloc(n, FRTrue); }
	void alloc(size_t n)  { mAlloc(n, FRFalse); }

//	void		operator()(size_t n)		{ resize(n); }

	FRArray<T>& operator=(const FRArray<T>& X) ;

	operator const T* () const { return mPtr; }
	operator T* () { return mPtr; }

	const T& operator[](size_t pos) const { return mPtr[pos]; }
	T& operator[](size_t pos) { return mPtr[pos]; }
protected:
	void init(FRBoolean);
	void mAlloc(size_t, FRBoolean = FRFalse);
	T*	mPtr;
	size_t	mSize;

}; // FRArray


template <class T>
inline void FRArray<T>::init(FRBoolean constructed)
{
	if(constructed && mPtr != 0)
		delete [] mPtr;
	mPtr = 0;
	mSize = 0;
}

template <class T>
inline void FRArray<T>::mAlloc(size_t n, FRBoolean saved)
{
	if(n == 0)
		init(1);
	else if(n == mSize)
		;	// nothing to do.
	else
	{	T* p = new T [n];
		if(mPtr != 0)
		{	if(saved)
			{	size_t m = mSize < n ? mSize : n;
				size_t i;
				T *src, *dst;
				for(src=mPtr, dst=p, i=0; i<m; i++)
					*dst++ = *src++;
			}
			delete [] mPtr;
		}
		mPtr = p;
		mSize = n;
	}
}


template <class T>
inline FRArray<T>& FRArray<T>::assign(const T& t, size_t n)
{
	size_t i;
	T* dst;

	mAlloc(n);
	for(i=0, dst=mPtr; i<n; i++)
		*dst++ = t;
	return *this;
}


template <class T>
inline FRArray<T>& FRArray<T>::assign(const T *src, size_t n)
{
	size_t i;
	T* dst;

	mAlloc(n);
	for(i=0, dst=mPtr; i<n; i++)
		*dst++ = *src++;
	return *this;
}


template <class T>
inline FRArray<T>& FRArray<T>::assign(const FRArray<T>& X)
{
	if(this != &X)
	{	mAlloc(X.mSize);
		assign(X.mPtr, X.mSize);
	}
	return *this;
}

template <class T>
inline FRArray<T>::FRArray()
	{ init(0); }
template <class T>
inline FRArray<T>::FRArray(size_t n)
	{ init(0); mAlloc(n); }
template <class T>
inline FRArray<T>::FRArray(const T& src, size_t n)
	{ init(0); assign(src, n); }
template <class T>
inline FRArray<T>::FRArray(const T* src, size_t n)
	{ init(0); assign(src, n); }
template <class T>
inline FRArray<T>::FRArray(const FRArray<T>& X)
	{ init(0); assign(X); }
template <class T>
inline FRArray<T>::~FRArray()
	{ init(1); }

template <class T>
inline FRArray<T>& FRArray<T>::operator=(const FRArray<T>& X) 
	{ return assign(X); }

#endif
