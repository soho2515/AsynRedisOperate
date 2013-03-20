#pragma once
#include <cassert>

class NullDelegateType
{
};

const NullDelegateType NullDelegate = NullDelegateType();



//------------------------------------------------------------------
//
//
//         General delegates with non-void return values.
//
//
//------------------------------------------------------------------


template<typename RT>
class Delegate0
{
public:
	typedef RT (*FuncType)();

public:
	Delegate0()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate0( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate0( ObjType * obj, RT (ObjType::*func)() )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate0( const ObjType * obj, RT (ObjType::*func)() const )
	{
		this->Bind( obj, func );
	}

	Delegate0( RT (*func)() )
	{
		this->Bind( func );
	}

	Delegate0<RT> & operator=( RT (*func)() )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate0<RT> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate0<RT> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, RT (ObjType::*func)() )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, RT (ObjType::*func)() const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( RT (*func)() )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	RT operator()()
	{
		RT rt;
		mDispatcher( mObj, mFunc, rt );
		return rt;
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, RT & rt );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, RT & rt )
		{
			typedef RT (ObjType::*ObjFunc)();

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)();
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt )
		{
			typedef RT (ObjType::*ObjFunc)() const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)();
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt )
		{
			typedef RT (*Func)();
			Func realFunc = (Func)func;
			rt = realFunc();
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


template<typename RT, typename T1>
class Delegate1
{
public:
	typedef RT (*FuncType)( T1 );

public:
	Delegate1()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate1( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate1( ObjType * obj, RT (ObjType::*func)( T1 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate1( const ObjType * obj, RT (ObjType::*func)( T1 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate1( RT (*func)( T1 ) )
	{
		this->Bind( func );
	}

	Delegate1<RT, T1> & operator=( RT (*func)( T1 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate1<RT, T1> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate1<RT, T1> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, RT (ObjType::*func)( T1 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, RT (ObjType::*func)( T1 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( RT (*func)( T1 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	RT operator()( T1 p1 )
	{
		RT rt;
		mDispatcher( mObj, mFunc, rt, p1 );
		return rt;
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, RT & rt, T1 p1 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1 )
		{
			typedef RT (ObjType::*ObjFunc)( T1 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1 )
		{
			typedef RT (ObjType::*ObjFunc)( T1 );

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1 )
		{
			typedef RT (*Func)( T1 );
			Func realFunc = (Func)func;
			rt = realFunc( p1 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


template<typename RT, typename T1, typename T2>
class Delegate2
{
public:
	typedef RT (*FuncType)( T1, T2 );

public:
	Delegate2()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate2( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate2( ObjType * obj, RT (ObjType::*func)( T1, T2 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate2( const ObjType * obj, RT (ObjType::*func)( T1, T2 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate2( RT (*func)( T1, T2 ) )
	{
		this->Bind( func );
	}

	Delegate2<RT, T1, T2> & operator=( RT (*func)( T1, T2 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate2<RT, T1, T2> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate2<RT, T1, T2> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, RT (ObjType::*func)( T1, T2 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, RT (ObjType::*func)( T1, T2 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( RT (*func)( T1, T2 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	RT operator()( T1 p1, T2 p2 )
	{
		RT rt;
		mDispatcher( mObj, mFunc, rt, p1, p2 );
		return rt;
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, RT & rt, T1 p1, T2 p2 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2 )
		{
			typedef RT (*Func)( T1, T2 );
			Func realFunc = (Func)func;
			rt = realFunc( p1, p2 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


template<typename RT, typename T1, typename T2, typename T3>
class Delegate3
{
public:
	typedef RT (*FuncType)( T1, T2, T3 );

public:
	Delegate3()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate3( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate3( ObjType * obj, RT (ObjType::*func)( T1, T2, T3 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate3( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate3( RT (*func)( T1, T2, T3 ) )
	{
		this->Bind( func );
	}

	Delegate3<RT, T1, T2, T3> & operator=( RT (*func)( T1, T2, T3 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate3<RT, T1, T2, T3> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate3<RT, T1, T2, T3> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, RT (ObjType::*func)( T1, T2, T3 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( RT (*func)( T1, T2, T3 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	RT operator()( T1 p1, T2 p2, T3 p3 )
	{
		RT rt;
		mDispatcher( mObj, mFunc, rt, p1, p2, p3 );
		return rt;
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2, T3 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2, p3 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2, T3 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2, p3 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3 )
		{
			typedef RT (*Func)( T1, T2, T3 );
			Func realFunc = (Func)func;
			rt = realFunc( p1, p2, p3 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


template<typename RT, typename T1, typename T2, typename T3, typename T4>
class Delegate4
{
public:
	typedef RT (*FuncType)( T1, T2, T3, T4 );

public:
	Delegate4()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate4( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate4( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate4( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate4( RT (*func)( T1, T2, T3, T4 ) )
	{
		this->Bind( func );
	}

	Delegate4<RT, T1, T2, T3, T4> & operator=( RT (*func)( T1, T2, T3, T4 ) )
	{
		this->Bind( func );
		return *this;
	}

	void Bind( RT (*func)( T1, T2, T3, T4 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool operator==( const Delegate4<RT, T1, T2, T3, T4> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate4<RT, T1, T2, T3, T4> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	RT operator()( T1 p1, T2 p2, T3 p3, T4 p4 )
	{
		RT rt;
		mDispatcher( mObj, mFunc, rt, p1, p2, p3, p4 );
		return rt;
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2, T3, T4 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2, p3, p4 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2, T3, T4 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2, p3, p4 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4 )
		{
			typedef RT (*Func)( T1, T2, T3, T4 );
			Func realFunc = (Func)func;
			rt = realFunc( p1, p2, p3, p4 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


template<typename RT, typename T1, typename T2, typename T3, typename T4, typename T5>
class Delegate5
{
public:
	typedef RT (*FuncType)( T1, T2, T3, T4, T5 );

public:
	Delegate5()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate5( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate5( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate5( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate5( RT (*func)( T1, T2, T3, T4, T5 ) )
	{
		this->Bind( func );
	}

	Delegate5<RT, T1, T2, T3, T4, T5> & operator=( RT (*func)( T1, T2, T3, T4, T5 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate5<RT, T1, T2, T3, T4, T5> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate5<RT, T1, T2, T3, T4, T5> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( RT (*func)( T1, T2, T3, T4, T5 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	RT operator()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
	{
		RT rt;
		mDispatcher( mObj, mFunc, rt, p1, p2, p3, p4, p5 );
		return rt;
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2, T3, T4, T5 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2, p3, p4, p5 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2, T3, T4, T5 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2, p3, p4, p5 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
		{
			typedef RT (*Func)( T1, T2, T3, T4, T5 );
			Func realFunc = (Func)func;
			rt = realFunc( p1, p2, p3, p4, p5 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


template<typename RT, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Delegate6
{
public:
	typedef RT (*FuncType)( T1, T2, T3, T4, T5, T6 );

public:
	Delegate6()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate6( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate6( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate6( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate6( RT (*func)( T1, T2, T3, T4, T5, T6 ) )
	{
		this->Bind( func );
	}

	Delegate6<RT, T1, T2, T3, T4, T5, T6> & operator=( RT (*func)( T1, T2, T3, T4, T5, T6 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate6<RT, T1, T2, T3, T4, T5, T6> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate6<RT, T1, T2, T3, T4, T5, T6> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( RT (*func)( T1, T2, T3, T4, T5, T6 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	RT operator()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
	{
		RT rt;
		mDispatcher( mObj, mFunc, rt, p1, p2, p3, p4, p5, p6 );
		return rt;
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2, T3, T4, T5, T6 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2, p3, p4, p5, p6 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2, T3, T4, T5, T6 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2, p3, p4, p5, p6 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
		{
			typedef RT (*Func)( T1, T2, T3, T4, T5, T6 );
			Func realFunc = (Func)func;
			rt = realFunc( p1, p2, p3, p4, p5, p6 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


template<typename RT, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Delegate7
{
public:
	typedef RT (*FuncType)( T1, T2, T3, T4, T5, T6, T7 );

public:
	Delegate7()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate7( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate7( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6, T7 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate7( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6, T7 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate7( RT (*func)( T1, T2, T3, T4, T5, T6, T7 ) )
	{
		this->Bind( func );
	}

	Delegate7<RT, T1, T2, T3, T4, T5, T6, T7> & operator=( RT (*func)( T1, T2, T3, T4, T5, T6, T7 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate7<RT, T1, T2, T3, T4, T5, T6, T7> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate7<RT, T1, T2, T3, T4, T5, T6, T7> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6, T7 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6, T7 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( RT (*func)( T1, T2, T3, T4, T5, T6, T7 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	RT operator()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
	{
		RT rt;
		mDispatcher( mObj, mFunc, rt, p1, p2, p3, p4, p5, p6, p7 );
		return rt;
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2, T3, T4, T5, T6, T7 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2, p3, p4, p5, p6, p7 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
		{
			typedef RT (ObjType::*ObjFunc)( T1, T2, T3, T4, T5, T6, T7 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			rt = (realObj->*realFunc)( p1, p2, p3, p4, p5, p6, p7 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, RT & rt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
		{
			typedef RT (*Func)( T1, T2, T3, T4, T5, T6, T7 );
			Func realFunc = (Func)func;
			rt = realFunc( p1, p2, p3, p4, p5, p6, p7 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


//------------------------------------------------------------------
//
//
//       Specialization of delegate with void return values.
//
//
//------------------------------------------------------------------


template<>
class Delegate0<void>
{
public:
	typedef void (*FuncType)();

public:
	Delegate0()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate0( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate0( ObjType * obj, void (ObjType::*func)() )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate0( const ObjType * obj, void (ObjType::*func)() const )
	{
		this->Bind( obj, func );
	}

	Delegate0( void (*func)() )
	{
		this->Bind( func );
	}

	Delegate0<void> & operator=( void (*func)() )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate0<void> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate0<void> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, void (ObjType::*func)() )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, void (ObjType::*func)() const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( void (*func)() )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	void operator()()
	{
		mDispatcher( mObj, mFunc );
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func )
		{
			typedef void (ObjType::*ObjFunc)();

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)();
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func )
		{
			typedef void (ObjType::*ObjFunc)() const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)();
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func )
		{
			typedef void (*Func)();
			Func realFunc = (Func)func;
			realFunc();
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


template<typename T1>
class Delegate1<void, T1>
{
public:
	typedef void (*FuncType)( T1 );

public:
	Delegate1()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate1( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate1( ObjType * obj, void (ObjType::*func)( T1 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate1( const ObjType * obj, void (ObjType::*func)( T1 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate1( void (*func)( T1 ) )
	{
		this->Bind( func );
	}

	Delegate1<void, T1> & operator=( void (*func)( T1 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate1<void, T1> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate1<void, T1> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, void (ObjType::*func)( T1 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, void (ObjType::*func)( T1 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( void (*func)( T1 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	void operator()( T1 p1 )
	{
		mDispatcher( mObj, mFunc, p1 );
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, T1 p1 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, T1 p1 )
		{
			typedef void (ObjType::*ObjFunc)( T1 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1 )
		{
			typedef void (ObjType::*ObjFunc)( T1 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1 )
		{
			typedef void (*Func)( T1 );
			Func realFunc = (Func)func;
			realFunc( p1 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


template<typename T1, typename T2>
class Delegate2<void, T1, T2>
{
public:
	typedef void (*FuncType)( T1, T2 );

public:
	Delegate2()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate2( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate2( ObjType * obj, void (ObjType::*func)( T1, T2 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate2( const ObjType * obj, void (ObjType::*func)( T1, T2 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate2( void (*func)( T1, T2 ) )
	{
		this->Bind( func );
	}

	Delegate2<void, T1, T2> & operator=( void (*func)( T1, T2 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate2<void, T1, T2> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate2<void, T1, T2> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, void (ObjType::*func)( T1, T2 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, void (ObjType::*func)( T1, T2 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( void (*func)( T1, T2 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	void operator()( T1 p1, T2 p2 )
	{
		mDispatcher( mObj, mFunc, p1, p2 );
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, T1 p1, T2 p2 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2 )
		{
			typedef void (*Func)( T1, T2 );
			Func realFunc = (Func)func;
			realFunc( p1, p2 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};



template<typename T1, typename T2, typename T3>
class Delegate3<void, T1, T2, T3>
{
public:
	typedef void (*FuncType)( T1, T2, T3 );

public:
	Delegate3()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate3( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate3( ObjType * obj, void (ObjType::*func)( T1, T2, T3 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate3( const ObjType * obj, void (ObjType::*func)( T1, T2, T3 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate3( void (*func)( T1, T2, T3 ) )
	{
		this->Bind( func );
	}

	Delegate3<void, T1, T2, T3> & operator=( void (*func)( T1, T2, T3 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate3<void, T1, T2, T3> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate3<void, T1, T2, T3> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, void (ObjType::*func)( T1, T2, T3 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, void (ObjType::*func)( T1, T2, T3 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( void (*func)( T1, T2, T3 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	void operator()( T1 p1, T2 p2, T3 p3 )
	{
		mDispatcher( mObj, mFunc, p1, p2, p3 );
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, T1 p1, T2 p2, T3 p3 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2, T3 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2, p3 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2, T3 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2, p3 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3 )
		{
			typedef void (*Func)( T1, T2, T3 );
			Func realFunc = (Func)func;
			realFunc( p1, p2, p3 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};



template<typename T1, typename T2, typename T3, typename T4>
class Delegate4<void, T1, T2, T3, T4>
{
public:
	typedef void (*FuncType)( T1, T2, T3, T4 );

public:
	Delegate4()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate4( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate4( ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate4( const ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate4( void (*func)( T1, T2, T3, T4 ) )
	{
		this->Bind( func );
	}

	Delegate4<void, T1, T2, T3, T4> & operator=( void (*func)( T1, T2, T3, T4 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate4<void, T1, T2, T3, T4> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate4<void, T1, T2, T3, T4> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( void (*func)( T1, T2, T3, T4 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	void operator()( T1 p1, T2 p2, T3 p3, T4 p4 )
	{
		mDispatcher( mObj, mFunc, p1, p2, p3, p4 );
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2, T3, T4 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2, p3, p4 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2, T3, T4 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2, p3, p4 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4 )
		{
			typedef void (*Func)( T1, T2, T3, T4 );
			Func realFunc = (Func)func;
			realFunc( p1, p2, p3, p4 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};



template<typename T1, typename T2, typename T3, typename T4, typename T5>
class Delegate5<void, T1, T2, T3, T4, T5>
{
public:
	typedef void (*FuncType)( T1, T2, T3, T4, T5 );

public:
	Delegate5()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate5( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate5( ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate5( const ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate5( void (*func)( T1, T2, T3, T4, T5 ) )
	{
		this->Bind( func );
	}

	Delegate5<void, T1, T2, T3, T4, T5> & operator=( void (*func)( T1, T2, T3, T4, T5 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate5<void, T1, T2, T3, T4, T5> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate5<void, T1, T2, T3, T4, T5> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( void (*func)( T1, T2, T3, T4, T5 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	void operator()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
	{
		mDispatcher( mObj, mFunc, p1, p2, p3, p4, p5 );
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2, T3, T4, T5 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2, p3, p4, p5 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2, T3, T4, T5 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2, p3, p4, p5 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
		{
			typedef void (*Func)( T1, T2, T3, T4, T5 );
			Func realFunc = (Func)func;
			realFunc( p1, p2, p3, p4, p5 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};



template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Delegate6<void, T1, T2, T3, T4, T5, T6>
{
public:
	typedef void (*FuncType)( T1, T2, T3, T4, T5, T6 );

public:
	Delegate6()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate6( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate6( ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5, T6 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate6( const ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5, T6 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate6( void (*func)( T1, T2, T3, T4, T5, T6 ) )
	{
		this->Bind( func );
	}

	Delegate6<void, T1, T2, T3, T4, T5, T6> & operator=( void (*func)( T1, T2, T3, T4, T5, T6 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate6<void, T1, T2, T3, T4, T5, T6> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate6<void, T1, T2, T3, T4, T5, T6> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5, T6 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5, T6 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( void (*func)( T1, T2, T3, T4, T5, T6 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	void operator()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
	{
		mDispatcher( mObj, mFunc, p1, p2, p3, p4, p5, p6 );
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2, T3, T4, T5, T6 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2, p3, p4, p5, p6 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2, T3, T4, T5, T6 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2, p3, p4, p5, p6 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
		{
			typedef void (*Func)( T1, T2, T3, T4, T5, T6 );
			Func realFunc = (Func)func;
			realFunc( p1, p2, p3, p4, p5, p6 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};



template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Delegate7<void, T1, T2, T3, T4, T5, T6, T7>
{
public:
	typedef void (*FuncType)( T1, T2, T3, T4, T5, T6, T7 );

public:
	Delegate7()
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	Delegate7( NullDelegateType )
		: mDispatcher( NULL )
		, mFunc( NULL )
		, mObj( NULL )
	{
	}

	template<typename ObjType>
	Delegate7( ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5, T6, T7 ) )
	{
		this->Bind( obj, func );
	}

	template<typename ObjType>
	Delegate7( const ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5, T6, T7 ) const )
	{
		this->Bind( obj, func );
	}

	Delegate7( void (*func)( T1, T2, T3, T4, T5, T6, T7 ) )
	{
		this->Bind( func );
	}

	Delegate7<void, T1, T2, T3, T4, T5, T6, T7> & operator=( void (*func)( T1, T2, T3, T4, T5, T6, T7 ) )
	{
		this->Bind( func );
		return *this;
	}

	bool operator==( const Delegate7<void, T1, T2, T3, T4, T5, T6, T7> & rhs )
	{
		return mObj == rhs.mObj && mFunc == rhs.mFunc;
	}

	bool operator!=( const Delegate7<void, T1, T2, T3, T4, T5, T6, T7> & rhs )
	{
		return ! operator==( rhs );
	}

	template<typename ObjType>
	void Bind( ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5, T6, T7 ) )
	{
		assert( obj );
		assert( func );

		mDispatcher = Helper<ObjType>::Dispatcher;
		mObj = obj;
		mFunc = *(void**)( &func );
	}

	template<typename ObjType>
	void Bind( const ObjType * obj, void (ObjType::*func)( T1, T2, T3, T4, T5, T6, T7 ) const )
	{
		assert( obj );
		assert( func );

		mDispatcher = ConstHelper<ObjType>::Dispatcher;
		mObj = (void*)obj;
		mFunc = *(void**)( &func );
	}

	void Bind( void (*func)( T1, T2, T3, T4, T5, T6, T7 ) )
	{
		assert( func );

		mDispatcher = NonMemberHelper::Dispatcher;
		mObj = NULL;
		mFunc = reinterpret_cast<void*>( func );
	}

	bool IsBound() const
	{
		return mDispatcher != NULL;
	}

	void operator()( T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
	{
		mDispatcher( mObj, mFunc, p1, p2, p3, p4, p5, p6, p7 );
	}

private:
	typedef void (*DispatcherFunc)( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 );

	template<typename ObjType>
	struct Helper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2, T3, T4, T5, T6, T7 );

			ObjType * realObj = static_cast<ObjType*>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2, p3, p4, p5, p6, p7 );
		}
	};

	template<typename ObjType>
	struct ConstHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
		{
			typedef void (ObjType::*ObjFunc)( T1, T2, T3, T4, T5, T6, T7 ) const;

			const ObjType * realObj = static_cast<const ObjType *>( obj );
			ObjFunc realFunc = *(ObjFunc*)( &func );
			(realObj->*realFunc)( p1, p2, p3, p4, p5, p6, p7 );
		}
	};

	struct NonMemberHelper
	{
		static void Dispatcher( void * obj, void * func, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
		{
			typedef void (*Func)( T1, T2, T3, T4, T5, T6, T7 );
			Func realFunc = (Func)func;
			realFunc( p1, p2, p3, p4, p5, p6, p7 );
		}
	};

	DispatcherFunc mDispatcher;
	void * mFunc;
	void * mObj;
};


//------------------------------------------------------------------
//
//
//                    MakeDelegate functions.
//
//
//------------------------------------------------------------------


template<typename ObjType, typename RT>
Delegate0<RT> MakeDelegate( ObjType * obj, RT (ObjType::*func)() )
{
	assert( obj );
	assert( func );

	Delegate0<RT> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1>
Delegate1<RT, T1> MakeDelegate( ObjType * obj, RT (ObjType::*func)( T1 ) )
{
	assert( obj );
	assert( func );

	Delegate1<RT, T1> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1, typename T2>
Delegate2<RT, T1, T2> MakeDelegate( ObjType * obj, RT (ObjType::*func)( T1, T2 ) )
{
	assert( obj );
	assert( func );

	Delegate2<RT, T1, T2> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1, typename T2, typename T3>
Delegate3<RT, T1, T2, T3> MakeDelegate( ObjType * obj, RT (ObjType::*func)( T1, T2, T3 ) )
{
	assert( obj );
	assert( func );

	Delegate3<RT, T1, T2, T3> ret;
	ret.Bind( obj, func );
	return ret;
}

template<typename ObjType, typename RT, typename T1, typename T2, typename T3, typename T4>
Delegate4<RT, T1, T2, T3, T4> MakeDelegate( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4 ) )
{
	assert( obj );
	assert( func );

	Delegate4<RT, T1, T2, T3, T4> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1, typename T2, typename T3, typename T4, typename T5>
Delegate5<RT, T1, T2, T3, T4, T5> MakeDelegate( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5 ) )
{
	assert( obj );
	assert( func );

	Delegate5<RT, T1, T2, T3, T4, T5> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
Delegate6<RT, T1, T2, T3, T4, T5, T6> MakeDelegate( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6 ) )
{
	assert( obj );
	assert( func );

	Delegate6<RT, T1, T2, T3, T4, T5, T6> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
Delegate7<RT, T1, T2, T3, T4, T5, T6, T7> MakeDelegate( ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6, T7 ) )
{
	assert( obj );
	assert( func );

	Delegate7<RT, T1, T2, T3, T4, T5, T6, T7> ret;
	ret.Bind( obj, func );
	return ret;
}




template<typename ObjType, typename RT>
Delegate0<RT> MakeDelegate( const ObjType * obj, RT (ObjType::*func)() const )
{
	assert( obj );
	assert( func );

	Delegate0<RT> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1>
Delegate1<RT, T1> MakeDelegate( const ObjType * obj, RT (ObjType::*func)( T1 ) const )
{
	assert( obj );
	assert( func );

	Delegate1<RT, T1> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1, typename T2>
Delegate2<RT, T1, T2> MakeDelegate( const ObjType * obj, RT (ObjType::*func)( T1, T2 ) const )
{
	assert( obj );
	assert( func );

	Delegate2<RT, T1, T2> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1, typename T2, typename T3>
Delegate3<RT, T1, T2, T3> MakeDelegate( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3 ) const )
{
	assert( obj );
	assert( func );

	Delegate3<RT, T1, T2, T3> ret;
	ret.Bind( obj, func );
	return ret;
}

template<typename ObjType, typename RT, typename T1, typename T2, typename T3, typename T4>
Delegate4<RT, T1, T2, T3, T4> MakeDelegate( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4 ) const )
{
	assert( obj );
	assert( func );

	Delegate4<RT, T1, T2, T3, T4> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1, typename T2, typename T3, typename T4, typename T5>
Delegate5<RT, T1, T2, T3, T4, T5> MakeDelegate( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5 ) const )
{
	assert( obj );
	assert( func );

	Delegate5<RT, T1, T2, T3, T4, T5> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
Delegate6<RT, T1, T2, T3, T4, T5, T6> MakeDelegate( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6 ) const )
{
	assert( obj );
	assert( func );

	Delegate6<RT, T1, T2, T3, T4, T5, T6> ret;
	ret.Bind( obj, func );
	return ret;
}


template<typename ObjType, typename RT, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
Delegate7<RT, T1, T2, T3, T4, T5, T6, T7> MakeDelegate( const ObjType * obj, RT (ObjType::*func)( T1, T2, T3, T4, T5, T6, T7 ) const )
{
	assert( obj );
	assert( func );

	Delegate7<RT, T1, T2, T3, T4, T5, T6, T7> ret;
	ret.Bind( obj, func );
	return ret;
}