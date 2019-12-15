#pragma once

template<typename T>
class Vector
{
	T* mData; 
	unsigned int mSize; 
	unsigned int mCapacity;
	// *** mCapacity = 15 *** For tests.
	T mUndefined;

public:

	class Iterator
	{
		Vector* mVectorPtr;
		int mIndex;
		T mUndefined;

	public:
		Iterator(Vector& tVec, int tPos)
		{
			mVectorPtr = &tVec;
			if (tPos > mVectorPtr->Size()) {
				tPos = mVectorPtr->Size();
			}
			if (tPos < 0) {
				tPos = 0;
			}
			mIndex = tPos;
		}
		T& GetData()
		{
			if (mIndex < 0 || mIndex >= (mVectorPtr->Size())) return mUndefined;

			return mVectorPtr->At(mIndex);
		}
		void Next()// As in "Move to the next item please"
		{
			if (mIndex > mVectorPtr->Size()) return;
			else mIndex++;
		}
		void Back() 
		{
			if (mIndex <= 0) return;
			else mIndex--;
		}
		bool IsEqual(const Iterator& tRHS)
		{
			return (mVectorPtr == tRHS.mVectorPtr && mIndex == tRHS.mIndex);
		}
		int GetIndex() 
		{
			return mIndex;
		}
		bool operator == (const Iterator& tRHS) 
		{
			return IsEqual(tRHS);
		}
		bool operator != (const Iterator& tRHS) 
		{
			return !(IsEqual(tRHS));
		}
	};

	//Default constructor
	//O(1)
	Vector( )
	{
		mSize = 0;
		mData = nullptr;
		Reserve( 15 );
	}

	//Destructor
	~Vector( )
	{
		delete[] mData;
	}

	/*
	Copy Constructor
	@PARAM tOther - const ref to existing Vector

	Replicates an existing vector in every way.
	Makes a copy of the internal data array and
	points mData to that.
	O(n)
	*/
	Vector( const Vector<T>& tOther ) : Vector( )
	{
		*this = tOther; 
		//uses assignment operator to avoid code duplication
	}

	/*
	Assignment Operator
	@PARAM tRHS - const ref to existing vector
	(On the right hand side of the assignment operator)

	Similar function to the copy constructor, however only
	called by an instantiated object. Must take care of existing
	data array to prevent memory leak. 

	O(n)
	*/
	Vector& operator =( const Vector<T>& tRHS )
	{
		if (this != &tRHS)//selfcopy bad
		{
			mSize = 0;	//set mSize = 0 before calling Reserve()
						//avoids unneccessary copying in Reserve()
			Reserve(tRHS.mCapacity);
			mSize = tRHS.mSize;
			for (int i = 0; i < mSize; i++)
			{
				mData[i] = tRHS.mData[i];
			}
		}
		return *this;
	}


	/*
	**Replaced function and classnames as appropriate for this situation**

	"Effective C++ 3rd Edition" Pages 19-26

	"When const and non-const member functions have 
	essentially identical implementations, code duplication 
	can be avoided by having the non-const version 
	call the const version." -Page 26

	"As you can see, the code has two casts, not one. We want the nonconst 
	operator[] to call the const one, but if, inside the non-const 
	operator[], we just call operator[], we’ll recursively call ourselves. That’s
	only entertaining the first million or so times. To avoid infinite recursion, 
	we have to specify that we want to call the const operator[], but
	there’s no direct way to do that. Instead, we cast *this from its native
	type of Vector& to const Vector&. Yes, we use a cast to add const!
	So we have two casts: one to add const to *this (so that our call to
	operator[] will call the const version), the second to remove the const
	from the const operator[]’s return value." -Page 25

	Outstanding book so far.

	*/
	
	const T& operator [](int tWhere) const 
	{
		if (tWhere >= mSize || tWhere < 0)
		{
			return mUndefined;
		}
		return mData[tWhere];
	}

	T& operator [](int tWhere) {
		return const_cast<T&>
			(
				static_cast<const Vector&>(*this)[tWhere]
			);
	}

	/*
	Function - PushBack
	@PARAM tItem - const ref of data to add

	takes the parameter and adds a copy of it to the end of the vector.
	always resizes and reserves additional memory as appropriate.

	O(1) - sometimes O(n) if Reserve called
	*/
	void PushBack( const T& tItem )
	{
		if (mSize >= mCapacity) 
		{ 
			Reserve(mCapacity * 2);
		}

		mData[mSize] = tItem;
		mSize++;
	}

	/*
	Function - PopBack
	NO PARAMS

	Delete the last element in the vector. Always resizes the vector and never reserves
	additional memory.

	returns void

	O(1)
	*/
	void PopBack( )
	{
		mData[mSize - 1] = mUndefined; //Not required. Makes me feel more comfortable overwriting the memory.
		mSize--;
	}

	/*
	Function - At
	@PARAM tWhere - integer index of requested element

	returns the tWhere'th element of the vector.

	O(1)
	*/
	T& At( int tWhere )
	{
		if ( tWhere >= 0 && tWhere < mSize )
		{
			return mData[tWhere];
		}
		else return mUndefined;
	}

	/*
	Function - Clear
	NO PARAMS

	clears the array

	O(1)
	*/
	void Clear( )
	{
		mSize = 0;

		/*	
			Still works like this. It seems like a poor choice to leave data
			hanging around in the array when it's supposedly been cleared though.
			Whatevs. I'm cognizant of it. See PopBack()
		*/
	}

	/*
	Function - Size
	NO PARAMS

	returns the integer number of elements in the vector

	O(1)
	*/
	int Size( )
	{
		return mSize;
	}

	/*
	Function - Reserve
	@PARAM int tCount - Size array to create

	create new array[tCount]. Adjust mCapcity.
	If mData is nullptr - we're done
	else copy the data, clean up, and point mData to new array

	O(n)
	*/
	void Reserve( int tCount )
	{
		T* tTempData = new T[tCount];
		mCapacity = tCount;
		
		if ( mData == nullptr )
		{
			mData = tTempData;
			return;
		}
		
		else
		{
			for (int i = 0; i < mSize; i++)//Copy data
			{
				*(tTempData + i) = *(mData + i);
				//	Equivalent to
				//	tTempData[i] = mData[i];
				//	Just familiarizing myself 
				//	with pointer math.
			}
			delete[] mData;
			mData = tTempData;

			return;
		}
		
	}

	/*
	Function - Capacity
	NO PARAMS

	returns the integer capacity of the vector.

	O(1)
	*/
	int Capacity( )
	{
		return mCapacity;
	}


	Iterator Insert(Iterator &tIt, const T& tDataToAdd) {
		PushBack(mData[mSize - 1]);
		for (int i = mSize - 2; i >= tIt.GetIndex(); i--) {
			mData[i + 1] = mData[i];
		}
		mData[tIt.GetIndex()] = tDataToAdd;
		tIt.Next();
		return tIt;
	}

	Iterator Erase(Iterator &tIt) {
		for (int i = tIt.GetIndex(); i < mSize-1; i++) {
			mData[i] = mData[i+1];
		}
		PopBack();
		return tIt;
	}
	
	Iterator Begin() {
		return Iterator(*this, 0);
	}

	Iterator End() {
		return Iterator(*this, mSize);
	}
};