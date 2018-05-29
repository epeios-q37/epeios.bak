/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

// Native 4(for) Java Runtime Environment
// Extension of the 'N4ALL' library for use with Java.
// Common part between wrapper and component ('scln4a').

#ifndef N4JRE_INC_
# define N4JRE_INC_

# define N4JRE_NAME		"N4JRE"

# if defined( E_DEBUG ) && !defined( N4JRE_NODBG )
#  define N4JRE_DBG
# endif

# include "n4all.h"
# include "err.h"
# include "tol.h"

# ifdef H
#  define N4JRE_H_ H
#  undef H
# endif

namespace n4jre {
	// Mimics the 'Java' headers.
	typedef unsigned char sJBoolean;
	typedef bso::sS8 sJByte;
	typedef bso::sS16 sJShort;
	typedef bso::sS32 sJInt;
	typedef bso::sS64 sJLong;

	typedef long sJSize;
	qCDEF( long, SizeMax, LONG_MAX );

	qENUM( Handling )
	{
		hCopy,		// Data are copied.
		hOriginal,	// Data are referenced.
		h_amount,
		h_Undefined
	};

	typedef void *( *fMalloc )( size_t Size );
	typedef void( *fFree )( void *Pointer );

	// Both below function have to be defined by user.
	extern fMalloc N4JREMalloc;
	extern fFree N4JREFree;

	// Only for primitive types.
	template <typename type> class rJArray_
	{
	private:
		long Size_;	// If < 0, 'Core_' is a reference and should not be freed.
		union {
			type *V;
			const type *C;
		} Core_;
		// Should be optimized, by using 'realloc()'.
		void Init_( long Size )
		{
			reset();

			if ( Size != 0 ) {
				Core_.V = (type *)N4JREMalloc( Size * sizeof( type ) );
				if ( Core_.V == NULL )
					qRAlc();
			}
		}
	public:
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( ( Core_.V != NULL ) && ( Size_ > 0 ) )
					N4JREFree( Core_.V );
			}

			Core_.V = NULL;
			Size_ = 0;
		}
		qCDTOR( rJArray_ );
		void Init( void )
		{
			reset();
		}
		void Init(
			long Size,
			const type *Buffer,
			eHandling Handling )
		{
			reset();

			if ( Size != 0 ) {
				if ( Buffer == NULL )
					qRFwk();

				switch ( Handling ) {
				case hCopy:
					Init_( Size );
					memcpy( Core_.V, Buffer, sizeof( type ) * Size );
					Size_ = Size;
					break;
				case hOriginal:
					Core_.C = Buffer;
					Size_ = -Size;
					break;
				default:
					qRFwk();
					break;
				}
			}
		}
		void Init( long Size )
		{
			Init_( Size );
			Size_ = Size;
		}
		long Size( void ) const
		{
			return ( Size_ < 0 ? -Size_ : Size_ );
		}
		const type *Core( void ) const
		{
			return Core_.C;
		}
		type *Core( void )
		{
			if ( Size_ < 0 )
				qRFwk();

			return Core_.V;
		}
	};

	template <typename type> class rJBytes_
	: public rJArray_<type>
	{
	public:
		using rJArray_<type>::Init;
		void Init(
			const char *Buffer,
			eHandling Handling )
		{
			size_t Size = strlen( Buffer );

			if ( Size > LONG_MAX )
				qRLmt();

			rJArray_<type>::Init( (long)Size, (const type *)Buffer, Handling );
		}
		void Init( const str::dString &String )
		{
		qRH
			qCBUFFERr Buffer;
		qRB
			Init( String.Convert( Buffer ), hCopy );
		qRR
		qRT
		qRE
		}
	};

	class rJByteArray
	: public rJBytes_<sJByte>
	{};

	class rJString
	: public rJBytes_<char>
	{};

	qENUM( Type_ )
	{
		tByte,
		tInt,
		tLong,
		tString,
		tByteArray,
		t_amount,
		t_Undefined
	};


	struct sValue {
	public:
		eType_ Type;
		union {
			sJByte Byte;
			sJInt Int;
			sJLong Long;
			void *Generic;
			rJString *String;
			rJByteArray *ByteArray;
		};
		void reset( bso::sBool = true )
		{
			Type = t_Undefined;
			Generic = NULL;
		}
		qCDTOR( sValue );
		void Init( void )
		{
			this->Type = t_Undefined;
			Generic = NULL;
		}
		void Init( sJByte Byte )
		{
			Type = tByte;
			this->Byte = Byte;
		}
		void Init( sJInt Int )
		{
			Type = tInt;
			this->Int = Int;
		}
		void Init( sJLong Long )
		{
			Type = tLong;
			this->Long = Long;
		}
		void Init( rJString &String )
		{
			Type = tString;
			this->String = &String;
		}
		void Init( rJByteArray &ByteArray )
		{
			Type = tByteArray;
			this->ByteArray = &ByteArray;
		}
	};

	class cObject {
	protected:
		virtual void N4JRESet(
			const char *Name,
			const char *Signature,
			cObject *Value ) = 0;
		virtual void N4JRECallObjectMethod(
			const char *Method,
			const char *Signature,
			sValue &Object,	// Will contain the returned object.
			int ArgC,
			sValue *ArgV ) = 0;
		// Both below methods works only on objects array ('jobjectsArray' casted to 'jobject').
		virtual sJSize N4JREGetLength( void ) = 0;
		virtual cObject *N4JREGetElement( sJSize Index ) = 0;
		virtual void N4JRESetElement(
			sJSize Index,
			cObject *Object ) = 0;
# define H( type, name )\
	protected:\
		virtual type N4JRECall##name##Method(\
			const char *Method,\
			const char *Signature,\
			int ArgC,\
			sValue *ArgV ) = 0;\
	public:\
		type Call##name##Method(\
			const char *Method,\
			const char *Signature,\
			int ArgC,\
			sValue *ArgV )\
		{\
			return N4JRECall##name##Method( Method, Signature, ArgC, ArgV );\
		}
		H( void, Void );
		H( sJBoolean, Boolean );
		H( sJShort, Short );
		H( sJInt, Int );
		H( sJLong, Long );
