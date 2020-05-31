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

//D Console Input/Output

#ifndef CIO_INC_
# define CIO_INC_

# define CIO_NAME		"CIO"

# if defined( E_DEBUG ) && !defined( CIO_NODBG )
#  define CIO_DBG
# endif

# include "err.h"
# include "iof.h"
# include "txf.h"
# include "cpe.h"
# include "flx.h"

namespace cio {

	extern iop::descriptor__ CInDescriptor, COutDescriptor, CErrDescriptor;

	namespace {
		template <typename d> d &_TestAndGet( d* D )
		{
			if ( D == NULL )
				qRFwk();

			return *D;
		}
	}

	qENUM( Type )
	{
		tTerminal,	// Read from/write to a terminal (mainly standard input/output).
		tVoid,		// No reading available, and write is lost (useful for windows services).
		tUser,		// Read from/write to user defined services ('C(Out|In|Err)Driver' have to be initialized).
		tCurrent = tUser,
		t_amount,
		t_Undefined,
		t_Default = tTerminal
	};

	class set__
	{
	private:
		eType Type_;
		fdr::iflow_driver_base___ *_In;
		fdr::oflow_driver_base___
			*_Out,
			*_Err;
	public:
		void reset( bso::bool__ = true )
		{
			Type_ = t_Undefined;
			_In = NULL;
			_Out = NULL;
			_Err = NULL;
		}
		E_CDTOR( set__ );
		set__(
			fdr::iflow_driver_base___ &In,
			fdr::oflow_driver_base___ &Out,
			fdr::oflow_driver_base___ &Err,
			eType Type )
		{
			reset( false );

			Init( In, Out, Err, Type );
		}
		void Init(
			fdr::iflow_driver_base___ &In,
			fdr::oflow_driver_base___ &Out,
			fdr::oflow_driver_base___ &Err,
			eType Type )
		{
			_In = &In;
			_Out = &Out;
			_Err = &Err;
			Type_ = Type;
		}
		eType Type(void) const
		{
			return Type_;
		}
		fdr::iflow_driver_base___ &In( void ) const
		{
			return _TestAndGet( _In );
		}
		fdr::oflow_driver_base___ &Out( void ) const
		{
			return _TestAndGet( _Out );
		}
		fdr::oflow_driver_base___ &Err( void ) const
		{
			return _TestAndGet( _Err );
		}
	};

	const set__ &GetConsoleSet( void );	// Returns a set to read/write to/from console. NOT ALWAYS USABLE !

	const set__ &GetVoidSet( void );	// Returns a set to read/write to/from void.

	const set__ &GetCurrentSet( void );	// Returns current set.

	class cof___
	: public flw::oflow__
	{
	private:
		flw::byte__ _Cache[IOP__BUFFER_SIZE];
	public:
		void Init( fdr::oflow_driver_base___ &Driver )
		{
			oflow__::Init( Driver, _Cache, sizeof( _Cache ) );
		}
		void Init(
			fdr::oflow_driver_base___ &,
			fdr::thread_safety__ )
				// Une ancienne version de cette bibliothque avait une mthode avec ces paramtres. Un appel  cette mthode appelait la mthode ci-dessus, ce qui provoque une erreur.
				// Cette mthode a donc t mise en palce pour dtecter et corrger ce cas de figure...
		{
			qRFwk();
		}
	};


	class cif__
	: public flw::standalone_iflow__<>
	{
	public:
		void Init( fdr::iflow_driver_base___ &Driver )
		{
			standalone_iflow__::Init( Driver );
		}
		void Init(
			fdr::oflow_driver_base___ &Driver,
			fdr::thread_safety__ )
				// Une ancienne version de cette bibliothque avait une mthode avec ces paramtres. Un appel  cette mthode appelait la mthode ci-dessus, ce qui provoque une erreur.
				// Cette mthode a donc t mise en place pour dtecter et corrger ce cas de figure...
		{
			qRFwk();
		}
		void Init(
			fdr::iflow_driver_base___ &,
			fdr::thread_safety__ )
				// Idem que ci-dessus.
		{
			qRFwk();
		}
	};

	// 'thread safe'.
	extern cif__ CInF;
	extern cof___ COutF, CErrF;


	// 'thread unsafe'.
	extern txf::text_iflow__ CIn;
	extern txf::text_oflow__ COut, CErr;

	const set__ &GetSet(eType Type);

		void Initialize( eType Type );

	void Initialize( const set__ &Set );

	eType Type( void );

	inline bso::bool__ IsInitialized( void )
	{
		return Type() != t_Undefined;
	}

	fdr::rRDriver &GetInDriver( void );
	fdr::rWDriver &GetOutDriver( void );
	fdr::rWDriver &GetErrDriver( void );
}

#endif
