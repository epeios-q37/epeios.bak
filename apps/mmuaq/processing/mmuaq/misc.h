/*
	Copyright (C) 2016-2017 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq.

    'MMUAq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'MMUAq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'MMUAq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MISC__INC
# define MISC__INC

# include "cio.h"
# include "csdbnc.h"
# include "fdr.h"
# include "flw.h"
# include "flx.h"
# include "rgstry.h"

namespace misc {
	inline void Dump( fdr::rIDriver &Driver )
	{
	qRH
		flw::sDressedIFlow<> Flow;
	qRB
		Flow.Init( Driver );

		while ( !Flow.EndOfFlow() )
			cio::COut << Flow.Get();
	qRR
	qRT
	qRE
	}

	inline void Dump(
		fdr::rIDriver &Driver,
		str::dString &Content )
	{
	qRH
		flx::rStringODriver ODriver;
	qRB
		ODriver.Init( Content, fdr::ts_Default );

		fdr::Copy( Driver, ODriver );
	qRR
	qRT
	qRE
	}

	typedef fdr::rIODressedDriver rIODriver_;

	bso::sBool IsVerboseActivated( void );

	class rVerboseIODriver
	: public rIODriver_
	{
	private:
		csdbnc::rIODriver Driver_;
		bso::sBool Activated_;
		bso::sBool Commited_;
	protected:
		virtual fdr::sSize FDRRead(
			fdr::sSize Maximum,
			fdr::sByte *Buffer ) override
		{
			return Driver_.Read( Maximum, Buffer, fdr::bNonBlocking );
		}
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{
			return Driver_.Dismiss( Unlock );
		}
		virtual fdr::sTID FDRITake( fdr::sTID Owner )
		{
			return Driver_.ITake( Owner );
		}
		virtual fdr::sSize FDRWrite(
			const fdr::sByte *Buffer,
			fdr::sSize Maximum ) override
		{
			if ( Activated_ ) {
				if ( Commited_ ) {
					cio::COut << "-> ";
					Commited_ = false;
				}
			}

			Maximum = Driver_.Write( Buffer, Maximum );

			if ( Activated_ && ( Maximum != 0 ) )
				cio::COutF.Write( Buffer, Maximum );

			return Maximum;
		}
		virtual void FDRCommit( bso::sBool Unlock ) override
		{
			Driver_.Commit( Unlock );
			Commited_ = true;
		}
		virtual fdr::sTID FDROTake( fdr::sTID Owner ) override
		{
			return Driver_.OTake( Owner );
		}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Driver_, Activated_, Commited_ );
			rIODriver_::reset( P );
		}
		qCVDTOR( rVerboseIODriver );
		void Init(
			const rgstry::rEntry &HostPortEntry,
			bso::sBool Activate );
		bso::sBool IsConnected( void )
		{
			return Driver_.IsConnected();
		}
	};
}

#endif
