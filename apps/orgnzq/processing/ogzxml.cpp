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

#include "ogzxml.h"

#include "stsfsm.h"

using namespace ogzxml;

using xml::dWriter;

namespace {
	stsfsm::wAutomat RecordTagAutomats_[RecordLastVersion+1];
	stsfsm::wAutomat RecordAttributeAutomats_[RecordLastVersion+1];
}

const char *ogzxml::GetLabel(
	eRecordTag Tag,
	sVersion Version )
{
	switch ( Tag ) {
	case rtRecord:
		return "Record";
		break;
	case rtFields:
		return "Fields";
		break;
	case rtField:
		return "Field";
		break;
	case rtData:
		return "Data";
		break;
	case rtDatum:
		return "Datum";
		break;
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avod a warning;
}

const char *ogzxml::GetLabel(
	eRecordAttribute Attribute,
	sVersion Version )
{
	switch ( Attribute ) {
	case raId:
		return "Id";
		break;
	case raAmount:
		return "Amount";
		break;
	case raType:
		return "Type";
		break;
	case raNumber:
		return "Number";
		break;
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avod a warning;
}

#define T( tag )	GetLabel( rt##tag )
#define A( tag )	GetLabel( ra##tag )

namespace {
		template <typename type> inline void InitAndFillAutomat_(
		stsfsm::dAutomat &Automat,
		sVersion Version,
		type Amount )
	{
		Automat.Init();
		stsfsm::Fill<type, sVersion>( Version, Automat, Amount, GetLabel );
	}

	template <typename type>  void InitAndFillAutomats_(
		stsfsm::dAutomat *Automats,
		sVersion LastVersion,
		type Amount )
	{
		for ( int i = 0; i <= RecordLastVersion; i++ )
			InitAndFillAutomat_(Automats[i], i, Amount );
	}

	void InitAndFillRecordAutomats_( void )
	{
		InitAndFillAutomats_( RecordTagAutomats_, RecordLastVersion, rt_amount );
		InitAndFillAutomats_( RecordAttributeAutomats_, RecordLastVersion, ra_amount );
	}

	void InitAndFillAutomats_( void )
	{
		InitAndFillRecordAutomats_();
	}
}

qGCTOR( ogzxml )
{
	InitAndFillAutomats_();
}



