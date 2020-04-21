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

// Hypertext Transfert Protocol

#ifndef HTP_INC_
# define HTP_INC_

#define HTP_NAME		"HTP"

# if defined( E_DEBUG ) && !defined( HTP_NODBG )
#  define HTP_DBG
# endif

# include "err.h"
# include "flw.h"
# include "bso.h"
# include "str.h"
# include "txf.h"
# include "crt.h"

namespace htp {
	qENUM( Status ) {
		//i OK (HTTP code 200).
		sOK,
		//i Continue (HTTP code 100)
		sContinue,
		//i NotFound (HTTP code 404).
		sFound,
		//i Found (HTTP code 302).
		sNotFound,
		//i Unhandle HTTP response code.
		sUnhandledCode,
		//i Not HTTP protocol or bad version.
		sUnknowProtocol,
		//i Amount of status.
		s_amount,
		s_Undefined,
	};

	qENUM( Field ) {
		fURL,
		fHost,
		fReferrer,
		fAccept,
		fContentType,
		fContentLength,
		fTransferEncoding,
		fLocation,
		fConnection,
		f_amount,
		f_Undefined
	};

	typedef str::dString	dValue;
	typedef str::wString	wValue;

	using str::sHook;

	class dField {
	public :
		struct s {
			eField Name;
			dValue::s Value;
		} &S_;
		dValue Value;
		dField( s &S )
		: S_( S ),
		  Value( S.Value )
		{}
		void reset( bso::bool__ P = true )
		{
			Value.reset( P );
			S_.Name = f_Undefined;
		}
		void plug( sHook &Hook )
		{
			Value.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Value.plug( AS );
		}
		dField &operator =( const dField &F )
		{
			Value = F.Value;
			S_.Name = F.S_.Name;

			return *this;
		}
		void Init( void )
		{
			S_.Name = f_Undefined;
			Value.Init();
		}
		void Init(
			eField Name,
			const str::string_ &Value )
		{
			S_.Name = Name;
			this->Value.Init( Value );
		}
		void Init(
			eField Name,
			const char *Value )
		{
			S_.Name = Name;
			this->Value.Init( Value );
		}
		E_RODISCLOSE_( eField, Name )
	};

	qW( Field );

	typedef crt::qCRATEdl( dField )	dFields;
	qW( Fields );

	typedef bso::sUInt sContentLength;

	typedef fdr::iflow_driver___<> rIFlowDriver;

	class rBlocFlow
	: public rIFlowDriver
	{
	private:
		fdr::size__ Size_;	// Size of current chunk.
		qRMV( flw::sRFlow, F_, Flow_ );
	protected:
		fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			fdr::size__ Size = 0;

			if ( Size_ == 0 )
				return 0;

			Size = F_().ReadUpTo( Size_ > Maximum ? Maximum : Size_, Buffer );

			if ( Size == 0 )
				qRFwk();

			Size_ -= Size;

			return Size;
		}
		void FDRDismiss( bso::sBool Unlock ) override
		{
			if ( Size_ != 0 )
				qRGnr();

			F_().Dismiss( Unlock );
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			 return F_().IDriver().RTake( Owner );
		}

	public:
		void reset( bso::sBool P = true )
		{
			rIFlowDriver::reset( P );
			Flow_ = NULL;
		}
		qCDTOR( rBlocFlow );
		void Init(
			fdr::size__ Size,
			flw::iflow__ &Flow )
		{
			Size_ = Size;
			Flow_ = &Flow;

			rIFlowDriver::Init( fdr::ts_Default );
		}
	};

	class rChunkFlow
	: public rIFlowDriver
	{
	private:
		fdr::size__ Size_;	// Size of current chunk.
		qRMV( flw::sRFlow, F_, Flow_ );
		bso::sBool GetSize_( void  );
	protected:
		fdr::size__ FDRRead(
			fdr::size__ Maximum,
			fdr::byte__ *Buffer ) override
		{
			fdr::size__ Size = 0;

			if ( Size_ == 0 )
				if ( !GetSize_() )
					return 0;

			Size = F_().ReadUpTo( Size_ > Maximum ? Maximum : Size_, Buffer );

			if ( Size == 0 )
				qRFwk();

			Size_ -= Size;

			if ( Size_ == 0 )
				F_().Skip( 2 );	// To remove the NL.

			return Size;
		}
		void FDRDismiss( bso::sBool Unlock ) override
		{
			if ( Size_ != 0 )
				qRGnr();

			F_().Dismiss( Unlock );
		}
		virtual fdr::sTID FDRRTake( fdr::sTID Owner ) override
		{
			 return F_().IDriver().RTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			Size_ = 0;
			rIFlowDriver::reset( P );
			Flow_ = NULL;
		}
		qCDTOR( rChunkFlow );
		void Init( flw::iflow__ &Flow )
		{
			Size_ = 0;
			Flow_ = &Flow;

			rIFlowDriver::Init( fdr::ts_Default );
		}
	};

	class rHeader
	{
	private:
		rBlocFlow BlocFlow_;
		rChunkFlow ChunkFlow_;
		flw::sDressedRFlow<> Flow_;
		void FillField_( flw::iflow__ &IFlow );
		bso::sU16 StatusCode_;
	public:
		str::wString Location;
		str::wString ContentType;
		void reset( bso::sBool P = true )
		{
			BlocFlow_.reset( P );
			ChunkFlow_.reset( P );
			Flow_.reset( P );
			Location.reset( P );
			ContentType.reset( P );
			StatusCode_ = 0;
		}
		qCDTOR( rHeader );
		void Init( void )
		{
			Location.Init();
			ContentType.Init();
			StatusCode_ = 0;
			// Other members initialized when needed.
		}
		eStatus Parse( flw::iflow__ &IFlow );
		flw::iflow__ &Flow( void )
		{
			return Flow_;
		}
		qRODISCLOSEr( bso::sU16, StatusCode );
	};

	qENUM( Method )
	{
		mPost,
		mGet,
		mPut,
		mDelete,
		m_amount,
		m_Undefined
	};

	const char *GetLabel( eMethod Method );

	eMethod GetMethod( const str::dString &Pattern );

	void Send(
		eMethod Method,
		const str::string_ &URL,
		const dFields &Fields,
		const str::string_ &Content,
		txf::text_oflow__ &Flow );
}

#endif
