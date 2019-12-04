/*
	Copyright (C) 2016-2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'MMUAq' software.

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

// MUA Internet Mail Format (RFC 5322)

#ifndef MUAIMF_INC_
# define MUAIMF_INC_

# ifdef XXX_DBG
#	define MUAIMF__DBG
# endif

# include "muabsc.h"

# include "lstbch.h"
# include "lstcrt.h"

namespace muaimf {

	qENUM( Field ) {
		fSubject,
		f_FirstOptional,
		fTo = f_FirstOptional,
		fFrom,
		fMessageId,
		fDate,
		f_amount,
		f_Unknown,	// Not a known field.
		f_Undefined
	};

	const char *GetLabel( eField Field );

	eField GetField( const str::dString &Label );

	// NOT thread-safe.
	const char *GetWording( eField Field );

	qROW( SRow_ );	// String row.

	class sField_ {
	public:
		eField Field;
		sSRow_
			Name,
			Body;
		void reset( bso::sBool P = true )
		{
			Field = f_Undefined;
			Name = Body = qNIL;
		}
		qCDTOR( sField_ );
		void Init( void )
		{
			Field = f_Undefined;
			Name = Body = qNIL;
		}
	};

	qROW( FRow );	// Field row.

	typedef bch::qBUNCHdl( sFRow ) dFRows;
	qW( FRows );

	typedef lstbch::qLBUNCHd( sField_, sFRow ) dFields_;

	typedef lstcrt::qLMCRATEd( str::dString, sSRow_ ) dStrings_;

	class dFields
	{
	private:
		sSRow_ Append_( const str::dString &Entry )
		{
			sSRow_ Row = Strings.New();

			Strings( Row ).Init( Entry );
			Strings.Flush();

			return Row;
		}
	public:
		struct s {
			dStrings_::s Strings;
			dFields_::s Fields;
		};
		dStrings_ Strings;
		dFields_ Fields;
		dFields( s &S )
		: Strings( S.Strings ),
		  Fields( S.Fields )
		{}
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Strings, Fields );
		}
		void plug( qASd *AS )
		{
			tol::plug( AS, Strings, Fields );
		}
		dFields &operator =(const dFields &F)
		{
			Strings = F.Strings;
			Fields = F.Fields;

			return *this;
		}
		void Init( void )
		{
			tol::Init( Strings, Fields );
		}
		sFRow Append(
			const str::dString &Name,
			const str::dString &Body );
		void Search(
			eField Field,
			dFRows &Rows ) const;
		sFRow Search( eField Field ) const;
		eField GetField( sFRow Row ) const
		{
			return Fields( Row ).Field;
		}
		const str::dString &GetName(
			sFRow Row,
			str::dString &Value ) const 
		{
			Strings.Recall( Fields( Row ).Name, Value );

			return Value;
		}
		const str::dString &GetBody(
			sFRow Row,
			str::dString &Value ) const 
		{
			Strings.Recall( Fields( Row ).Body, Value );

			return Value;
		}
		qNAV( Fields., sFRow );
	};

	qW( Fields );

	typedef dFields dHeader;
	qW( Header );

	const str::dString &GetField(
		eField Field,
		fdr::rIDriver &Driver,
		str::dString &Body );

	void Fill(
		flw::sIFlow &Flow,
		dFields &Fields );

	void Fill(
		fdr::rIDriver &Driver,
		dFields &Fields );

	void Dump(
		const dFields &Fields,
		txf::rOFlow Flow );

	void Dump(
		const dFields &Fields,
		fdr::rODriver &Driver );
}


#endif
