/*
	'tol.h' by Claude SIMON (http://zeusw.org/).

	'tol' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TOL__INC
# define TOL__INC

# define TOL_NAME		"TOL"

# if defined( E_DEBUG ) && !defined( TOL_NODBG )
#  define TOL_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// TOoL


# include <time.h>
# include <signal.h>
# include <locale.h>

# include "cpe.h"

# ifdef CPE_POSIX
#  ifdef CPE_XCODE
#   define TOL__MAC
# else
#   define TOL__POSIX
# endif
# elif defined( CPE_WIN )
#  define TOL__WIN
#  include "Windows.h"
# else
#  error "Undefined compilation enviroment."
# endif


# ifdef TOL__WIN
#  include <sys/utime.h>
# elif defined( TOL__MAC )
#  include <mach/mach_time.h>
# elif defined( TOL__POSIX )
#  include <utime.h>
# else
#  error
# endif

#if defined( CPE_VC ) || defined( CPE_GCC )
#	include <sys/timeb.h>
#else
#	error "Unknown compiler"
#endif

#include "err.h"

# include "bso.h"

namespace tol
{
	typedef char buffer__[20];
}


#ifdef CPE__VC
#	ifdef CPE__WARNING_SUPPRESSION_ENABLED
#		pragma warning( disable : 4284 )
#	endif
#endif

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

// Similaire à un 'typedef type alias', sauf que 'type' et 'alias' ne sont pas interchangeable.
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

// Similaire à un 'T_MIMIC__( type, alias )', mais sans opérateurs de comparaisons.
# define E_TRMIMIC__( type, alias )\
struct alias\
{\
	E__TMIMIC__( type, alias )\
}


// Similaire à 'E_TMIMIC__( type, alias)', mais pour des objets dynamiques.
// La version instanciable et la version '_' sont toutes deux créees.
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


// Pour les objets dynamiques, similaire à 'typedef type alias', mais
// avec création de la version instanciable et de la version '_'.
# define E_TCLONE( type, alias )\
	typedef type##_ alias##_;\
	typedef type alias;


// Similaire à 'typedef type alias', mais permet la mise en oeuvre de 'template'.
// Pour objets statiques uniquement.
// Exemple d'utilisation.
// template <typename r> E_TTCLONE__( tree_seeker__<r>, seeker__ );
# define E_TTCLONE__( type, alias )\
	class alias\
	: public type\
	{\
	}


// Similaire à 'typedef type  alias', mais permet la mise en oeuvre de 'template'.
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


// Similaire à 'E_TTCLONE_( type ,alias )', mais crée la version instanciable en plus de la version '_'.
// ATTENTION : 'type' doit être la version AVEC '_', 'alias' la version SANS '_'.
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

/* Permet de transformer 2 arguments en 1; si un argument d'une macro
a besoin de contenir une virgule, cette macro est là pour ça
'E_COVER2( a, b )' donne 'a, b' */
# define E_COVER2(a, b)	a, b

/* Permet de convertir un entier en chaîne (l'encadre par des '"').
Utile pour afficher le numéro de ligne dans un #pragma message (...). */
# define E__STRING(x) #x
# define E_STRING(x) E__STRING(x)

// Inspiré du site msdn.microsoft.com.
// Le ' :' à la fin est nécessaire pour que Visual C++ puisse se positionner sur la bonne ligne.
# define __LOC__ __FILE__ "(" E_STRING(__LINE__) ") :"

// Utilisation :
// #pragma message(__LOC__ " : Message")




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



/*********************************************/
/* CLASSE DE GESTION DU POINTEUR UTILISATEUR */
/*********************************************/

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


	//f Return the current date.
	const char *Date( buffer__ &Buffer );

	//f Return current time.
	const char *Time( buffer__ &Buffer );

	//f Return current date é time.
	const char *DateAndTime( buffer__ &Buffer );


#ifndef CPE__MT
	inline const char *Date( void )
	{
		static buffer__ Buffer;

		return Date( Buffer );
	}

	inline const char *Time( void )
	{
		static buffer__ Buffer;

		return Time( Buffer );
	}

	inline const char *DateAndTime( void )
	{
		static buffer__ Buffer;

		return DateAndTime( Buffer );
	}
#endif

	typedef bso::uint__ diff__;
