/*
	Copyright (C) 2016 Claude SIMON (http://zeusw.org/epeios/contact.html).

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

#ifndef MUAIMF__INC
# define MUAIMF__INC

# ifdef XXX_DBG
#	define MUAIMF__DBG
# endif

# include "muabsc.h"

# include "lstbch.h"
# include "lstcrt.h"

namespace muaimf {
	qROW( SRow_ );	// String row.

	typedef lstcrt::qLMCRATEd( str::dString, sSRow_ ) dStrings_;

	class sField_ {
	public:
		sSRow_
			Name,
			Body;
		void reset( bso::sBool P = true )
		{
			Name = Body = qNIL;
		}
		qCDTOR( sField_ );
		void Init( void )
		{
			Name = Body = qNIL;
		}
	};

	qROW( FRow );	// Field row.

	typedef lstbch::qLBUNCHd( sField_, sFRow ) dFields_;

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
			const str::dString &Body )
		{
			sField_ Field;

			Field.Init();

			Field.Name = Append_( Name );
			Field.Body = Append_( Body );

			return Fields.Add( Field );
		}
		sFRow Search( const str::dString &Name ) const;
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
