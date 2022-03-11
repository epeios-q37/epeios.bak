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

// Socle Java Runtime Environment

#ifndef SCLJRE_INC_
# define SCLJRE_INC_

# define SCLJRE_NAME		"SCLJRE"

# if defined( E_DEBUG ) && !defined( SCLJRE_NODBG )
#  define SCLJRE_DBG
# endif

# include "n4jre.h"
# include "scln4a.h"

#ifdef H
#  define SCLJRE_H_ H
#  undef H
# endif

namespace scljre {
	using namespace n4jre;

	extern n4jre::fNewObject NewObject_;
	extern n4jre::fNewObjectArray NewObjectArray_;
	extern n4jre::fThrow Throw_;

	typedef crt::qCRATEdl( str::dStrings ) dXStrings;
	qW( XStrings );

	// Termination method.
	inline void Fill_(
		int Indice,
		sValue *Values )
	{}

	template <typename arg, typename ...args> inline void Fill_(
		int Indice,
		sValue *Values,
		arg &Arg,
		args&... Args )
	{
		Values[Indice].Init( Arg );
		Fill_( Indice + 1, Values, Args... );
	}

	template <typename ...args> cObject *New(
		sEnv *Env,
		const char *Class,
		const char *Signature,
		args&... Args )
	{
		sValue Values[sizeof...(args)+1];	/// '+1' only to avoid attempt to create a array of size 0.
		Values[sizeof...( args )].Type = t_Undefined;

		Fill_( 0, Values, Args... );

		return NewObject_( Env, Class, Signature, sizeof...( args ), Values );
	}

	inline void Throw(
		sEnv *Env,
		const char *Message )
	{
		return Throw_( Env, Message );
	}

	void Throw(
		sEnv *Env,
		const str::dString &Text );

	class rObject {
	private:
		qRMV( cObject, O_, Object_ );
	public:
		void reset( bso::sBool P = true );
		qCDTOR( rObject );
		void Init(n4jre::cObject *Object = NULL)
		{
			reset();

			Object_ = Object;
		}
		cObject *operator()( void )
		{
			return &O_();
		}
		void Set(
			const char *Name,
			const char *Signature,
			sJObject Value )
		{
			return O_().Set( Name, Signature, Value );
		}
		template <typename object> void Set(
			const char *Name,
			object &Object )
		{
			qRVct();
			Set( Name, NULL, Object );
		}
		// 2 below methods are for only arrays.
		sJSize GetLength( void )
		{
			return O_().GetLength();
		}
		cObject *GetElement( sJSize Index )
		{
			return O_().GetElement( Index );
		}
		void SetElement(
			sJSize Index,
			cObject * Object )
		{
			return O_().SetElement( Index, Object );
		}
		template <typename object, typename ...args> void CallObjectMethod(
			const char *Method,
			const char *Signature,
			object &Object,
			args&... Args )
		{
			sValue Values[sizeof...(args)+1];	/// '+1' only to avoid attempt to create a array of size 0.
			sValue Return;

			Values[sizeof...( args )].Type = t_Undefined;

			Fill_( 0, Values, Args... );

			Return.Init( Object );

			O_().CallObjectMethod( Method, Signature, Return, sizeof...( args ), Values );
		}
# define SCLJRE_M_( type, name )\
		template <typename ...args> type Call##name##Method(\
			const char *Method,\
			const char *Signature,\
			args&... Args )\
		{\
			sValue Values[sizeof...(args)+1];\
			Values[sizeof...( args )].Type = t_Undefined;\
\
			Fill_( 0, Values, Args... );\
\
			return O_().Call##name##Method( Method, Signature, sizeof...( args ), Values );\
		}
		SCLJRE_M_( void, Void );
		SCLJRE_M_( sJBoolean, Boolean );
		SCLJRE_M_( sJShort, Short );
		SCLJRE_M_( sJInt, Int );
		SCLJRE_M_( sJLong, Long );
# undef SCLJRE_M_
# define SCLJRE_SG_( type, name )\
    void Set##name(\
      const char *Name,\
      type Value)\
    {\
      return O_().Set##name(Name, Value);\
    }\
    type Get##name(const char *Name)\
    {\
      return O_().Get##name(Name);\
    }
		SCLJRE_SG_( sJBoolean, Boolean );
		SCLJRE_SG_( sJShort, Short );
		SCLJRE_SG_( sJInt, Int );
		SCLJRE_SG_( sJLong, Long );
# undef SCLJRE_SG_
	};

	class rCore_
	{
	protected:
		rObject Object_;
	public:
		void reset( bso::sBool P = true )
		{
			Object_.reset( P );
		}
		qCDTOR( rCore_ );
		void Init( n4jre::cObject *Object )
		{
			Object_.Init( Object );
		}
		cObject *operator()( void )
		{
			return Object_.operator()();
		}
	};


# ifdef B
#  define SCKJRE_B_ B
# undef B
# endif

# ifdef A
#  define SCKJRE_A_ A
# undef A
# endif

