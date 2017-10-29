/*
	Copyright (C) 2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'ZNDq'.

    'ZNDq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'ZNDq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'ZNDq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

#include "registry.h"
#include "zndq.h"

#include "n4znd.h"
#include "n4allw.h"
#include "sclmisc.h"
#include "sclargmnt.h"

using namespace main;

namespace{
	err::err___ Error_;
	sclerror::rError SCLError_;
	scllocale::rRack Locale_;
	sclmisc::sRack Rack_;

	void ERRFinal_( void )
	{
		sclmisc::ErrFinal();
	}


	void GetInfo_( str::dString &Info )
	{
	qRH
		flx::rStringOFlow BaseFlow;
		txf::sOFlow Flow;
	qRB
		BaseFlow.Init( Info );
		Flow.Init( BaseFlow );

		Flow << sclmisc::SCLMISCProductName << " v" << VERSION << " - PHP v" << PHP_VERSION << ", API v" <<  PHP_API_VERSION
#ifdef CPE_S_WIN
			<< ", Compiler ID: " PHP_COMPILER_ID
#endif
			<< txf::nl << txf::pad << "Build : " __DATE__ " " __TIME__ " (" <<  cpe::GetDescription() << ')';
	qRR
	qRT
	qRE
	}
}

// To avoid inclusion of 'err.h' in the caller source file.
void main::ThrowGenericError( void )
{
	qRGnr();
}

void main::Init( void )
{
qRFH;
	str::wString Location;
qRFB;
	cio::Initialize( cio::GetConsoleSet() );
	Rack_.Init( Error_, SCLError_, cio::GetSet( cio::t_Default ), Locale_ );

	Location.Init();
	// TODO : Find a way to fill 'Location' with the path of the binary.


	sclmisc::Initialize( Rack_, Location );
qRFR;
qRFT;
qRFE( ERRFinal_() );
}

namespace {
	namespace {
		n4znd::gShared Shared_;
	}

	void Register_( const str::dString &Arguments )
	{
	qRH;
		str::wString ComponentFilename;
		bso::sBool SkipComponentUnloading = false;	// When at 'true', the component is unloading when program quitting, not explicitly.
	qRB;
		sclargmnt::FillRegistry( Arguments, sclargmnt::faIsArgument, sclargmnt::uaReport );

		ComponentFilename.Init();
		sclmisc::MGetValue( registry::parameter::ComponentFilename, ComponentFilename );

#ifdef CPE_S_GNULINUX
		SkipComponentUnloading = true;	// Temporary workaround (I hope) to avoid 'SegFault' under 'GNU/Linux'.
#endif

		n4allw::Register( ComponentFilename, dlbrry::nPrefixAndExt, Rack_, &Shared_, SkipComponentUnloading );
	qRR;
	qRT;
	qRE;
	}
}

void main::Register(
	const char *RawArguments,
	size_t Length )
{
qRFH;
	str::wString Arguments;
qRFB;
	Arguments.Init();

	Arguments.Append( RawArguments, Length );

	Register_( Arguments );
qRFR;
qRFT;
qRFE( ERRFinal_() );
}

const char *main::WrapperInfo( void )
{
	static char Buffer[1000];
qRH;
	str::wString Info;
qRB;
	Info.Init();
	GetInfo_( Info );

	if ( Info.Amount() >= sizeof( Buffer ) )
		qRLmt();

	Info.Recall( Info.First(), Info.Amount(), Buffer );
	Buffer[Info.Amount()] = 0;
qRR;
qRT;
qRE;
	return Buffer;
}

const char *main::ComponentInfo( void )
{
	static char Buffer[1000];
qRFH;
	str::wString Info;
qRFB;
	Info.Init();

	if ( !n4allw::GetLauncherInfo( Info ) )
		sclmisc::GetBaseTranslation( "NoRegisteredComponent", Info );

	if ( Info.Amount() >= sizeof( Buffer ) )
		qRLmt();

	Info.Recall( Info.First(), Info.Amount(), Buffer );
	Buffer[Info.Amount()] = 0;
qRFR;
qRFT;
qRFE( ERRFinal_() );
	return Buffer;
}

namespace {
	typedef n4all::cCaller cCaller_;

	void Get_(
		zval &Val,
		str::dString &String )
	{
		if ( Z_TYPE( Val ) != IS_STRING )
			qRGnr();

		String.Append( Z_STRVAL( Val ), Z_STRLEN( Val ) );
	}

	typedef fdr::rIODressedDriver rDriver_;

	class rStream_
	: public rDriver_
	{
	private:
		qPMV( php_stream, S_, Stream_ );
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte *Buffer ) override
		{
			if ( php_stream_eof( S_() ) )
				return 0;
			else
				return php_stream_read( S_(), (char *)Buffer, Maximum );
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			// Nothing to do !
		}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			// Nothing to do !

			return THT_UNDEFINED_THREAD_ID;
		}
		virtual fdr::sSize FDRWrite(
			const fdr::sByte *Buffer,
			fdr::sSize Maximum ) override
		{
			return php_stream_write( S_(), (char *)Buffer, Maximum );
		}
		virtual void FDRCommit( bso::sBool Unlock ) override
		{
			php_stream_flush( S_() );
		}
		virtual fdr::sTID FDROTake( fdr::sTID Owner ) override
		{
			// Nothing to do !

			return THT_UNDEFINED_THREAD_ID;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rDriver_::reset( P );
			Stream_ = NULL;
		}
		qCVDTOR( rStream_ );
		void Init( php_stream *Stream )
		{
			rDriver_::Init( fdr::ts_Default );
			Stream_ = Stream;
		}
	};

	void Get_(
		zval &Val,
		fdr::rIODriver *&Driver )
	{
	qRH;
		php_stream *RawStream = NULL;
		rStream_ *Stream = NULL;
	qRB;
		if ( Z_TYPE( Val ) != IS_RESOURCE )
			qRGnr();

		php_stream_from_zval_no_verify( RawStream, &Val );

		Stream = new rStream_;

		if ( Stream == NULL )
			qRAlc();

		Stream->Init( RawStream );

		Driver = Stream;
	qRR;
		if ( Stream != NULL )
			delete Stream;
	qRT;
	qRE;
	}

	void Get_(
		zval &Val,
		bso::sS64 &Long )
	{
		if ( Z_TYPE( Val ) != IS_LONG )
			qRGnr();

		Long = Z_LVAL( Val );
	}

	void Get_(
		zval &Val,
		bso::sBool &Bool )
	{
		if ( Z_TYPE( Val ) == IS_TRUE )
			Bool = true;
		else if ( Z_TYPE( Val ) == IS_FALSE )
			Bool = false;
		else
			qRGnr();
	}

	template <typename type> inline void Get_(
		zval *varargs,
		int Index,
		void *Value )
	{
		return Get_( varargs[Index], *(type *)Value );
	}

	class sCaller_
	: public cCaller_
	{
	private:
		void ***tsrm_ls_;
		int num_varargs_;
		zval *varargs_;
		zval *return_value_;
	protected:
		virtual void N4ALLGetArgument(
			bso::sU8 Index,
			n4all::sType Type,
			void *Value ) override
		{
			if ( Index >= num_varargs_ )
				qRGnr();

			switch ( Type ) {
			case n4znd::tString:
				Get_<str::dString>( varargs_, Index, Value );
				break;
			case n4znd::tStream:
				Get_<fdr::rIODriver *>( varargs_, Index, Value );
				break;
			case n4znd::tLong:
				Get_<bso::sS64>( varargs_, Index, Value );
				break;
			case n4znd::tBool:
				Get_<bso::sBool>( varargs_, Index, Value );
				break;
			default:
				qRGnr();
				break;
			}
		}
		virtual void N4ALLSetReturnValue(
			n4all::sType Type,
			const void *Value ) override
		{
		qRH;
			qCBUFFERr Buffer;
		qRB;
			switch ( Type ) {
			case n4znd::tString:
				ZVAL_STRING( return_value_, ( ( str::dString * )Value )->Convert( Buffer ) );
				break;
			case n4znd::tLong:
				ZVAL_LONG( return_value_, *( bso::sS64 * )Value );
				break;
			case n4znd::tBool:
				ZVAL_BOOL( return_value_, *( bso::sBool * )Value );
				break;
			default:
				qRGnr();
				break;
			}

		qRR;
		qRT;
		qRE;
		}
	public:
		void reset( bso::sBool P = true )
		{
			tsrm_ls_ = NULL;
			num_varargs_ = 0;
			varargs_ = NULL;
			return_value_ = NULL;
		}
		qCDTOR( sCaller_ );
		void Init(
			void ***tsrm_ls,
			int num_varargs,
			zval *varargs,
			zval *return_value )
		{
			tsrm_ls_ = tsrm_ls;
			num_varargs_ = num_varargs;
			varargs_ = varargs;
			return_value_ = return_value;
		}
	};

}

void main::Launch(
	zend_long Index,
	int num_varargs,
	zval *varargs,
	zval *return_value TSRMLS_DC )
{
qRFH;
	sCaller_ Caller;
qRFB;
#ifdef ZTS
//	Caller.Init( trsrm_ls, num_varargs, varargs );	// 'tsrm_ls' no more available in 'ZTS' !?
	Caller.Init( NULL, num_varargs, varargs, return_value );	// 'tsrm_ls' no more available in 'ZTS' !?
#else
	Caller.Init( NULL, num_varargs, varargs, return_value );	// 'tsrm_ls' no more available in 'ZTS' !?
#endif
	n4allw::Launch( Index, Caller );
qRFR;
qRFT;
qRFE( ERRFinal_() );
}

const char *sclmisc::SCLMISCTargetName = NAME_LC;
const char *sclmisc::SCLMISCProductName = NAME_MC;

qGCTOR( njsq )
{
	Error_.Init();
	SCLError_.Init();
	Locale_.Init();
};
