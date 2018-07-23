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

// CoNVersion Flow DRiver

#ifndef CNVFDR_INC_
# define CNVFDR_INC_

# define CNVFDR_NAME		"CNVFDR"

# if defined( E_DEBUG ) && !defined( CNVFDR_NODBG )
#  define CNVFDR_DBG
# endif

# include "err.h"
# include "fdr.h"
# include "flw.h"
# include "flx.h"

namespace cnvfdr {

	class cConverter
	{
	protected:
		virtual void CNVFDRConvert(
			flw::sRFlow &In,
			fdr::sSize InMax,	// Maximum amount of data available in 'In'. There can be less, so you have also to take care of EOF. Not all data must be red, but at least one.
			flw::sWFlow &Out,
			fdr::sSize OutMax )	// Amount of which can be written in 'Out'. At least one character must be written.
		{
			qRGnr();
		}
		virtual void CNVFDRConvert(
			fdr::rRDriver &In,
			fdr::sSize InMax,	// Maximum amount of data available in 'In'. There can be less, so you have also to take care of EOF. Not all data must be red, but at least one.
			fdr::rWDriver &Out,
			fdr::sSize OutAmount )	// Amount of which can be written in 'Out'. At least one character must be written.
		{
		qRH;
			flw::sDressedRFlow<> FIn;
			flw::sDressedWFlow<> FOut;
		qRB;
			FIn.Init( In );
			FOut.Init( Out );

			CNVFDRConvert( FIn, InMax, FOut, OutAmount );

			// 'FIn' and 'FOut' are only used to ease the handling of 'In' and 'Out'. All data may not be red/written, so avoid that the internal counter are reseted, 'false' is given as parameter below.
			FIn.Dismiss( false );
			FOut.Commit( false );
		qRR;
		qRT;
		qRE;
		}
	public:
		qCALLBACK( Converter );
		virtual void Convert(
			fdr::rRDriver &In,
			fdr::sSize InMax,	// Maximum amount of data available in 'In'. There can be less, so you have also to take care of EOF. Not all data must be red, but at least one.
			fdr::rWDriver &Out,
			fdr::sSize OutAmount )	// Amount of which can be written in 'Out'. At least one character must be written.
		{
			return CNVFDRConvert( In, InMax, Out, OutAmount );
		}
	};

	template <typename idriver, typename edriver> class sDriver_
	: public edriver
	{
	protected:
		qRMV( cConverter, C_, Converter_ );
		qRMV( idriver, D_, Driver_ );
		fdr::thread_safety__ ThreadSafety_;
	public:
		void reset( bso::sBool P = true )
		{
			edriver::reset( P );
			Converter_ = NULL;
			Driver_ = NULL;
			ThreadSafety_ = fdr::ts_Undefined;
		}
		qCVDTOR( sDriver_ );
		void Init(
			cConverter &Converter,
			idriver &Driver,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			Converter_ = &Converter;
			Driver_ = &Driver;
			ThreadSafety_ = ThreadSafety;
			edriver::Init( ThreadSafety );
		}
	};

	template <typename flow, typename driver> class rFlow_
	: public flow
	{
	public:
		void Init(
			cConverter &Converter,
			driver &Driver,
			fdr::thread_safety__ ThreadSafety = fdr::ts_Default )
		{
			flow::Driver_.Init( Converter, Driver, ThreadSafety );
			flow::subInit();
		}
	};

	class rConverterRDriver
	: public sDriver_<fdr::rRDriver, fdr::rRDressedDriver>
	{
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte *Buffer ) override
		{
		qRH;
			flx::buffer_oflow_driver___ Out;
		qRB;
			Out.Init( Buffer, ThreadSafety_, Maximum );

			C_().Convert( D_(), FDR_SIZE_MAX, Out, Maximum );

			Maximum = Out.AmountWritten();
		qRR;
		qRT;
		qRE;
			return Maximum;
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			return D_().Dismiss( Unlock );
		}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return D_().ITake( Owner );
		}
	};

	typedef rFlow_<flw::rDressedRFlow<rConverterRDriver>, fdr::rRDriver> rConverterRFlow;

	class rConverterWDriver
	: public sDriver_<fdr::rWDriver, fdr::rWDressedDriver>
	{
	protected:
		virtual fdr::sSize FDRWrite(
			const fdr::sByte *Buffer,
			fdr::sSize Maximum ) override
		{
		qRH;
			flx::buffer_iflow_driver___ In;
		qRB;
			In.Init( Buffer, ThreadSafety_, Maximum );

			C_().Convert( In, Maximum, D_(), FDR_SIZE_MAX );

			Maximum = In.AmountRed();
		qRR;
		qRT;
		qRE;
			return Maximum;
		}
		// Returns 'false' when underlying write fails, 'true' otherwise.
		virtual void FDRCommit( bso::sBool Unlock ) override
		{
			D_().Commit( Unlock );
		}
		virtual fdr::sTID FDROTake( fdr::sTID Owner ) override
		{
			return D_().OTake( Owner );
		}
	};

	typedef rFlow_<flw::rDressedWFlow<rConverterWDriver>, fdr::rWDriver> rConverterWFlow;
}

#endif