// Before
# define B( name )\
	class r##name\
	: public rCore_\
	{\
	public:\
		using rCore_::Init

// After
# define A	}


	namespace java {
		namespace io {
			B( InputStream );
			sJInt Read( void )
			{
				return Object_.CallIntMethod( "read", "()I" );
			}
			sJInt Read(
				rJByteArray &b,
				sJInt off,
				sJInt len )
			{
				return Object_.CallIntMethod( "read", "([BII)I", b, off, len );
			}
			A;
		}

		namespace lang {
			B( Boolean );
				void Init(
					sEnv *Env,
					sJBoolean Boolean )
				{
					Init( New( Env, "Ljava/lang/Boolean;", "(Z)V", Boolean ) );
				}
				sJBoolean BooleanValue( void )
				{
					return Object_.CallBooleanMethod( "booleanValue", "()Z" );
				}
			A;

			B( Short );
				void Init(
					sEnv *Env,
					sJShort Short )
				{
					Init( New( Env, "Ljava/lang/Short;", "(S)V", Short ) );
				}
				sJShort ShortValue( void )
				{
					return Object_.CallShortMethod( "shortValue", "()S" );
				}
			A;

			B( Integer );
				void Init(
					sEnv *Env,
					sJInt Integer )
				{
					Init( New( Env, "Ljava/lang/Integer;", "(I)V", Integer ) );
				}
				sJInt IntValue( void )
				{
					return Object_.CallIntMethod( "intValue", "()I" );
				}
			A;

			B( Long );
				void Init(
					sEnv *Env,
					sJLong Long )
				{
					Init( New( Env, "Ljava/lang/Long;", "(J)V", Long ) );
				}
				sJLong LongValue( void )
				{
					return Object_.CallLongMethod( "longValue", "()J" );
				}
			A;

			B( String );
				void Init(
					sEnv *Env,
					rJByteArray &bytes,
					rJString &charsetName )
				{
					Init( New( Env, "Ljava/lang/String;", "([BLjava/lang/String;)V", bytes, charsetName ) );
				}
				void GetBytes(
					rJString &charsetName,
					rJByteArray &Result )
				{
					return Object_.CallObjectMethod( "getBytes", "(Ljava/lang/String;)[B", Result, charsetName );
				}
			A;
		}
	}

	namespace {
    qCDEF( char *, JCorePointerMemberName_, "pointer" );
	}

	template <typename t> class rJCore
	: public rObject
	{
  private:
    t *GetPointer_(qRPD)
    {
      t *Pointer = (t *)GetLong(JCorePointerMemberName_);

      if ( qRPT )
        qRFwk();

      return Pointer;
    }
    qRMV(t, P_, Pointer_);
  public:
    void reset(bso::sBool P = true)
    {
      rObject:: reset(P);

      Pointer_ = NULL;
    }
		void Init(n4jre::cObject *Object = NULL)
		{
		  reset();

		  rObject::Init(Object);

		  if ( Object != NULL )
        Pointer_ = GetPointer_(err::hUserDefined);
		}
    t &operator()(void)
    {
      return P_();
    }
    void Delete(void)
    {
      if ( Pointer_ != NULL )
      {
        SetLong(JCorePointerMemberName_, NULL);
        qDELETE(Pointer_);
      }
    }
	};

	typedef scln4a::sCaller sCaller_;

	class sCaller
	: public sCaller_
	{
	private:
		bso::sU8 Index_;
		void Get_(
			cObject *Object,
			str::dString &String )
		{
		qRH;
			scljre::java::lang::rString JString;
			rJString Charset;
			rJByteArray Result;
		qRB;
			Charset.Init( sizeof( "UTF-8" ), "UTF-8", n4jre::hOriginal );
			JString.Init( Object );

			Result.Init();
			JString.GetBytes( Charset, Result );

			String.Append( (bso::char__ *)Result.Core(), Result.Size() );
		qRR;
		qRT;
		qRE;
		}
		void Get_( str::dString &String )
		{
			Get_( GetObject(), String );
		}
		void Get_(
			rObject &Object,
			str::dStrings &Strings )
		{
		qRH;
			scljre::java::lang::rString JString;
			n4jre::sJSize Length = 0, Index = 0;
			str::wString String;
		qRB;
			Length = Object.GetLength();

			while ( Index < Length ) {
				String.Init();
				Get_( Object.GetElement( Index++ ), String );
				Strings.Append( String );
			}
		qRR;
		qRT;
		qRE;
		}
		void Get_( str::dStrings &Strings )
		{
		qRH;
			rObject Object;
		qRB;
			Object.Init( GetObject() );

			Get_( Object, Strings );

		qRR;
		qRT;
		qRE;
		}
		void Get_( dXStrings &XStrings )
		{
		qRH;
			rObject JArray, JStrings;
			n4jre::sJSize Length = 0, Index = 0;
			str::wStrings Strings;
		qRB;
			JArray.Init( GetObject() );

			Length = JArray.GetLength();

			while ( Index < Length ) {
				JStrings.Init(JArray.GetElement( Index++ ) );
				Strings.Init();
				Get_( JStrings, Strings );
				XStrings.Append( Strings );
			}
		qRR;
		qRT;
		qRE;
		}
		template <typename t> void Get_(rJCore<t> &Core)
		{
		  return Core.Init(GetObject());
		}
		void Get_(rObject &Object)
		{
		  Object.Init(GetObject());
		}
    // Termination method.
		void Get( void ) {}
	public:
		void reset( bso::sBool P = true )
		{
			sCaller_::reset( P );
			Index_ = 0;
		}
		qCDTOR( sCaller );
		void Init( n4all::cCaller &Caller )
		{
			sCaller_::Init( Caller );

			Index_ = 0;
		}
		void Get( sJObject &Object )
		{
			C_().GetArgument( Index_++, n4jre::t_Undefined, &Object );
		}
		/*
		Not simply called 'Get()', otherwise it will be called
		as termination method from below template variadics method
		*/
		sJObject GetObject( void )
		{
			sJObject Object;

			Get( Object );

			return Object;
		}
		void Get( str::dString &String )
		{
			return Get_( String );
		}
		void Get( str::dStrings &Strings )
		{
			return Get_( Strings );
		}
		template <typename t> void Get(rJCore<t> &Core)
		{
		  return Get_(Core);
		}
		void Get(rObject &Object)
		{
		  return Get_(Object);
		}
		template <typename arg, typename ...args> void Get(
			arg &Arg,
			args&... Args )
		{
			Get_( Arg );
			Get( Args... );
		}
	};

	typedef sJObject ( fFunction )(
		sEnv *Env,
		sCaller &Caller );

# undef B
# undef A

# ifdef SCLJRE_B_
# define B SCLJRE_B_
# endif

# ifdef SCLJRE_A_
# define A SCLJRE_A_
# endif

	sJObject Null( void );
	sJObject Boolean(
		sEnv *Env,
		sJBoolean Boolean );
	sJObject Integer(
		sEnv *Env,
		sJInt Integer );
	sJObject Long(
		sEnv *Env,
		sJLong Long );
	sJObject String(
		sEnv *Env,
		const str::dString &UTF );
	sJObject Strings(
		sEnv *Env,
		const str::dStrings &Strings );

	// To ease the handling of user object.
	template <typename t> inline sJObject ConvertUO(
		sEnv *Env,
		const t* UO )
	{
		return Long( Env, (sJLong)UO );
	}

	template <typename t> inline sJObject ConvertUO(
		sEnv *Env,
		const t& UO )
	{
		return ConvertUO<t>( &UO );
	}

	template <typename t, typename... args> inline sJObject CreateUO(
		sEnv *Env,
		args& ...Args )
	{
		t *UO;
	qRH;
	qRB;
		UO = new t;

		if ( UO == NULL )
			qRAlc();

		UO->Init( Args... );
	qRR;
	qRT;
	qRE;
		return scljre::ConvertUO<t>( Env, UO );
	}

	template <typename t> inline t *GetUOP( sJObject Object )
	{
		t *UO = NULL;
	qRH;
		scljre::java::lang::rLong Long;
	qRB;
		Long.Init( Object );

		UO = (t *)Long.LongValue();

		if ( UO == NULL )
			qRGnr();
	qRR;
	qRT;
	qRE;
		return UO;
	}

	template <typename t> t *GetUOP( scljre::sCaller &Caller )
	{
		return GetUOP<t>( Caller.GetObject() );
	}

	template <typename t> inline t &GetUO( sJObject Object )
	{
		return *GetUOP<t>( Object );
	}

	template <typename t> t &GetUO( scljre::sCaller &Caller )
	{
		return *GetUOP<t>( Caller );
	}

	template <typename t> inline sJObject DeleteUO( scljre::sCaller &Caller )
	{
		delete GetUOP<t>( Caller );

		return Null();
	}

	typedef fdr::rRDressedDriver rIDriver_;

	class rInputStreamRDriver
	: public rIDriver_ {
	private:
		java::io::rInputStream Stream_;
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte *Buffer ) override;
		virtual bso::sBool FDRDismiss(
			bso::sBool Unlock,
			qRPN ) override
		{
			return true;
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			return Owner;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rIDriver_::reset( P );
			tol::reset( P, Stream_ );
		}
		qCVDTOR( rInputStreamRDriver );
		void Init( sCaller &Caller )
		{
			rIDriver_::Init( fdr::ts_Default );
			Stream_.Init( Caller.GetObject() );
		}
	};


	typedef scln4a::sRegistrar<fFunction> sRegistrar;

	void SCLJREInfo( txf::sWFlow &Flow );	// To define by user.
	const scli::sInfo &SCLJRERegister( sRegistrar &Registrar );	// To define by user
}

# define SCLJRE_F( name ) scljre::sJObject name(\
	scljre::sEnv *Env,\
	scljre::sCaller &Caller )

# ifdef SCLJRE_H_
#  define H SCLJRE_H_
# endif

#endif
