/*
	Copyright (C) 2015-2016 Claude SIMON (http://q37.info/contact/).

	This file is part of 'orgnzq' software.

    'orgnzq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'orgnzq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'orgnzq'.  If not, see <http://www.gnu.org/licenses/>.
*/

// OrGaniZer DaTuM

#ifndef OGZDTA__INC
# define OGZDTA__INC

# ifdef XXX_DBG
#	define OGZDTA__DBG
# endif

# include "ogzbsc.h"
# include "ogztyp.h"

# include "bch.h"
# include "lstctn.h"

namespace ogzdta {
	using ogzbsc::dDatum;
	using ogzbsc::wDatum;

	qROW( Row );

	class cData {
	protected:
		// If 'Row' != 'qNIL', it must be used.
		virtual sRow OGZDTANew(
			ogztyp::sRow Type,	// If == 'qNIL', it's the mandatory text type.
			sRow Row ) = 0;
		// if 'Row' == 'qNIL', the entire content must be erased.
		virtual void OGZDTADelete( sRow Row ) = 0;
		virtual void OGZDTAStore(
			const dDatum &Datum,
			ogztyp::sRow Type,	// If == 'qNIL', it's the mnadatory text type.
			sRow Row )= 0;
		virtual void OGZDTARecall(
			sRow Row,
			ogztyp::sRow Type,	// If == 'qNIL', it's the mnadatory text type.
			dDatum &Datum ) = 0;
	public:
		qCALLBACK( Data );
		void Wipe( void )
		{
			OGZDTADelete( qNIL );
		}
		sRow New(
			ogztyp::sRow Type,	// If == 'qNIL', it's the mandatory text type.
			sRow Row = qNIL )
		{
			return OGZDTANew( Type, Row );
		}
		void Delete( sRow Row )
		{
			OGZDTADelete( Row );
		}
		void Store(
			sRow Row,
			ogztyp::sRow Type,
			const dDatum &Datum )
		{
			OGZDTAStore( Datum, Type, Row );
		}
		void Recall(
			sRow Row,
			ogztyp::sRow Type,
			dDatum &Datum )
		{
			return OGZDTARecall( Row, Type, Datum );
		}
	};

	class sData
	{
	private:
		qRMV( cData, C_, Callback_ );
	public:
		void reset( bso::bool__ = true )
		{
			Callback_ = NULL;
		}
		qCDTOR( sData );
		void Init( cData &Callback )
		{
			Callback_ = &Callback;
		}
		void Wipe( void ) const
		{
			C_().Delete( qNIL );
		}
		sRow New(
			ogztyp::sRow Type,	// If == 'qNIL', it's the mandatory text type.
			sRow Row = qNIL )
		{
			return C_().New( Type, Row );
		}
		void Delete( sRow Row ) const
		{
			C_().Delete( Row );
		}
		void Store(
			const dDatum &Datum,
			ogztyp::sRow Type,	// If == 'qNIL', it's the mnadatory text type.
			sRow Row ) const
		{
			C_().Store( Row, Type, Datum );
		}
		void Recall(
			sRow Row,
			ogztyp::sRow Type,
			dDatum &Datum ) const
		{
			if ( Row != qNIL )
				return C_().Recall( Row, Type, Datum );
		}
	};

	typedef str::dString dBytes;
	qW( Bytes );

	class rRegularCallback
	: public cData
	{
	private:
		lstctn::qLMCONTAINERw( dBytes, sRow ) Container_;
	protected:
		virtual sRow OGZDTANew(
			ogztyp::sRow Type,
			sRow Row ) override 
		{
			Row = Container_.New( Row );

			Container_(Row).Init();
			Container_.Flush();

			return Row;
		}
		virtual void OGZDTADelete( sRow Row ) override 
		{
			if ( Row == qNIL )
				Container_.reset();
			else
				Container_.Remove( Row );
		}
		virtual void OGZDTAStore(
			const dDatum &Datum,
			ogztyp::sRow Type,
			sRow Row ) override 
		{
			Container_.Store( Datum, Row );
		}
		virtual void OGZDTARecall(
			sRow Row,
			ogztyp::sRow Type,
			dDatum &Datum ) override 
		{
			Container_.Recall( Row, Datum );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Container_.reset( P );
		}
		qCVDTOR( rRegularCallback );
		void Init( void )
		{
			Container_.Init();
		}
	};

	using bch::cHook;
}


#endif
