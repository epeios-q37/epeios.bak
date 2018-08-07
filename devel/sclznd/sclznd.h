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

// SoCLe ZeND (PHP)

#ifndef SCLZND_INC_
# define SCLZND_INC_

# define SCLZND_NAME		"SCLZND"

# if defined( E_DEBUG ) && !defined( SCLZND_NODBG )
#  define SCLZND_DBG
# endif

# include "bso.h"
# include "cpe.h"
# include "err.h"
# include "n4znd.h"
# include "scln4a.h"
# include "tol.h"

namespace sclznd {
	typedef scln4a::sCaller sCaller_;
# ifdef CPE_F_32BITS
	typedef bso::sS32 sLong;
# endif

# ifdef CPE_F_64BITS
	typedef bso::sS64 sLong;
# endif

	class sCaller
	: public sCaller_ {
	private:
		bso::sU8 Index_;
		void Get_( str::dString &String )
		{
			C_().GetArgument( Index_++, n4znd::tString, &String );
		}
		void Get_( sLong &Long )
		{
			C_().GetArgument( Index_++, n4znd::tLong, &Long );
		}
		void Get_( bso::sBool &Bool )
		{
			C_().GetArgument( Index_++, n4znd::tBool, &Bool );
		}
		void Get_( str::dStrings &Strings )
		{
			C_().GetArgument( Index_++, n4znd::tStrings, &Strings );
		}
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
		void Get( str::dString &String )
		{
			Get_( String );
		}
		fdr::rRWDriver *GetStream( void )
		{
			fdr::rRWDriver *Driver = NULL;

			C_().GetArgument( Index_++, n4znd::tStream, &Driver );

			return Driver;
		}
		void Get( sLong &Long )
		{
			Get_( Long );
		}
		void Get( bso::sBool &Bool )
		{
			Get_( Bool );
		}
		void Get( str::dStrings &Strings )
		{
			Get_( Strings );
		}
		// Termination method for below method.
		void Get( void )
		{}
		template <typename arg, typename ...args> void Get(
			arg &Arg,
			args&... Args )
		{
			Get_( Arg );
			Get( Args... );
		}
		void SetReturnValue( const str::dString &String )
		{
			C_().SetReturnValue( n4znd::tString, &String );
		}
		void SetReturnValue( sLong Long )
		{
			C_().SetReturnValue( n4znd::tLong, &Long );
		}
		void SetReturnValue( bso::sBool Bool )
		{
			C_().SetReturnValue( n4znd::tBool, &Bool );
		}
		void SetReturnValue( const str::dStrings &Strings )
		{
			C_().SetReturnValue( n4znd::tStrings, &Strings );
		}
	};

	typedef void ( fFunction )( sCaller &Caller );

	typedef scln4a::sRegistrar<fFunction> sRegistrar;

	typedef fdr::rRWDressedDriver rDriver_;

	class rStreamDriver
	: public rDriver_
	{
	private:
		qRMV( fdr::rRWDriver, D_, Driver_ );
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte *Buffer ) override
		{
			return D_().Read( Maximum, Buffer, fdr::bNonBlocking );
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			D_().Dismiss( Unlock );
		}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return D_().ITake( Owner );
		}
		virtual fdr::sSize FDRWrite(
			const fdr::sByte *Buffer,
			fdr::sSize Maximum ) override
		{
			return D_().Write( Buffer, Maximum );
		}
		virtual void FDRCommit( bso::sBool Unlock ) override
		{
			D_().Commit( Unlock );
		}
		virtual fdr::sTID FDROTake( fdr::sTID Owner ) override
		{
			return D_().OTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			rDriver_::reset( P );

			if ( P ) {
				if ( Driver_ != NULL )
					delete Driver_;
			}

			Driver_ = NULL;
		}
		qCVDTOR( rStreamDriver );
		void Init( sCaller &Caller )
		{
			reset();

			rDriver_::Init( fdr::ts_Default );
			Driver_ = Caller.GetStream();
		}
	};

	inline void Die( const str::dString &Message )
	{
		qRVct();
	}

	void SCLZNDInfo( txf::sWFlow &Flow );	// To define by user.
	const scli::sInfo &SCLZNDRegister( sRegistrar &Registrar );	// To define by user
}

# define SCLZND_F( name ) void name( sclznd::sCaller &Caller )

#endif