# undef H
	public:
		qCALLBACK( Object );
		void Set(
			const char *Name,
			const char *Signature,
			cObject *Value )
		{
			return N4JRESet( Name, Signature, Value );
		}
		void CallObjectMethod(
			const char *Method,
			const char *Signature,
			sValue &Object,	// Will contain the returned object.
			int ArgC,
			sValue *ArgV )
		{
			return N4JRECallObjectMethod( Method, Signature, Object, ArgC, ArgV );
		}
		sJSize GetLength( void )
		{
			return N4JREGetLength();
		}
		cObject *GetElement( sJSize Index )
		{
			return N4JREGetElement( Index );
		}
		void GetElement(
			sJSize Index,
			cObject *Object )
		{
			return N4JRESetElement( Index, Object );
		}
		void SetElement(
			sJSize Index,
			cObject *Object )
		{
			return N4JRESetElement( Index, Object );
		}
	};

	typedef cObject sJObject_;
	typedef sJObject_* sJObject;

	using n4all::sEnv;

	typedef cObject *(* fNewObject )(
		sEnv *Env,
		const char *Class,
		const char *Signature,
		int ArgC,
		sValue *ArgV );

	typedef cObject *(* fNewObjectArray)(
		sEnv *Env,
		sJSize Length,
		const char *Class );

	typedef void( *fDelete )( cObject * );

	typedef void ( *fThrow )(
		sEnv *Env,
		const char *Message );
#pragma pack(push, 1)
	struct gShared {
	public:
		fNewObject NewObject;
		fNewObjectArray NewObjectArray;
		// Below three functions purpose is that resources allocation/deallocation occurs in the same binary, otherwise there would be crashes.
		fDelete Delete;
		fMalloc Malloc;
		fFree Free;
		fThrow Throw;
		void reset( bso::sBool P = true )
		{
			NewObject = NULL;
			NewObjectArray = NULL;
			Delete = NULL;
			Malloc = NULL;
			Free = NULL;
			Throw = NULL;
		}
		qCDTOR( gShared );
	};
#pragma pack(pop)
}

# ifdef N4JRE_H_
#  define H N4JRE_H_
# endif

#endif
