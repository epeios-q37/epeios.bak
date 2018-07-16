/*
	Copyright (C) 2017 Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of 'XDHq' software.

    'XDHq' is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'XDHq' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with 'XDHq'.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "newlnch.h"

#include "prtcl.h"

namespace {
	typedef bso::sU8 sAmount_;

	// We use bunch of string *, instead of crate of strings, because we have to access
	// to several members at once.
	typedef bch::qBUNCHwl( str::wString *) rStrings_;

	namespace {
		// Currently, we merge, then split, due to how the 'GetValue(...)' method works.
		// In the future, there will be a 'GetValue_s_(...)' which will handle merged ids,
		// so this function will be useless.
		void GetContents_(
			const str::dString &MergedIds,
			xdhdws::sProxy &Proxy,
			str::dStrings &Contents  )
		{
		qRH;
			str::wStrings Ids;
			str::wString Content;
			sdr::sRow Row = qNIL;
		qRB;
			Ids.Init();

			xdhcmn::FlatSplit( MergedIds, Ids );

			Row = Ids.First();

			while ( Row != qNIL ) {
				Content.Init();
				Proxy.GetValue( Ids( Row ), Content );

				Contents.Append( Content );

				Row = Ids.Next( Row );
			}
		qRR;
		qRT;
		qRE;
		}
	}

	// Type of the returned value.
	qENUM( Type_ )
	{
		tVoid,
			tString,
			tStrings,
			t_amount,
			t_Undefined
	};

	// If returns true, the value in 'Buffer' have to be returned.
	eType_ Launch_(
		prtcl::eCommand Command,
		const rStrings_ &Strings,
		const rStrings_ &MergedStrings,
		xdhdws::sProxy &Proxy,
		str::dString &ResultString,
		str::dStrings &ResultStrings )	// Which Result, or none, given by returned value.
	{
		eType_ ResultType = tVoid;

		switch ( Command ) {
		case prtcl::cExecute_1:
			Proxy.Execute( *Strings( 0 ), ResultString );
			ResultType = tString;
			break;
		case prtcl::cAlert_1:
			Proxy.Alert( *Strings( 0 ), *Strings( 1 ), *Strings( 2 ), *Strings( 3 ) );
			break;
		case prtcl::cConfirm_1:
			Proxy.Confirm( *Strings( 0 ), *Strings( 1 ), *Strings( 2 ), *Strings( 3 ) );
			break;
		case prtcl::cSetLayout_1:
			Proxy.SetLayout( *Strings( 0 ), *Strings( 1 ), *Strings( 2 ) );
			break;
		case prtcl::cGetContents_1:
			GetContents_( *MergedStrings( 0 ), Proxy, ResultStrings );
			ResultType = tString;
			break;
		case prtcl::cSetContents_1:
			Proxy.SetContents( *MergedStrings( 0 ), *MergedStrings( 1 ) );
			qRVct();
			break;
		case prtcl::cDressWidgets_1:
			Proxy.DressWidgets( *Strings( 0 ) );
			break;
		case prtcl::cAddClasses_1:
			Proxy.AddClasses( *MergedStrings( 0 ), *MergedStrings( 1 ) );
			break;
		case prtcl::cRemoveClasses_1:
			Proxy.RemoveClasses( *MergedStrings( 0 ), *MergedStrings( 1 ) );
			break;
		case prtcl::cToggleClasses_1:
			Proxy.ToggleClasses( *MergedStrings( 0 ), *MergedStrings( 1 ) );
			break;
		case prtcl::cEnableElements_1:
			Proxy.EnableElements( *MergedStrings( 0 ) );
			break;
		case prtcl::cDisableElements_1:
			Proxy.DisableElements( *MergedStrings( 0 ) );
			break;
		case prtcl::cSetAttribute_1:
			Proxy.SetAttribute( *Strings( 0 ), *Strings( 1 ), *Strings( 2 ) );
			break;
		case prtcl::cGetAttribute_1:
			Proxy.GetAttribute( *Strings( 0 ), *Strings( 1 ), ResultString );
			ResultType = tString;
			break;
		case prtcl::cRemoveAttribute_1:
			Proxy.RemoveAttribute( *Strings( 0 ), *Strings( 1 ) );
			break;
		case prtcl::cSetProperty_1:
			Proxy.SetProperty( *Strings( 0 ), *Strings( 1 ), *Strings( 2 ) );
			break;
		case prtcl::cGetProperty_1:
			Proxy.GetProperty( *Strings( 0 ), *Strings( 1 ), ResultString );
			ResultType = tString;
			break;
		case prtcl::cFocus_1:
			Proxy.Focus( *Strings( 0 ) );
			break;
		default:
			qRGnr();
			break;
		}

		return ResultType;
	}

}

namespace {
	void GetStrings_(
		flw::sRWFlow &Flow,
		sAmount_ Amount,
		rStrings_ &Strings )
	{
	qRH;
		str::wString *String = NULL;
	qRB;
		while ( Amount-- ) {
			if ( (String = new str::wString) == NULL )
				qRAlc();

			String->Init();

			prtcl::Get( Flow, *String );

			Strings.Push( String );

			String = NULL;
		}
	qRR;
		if ( String != NULL )
			delete String;
	qRT;
	qRE;
	}

	void GetMergedStrings_(
		flw::sRWFlow &Flow,
		sAmount_ Amount,
		rStrings_ &MergedStrings )
	{
	qRH;
		str::wString *String = NULL;
		str::wStrings Strings;
	qRB;
		while ( Amount-- ) {
			Strings.Init();

			prtcl::Get( Flow, Strings );

			if ( (String = new str::wString) == NULL )
				qRAlc();

			String->Init();

			xdhcmn::FlatMerge( Strings, *String, false );

			MergedStrings.Push( String );

			String = NULL;
		}
	qRR;
		if ( String != NULL )
			delete String;
	qRT;
	qRE;
	}

	void Free_( rStrings_ &Strings )
	{
		sdr::sRow Row = Strings.First();

		while ( Row != qNIL ) {
			if ( Strings( Row ) != NULL )
				delete Strings( Row );

			Strings.Store( NULL, Row );

			Row = Strings.Next( Row );
		}
	}
}


void newlnch::Launch(
	flw::sRWFlow & Flow,
	xdhdws::sProxy & Proxy )
{
qRH;
	str::wString Command;
	sAmount_
		StringsAmount = 0,
		MergedStringsAmount = 0;
	rStrings_ 
		Strings,
		MergedStrings;
	str::wString ResultString;
	str::wStrings ResultStrings;
	eType_ ResultType = t_Undefined;
qRB;
	Command.Init();
	prtcl::Get( Flow, Command );

	prtcl::Get( Flow, StringsAmount );
	prtcl::Get( Flow, MergedStringsAmount );

	Strings.Init();

	if ( StringsAmount )
		GetStrings_( Flow, StringsAmount, Strings );

	MergedStrings.Init();

	if ( StringsAmount )
		GetMergedStrings_( Flow, MergedStringsAmount, MergedStrings );

	Flow.Dismiss();

	tol::Init( ResultString, ResultStrings );

	ResultType = Launch_( prtcl::GetCommand( Command ), Strings, MergedStrings, Proxy, ResultString, ResultStrings );

	prtcl::PutRequest( prtcl::rReady_1, Flow );

	switch ( ResultType ) {
	case tVoid:
		break;
	case tString:
		prtcl::Put( ResultString, Flow );
		break;
	case tStrings:
		prtcl::Put( ResultStrings, Flow );
		break;
	default:
		qRGnr();
		break;
	}

	Flow.Commit();
qRR;
qRT;
	Free_( Strings );
	Free_( MergedStrings );
qRE;
}
