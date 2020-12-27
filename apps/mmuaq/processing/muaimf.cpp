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

#include "muaimf.h"

#include "stsfsm.h"

using namespace muaimf;

#define C( name )	case f##name : return #name; break

const char *muaimf::GetLabel( eField Field )
{
	switch ( Field ) {
	C( To );
	C( From );
	C( Subject );
	C( MessageId );
	C( Date );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a warning.
}

#undef C

namespace {
	stsfsm::wAutomat FieldLabelAutomat_;

	void FillFieldLabelAutomat_( void )
	{
		FieldLabelAutomat_.Init();
		stsfsm::Fill<eField>( FieldLabelAutomat_, f_amount, GetLabel );
	}
}

eField muaimf::GetField( const str::dString &Label )
{
	return stsfsm::GetId( Label, FieldLabelAutomat_, f_Undefined, f_amount, qRPU );
}

namespace  {
	typedef flw::sByte sByte_;
}

namespace {
	inline void ConvertToWording_(
		flw::rRFlow &IFlow,
		flw::rWFlow &OFlow)
	{
		sByte_ Byte;
		bso::sBool First = true;

		while ( !IFlow.EndOfFlow() ) {
			Byte = IFlow.Get();

			if ( isupper( Byte ) )
				if ( !First )
					OFlow.Put( '-' );

			First = false;

			OFlow.Put( Byte );
		}
	}

	inline void ConvertToWording_(
		flw::rRFlow &IFlow,
		str::dString &Wording )
	{
	qRH
		flx::rStringOFlow OFlow;
	qRB
		OFlow.Init( Wording );
		ConvertToWording_( IFlow, OFlow );
	qRR
	qRT
	qRE
	}

	inline const char *ConvertToWording_( const char *Label )
	{
		static qCBUFFERh Buffer;
	qRH
		flx::sStringIFlow Flow;
		str::wString
			LabelAsString,
			Wording;
	qRB
		LabelAsString.Init( Label );
		Flow.Init( LabelAsString );

		Wording.Init();

		ConvertToWording_( Flow, Wording );

		Wording.Convert( Buffer );
	qRR
	qRT
	qRE
		return Buffer;
	}
}

const char *muaimf::GetWording( eField Field )
{
	return ConvertToWording_( GetLabel( Field ) );
}

namespace {
	stsfsm::wAutomat FieldWordingAutomat_;

	// Not thhread-safe.
	const char *GetUppercaseWording_( eField Field )
	{
		static qCBUFFERh Buffer;
	qRH
		str::wString Wording;
	qRB
		Wording.Init( GetWording( Field ) );

		str::ToUpper( Wording );

		Wording.Convert( Buffer );
	qRR
	qRT
	qRE
		return Buffer;
	}

	void FillFieldWordingAutomat_( void )
	{
		FieldWordingAutomat_.Init();
		stsfsm::Fill<eField>( FieldWordingAutomat_, f_amount, GetUppercaseWording_ );
	}
}

namespace {
	eField Get_( const str::dString &Name )
	{
		eField Field = f_Undefined;
	qRH
		str::wString Upper;
	qRB
		Upper.Init( Name );

		str::ToUpper( Upper );

		Field = stsfsm::GetId( Upper, FieldWordingAutomat_, f_Undefined, f_amount, qRPU );

		if ( Field == f_Undefined )
			Field = f_Unknown;
	qRR
	qRT
	qRE
		return Field;
	}
}

sFRow muaimf::dFields::Append(
	const str::dString &Name,
	const str::dString &Body )
{
	sField_ Field;

	Field.Init();

	Field.Field = Get_( Name );

	Field.Name = Append_( Name );
	Field.Body = Append_( Body );

	return Fields.Add( Field );
}

void muaimf::dFields::Search(
	eField Field,
	dFRows &Rows ) const
{
	sFRow Row = First();

	while ( Row != qNIL ) {
		if ( Fields( Row ).Field == Field )
			Rows.Append( Row );

		Row = Next( Row );
	}
}

sFRow muaimf::dFields::Search( eField Field ) const
{
	sFRow Row = qNIL;
qRH
	wFRows Rows;
qRB
	Rows.Init();

	Search( Field, Rows );

	if ( Rows.Amount() != 0 )
		Row = Rows( Rows.First() );
qRR
qRT
qRE
	return Row;
}


namespace {
	qCDEF( char, CR_, '\r' ) ;
	qCDEF( char, LF_, '\n' ) ;

	typedef flw::sByte sByte_;

	inline bso::sBool IsWSP_( sByte_ Byte )
	{
		return ( Byte == ' ' ) || ( Byte == '\t' );
	}

	// If returns 'false', there is no more header.
	bso::sBool GetField_(
		flw::rRFlow &Flow,
		str::dString &Name,
		str::dString &Body )
	{
		bso::sBool Continue = true;
		bso::sBool BodyPart = false;
		bso::sBool TestFolding = false;
		sByte_ Byte;

		if ( Flow.EndOfFlow() )
			return false;
		else if ( Flow.View() == CR_ ) {
			Flow.Skip();

			if ( !Flow.EndOfFlow() )
				if ( Flow.View() == LF_ )
					Flow.Skip();

			return false;
		} else if ( Flow.View() == LF_ ) {
			Flow.Skip();
			return false;
		}

		while ( Continue ) {
			Byte = Flow.Get();

			if ( Byte == CR_ ) {
				if ( Flow.EndOfFlow() )
					Continue = false;
				else
					if ( Flow.View() == LF_ ) {
						Flow.Skip();
						TestFolding = true;
					}
			} else if ( Byte == LF_ ) {
				TestFolding = true;
			} else if ( Byte == ':' ) {
				if ( BodyPart )
					Body.Append( Byte );
				else {
					BodyPart = true;
					switch ( Flow.Get() ) {
					case ' ':
						break;// Skipping the heading space.
					case CR_:
						if ( Flow.Get() != LF_ )
							qRGnr();
						if ( !IsWSP_(Flow.View()) ) {
							BodyPart = false;
							Continue = false;
						}
						break;
					default:
						qRGnr();
						break;
					}
				}
			} else {
				if ( BodyPart )
					Body.Append( Byte );
				else
					Name.Append( Byte );
			}

			if ( TestFolding ) {
				if ( Flow.EndOfFlow() )
					Continue = false;
				else if ( IsWSP_(Flow.View() ) )
					TestFolding = false;
				else
					Continue = false;
			}

		}

		return true;
	}
}

const str::dString &muaimf::GetField(
	eField Field,
	fdr::rRDriver &Driver,
	str::dString &Body )
{
qRH
	wFields Fields;
	muaimf::sFRow Row = qNIL;
qRB
	Fields.Init();
	muaimf::Fill( Driver, Fields );

	Row = Fields.Search( muaimf::fSubject );

	if ( Row == qNIL )
		qRGnr();

	Fields.GetBody( Row, Body );
qRR
qRT
qRE
	return Body;
}

void muaimf::Fill(
	flw::rRFlow &Flow,
	dFields &Fields )
{
qRH
	str::wString Name, Body;
qRB
	Name.Init();
	Body.Init();

	while ( GetField_( Flow, Name, Body ) ) {
		Fields.Append( Name, Body );

		Name.Init();
		Body.Init();
	}
qRR
qRT
qRE
}

void muaimf::Fill(
	fdr::rRDriver &Driver,
	dFields &Fields )
{
qRH
	flw::rDressedRFlow<> Flow;
qRB
	Flow.Init( Driver );

	Fill( Flow, Fields );
qRR
qRT
qRE
}

void muaimf::Dump(
	const dFields &Fields,
	txf::rWFlow Flow )
{
qRH
	sFRow Row = qNIL;
	str::wString Entry;
qRB
	Row = Fields.First();

	while ( Row != qNIL ) {
		if ( Fields.GetField( Row ) == f_Unknown ) {
			Entry.Init();
			Flow << Fields.GetName( Row, Entry );
		} else
			Flow << "- " << GetLabel( Fields.GetField( Row ) );

		Flow << ": ";

		Entry.Init();
		Flow << Fields.GetBody( Row, Entry ) << txf::nl;

		Row = Fields.Next( Row );
	}
qRR
qRT
qRE
}

void muaimf::Dump(
	const dFields &Fields,
	fdr::rWDriver &Driver )
{
qRH
	txf::rWFlow Flow;
qRB
	Flow.Init( Driver );

	Dump( Fields, Flow );
qRR
qRT
qRE
}

namespace {
	void FillAutomats_( void )
	{
		FillFieldLabelAutomat_();
		FillFieldWordingAutomat_();
	}
}

qGCTOR( muaimf )
{
	FillAutomats_();
}




