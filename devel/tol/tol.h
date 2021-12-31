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

#ifndef TOL_INC_
# define TOL_INC_

# define TOL_NAME		"TOL"

# if defined( E_DEBUG ) && !defined( TOL_NODBG )
#  define TOL_DBG
# endif

// TOoL


# include <time.h>
# include <signal.h>
# include <locale.h>

# include "cpe.h"
# include "strng.h"

# ifdef CPE_S_POSIX
#  ifdef CPE_S_DARWIN
#   define TOL__DARWIN
#  else
#   define TOL__POSIX
#  endif
# elif defined( CPE_S_WIN )
#  define TOL__WIN
#  include <Windows.h>
# else
#  error "Undefined compilation enviroment."
# endif


# ifdef TOL__WIN
#  include <sys/utime.h>
#  include <sys/timeb.h>
# elif defined( TOL__DARWIN )
#  include <mach/mach_time.h>
# elif defined( TOL__POSIX )
#  include <utime.h>
# else
#  error
# endif

# include "err.h"

# include "bso.h"

// Predeclarations.

// Should be 'uys::sHook …", but cannot predeclare typedefs.
namespace sdr {
  class storage_driver__;
}

namespace ags {
	class aggregated_storage_;
}

/*****************/
/****** New ******/
/*****************/

namespace str {
	class string_;

	typedef string_ dString;
}

# define qENUM( name )	enum e##name : bso::tEnum

# define qXENUM( name, prefix )	typedef tol::extended_enum__<e##name##_, prefix##_False, prefix##_Error, prefix##_Undefined> e##name

# define TOL_ROW_( name ) E_TMIMIC__( sdr::bRow, name )

# define qROW( name ) TOL_ROW_( s##name )
// Nota: although a static object, defining it as resource containing object
// that it is pointing to an object which has to be released by the proper function.
# define qROWr( name ) TOL_ROW_( r##name )