# define TOL_DIFF_MAX			BSO_UINT_MAX
# define TOL_TICK_DIFF_OVERFLOW	TOL_DIFF_MAX

	typedef bso::u32__ coeff__;
# define TOL_COEFF_MAX	BSO_U32_MAX

// Horloge de précision. N'est utile que pour comparer 2 
# ifdef TOL__WIN
	E_TRMIMIC__( LARGE_INTEGER, tick__ );
	extern LARGE_INTEGER	_TickFrequence;

	inline tick__ Tick( void )
	{
		LARGE_INTEGER Counter;

		if ( QueryPerformanceCounter( &Counter ) == 0 )
			ERRSys();

		return Counter;
	}

	inline diff__ _Diff(
		tick__ Op1,
		tick__ Op2,
		coeff__ Coeff)
	{
		if ( Op1->QuadPart < Op2->QuadPart )
			ERRPrm();

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

# elif  defined( TOL__MAC )
	E_TRMIMIC__( uint64_t, tick__ );
	extern uint64_t _Numer;
	extern uint32_t _Denom;

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
			ERRPrm();

		uint64_t Elapsed = *Op1 - *Op2;

		if ( ( TOL_DIFF_MAX / _Numer ) < Elapsed )
			ERRLmt();

		return ( Elapsed * _Numer / _Denom  ) / ( 1000000000 / Coeff );
	}

	inline time_t _Time( void )
	{
		return time( NULL );
	}
# elif defined( TOL__POSIX )
	E_TRMIMIC__( timespec, tick__ );

	inline tick__ Tick( void )
	{
		timespec TP;

		if ( clock_gettime( CLOCK_MONOTONIC, &TP ) != 0 )
			ERRLbr();

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
			ERRPrm();

		if( Op1->tv_sec == Op2->tv_sec )
			if ( Op1->tv_nsec < Op2->tv_nsec )
				ERRPrm();

		Intermediate->tv_nsec = ( ( CarryFlag ? 1000000000 : 0 ) + Op1->tv_nsec ) - Op2->tv_nsec;

		Intermediate->tv_sec = Op1->tv_sec - Op2->tv_sec - ( CarryFlag ? 1 : 0 );

		if ( Intermediate->tv_sec > ( BSO_UINT_MAX / Coeff ) )
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

	inline time_t EpochTime( bso::bool__ Discrimination )	// Mettre 'Discrimination' à 'true' pour être sûr que deux
													// appels successifs à cette focntion renvoit deux valeurs différentes.
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
		}
		timer__( void )
		{
			reset( false );
		}
		~timer__( void )
		{
			reset( );
		}
		void Init( delay__ Delay )
		{
			_Start = Tick();
			_Delay = Delay;
		}
		void Launch( void )
		{
			_Start = Tick();
		}
		bso::bool__ IsElapsed( void ) const
		{
			return MilliSecDiff( Tick(), _Start ) >= _Delay;
		}
	};

	inline bso::char__ GetLocaleDecimalSeparator( void )
	{
		lconv *LConv = localeconv();

# ifdef CPE_ANDROID	// Pas de membre 'decimal_point' défini dans ce cas...
		ERRLmt();
		return 0;	// Pour éviter un 'warning'.
# else
		if ( LConv->decimal_point == NULL )
			ERRSys();

		if ( LConv->decimal_point[0] == 0 )
			ERRSys();

		if ( LConv->decimal_point[1] != 0 )
			ERRSys();

		return *LConv->decimal_point;
#endif
	}

	template <typename i> inline void Swap(
		i &I1,
		i &I2 )	// Echange le contenu des entiers I1 et I2. Retourne I1 (avant échange).
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
	
/* Lorsque 'Object' contient une virgule, on ne peut utiliser E_XNAVt(...), même en utilisant E_COVER2(...)
car 'E_XNAVt(...)' fait elle-même appel à une macro. Aussi faudra-t'il explicitement appelé 'E_NAVt(...)' et cette macro-ci
pour parvenir au même résultat que 'E_XNAVt(...)'. */
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
	}\
	void Allocate(\
		sdr::size__ Size,\
		aem::mode__ Mode = aem::m_Default )\
	{\
		Object Allocate( Size, Mode );\
	}

# define E_XNAVt( Object, Type )\
		E_NAVt( Object, Type )\
		E_NAVXt( Object, Type )

