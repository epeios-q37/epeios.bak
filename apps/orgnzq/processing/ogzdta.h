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

# include "bch.h"
# include "lstctn.h"

namespace ogzdta {
	using ogzbsc::fSize;

	using ogzbsc::fDRow;
	using ogzbsc::fTRow;
	using ogzbsc::fByte;
	using ogzbsc::vDatum;
	using ogzbsc::iDatum;

	class fCallback {
	protected:
		// If 'Row' != 'qNIL', it must be used.
		virtual fDRow OGZDTANew(
			fTRow Type,
			fDRow Row ) = 0;
		// if 'Row' == 'qNIL', the entire content must be erased.
		virtual void OGZDTADelete( fDRow Row ) = 0;
		virtual void OGZDTAStore(
			fDRow Row,
			fTRow Type,
			const vDatum &Datum ) = 0;
		virtual void OGZDTARecall(
			fDRow Row,
			fTRow Type,
			vDatum &Datum ) = 0;
	public:
		void reset( bso::bool__ = true )
		{
			//Standardisation.
		}
		qCVDTOR( fCallback );
		void Init( void )
		{
			// Standardisation.
		}
		void Wipe( void )
		{
			OGZDTADelete( qNIL );
		}
		fDRow New(
			fTRow Type,
			fDRow Row = qNIL )
		{
			return OGZDTANew( Type, Row );
		}
		void Delete( fDRow Row )
		{
			OGZDTADelete( Row );
		}
		void Store(
			fDRow Row,
			fTRow Type,
			const vDatum &Datum )
		{
			OGZDTAStore( Row, Type, Datum );
		}
		void Recall(
			fDRow Row,
			fTRow Type,
			vDatum &Datum )
		{
			return OGZDTARecall( Row, Type, Datum );
		}
	};

	class fData
	{
	private:
		qRVM( fCallback, C_, Callback_ );
	public:
		void reset( bso::bool__ = true )
		{
			Callback_ = NULL;
		}
		qCDTOR( fData );
		void Init( fCallback &Callback )
		{
			Callback_ = &Callback;
		}
		void Wipe( void ) const
		{
			C_().Delete( qNIL );
		}
		fDRow New(
			fTRow Type,
			fDRow Row = qNIL ) const
		{
			return C_().New( Type, Row );
		}
		void Delete( fDRow Row ) const
		{
			C_().Delete( Row );
		}
		void Store(
			fDRow Row,
			fTRow Type,
			const vDatum &Datum ) const
		{
			C_().Store( Row, Type, Datum );
		}
		void Recall(
			fDRow Row,
			fTRow Type,
			vDatum &Datum ) const
		{
			return C_().Recall( Row, Type, Datum );
		}
	};

	typedef bch::qBUNCHvl( ogzdta::fByte ) vBytes;
	qW( Bytes );

	class rRegularCallback
	: public fCallback
	{
	private:
		lstctn::qLMCONTAINERi( vBytes, ogzdta::fDRow ) Container_;
	protected:
		virtual ogzdta::fDRow OGZDTANew(
			ogzdta::fTRow Type,
			ogzdta::fDRow Row ) override 
		{
			Row = Container_.New( Row );

			Container_(Row).Init();
			Container_.Flush();

			return Row;
		}
		virtual void OGZDTADelete( ogzdta::fDRow Row ) override 
		{
			if ( Row == qNIL )
				Container_.reset();
			else
				Container_.Remove( Row );
		}
		virtual void OGZDTAStore(
			ogzdta::fDRow Row,
			ogzdta::fTRow Type,
			const vDatum &Datum ) override 
		{
			Container_.Store( Datum, Row );
		}
		virtual void OGZDTARecall(
			ogzdta::fDRow Row,
			ogzdta::fTRow Type,
			ogzdta::vDatum &Datum ) override 
		{
			Container_.Recall( Row, Datum );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			fCallback::reset( P );
			Container_.reset( P );
		}
		qCVDTOR( rRegularCallback );
		void Init( void )
		{
			fCallback::Init();
			Container_.Init();
		}
	};

	typedef bch::qBUNCHvl( fDRow ) vDatumList;
	qW( DatumList );
}


#endif