#define qROWS( name )\
	typedef bch::qBUNCHdl( s##name ) d##name##s;\
	qW( name##s )


// Declaration of standardized constructor.
# define TOL_CTORD_( name )\
	name( void )

// Body of standardized constructor.
# define TOL_CTORB_\
	{\
		reset( false );\
	}\

// Standardized constructor.
# define qCTOR( name )\
	TOL_CTORD_( name )\
	TOL_CTORB_

// Standardized destructor.
# define qDTOR( name )\
	~name( void )\
	{\
		reset();\
	}\

// Standardized virtual destructor.
# define qVDTOR( name )	virtual qDTOR( name )

// Standardized constructor and destructor.
# define qCDTOR( name )\
	qCTOR( name )\
	qDTOR( name )

// Standardized constructor and virtualdestructor.
# define qCVDTOR( name )\
	qCTOR( name )\
	qVDTOR( name )

# define qCALLBACK( name ) virtual ~c##name( void ) {}

// Make accessible the static member, for read-only access, of a dynamic object, named 'name' of type 'type'.
# define qRRODISCLOSEd(type, name )\
	const type &Get##name( void ) const\
	{\
		return S_.name;\
	}

// Make accessible the static member, for read-only access, of a dynamic object, named 'name' of type 'type'.
# define qRODISCLOSEd(type, name )\
	qRRODISCLOSEd( type, name )\
	const type &name( void ) const\
	{\
		return S_.name;\
	}

# define qRWODISCLOSEd(type, name )\
	void Set##name( const type &V )\
	{\
		S_.name = V;\
	}

# define qWODISCLOSEd(type, name )\
	qRWODISCLOSEd( type, name )\
	type &name( void )\
	{\
		return S_.name;\
	}

// Make accessible the static member, for read-write access, of a dynamic object, named 'name' of type 'type'.
# define qRRWDISCLOSEd(type, name )\
	qRRODISCLOSEd( type, name )\
	qRWODISCLOSEd( type, name )

// Make accessible the static member, for read-write access, of a dynamic object, named 'name' of type 'type'.
# define qRWDISCLOSEd( type, name )\
	qRODISCLOSEd( type, name )\
	qWODISCLOSEd( type, name )

# define qRRODISCLOSEs( type, name )\
	const type &Get##name( void ) const\
	{\
		return name##_;\
	}

// Make accessible the member, for read-only access, of a static object, named 'name' of type 'type'.
# define qRODISCLOSEs(type, name )\
	qRRODISCLOSEs( type, name )\
	const type &name( void ) const\
	{\
		return name##_;\
	}

# define qRWODISCLOSEs(type, name )\
	void Set##name( const type &V )\
	{\
		name##_ = V;\
	}

# define qWODISCLOSEs(type, name )\
	qRWODISCLOSEs( type, name )\
	type &name( void )\
	{\
		return name##_;\
	}

# define qRRWDISCLOSEs(type, name )\
	qRRODISCLOSEs( type, name )\
	qRWODISCLOSEs( type, name )

# define qRWDISCLOSEs(type, name )\
	qRODISCLOSEs( type, name )\
	qWODISCLOSEs( type, name )

# define qRRODISCLOSEr( type, name )	qRRODISCLOSEs( type, name )
# define qRODISCLOSEr(type, name )		qRODISCLOSEs(type, name )
# define qWODISCLOSEr(type, name )		qWODISCLOSEs(type, name )
# define qRRWDISCLOSEr(type, name )		qRRWDISCLOSEs(type, name )
# define qRWDISCLOSEr(type, name )		qRWDISCLOSEs(type, name )

# define qW_( name )	\
class w##name\
: public d##name\
{\
public:\
	d##name::s static_;\
	w##name( void )\
	: d##name( static_ )\
	{\
		reset( false );\
	}\
	~w##name( void )\
	{\
		reset( true );\
	}\
	w##name &operator =( const w##name &O )\
	{\
		d##name::operator =( O );\
\
		return *this;\
	}\
	w##name &operator =( const d##name &O )\
	{\
		d##name::operator =( O );\
\
		return *this;\
	}\
	const d##name &operator *( void ) const\
	{\
		return *this;\
	}\
	d##name &operator *( void )\
	{\
		return *this;\
	}

// Wraps 'vName' to make it instantiable.
# define qW( name )\
	qW_( name )\
	};

//m Same as 'qW()', but with one template parameter.
# define qW1( name )\
template <typename t> class w##name\
: public d##name<t>\
{\
public:\
	typename d##name<t>::s static_;\
	w##name( void )\
	: d##name<t>( static_ )\
	{\
		d##name<t>::reset( false );\
	}\
	~w##name( void )\
	{\
		d##name<t>::reset( true );\
	}\
	w##name &operator =( const w##name &S )\
	{\
		d##name<t>::operator =( S );\
\
		return *this;\
	}\
	w##name &operator =( const d##name<t> &S )\
	{\
		d##name<t>::operator =( S );\
\
		return *this;\
	}\
	d##name<t> &operator *( void )\
	{\
		return *this;\
	}\
	const d##name<t> &operator *( void ) const\
	{\
		return *this;\
	}\
};

// Same as 'qW()', but with 2 template parameters.
# define qW2( name )\
template <typename t, typename u> class w##name\
: public d##name<t,u>\
{\
public:\
	typename d##name<t,u>::s static_;\
	w##name( void )\
	: d##name<t,u>( static_ )\
	{\
		d##name<t,u>::reset( false );\
	}\
	~w##name( void )\
	{\
		d##name<t,u>::reset( true );\
	}\
	w##name &operator =( const w##name &S )\
	{\
		d##name<t,u>::operator =( S );\
\
		return *this;\
	}\
	w##name &operator =( const d##name<t,u> &S )\
	{\
		d##name<t,u>::operator =( S );\
\
		return *this;\
	}\
	d##name<t,u> &operator *( void )\
	{\
		return *this;\
	}\
	const d##name<t,u> &operator *( void ) const\
	{\
		return *this;\
	}\
};

// Same as 'qW()', but with 3 template parameters.
# define qW3( name )\
template <typename t, typename u, typename v> class w##name\
: public d##name<t,u,v>\
{\
public:\
	typename d##name<t,u,v>::s static_;\
	w##name( void )\
	: d##name<t,u,v>( static_ )\
	{\
		d##name<t,u,v>::reset( false );\
	}\
	~w##name( void )\
	{\
		d##name<t,u,v>::reset( true );\
	}\
	w##name &operator =( const w##name &S )\
	{\
		d##name<t,u,v>::operator =( S );\
\
		return *this;\
	}\
	w##name &operator =( const d##name<t,u,v> &S )\
	{\
		d##name<t,u,v>::operator =( S );\
\
		return *this;\
	}\
	d##name<t,u,v> &operator *( void )\
	{\
		return *this;\
	}\
	const d##name<t,u,v> &operator *( void ) const\
	{\
		return *this;\
	}\
};

// Pointer Method
#define qPM( type, method, variable )\
	type *method( void ) const\
	{\
		if ( variable == NULL )\
			qRFwk();\
\
		return variable;\
	}

// Pointer variable and method.
#define qPMV( type, method, variable )\
	type *variable;\
	qPM( type, method, variable )

// Reference Method.
#define qRM( type, method, variable )\
	type &method( void ) const\
	{\
		if ( variable == NULL )\
			qRFwk();\
\
		return *variable;\
	}

// Reference variable and method.
#define qRMV( type, method, variable )\
	type *variable;\
	qRM( type, method, variable )

// 'const' reference Method.
#define qCRM( type, method, variable )\
	const type &method( void ) const\
	{\
		if ( variable == NULL )\
			qRFwk();\
\
		return *variable;\
	}

// 'const' reference variable and method.
#define qCRMV( type, method, variable )\
	const type *variable;\
	qCRM( type, method, variable )

# define qCDEF( type, name, value ) const type name = value
# define qCDEFS(name, value) qCDEF(str::wString, name, value)

# define qFLAG( name, value )	qCDEF( bso::sFlag, f##name, 2 << ( value ) )

/* Transforms n arguments in 1.
Useful when a macro argument contains one or more coma.
ex. : 'qCOVER2( a, b )' -> 'a, b' */
# define qCOVER2(a, b)					a, b
# define qCOVER3(a, b, c)				a, b, c
# define qCOVER4(a, b, c, d)			a, b, c, d
# define qCOVER5(a, b, c, d, e)			a, b, c, d, e
# define qCOVER6(a, b, c, d, e, f)		a, b, c, d, e, f
// To modify using macros variadics ?

# define qTMIMICd( type, alias )\
class d##alias\
: public d##type\
{\
public:\
	d##alias( s &S )\
	: d##type( S )\
	{}\
	struct s\
	: public d##type::s\
	{};\
	d##alias &operator =( const d##alias &S )\
	{\
		((d##type *)this)->operator =( S );\
\
		return *this;\
	}\
	d##alias &operator =( const d##type &T )\
	{\
		((d##type *)this)->operator =( T );\
		\
		return *this;\
	}\
	const d##type operator *( void ) const\
	{\
		return *this;\
	}\
};

# define qTMIMICw( type, alias )\
class w##alias\
: public d##alias\
{\
public:\
	d##alias::s static_;\
	w##alias( void )\
	: d##alias( static_ )\
	{\
		reset( false );\
	}\
	~w##alias( void )\
	{\
		reset( true );\
	}\
	w##alias &operator =( const w##alias &S )\
	{\
		d##alias::operator =( S );\
\
		return *this;\
	}\
	w##alias &operator =( const d##alias &S )\
	{\
		d##alias::operator =( S );\
\
		return *this;\
	}\
	w##alias &operator =( const d##type &T )\
	{\
		((d##type *)this)->operator =( T );\
		\
		return *this;\
	}\
};

#define qTMIMICdw( type, alias )\
qTMIMICd( type, alias )\
qTMIMICw( type, alias )

# define qMIMICs( type, alias )	E_TMIMIC__( type, alias )


// For static objects only.
// Example of use : 'template <typename r> qTCLONEf( object1<r>, object2 );'
// Example of use : 'template <typename r, typename s> qTCLONEf( object1<qCOVER2(r,s)>, object2 );'
# define qTCLONEs( type, alias )\
	class alias\
	: public type\
	{\
	}

# define qTCLONE( type, alias )\
	class alias\
	: public type\
	{\
	}

# define qTCLONEd( type, alias )\
	class alias\
	: public type\
	{\
	public:\
		struct s\
		: public type::s\
		{};\
		alias( s &S )\
		: type( S )\
		{}\
	}\



# define TOL_ERRP_	err::handling__ ErrHandling = err::h_Default

namespace tol{
	typedef char bDateAndTime[20];
	typedef bDateAndTime bDate;
	typedef bDateAndTime bTime;

	template <typename t> class hBuffer // Dynamic buffer of objects of type 't'. Its size never shrinks, so it can(t be used to know the true amount of objects it contains.
	{
	private:
		t *_Pointer;
		bso::size__ _Extent;
		virtual bso::bool__ _Allocate(
			bso::size__ Size,
			err::handling__ ErrHandling )
		{
			if ( _Extent < Size ) {
				void *P = realloc( _Pointer, Size * sizeof( t ) );

				if ( P == NULL ) {
					if ( ErrHandling == err::hThrowException )
						qRAlc();

					return false;
				} else
					_Pointer = (t *)P;

				_Extent = Size;
			}

			return true;
		}
		virtual void _Free( void *P )
		{
			free( _Pointer );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _Pointer != NULL )
					_Free( _Pointer );

			_Pointer = NULL;
			_Extent = 0;
		}
		qCDTOR(hBuffer)
		void Init( void )
		{
			qRFwk();	// C'est un 'buffer' ; pas d'initailisation.
		}
		void Forget( void )	// Evite que le ponteur sous-jacent soit effac  la destruction de l'objet.
		{
			reset( false );

		}
		t *Malloc(
			bso::size__ Amount,
			TOL_ERRP_ )
		{
			if ( !_Allocate( Amount, ErrHandling ) )
				return NULL;

			return _Pointer;
		}
		t *Calloc(
			bso::size__ Amount,
			TOL_ERRP_ )
		{
			if ( !_Allocate( Amount, ErrHandling ) )
				return NULL;

			memset( _Pointer, 0, Amount * sizeof( t ) );

			return _Pointer;
		}
		t *Realloc(
			bso::size__ Size,
			TOL_ERRP_ )
		{
			if ( !_Allocate( Size, ErrHandling ) )
				return NULL;

			return _Pointer;
		}
		bso::size__ Extent( void ) const	// ATTENTION : n'est PAS le nombre d'octets significatifs !
		{
			return _Extent;
		}
		operator t*( void ) const
		{
			return _Pointer;
		}
		hBuffer &operator =( const hBuffer & )
		{
			qRFwk();

			return *this;
		}
		t* operator()( void ) const
		{
			return _Pointer;
		}
	};
}

# define qBUFFERh( t )	tol::hBuffer<t>
# define qCBUFFERh		qBUFFERh( bso::char__ )

namespace tol {
	// A basic object 't' becomes a normal object.
	template <class t> class dObject
	{
	public:
		struct s
		{
			t Object;
		} &S_;
		dObject( s &S )
		: S_( S )
		{}
		void reset( bool P = true )
		{
//			S_.Object.reset( P );	// The object is already destroyed by the one which features the reference.
		}
		qCVDTOR( dObject );
		void plug(sdr::storage_driver__ &Hook)  // Should be 'uys::sHook …", but cannot predeclare typedefs (see top of file).
		{
			// Standardization.
		}
		void plug( ags::aggregated_storage_ *AS )
		{
			// Standardization.
		}
		dObject &operator =( const dObject &O )
		{
			S_.Object = O.S_.Object;

			return *this;
		}
		t &operator()( void )
		{
			return S_.Object;
		}
		const t &operator()( void ) const
		{
			return S_.Object;
		}
	};

	qW1( Object );

	typedef char bUUID[37];

	const char *UUIDGen( bUUID &UUID );
}

# define qGCTOR(discriminator)	Q37_GCTOR( discriminator )
# define qGDTOR(discriminator)	Q37_GDTOR( discriminator )

/*************************/
/****** Old version ******/
/*************************/

# define E_ENUM( name )	enum name##__ : bso::enum__

namespace ntvstr {
	class string___;
}

namespace tol
{
	typedef bDateAndTime buffer__;

	enum report__ {
		rFailure = 0,
		rSuccess,
		r_amount,
		r_Undefined,
	};

	enum extended_boolean__ {
		xbFalse,
		xbNo = xbFalse,
		xbTrue,
		xbYes = xbTrue,
		xb_amount,
		xb_Undefined
	};

	class xbool__ {
	private:
		extended_boolean__ _Boolean;
	public:
		void reset( bso::bool__ = true )
		{
			_Boolean = xb_Undefined;
		}
		~xbool__( void )
		{
			reset( true );
		}
		xbool__( extended_boolean__ Boolean = xb_Undefined )
		{
			reset( false );
			_Boolean = Boolean;
		}
		void Init( void )
		{
			_Boolean = xb_Undefined;
		}
		operator extended_boolean__( void ) const
		{
			return _Boolean;
		}
		extended_boolean__ operator *( void ) const
		{
			return _Boolean;
		}
		operator bso::bool__( void ) const
		{
			switch ( _Boolean ) {
			case xbFalse:
				return false;
				break;
			case xbTrue:
				return true;
				break;
			default:
				qRFwk();
				break;
			}

			return false;	// Pour viter un 'warning'.
		}
	};

	// NOTA : 'qNIL' pas encore connu, d'o utilisation de '-1'.
	template <typename r> class extended_row__	// Pour les fonctions renvoyant un 'Row' dont une valeur diffrente de 'qNIL' reprsente une erreur.
	{
	private:
		r _Row;
	public:
		void reset( bso::bool__ = true )
		{
			_Row = -1;
		}
		extended_row__( r Row = -1 )
		{
			reset( false );
			_Row = Row;
		}
		~extended_row__( void )
		{
			reset( true );
		}
		void Init( void )
		{
			_Row = -1;
		}
		operator bool( void ) const
		{
			return _Row == -1;
		}
		operator r( void ) const
		{
			return _Row;
		}
	};

	/*
		enum example {
			// First value(s) correspond to true.
			xA,
			xB,
			xC,
			...
			x_False,
			xD = x_False,
			xE,
			xF,
			x_Error,
			xG = x_Error,
			xH,
			xI,
			...
			x_amount,
			x_Undefined
		}
	*/

	enum flavor_ {	// new notation.
		fTrue,
		fFalse,
		fError,
		f_amount,
		f_Undefined
	};

	template <typename type, type False, type Error, type Undefined> class extended_enum__
	{
	private:
		type Value_;
		flavor_ F_( void ) const
		{
			if ( Value_ >= Undefined )
				qRFwk();
			else if ( Value_ >= Error )
				return fError;
			else if ( Value_ >= False )
				return fFalse;

			return fTrue;
		}
	public:
		void reset( bso::bool__ = true )
		{
			Value_ = Undefined;
		}
		extended_enum__( type Value = Undefined )
		{
			reset();
			Value_ = Value;
		}
		~extended_enum__( void )
		{
			reset( false );
		}
		void Init( void )
		{
			Value_ = Undefined;
		}
		type Value( void ) const
		{
			return Value_;
		}
		bso::bool__ IsError( void ) const
		{
			return F_() == fError;
		}
		bso::bool__ IsFalse( void ) const
		{
			return F_() == fFalse;
		}
		bso::bool__ IsTrue( void ) const
		{
			return F_() == fTrue;
		}
		bso::bool__ Boolean( void ) const
		{
			if ( IsError() )
				qRFwk();

			return IsTrue();
		}
	};

# define E_XENUM( name, prefix )	typedef tol::extended_enum__<_##name##__, prefix##_False, prefix##_Error, prefix##_Undefined> name##__
}

# define E_XROWt( type )	extended_row__<type>

# define E_XROW	E_XROWt( sdr::row__ )

// Dfinition d'une constante par valeur.
# define E_CDEF( type, name, value ) qCDEF( type, name, value )

// Pose problme d'ordre d'initialisation. La variable pointe sur NULL au dpart est n'est correctement initialise que plus tard.
# if 0
// Dfinition d'une constante par rfrence.
# define E_CRDEF( type, name, value )\
	E_CVDEF( type, &name, value )
# endif

# ifdef CPE_C_MSC
#  pragma warning( disable : 4284 )
# endif

// Usage interne.
# define E__TMIMIC__( type ,alias )\
	private:\
		type V_;\
	public:\
		alias( void ) {}\
		alias( type T )\
		{\
			V_ = T;\
		}\
		type *operator ->( void )\
		{\
			return &V_;\
		}\
		const type *operator ->( void ) const\
		{\
			return &V_;\
		}\
		type &operator *( void )\
		{\
			return V_;\
		}\
		const type &operator *( void ) const\
		{\
			return V_;\
		}\

// Similaire  un 'typedef type alias', sauf que 'type' et 'alias' ne sont pas interchangeable.
// Uniqumement pour des objets statiques.
// Utiliiser 'E_TMIMIC( type, alias)' pour des objets dynamiques.
# define E_TMIMIC__( type, alias )\
	struct alias\
	{\
		E__TMIMIC__( type, alias )\
		bool operator ==( alias A ) const\
		{\
			return V_ == A.V_;\
		}\
		bool operator !=( alias A ) const\
		{\
			return V_ != A.V_;\
		}\
		bool operator ==( type T ) const\
		{\
			return V_ == T;\
		}\
		bool operator !=( type T ) const\
		{\
			return V_ != T;\
		}\
	}

// Similaire  un 'T_MIMIC__( type, alias )', mais sans oprateurs de comparaisons.
# define E_TRMIMIC__( type, alias )\
struct alias\
{\
	E__TMIMIC__( type, alias )\
}


// Similaire  'E_TMIMIC__( type, alias)', mais pour des objets dynamiques.
// La version instanciable et la version '_' sont toutes deux crees.
# define E_TMIMIC( type, alias )\
class alias##_\
: public type##_\
{\
public:\
	alias##_( s &S )\
	: type##_( S )\
	{}\
	struct s\
	: public type##_::s\
	{};\
	alias##_ &operator =( const alias##_ &S )\
	{\
		((type##_ *)this)->operator =( S );\
\
		return *this;\
	}\
	alias##_ &operator =( const type##_ &T )\
	{\
		((type##_ *)this)->operator =( T );\
		\
		return *this;\
	}\
};\
\
class alias\
: public alias##_\
{\
public:\
	alias##_::s static_;\
	alias( void )\
	: alias##_( static_ )\
	{\
		reset( false );\
	}\
	~alias( void )\
	{\
		reset( true );\
	}\
	alias &operator =( const alias &S )\
	{\
		alias##_::operator =( S );\
\
		return *this;\
	}\
	alias &operator =( const alias##_ &S )\
	{\
		alias##_::operator =( S );\
\
		return *this;\
	}\
	alias &operator =( const type##_ &T )\
	{\
		((type##_ *)this)->operator =( T );\
		\
		return *this;\
	}\
};

// Pour conserver une certaine cohrence avec la suite.
# define E_TCLONE__( type, alias )	typedef type alias

// Pour les objets dynamiques, similaire  'typedef type alias', mais
// avec cration de la version instanciable et de la version '_'.
# define E_TCLONE( type, alias )\
	typedef type##_ alias##_;\
	typedef type alias


// Similaire  'typedef type alias', mais permet la mise en oeuvre de 'template'.
// Pour objets statiques uniquement.
// Exemple d'utilisation.
// template <typename r> E_TTCLONE__( tree_seeker__<r>, seeker__ );
# define E_TTCLONE__( type, alias )\
	class alias\
	: public type\
	{\
	}


// Similaire  'typedef type  alias', mais permet la mise en oeuvre de 'template'.
// Pour objets dynamiques uniquement.
// Exemple :
// template <typename field_ref__> E_TTCLONE( lstctn::E_LXMCONTAINERt_( comparison_item_<field_ref__>, item_row__ ), comparison_items_ );
# define E_TTCLONE_( type, alias )\
	class alias\
	: public type\
	{\
	public:\
		struct s\
		: public type::s\
		{};\
		alias( s &S )\
		: type( S )\
		{}\
	}\


// Similaire  'E_TTCLONE_( type ,alias )', mais cre la version instanciable en plus de la version '_'.
// ATTENTION : 'type' doit tre la version AVEC '_', 'alias' la version SANS '_'.
// Version pour 1 'template".
// Exemple :
// template <typename field_ref__> E_TT1CLONE( lstctn::E_LXMCONTAINERt_( comparison_item_<field_ref__>, item_row__ ), comparison_items );
# define E_TT1CLONE( type, alias )\
	E_TTCLONE_( type, alias##_ );\
	E_AUTO1( alias )

// Idem que ci-dessus, mais pour 2 'templates's.
# define E_TT2CLONE( type, alias )\
	E_TTCLONE_( type, alias##_ );\
	E_AUTO2( alias )

// Idem que ci-dessus, mais pour 3 'templates's.
# define E_TT3CLONE( type, alias )\
	E_TTCLONE_( type, alias##_ );\
	E_AUTO3( alias )


//d Create a new type for a row named 'Type'.
# define E_ROW( Type )	E_TMIMIC__( sdr::row_t__, Type )

/* Permet de transformer n arguments en 1; si un argument d'une macro
a besoin de contenir une virgule, cette macro est l pour a.
ex. : 'E_COVER2( a, b )' donne 'a, b' */
# define E_COVER2(a, b)					a, b
# define E_COVER3(a, b, c)				a, b, c
# define E_COVER4(a, b, c, d)			a, b, c, d
# define E_COVER5(a, b, c, d, e)		a, b, c, d, e
# define E_COVER6(a, b, c, d, e, f)		a, b, c, d, e, f
// A modifier en utilisant les 'variadics macros' ?


/* Permet de convertir un entier en chane (l'encadre par des '"').
Utile pour afficher le numro de ligne dans un #pragma message (...). */
# define E__STRING(x) #x
# define E_STRING(x) E__STRING(x)

// Inspir du site msdn.microsoft.com.
// Le ' : '  la fin est ncessaire pour que Visual C++ puisse se positionner sur la bonne ligne.
# define __LOC__ __FILE__ "(" E_STRING(__LINE__) ") : "

// Utilisation :
// #pragma message(__LOC__ " : Message")

// Checkpoint.
# define CPq	cio::COut << '(' << tol::TUTime() << ") " __FILE__ ":" E_STRING(__LINE__) << txf::nl << txf::commit


# define E_AUTO_( Name )	\
class Name\
: public Name##_\
{\
public:\
	Name##_::s static_;\
	Name( void )\
	: Name##_( static_ )\
	{\
		reset( false );\
	}\
	~Name( void )\
	{\
		reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_ &S )\
	{\
		Name##_::operator =( S );\
\
		return *this;\
	}

# define E_AUTO( name )\
E_AUTO_( name )\
};


//m Same as 'E_AUTO()' but with one template parameter of type 'Name'
# define E_AUTO1( Name )	\
template < typename t > class Name\
: public Name##_<t>\
{\
public:\
	typename Name##_<t>::s static_;\
	Name ( void )\
	: Name##_<t>( static_ )\
	{\
		Name##_<t>::reset( false );\
	}\
	~Name ( void )\
	{\
		Name##_<t>::reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t> &S )\
	{\
		Name##_<t>::operator =( S );\
\
		return *this;\
	}\
	Name##_<t> &operator *( void )\
	{\
		return *this;\
	}\
	const Name##_<t> &operator *( void ) const\
	{\
		return *this;\
	}\
};


//m Same as 'E_AUTO()' but with two template parameter.
# define E_AUTO2( Name )	\
template < typename t, typename u > class Name\
: public Name##_<t,u>\
{\
public:\
	typename Name##_<t,u>::s static_;\
	Name ( void )\
	: Name##_<t,u>( static_ )\
	{\
		Name##_<t,u>::reset( false );\
	}\
	~Name ( void )\
	{\
		Name##_<t,u>::reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t,u>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t,u> &S )\
	{\
		Name##_<t,u>::operator =( S );\
\
		return *this;\
	}\
	Name##_<t,u> &operator *( void )\
	{\
		return *this;\
	}\
	const Name##_<t,u> &operator *( void ) const\
	{\
		return *this;\
	}\
};

//m Same as 'E_AUTO()' but with three template parameter.
# define E_AUTO3( Name )	\
template < typename t, typename u, typename v > class Name\
: public Name##_<t,u,v>\
{\
public:\
	typename Name##_<t,u,v>::s static_;\
	Name ( void )\
	: Name##_<t,u,v>( static_ )\
	{\
		Name##_<t,u,v>::reset( false );\
	}\
	~Name ( void )\
	{\
		Name##_<t,u,v>::reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t,u,v>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t,u,v> &S )\
	{\
		Name##_<t,u,v>::operator =( S );\
\
		return *this;\
	}\
	Name##_<t,u,v> &operator *( void )\
	{\
		return *this;\
	}\
	const Name##_<t,u,v> &operator *( void ) const\
	{\
		return *this;\
	}\
};

//m Same as 'E_AUTO()' but with four template parameter.
# define E_AUTO4( Name )	\
template < typename t, typename u, typename v, typename w > class Name\
: public Name##_<t,u,v,w>\
{\
public:\
	typename Name##_<t,u,v,w>::s static_;\
	Name ( void )\
	: Name##_<t,u,v,w>( static_ )\
	{\
		Name##_<t,u,v,w>::reset( false );\
	}\
	~Name ( void )\
	{\
		Name##_<t,u,v,w>::reset( true );\
	}\
	Name &operator =( const Name &S )\
	{\
		Name##_<t,u,v,w>::operator =( S );\
\
		return *this;\
	}\
	Name &operator =( const Name##_<t,u,v,w> &S )\
	{\
		Name##_<t,u,v,w>::operator =( S );\
\
		return *this;\
	}\
	Name##_<t,u,v,w> &operator *( void )\
	{\
		return *this;\
	}\
	const Name##_<t,u,v,w> &operator *( void ) const\
	{\
		return *this;\
	}\
};

// Helps to temporary disable the 'system' warning message when 'system' used by underlying library.
# define TOL_SYSTEM_MACRO	use_System_from_tol_library	// To force the use of 'tol::System(...)'.
# define system	TOL_SYSTEM_MACRO

namespace tol {
	class UP__
	{
	protected:
		void *UP_;
	public:
		UP__( void )
		{
			reset( false );
		}
		~UP__( void )
		{
			reset( true );
		}
		void reset( bool = true )
		{
			UP_ = NULL;
		}
		//f Return the current user pointer
		void *UP( void ) const
		{
			return UP_;
		}
		//f 'UP' becomes the new user pointer
		void UP( void *UP )
		{
			UP_ = UP;
		}
	};

	bso::bool__ IsSystemCommandAvailable( void );

	int System( const ntvstr::string___ &Command );

	void Launch( const ntvstr::string___ &Document );	// Ouvre un document (au sens large ; peut p. ex. tre galement une URL).

	//f Return the current date.
	const char *Date( bDate &Buffer );

	//f Return current time.
	const char *Time( bTime &Buffer );

	//f Return current date  time.
	const char *DateAndTime( bDateAndTime &Buffer );

# ifdef TOL__WIN
	bso::size__ GetMemoryUsage( void );
# endif

	inline const char *TUDate( void )
	{
		static buffer__ Buffer;

		return Date( Buffer );
	}

	inline const char *TUTime( void )
	{
		static buffer__ Buffer;

		return Time( Buffer );
	}

	inline const char *TUDateAndTime( void )
	{
		static buffer__ Buffer;

		return DateAndTime( Buffer );
	}

#ifndef CPE_F_MT
	inline const char *Date( void )
	{
	  return TUDate();
	}

	inline const char *Time( void )
	{
	  return TUTime();
	}

	inline const char *DateAndTime( void )
	{
	  return TUDateAndTime();
	}
#endif

	typedef bso::uint__ diff__;
# define TOL_DIFF_MAX			BSO_UINT_MAX
# define TOL_TICK_DIFF_OVERFLOW	TOL_DIFF_MAX

	typedef bso::u32__ coeff__;
# define TOL_COEFF_MAX	BSO_U32_MAX

// Horloge de prcision. N'est utile que pour comparer 2
# ifdef TOL__WIN
	E_TRMIMIC__( LARGE_INTEGER, tick__ );
	extern LARGE_INTEGER	_TickFrequence;

	inline void Reset( tick__ &Tick )
	{
		Tick->QuadPart = 0;
	}

	inline tick__ Tick( void )
	{
		LARGE_INTEGER Counter;

		if ( QueryPerformanceCounter( &Counter ) == 0 )
			qRSys();

		return Counter;
	}

	inline diff__ _Diff(
		tick__ Op1,
		tick__ Op2,
		coeff__ Coeff)
	{
		if ( Op1->QuadPart < Op2->QuadPart )
			qRFwk();

		LONGLONG Diff = ( Coeff * ( Op1->QuadPart - Op2->QuadPart ) ) / _TickFrequence.QuadPart;

		if ( Diff > TOL_DIFF_MAX )
			return TOL_TICK_DIFF_OVERFLOW;

		return (diff__)Diff;
	}

	inline time_t _Time( void )
	{
		timeb T;

		::ftime( &T );

		return T.time;
	}

# elif defined( TOL__DARWIN )
	E_TRMIMIC__( uint64_t, tick__ );
	extern uint64_t _Numer;
	extern uint32_t _Denom;

	inline void Reset( tick__ &Tick )
	{
		*Tick = 0;
	}

	inline tick__ Tick( void )
	{
		return mach_absolute_time();
	}

	inline diff__ _Diff(
		tick__ Op1,
		tick__ Op2,
		coeff__ Coeff)
	{
		if ( *Op1 < *Op2 )
			qRFwk();

		uint64_t Elapsed = *Op1 - *Op2;

		if ( ( TOL_DIFF_MAX / _Numer ) < Elapsed )
			qRLmt();

		return ( Elapsed * _Numer / _Denom  ) / ( 1000000000 / Coeff );
	}

	inline time_t _Time( void )
	{
		return time( NULL );
	}
# elif defined( TOL__POSIX )
	E_TRMIMIC__( timespec, tick__ );

	inline void Reset( tick__ &Tick )
	{
		Tick->tv_nsec = 0;
		Tick->tv_sec = 0;
	}

	inline tick__ Tick( void )
	{
		timespec TP;

		if ( clock_gettime( CLOCK_MONOTONIC, &TP ) != 0 )
			qRLbr();

		return TP;
	}

	inline diff__ _Diff(
		tick__ Op1,
		tick__ Op2,
		coeff__ Coeff)
	{
		tick__ Intermediate;
		bso::uint__ Result = 0;
		bso::bool__ CarryFlag = Op1->tv_nsec < Op2->tv_nsec;
		bso::uint__ Frac = 0;

		if ( Op1->tv_sec < Op2->tv_sec )
			qRFwk();

		if( Op1->tv_sec == Op2->tv_sec )
			if ( Op1->tv_nsec < Op2->tv_nsec )
				qRFwk();

		Intermediate->tv_nsec = ( ( CarryFlag ? 1000000000 : 0 ) + Op1->tv_nsec ) - Op2->tv_nsec;

		Intermediate->tv_sec = Op1->tv_sec - Op2->tv_sec - ( CarryFlag ? 1 : 0 );

		if ( (bso::sUInt)Intermediate->tv_sec > ( BSO_UINT_MAX / Coeff ) )
			return TOL_TICK_DIFF_OVERFLOW;

		Result = Intermediate->tv_sec * Coeff;

		Frac = Intermediate->tv_nsec / ( 1000000000 / Coeff );

		if ( ( Result + Frac  ) > BSO_UINT_MAX )
			return TOL_TICK_DIFF_OVERFLOW;

		return Result + Frac;
	}

	inline time_t _Time( void )
	{
		return time( NULL );
	}
# else
#  error "Unhandled platform !"
# endif

	inline diff__ SecDiff(
		tick__ Op1,
		tick__ Op2 )
	{
		return _Diff( Op1, Op2, 1 );
	}

	inline diff__ MilliSecDiff(
		tick__ Op1,
		tick__ Op2 )
	{
		return _Diff( Op1, Op2, 1000 );
	}

	inline diff__ MicroSecDiff(
		tick__ Op1,
		tick__ Op2 )
	{
		return _Diff( Op1, Op2, 1000000 );
	}

	inline diff__ NanoSecDiff(
		tick__ Op1,
		tick__ Op2 )
	{
		return _Diff( Op1, Op2, 1000000000 );
	}

	inline time_t EpochTime( bso::bool__ Discrimination )	// Mettre 'Discrimination'  'true' pour tre sr que deux
													// appels successifs  cette focntion renvoit deux valeurs diffrentes.
	{
		time_t Time = tol::_Time();

		if ( Discrimination ) {
			while ( Time == tol::_Time() );

			Time = tol::_Time();
		}

		return Time;
	}

	/*f Initialize the random generator using the date & time.
	The used value is returned to be used with the following
	function to make the random generator always start with the same value. */
	inline unsigned int InitializeRandomGenerator( void )
	{
		unsigned int Seed = (unsigned int)time( NULL );

		srand( Seed );

		return Seed;
	}

	//f Initialize the random generator with 'Seed'.
	inline void InitializeRandomGenerator( unsigned int Seed )
	{
		srand( Seed );
	}

	typedef bso::uint__ delay__;

	class timer__
	{
	private:
		delay__ _Delay;
		tick__ _Start;
	public:
		void reset( bso::bool__ = true )
		{
			_Delay = 0;
			Reset( _Start );
		}
		timer__( void )
		{
			reset( false );
		}
		~timer__( void )
		{
			reset( );
		}
		// Timer is dsactivated (i.e. 'IsElapsed()' returns always false) when 'Delay' == 0;
		// If not desactivated, the timer is considred as elapsed until call to 'Launch()'.
		void Init( delay__ Delay )
		{
			Reset( _Start );
			_Delay = Delay;
		}
		void Launch( void )
		{
			_Start = Tick();
		}
		bso::bool__ IsDisabled( void ) const
		{
			return _Delay == 0;
		}
		bso::bool__ IsEnabled( void ) const
		{
			return !IsDisabled();
		}
		diff__ ElapsedTime( void ) const
		{
			return MilliSecDiff( Tick(), _Start );
		}
		bso::bool__ IsElapsed( void ) const
		{
			if ( IsEnabled() )
				return ElapsedTime() >= _Delay;
			else
				return false;
		}
	};

	inline bso::char__ GetLocaleDecimalSeparator( void )
	{
		lconv *LConv = localeconv();

# ifdef CPE_S_ANDROID	// Pas de membre 'decimal_point' dfini dans ce cas...
		qRLmt();
		return 0;	// Pour viter un 'warning'.
# else
		if ( LConv->decimal_point == NULL )
			qRSys();

		if ( LConv->decimal_point[0] == 0 )
			qRSys();

		if ( LConv->decimal_point[1] != 0 )
			qRSys();

		return *LConv->decimal_point;
#endif
	}

	template <typename i> inline void Swap(
		i &I1,
		i &I2 )	// Echange le contenu des entiers I1 et I2. Retourne I1 (avant change).
	{
		I2 = (i)( I1 ^ I2 );
		I1 = (i)( I2 ^ I1 );
		I2 = (i)( I1 ^ I2 );
	}
}

//m Define navigation functions ( 'First', 'Next', Amount', ... ) using 'Object' and 'Type'.
# define E_NAVt( Object, Type )\
	Type First( void ) const\
	{\
		return Object  First();\
	}\
	Type Last( void ) const\
	{\
		return Object Last();\
	}\
	Type Previous( Type P ) const\
	{\
		return Object Previous( P );\
	}\
	Type Next( Type P ) const\
	{\
		return Object Next( P );\
	}\
	sdr::size__ Amount( void ) const\
	{\
		return Object Amount();\
	}\
	sdr::size__ Extent( void ) const\
	{\
		return Object Extent();\
	}\
	bso::bool__ IsEmpty( void ) const\
	{\
		return Object IsEmpty();\
	}\
	bso::bool__ Exists( Type P ) const\
	{\
		return Object Exists( P );\
	}

/* Lorsque 'Object' contient une virgule, on ne peut utiliser E_XNAVt(...), mme en utilisant E_COVER2(...)
car 'E_XNAVt(...)' fait elle-mme appel  une macro. Aussi faudra-t'il explicitement appel 'E_NAVt(...)' et cette macro-ci
pour parvenir au mme rsultat que 'E_XNAVt(...)'. */
# define E_NAVXt( Object, Type )\
	Type First( sdr::size__ Offset ) const\
	{\
		return Object  First( Offset );\
	}\
	Type Last( sdr::size__ Offset ) const\
	{\
		return Object Last( Offset );\
	}\
	Type Previous(\
		Type P,\
		sdr::size__ Offset ) const\
	{\
		return Object Previous( P, Offset );\
	}\
	Type Next(\
		Type P,\
		sdr::size__ Offset ) const\
	{\
		return Object Next( P, Offset  );\
	}

# define E_XNAVt( Object, Type )\
		E_NAVt( Object, Type )\
		E_NAVXt( Object, Type )

# define E_NAV( Object )	E_NAVt( Object, sdr::row__ )
# define E_NAVX( Object )	E_NAVXt( Object, sdr::row__ )
# define E_XNAV( Object )	E_XNAVt( Object, sdr::row__ )

namespace tol {

	template <typename t> E_TTCLONE_( dObject<t>, object_ );
	E_AUTO1( object );

	template <typename t> class _core_pointer___	// Classe de base de gestion d'un pointeur.
	{
	protected:
		t *P_;
	public:
		virtual void reset( bso::bool__ P = true )
		{
			P_ = NULL;
		}
		_core_pointer___( void )
		{
			reset( false );
		}
		virtual ~_core_pointer___( void )
		{
			reset( true );
		}
		void Init( void )
		{
			reset();
		}
		_core_pointer___ &operator =( const _core_pointer___ &Pointer )
		{
			qRFwk();	// Otherwise the same ressource is used twice ; which delete them ?

			return *this;
		}
		t *operator =( t *P )
		{
#ifdef TOL_DBG
			if ( P_ != NULL )
				qRFwk();
#endif

			P_ = P;

			return P_;
		}
		t *operator =( void *P )
		{
			return operator =( (t *)P );
		}
		operator t *( void )
		{
			return P_;
		}
		operator const t *( void ) const
		{
			return P_;
		}
		const t *operator ()( void ) const
		{
			return P_;
		}
		t *operator ()( void )
		{
			return P_;
		}
		const t *operator ->( void ) const
		{
			return P_;
		}
		t *operator ->( void )
		{
			return P_;
		}
		const t &operator *( void ) const
		{
			return *P_;
		}
		t &operator *( void )
		{
			return *P_;
		}
	};

	template <typename t> class free_pointer___	// Classe de gestion d'un pointeur devant tre dsallou par un 'free'.
	: public _core_pointer___<t>
	{
	public:
		virtual void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _core_pointer___<t>::P_ != NULL )
					free( _core_pointer___<t>::P_ );

			_core_pointer___<t>::reset( P );
		}
		t *operator =( t *P )
		{
			reset();

			return _core_pointer___<t>::operator =( P );
		}
		t *operator =( void *P )
		{
			reset();

			return _core_pointer___<t>::operator =( P );
		}
	};

# define E_FPOINTER___( t )	free_pointer___<t>

	template <typename t> class delete_pointer___	// Classe de gestion d'un pointeur devant tre dallou par un 'delete'.
	: public _core_pointer___<t>
	{
	public:
		virtual void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _core_pointer___<t>::P_ != NULL )
					delete _core_pointer___<t>::P_;

			_core_pointer___<t>::reset( P );
		}
		t *operator =( t *P )
		{
			reset();

			return _core_pointer___<t>::operator =( P );
		}
		t *operator =( void *P )
		{
			reset();

			return _core_pointer___<t>::operator =( P );
		}
	};

# define E_DPOINTER___( t )	delete_pointer___<t>

# define TOL__ERRP	TOL_ERRP_
	/*
		Les mthodes 'virtual' ne sont pas destines  tre surcharge par une quelconque classe mre, mais  viter des problmes lorsque cet objet est partag entre une application et une bibliothque dynamique.
		En effet, un pointeur allou dans une bibliothque dynamique peut poser problme s'il est dsallou dans l'application, et inversement. En utilisant des mthodes virtuelles pour l'allocation et le dsallocation
		proprement dites, cela permet de s'assurer que l'allocation et la dsallocation est toujours faite du cot de celui qui instanci l'objet, quelque soit le cot duquel est lance la mthode en appelent l'une ou l'autre
		des mthodes virtuelles.
	*/

# define buffer___	hBuffer

# define E_BUFFER___( t )	buffer___<t>
# define TOL_CBUFFER___ tol::E_BUFFER___( bso::char__ )

}

template <typename type, type False, type Error, type Undefined> bso::bool__ operator==(
	tol::extended_enum__<type, False, Error, Undefined> Op1,
	tol::extended_enum__<type, False, Error, Undefined> Op2 )
{
	return Op1.Value() == Op2.Value();
}

template <typename type, typename _type, type False, type Error, type Undefined> bso::bool__ operator==(
	_type Op1,
	tol::extended_enum__<type, False, Error, Undefined> Op2 )
{
	return Op1 == Op2.Value();
}

template <typename type, typename _type, type False, type Error, type Undefined> bso::bool__ operator==(
	tol::extended_enum__<type, False, Error, Undefined> Op1,
	_type Op2 )
{
	return Op1.Value() == Op2;
}

template <typename type, type False, type Error, type Undefined> bso::bool__ operator!=(
	tol::extended_enum__<type, False, Error, Undefined> Op1,
	tol::extended_enum__<type, False, Error, Undefined> Op2 )
{
	return Op1.Value() != Op2.Value();
}

template <typename type, typename _type, type False, type Error, type Undefined> bso::bool__ operator!=(
	_type Op1,
	tol::extended_enum__<type, False, Error, Undefined> Op2 )
{
	return Op1 != Op2.Value();
}

template <typename type, typename _type, type False, type Error, type Undefined> bso::bool__ operator!=(
	tol::extended_enum__<type, False, Error, Undefined> Op1,
	_type Op2 )
{
	return Op1.Value() != Op2;
}

#if 0
	//f Free 'Pointer' only if != NULL. Pointer value becomes 'NULL'.
	template <typename t> inline void Free( t *&Pointer )
	{
		if ( Pointer != NULL ) {
			free( (void *)Pointer );
			Pointer = NULL;
		}
	}
#endif

# define E_CTOR( name )		qCTOR( name )

# define E_DTOR( name )		qDTOR( name )

# define E_VDTOR( name )	qVDTOR( name )

# define E_CDTOR( name )	qCDTOR( name )

# define E_CVDTOR( name )	qCVDTOR( name )


//d Make accessible the static member, for read-only access, of a dynamic object, named 'name' of type 'type__'.
# define E_RRODISCLOSE_(type__, name )\
	const type__ &Get##name( void ) const\
	{\
		return S_.name;\
	}


//d Make accessible the static member, for read-only access, of a dynamic object, named 'name' of type 'type__'.
# define E_RODISCLOSE_(type__, name )\
	E_RRODISCLOSE_( type__, name )\
	const type__ &name( void ) const\
	{\
		return S_.name;\
	}

# define E_RWODISCLOSE_(type__, name )\
	void Set##name( const type__ &V )\
	{\
		S_.name = V;\
	}

# define E_WODISCLOSE_(type__, name )\
	E_RWODISCLOSE_( type__, name )\
	type__ &name( void )\
	{\
		return S_.name;\
	}

//d Make accessible the static member, for read-write access, of a dynamic object, named 'name' of type 'type__'.
# define E_RRWDISCLOSE_(type__, name )\
	E_RRODISCLOSE_( type__, name )\
	E_RWODISCLOSE_( type__, name )

//d Make accessible the static member, for read-write access, of a dynamic object, named 'name' of type 'type__'.
# define E_RWDISCLOSE_(type__, name )\
	E_RODISCLOSE_( type__, name )\
	E_WODISCLOSE_( type__, name )



# define E_RRODISCLOSE__(type__, name )\
	const type__ &Get##name( void ) const\
	{\
		return _##name;\
	}

//d Make accessible the member, for read-only access, of a static object, named 'name' of type 'type__'.
# define E_RODISCLOSE__(type__, name )\
	E_RRODISCLOSE__( type__, name )\
	const type__ &name( void ) const\
	{\
		return _##name;\
	}

# define E_RWODISCLOSE__(type__, name )\
	void Set##name( const type__ &V )\
	{\
		_##name = V;\
	}

# define E_WODISCLOSE__(type__, name )\
	E_RWODISCLOSE__( type__, name )\
	type__ &name( void )\
	{\
		return _##name;\
	}

# define E_RRWDISCLOSE__(type__, name )\
	E_RRODISCLOSE__( type__, name )\
	E_RWODISCLOSE__( type__, name )

# define E_RWDISCLOSE__(type__, name )\
	E_RODISCLOSE__( type__, name )\
	E_WODISCLOSE__( type__, name )

#if 0
# define Q37_GCTOR( discriminator )\
	__attribute__( ( constructor ) ) static void discriminator##_q37ctor( void )
#else
# define Q37_GCTOR( discriminator )\
	namespace {\
		class discriminator##_q37gctor\
		{\
		public:\
			discriminator##_q37gctor( void );\
		};\
		\
		discriminator##_q37gctor discriminator##_Q37gctor;\
	}\
	\
	discriminator##_q37gctor::discriminator##_q37gctor( void )
#endif

#if 0
# define Q37_GDTOR( discriminator )\
	__attribute__( ( destructor ) ) static void discriminator##_q37gdtor( void )
#else
# define Q37_GDTOR( discriminator )\
	namespace  {\
	class discriminator##_q37gdtor\
		{\
		public:\
			~discriminator##_q37gdtor( void );\
		};\
		\
		discriminator##_q37gdtor discriminator##_Q37gdtor;\
	}\
	\
	discriminator##_q37gdtor::~discriminator##_q37gdtor( void )
#endif

// Pointer Method DeFinition.
#define Q37_PMDF( type, name, variable )\
	type *name( void ) const\
	{\
		if ( variable == NULL )\
			qRFwk();\
\
		return variable;\
	}

// Reference Method DeFinition.
#define Q37_RMDF( type, name, variable )\
	type &name( void ) const\
	{\
		if ( variable == NULL )\
			qRFwk();\
\
		return *variable;\
	}

// Member and pointer method definitions.
#define Q37_MPMDF( type, name, variable )\
	type *variable;\
	Q37_PMDF( type, name, variable )

// Member and reference method definitions.
#define Q37_MRMDF( type, name, variable )\
	type *variable;\
	Q37_RMDF( type, name, variable )

/***************/
/***** NEW *****/
/***************/

/* 2 */
# define qHOOKS2D_( t1, n1, t2, n2 )\
		t1 &n1##_;\
		t2 &n2##_

# define qHOOKS2P_( t1, n1, t2, n2 )\
		t1 &n1,\
		t2 &n2

# define qHOOKS2C_( t1, n1, t2, n2 )\
		n1##_( n1 ),\
	    n2##_( n2 )

# define qHOOKS2( t1, n1, t2, n2 )\
	struct sHooks\
	{\
	qHOOKS2D_( t1, n1, t2, n2 );\
	public:\
		sHooks( qHOOKS2P_( t1, n1, t2, n2 ) )\
		: qHOOKS2C_( t1, n1, t2, n2 )\
		{}\
	}

/* 3 */
# define qHOOKS3D_( t1, n1, t2, n2, t3, n3 )\
		qHOOKS2D_( t1, n1, t2, n2 );\
		t3 &n3##_

# define qHOOKS3P_( t1, n1, t2, n2, t3, n3 )\
		qHOOKS2P_( t1, n1, t2, n2 ),\
		t3 &n3

# define qHOOKS3C_( t1, n1, t2, n2, t3, n3 )\
		qHOOKS2C_( t1, n1, t2, n2 ),\
	    n3##_( n3 )

# define qHOOKS3( t1, n1, t2, n2, t3, n3 )\
	struct sHooks\
	{\
	qHOOKS3D_( t1, n1, t2, n2, t3, n3 );\
	public:\
		sHooks( qHOOKS3P_( t1, n1, t2, n2, t3, n3 ) )\
		: qHOOKS3C_( t1, n1, t2, n2, t3, n3 )\
		{}\
	}

/* 4 */
# define qHOOKS4D_( t1, n1, t2, n2, t3, n3, t4, n4 )\
		qHOOKS2D_( t1, n1, t2, n2 );\
		qHOOKS2D_( t3, n3, t4, n4 )

# define qHOOKS4P_( t1, n1, t2, n2, t3, n3, t4, n4 )\
		qHOOKS2P_( t1, n1, t2, n2 ),\
		qHOOKS2P_( t3, n3, t4, n4 )

# define qHOOKS4C_( t1, n1, t2, n2, t3, n3, t4, n4 )\
		qHOOKS2C_( t1, n1, t2, n2 ),\
		qHOOKS2C_( t3, n3, t4, n4 )

# define qHOOKS4( t1, n1, t2, n2, t3, n3, t4, n4 )\
	struct sHooks\
	{\
	qHOOKS4D_( t1, n1, t2, n2, t3, n3, t4, n4 );\
	public:\
		sHooks( qHOOKS4P_( t1, n1, t2, n2, t3, n3, t4, n4 ) )\
		: qHOOKS4C_( t1, n1, t2, n2, t3, n3, t4, n4 )\
		{}\
	}

/* 5 */
# define qHOOKS5D_( t1, n1, t2, n2, t3, n3, t4, n4, t5, n5 )\
		qHOOKS4D_( t1, n1, t2, n2, t3, n3, t4, n4 );\
		t5 &n5##_

# define qHOOKS5P_( t1, n1, t2, n2, t3, n3, t4, n4, t5, n5 )\
		qHOOKS4P_( t1, n1, t2, n2, t3, n3, t4, n4 ),\
		t5 &n5

# define qHOOKS5C_( t1, n1, t2, n2, t3, n3, t4, n4, t5, n5 )\
		qHOOKS4C_( t1, n1, t2, n2, t3, n3, t4, n4 ),\
	    n5##_( n5 )

# define qHOOKS5( t1, n1, t2, n2, t3, n3, t4, n4, t5, n5 )\
	struct sHooks\
	{\
	qHOOKS5D_( t1, n1, t2, n2, t3, n3, t4, n4, t5, n5 );\
	public:\
		sHooks( qHOOKS5P_( t1, n1, t2, n2, t3, n3, t4, n4, t5, n5 ) )\
		: qHOOKS5C_( t1, n1, t2, n2, t3, n3, t4, n4, t5, n5 )\
		{}\
	}




# define qNAVl( Object )	E_NAV( Object )
# define qNAV( Object, Type )	E_NAVt( Object, Type )

// Predefinition.

namespace tol {
	typedef delay__ sDelay;
	typedef timer__ sTimer;
	typedef tick__ sTick;
	typedef time_t sTimeStamp;

	void Crash( void );	// Crashes deliberately the program. For testing of daemons watchdog.


	/* BEGIN 'HaveSameAmount' serialization */

	template <typename a, typename b> inline bso::sBool HaveSameAmount(
		const a &A,
		const b &B )
	{
		return A.Amount() == B.Amount();
	}

	template <typename a, typename b, typename ... o> inline bso::sBool HaveSameAmount(
		const a &A,
		const b &B,
		const o&... O )
	{
		if ( HaveSameAmount( A, B ) )
			return HaveSameAmount( B, O... );
		else
			return false;
	}

	/* END 'HaveSameAmount' serialization */

	/* BEGIN 'reset' serialization with parameter */

	template <typename t> inline void reset(
		bso::sBool P,
		t &O )
	{
		O.reset( P );
	}

	template <typename t> inline void reset(
		bso::sBool,
		t *&Pointer )
	{
		Pointer = NULL;
	}

	inline void reset(
		bso::sBool,
		bso::sBool &Boolean )
	{
		Boolean = false;
	}

	// Last arguments handled first : arguments has to be passed in the same order they were declared !
	template <typename f, typename ... o> inline void reset(
		bso::sBool P,
		f &F,
		o&... O )
	{
		reset( P, O... );
		reset( P, F );
	}

	/* END 'reset' serialization with parameter */

	/* BEGIN 'reset' serialization without parameter */

	template <typename t> void inline reset( t &O )
	{
		O.reset();
	}

	template <typename t> inline void reset( t *&Pointer )
	{
		Pointer = NULL;
	}

	// Last arguments handled first : arguments has to be passed in the same order they were declared !
	template <typename f, typename ... o> inline void reset(
		f &F,
		o&... O )
	{
		reset( O... );
		reset( F );
	}

	/* END 'reset' serialization without parameter */

	/* BEGIN 'plug' serialization */

	template <typename t> inline void plug(
		ags::aggregated_storage_ *AS,
		t &O )
	{
		O.plug( AS );
	}

	template <typename f, typename ... o> inline void plug(
		ags::aggregated_storage_ *AS,
		f &F,
		o&... O )
	{
		plug( AS, F );
		plug( AS, O... );
	}

	/* END 'plug' serialization */

	/* BEGIN 'Init' serialization */

	template <typename t> inline void Init( t &O )
	{
		O.Init();
	}

	template <typename f, typename ... o> inline void Init(
		f &F,
		o&... O )
	{
		Init( F );
		Init( O... );
	}

	/* End 'Init' serialization */

	bso::sBool EnvExists(const str::dString &Name);

	bso::sBool EnvExists(const char *Name);

	// 'false' if env var doesn't exist.
	bso::sBool GetEnv(
		const str::dString &Name,
		str::dString &Value );

	// 'false' if env var doesn't exist.
	bso::sBool GetEnv(
		const char *Name,
		str::dString &Value );

  // Returns true if in dev environment.
  inline bso::sBool IsDev(void)
  {
    return EnvExists("Q37_EPEIOS");
  }

	template <typename row, typename container> inline row Search(
		const str::dString &ID,
		const container &Container )
	{
		row Row = Container.First();

		// '-1' because 'qNIL' is not available.
		while ( ( Row != (row)-1 ) && ( Container( Row ).ID() != ID ) )
			Row = Container.Next( Row );

		return Row;
	}


	template <typename arg> inline arg Same_(
		arg Arg1,
		arg Arg2 )
	{
		if ( Arg1 != Arg2 )
			qRFwk();

		return Arg1;
	}

	template <typename arg, typename... args> inline arg Same_(
		arg Arg1,
		arg Arg2,
		args... Args )
	{
		return Same_( Same_( Arg1, Arg2 ), Args... );
	}

	// Issues an error if all arguments are not of same value, else returns this value.
	template <typename arg, typename... args> inline arg Same(
		arg Arg,
		args... Args )
	{
		return Same_( Arg, Args... );
	}
}


#endif