# define E_NAV( Object )	E_NAVt( Object, sdr::row__ )
# define E_NAVX( Object )	E_NAVXt( Object, sdr::row__ )
# define E_XNAV( Object )	E_XNAVt( Object, sdr::row__ )

// PRédéclaration.
namespace ags {
	class aggregated_storage_;
}

namespace sdr {
	struct row__;
}

namespace tol {

	// A basic object 't' becomes a normal object.
	template <class t, typename r = sdr::row__> class object_
	{
	public:
		struct s
		{
			t Object;
		} &S_;
		object_( s &S )
		: S_( S )
		{}
		void reset( bool = true )
		{
			// Pour des raisons de standardisation
		}
		void plug( class ags::aggregated_storage_ & )
		{
			// Pour des raisons de standardisation.
		}
		object_ &operator =( const object_ &O )
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
		operator t( void )
		{
			return S_.Object;
		}
	};

	E_AUTO2( object )

#if 0

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
			ERRFwk();	// Otherwise the same ressource is used twice ; which delete them ?

			return *this;
		}
		t *operator =( t *P )
		{
#ifdef TOL_DBG
			if ( P_ != NULL )
				ERRFwk();
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

	template <typename t> class free_pointer___	// Classe de gestion d'un pointeur devant être désalloué par un 'free'.
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

	template <typename t> class delete_pointer___	// Classe de gestion d'un pointeur devant être déalloué par un 'delete'.
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

#endif
# define TOL__ERRP	err::handling__ ErrHandling = err::h_Default

	template <typename t> class buffer___ // Gestion d'un 'buffer' d'objets de type 't' de taille dynamique. Sa taille ne diminue jamais.
	{
	private:
		t *_P;
		bso::size__ _Size;
		bso::bool__ _Allocate(
			bso::size__ Size,
			err::handling__ ErrHandling )
		{
			if ( _Size < Size ) {
				void *P = realloc( _P, Size * sizeof( t ) );

				if ( P == NULL ) {
					if ( ErrHandling == err::hThrowException )
						ERRAlc();

					return false;
				} else 
					_P = (t *)P;

				_Size = Size;
			}

			return true;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				if ( _P!= NULL )
					free( _P );

			_P = NULL;
				_Size = 0;
		}
		buffer___( void )
		{
			reset( false );
		}
		virtual ~buffer___ ( void )
		{
			reset();
		}
		void Init( void )
		{
			ERRFwk();	// C'est un 'buffer' ; pas d'initailisation.
		}
		t *Malloc(
			bso::size__ Amount,
			TOL__ERRP )
		{
			if ( !_Allocate( Amount, ErrHandling ) )
				return NULL;

			return _P;
		}
		t *Calloc(
			bso::size__ Amount,
			TOL__ERRP )
		{
			if ( !_Allocate( Amount, ErrHandling ) )
				return NULL;

			memset( _P, 0, Amount );

			return _P;
		}
		t *Realloc(
			bso::size__ Size,
			TOL__ERRP )
		{
			if ( !_Allocate( Size, ErrHandling ) )
				return NULL;

			return _P;
		}
		bso::size__ Size( void ) const
		{
			return _Size;
		}
		operator t*( void ) const
		{
			return _P;
		}
	};

# define E_BUFFER___( t )	buffer___<t>

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

// Déclaration de constructeur standardisé.
# define E_CTORD( name )\
	name( void )

// Corps ('body') d'un constructeur standardisé.
# define E_CTORB\
	{\
		reset( false );\
	}\

// Création d'un constructeur standardisé.
# define E_CTOR( name )\
	E_CTORD( name )\
	E_CTORB

// Création d'un destructeur standardisé.
# define E_DTOR( name )\
	~name( void )\
	{\
		reset();\
	}\

// Création d'un destructeur virtuel standardisé.
# define E_VDTOR( name )	virtual E_DTOR( name )

// Création d'un constructeur et d'un destructeur standardisé.
# define E_CDTOR( name )\
	E_CTOR( name )\
	E_DTOR( name )

// Création d'un constructeur et d'un destructeur virtuel standardisé.
# define E_CVDTOR( name )\
	E_CTOR( name )\
	E_VDTOR( name )


//d Make accessible the static member, for read-only access, of a dynamic object, named 'name' of type 'type__'.
# define E_RRODISCLOSE_(type__, name )\
	const type__ Get##name( void ) const\
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
	const type__ Get##name( void ) const\
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

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
