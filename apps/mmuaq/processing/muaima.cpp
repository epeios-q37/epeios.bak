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

#include "muaima.h"

#include "stsfsm.h"

using namespace muaima;

str::wString muaima::CurrentFolder;

const char *item::GetLabel( eName Name )
{
	switch ( Name ) {
	case nBody:
		return "Body";
		break;
	case nBodyWithSection:
		return "Body";
		break;
	case nBodyStructure:
		return "BodyStructure";
		break;
	case nEnvelope:
		return "Envelope";
		break;
	case nFlags:
		return "Flags";
		break;
	case nInternalDate:
		return "InternalDate";
		break;
	case nRFC822:
		return "RFC822";
		break;
	case nRFC822Header:
		return "RFC822.HEADER";
		break;
	case nRFC822Size:
		return "RFC822.SIZE";
		break;
	case nRFC822Text:
		return "RFC822.TEXT";
		break;
	case nUID:
		return "UID";
		break;
	default:
		qRGnr();
		break;
	}

	return NULL;	// To avoid a warning.
}

namespace item_ {
	using namespace muaima::item;

	namespace {
		stsfsm::wAutomat Automat_;
	}

	namespace {
		const str::dString &GetLabelForAutomat_(
			eName Name,
			str::dString &Label )
		{
			if ( Name == nBodyWithSection )
				Label.Append( "dummy" );	// Should be found as already existing 'nBody' label;
			else {
				Label.Append( GetLabel( Name ) );
				str::ToUpper( Label );
			}

			return Label;
		}
	}

	eName GetName( const str::dString &Pattern )
	{
		return stsfsm::GetId( Pattern, Automat_, n_Undefined, n_amount );
	}

	void FillAutomat( void )
	{
		Automat_.Init();
		stsfsm::Fill<eName>( Automat_, n_amount, GetLabelForAutomat_ );
	}

	void GetName(
		flw::rRFlow &Flow,
		str::dString &Name )
	{
		flw::sByte Byte = 0;

		while ( isalpha( Byte = Flow.View() ) || ( Byte == '.' ) )
			Name.Append( Byte );
	}
}

namespace {
	void RetrieveMessage_(
		rConsole &Console,
		str::dString &Message )
	{
	qRH
		flx::rStringODriver Driver;
	qRB
		Driver.Init( Message, fdr::ts_Default );

		fdr::Copy( Console.GetResponseDriver(), Driver );
	qRR
	qRE
	qRT
	}
}

void muaima::rSession::RetrieveMessage_( str::dString *Message )
{
	if ( Message == NULL )
		Console_.SkipResponse();
	else {
		Message->Init();
		::RetrieveMessage_( Console_, *Message );
	}
}

eStatus muaima::rSession::PurgeResponses_(
	str::dString *Message,
	qRPN )
{
	eStatus Status = s_Undefined;

	Console_.SkipRemainingReponses();

	Status = Console_.GetStatus();

	RetrieveMessage_( Message );

	return Status;
}

eStatus muaima::rSession::Connect_(
	const str::dString &Username,
	const str::dString &Password,
	str::dString *Message,
	qRPN )
{
	eStatus Status = s_Undefined;

	Connect( Console_ );
	Status = PurgeResponses_( Message, qRP );

	if ( Status == sOK ) {
		Login( Username, Password, Console_ );
		Status = PurgeResponses_( Message, qRP );
	}

	return Status;
}

namespace {
	void Get_(
		flw::rRFlow &Flow,
		bso::sByte Separator,
		str::dString &Value )
	{
		flw::sByte Byte = 0;

		while ( ( Byte = Flow.Get() ) != Separator )
			Value.Append( Byte );
	}

	void Get_(
		fdr::rRDriver &Driver,
		str::dString &Value )
	{
	qRH
		flw::rDressedRFlow<> Flow;
	qRB
		Flow.Init( Driver );

		while ( !Flow.EndOfFlow() )
			Value.Append( Flow.Get() );
	qRR
	qRT
	qRE
	}
}

namespace list_lsub_answer_  {
	void Get(
		flw::rRFlow &Flow,
		str::dString &Attributes,
		str::dString &Delimiter,
		str::dString &Name )
	{
		if ( Flow.Get() != '(' )
			qRGnr();

		Get_(Flow, ')', Attributes );

		if ( Flow.Get() != ' ' )
			qRGnr();

		if ( Flow.Get() != '"' )
			qRGnr();

		Get_(Flow, '"', Delimiter );

		if ( Flow.Get() != ' ' )
			qRGnr();

		if ( Flow.Get() != '"' )
			qRGnr();

		Get_(Flow, '"', Name );

		if ( !Flow.EndOfFlow() )
			qRGnr();
	}

	void Get(
		fdr::rRDriver &Driver,
		str::dString &Attributes,
		bso::sByte &Delimiter,
		str::dString &Name )
	{
	qRH
		rResponseDriver_ ValueDriver;
		str::wString RawDelimiter;
	qRB
		ValueDriver.Init( Driver, dNone );
		Get_( ValueDriver, Attributes);

		ValueDriver.Init( Driver, dNone);
		RawDelimiter.Init();
		Get_( ValueDriver, RawDelimiter);

		if ( RawDelimiter.Amount() == 1 )
			Delimiter = RawDelimiter( RawDelimiter.First() );
		else
			qRGnr();

		ValueDriver.Init( Driver, dNone );
		Get_( ValueDriver, Name);
	qRR
	qRT
	qRE
	}
}

eStatus muaima::rSession::FetchHierarchyDelimiter_(
	str::dString *Message,
	qRPN )
{
	eStatus Status = s_Undefined;
qRH
	str::wString Dummy;
qRB
	if ( Delimiter_ != 0 )
		qRGnr();

	muaima::List( str::wString(""), str::wString(""), Console_ );

	if ( !Console_.Search( rcList ) )
		qRGnr();

	Dummy.Init();
	list_lsub_answer_::Get( Console_.GetResponseDriver(), Dummy, Delimiter_, Dummy );

	Status = PurgeResponses_( Message, qRP );
qRR
qRT
qRE
	return Status;
}

eStatus muaima::rSession::Disconnect_(
	str::dString *Message,
	qRPN )
{
	Logout( Console_ );
	return PurgeResponses_( Message, qRP );
}

namespace common_ {
	void NormalizeFolderName(
		const bso::sChar Delimiter,
		bso::sBool AppendDelimiter,
		str::dString &Name )
	{
		if ( Delimiter == 0 )
			qRGnr();

		if ( AppendDelimiter )
			if ( ( Name.Amount() != 0) && ( Name( Name.Last() ) != '/' ) )
				Name.Append('/' );

		Name.Replace( '/', Delimiter );
	}

	bso::sUInt GetNumber( fdr::rRDriver &Driver )
	{
		bso::sUInt Result = 0;
	qRH
		flw::rDressedRFlow<> Flow;
		bso::sByte Byte = 0;
	qRB
		Flow.Init( Driver );

		while ( !Flow.EndOfFlow() ) {
			if ( Result >= ( bso::UIntMax / 10 - 1 ) )
				qRLmt();

			if ( !isdigit( Byte = Flow.Get() ) )
				qRGnr();

			Result = Result * 10 + (Byte - '0' );
		}
	qRR
	qRT
	qRE
		return Result;
	}

	void GetString(
		fdr::rRDriver &Driver,
		str::dString &String )
	{
	qRH
		flw::rDressedRFlow<> Flow;
	qRB
		Flow.Init( Driver );

		while ( !Flow.EndOfFlow() ) {
			String.Append(Flow.Get() );
		}
	qRR
	qRT
	qRE
	}
}

namespace get_folders_ {
	void Crop_(
		const char Delimiter,
		str::dString &String )
	{
		if ( String.Amount() != 0 ) {
			sdr::sRow Row = qNIL, PRow = qNIL;

			Row = PRow = String.Search( Delimiter );

			while ( Row != qNIL ) {
				PRow = Row = String.Next( Row );
				Row = String.Search( Delimiter, Row );
			}

			if ( PRow != qNIL )
				String.Crop(PRow, String.Last() );
		}
	}
}

void muaima::rFolders::GetFolderName_( str::dString &Folder )
{
qRH
	str::wString Dummy, Mailbox;
	bso::sByte Delimiter = 0;
qRB
	tol::Init( Dummy, Mailbox );
	list_lsub_answer_::Get( C_().GetResponseDriver(), Dummy, Delimiter, Mailbox );

	get_folders_::Crop_( Delimiter, Mailbox );

	Folder = Mailbox;
qRR
qRT
qRE
}

bso::sBool muaima::rSession::GetFolders(
	const str::dString &Folder,
	rFolders &Folders,
	qRPN )
{
qRH
	str::wString Reference;
qRB
	Reference.Init( Folder );

	common_::NormalizeFolderName( Delimiter_, true, Reference );

	muaima::List( Reference, str::wString( "%" ), Console_ );

	Folders.Init_( *this );
qRR
qRT
qRE
	return true;
}

namespace fetch_ {
	bso::sUInt GetSequence( fdr::rRDriver &Driver )
	{
		bso::sUInt Sequence = 0;
	qRH
		rResponseDriver_ ValueDriver;
	qRB
		ValueDriver.Init( Driver, dNone );
		Sequence = common_::GetNumber( ValueDriver );
	qRR
	qRT
	qRE
		return Sequence;
	}

	void SearchValue(
		item::eName WantedItemName,
		fdr::rRDriver &Driver )
	{
	qRH
		str::wString ItemName;
		bso::sBool Continue = true;
		rResponseDriver_ ValueDriver;
	qRB
		while ( Continue ) {
			ItemName.Init();
			ValueDriver.Init( Driver, dNone );
			common_::GetString(ValueDriver, ItemName );

			if ( item_::GetName( ItemName ) == WantedItemName )
				Continue = false;
			else {
				ValueDriver.Init( Driver, dNone );
				fdr::Purge( ValueDriver );
			}
		}

		if ( Driver.EndOfFlow() )
			qRGnr();
	qRR
	qRT
	qRE
	}
}

void muaima::rFetch_::GetValue_( item::eName ItemName )
{
qRH
	str::wString Name;
qRB
	fetch_::GetSequence( Global_ );

	fetch_::SearchValue( ItemName, Items_ );
qRR
qRT
qRE
}

bso::sBool muaima::rSession::GetFetchItem_(
	item::eName ItemName,
	eFlavor Flavor,
	const str::dString &RawFolder,
	bso::sUInt Number,
	class rFetch_ &Fetch,
	qRPN )
{
	bso::sBool Success = false;
qRH
	str::wString Folder;
	eResponseCode Code = rc_None;
	bso::pInteger Buffer;
	eStatus Status = s_Undefined;
qRB
	Folder.Init( RawFolder );
	common_::NormalizeFolderName( Delimiter_, false, Folder );

	if ( RawFolder.Amount() != 0 ) {
		muaima::Select( Folder, Console_ );
		Status = PurgeResponses_( &PendingMessage_, qRP );
	} else
		Status = sOK;

	if ( Status == sOK ) {
		muaima::Fetch( Flavor, str::wString( bso::Convert( Number, Buffer ) ), str::wString( item::GetLabel( ItemName ) ), Console_ );

		while ( ( ( Code = Console_.GetPendingResponseCode() ) != rc_None ) && ( Code != rcFetch ) )
			Console_.SkipResponse();

		if ( Code == rc_None ) {
			PendingStatus_ = Console_.GetStatus();
			RetrieveMessage_( &PendingMessage_ );
		} else {
			Fetch.Init_( ItemName, *this );
			Success = true;
		}
	} else
		PendingStatus_ = Status;
qRR
qRT
qRE
	return Success;
}

bso::sBool muaima::rSession::Select(
	const str::dString &RawFolder,
	qRPN )
{
	bso::sBool Success = false;
qRH
	str::wString Folder;
	eStatus Status = s_Undefined;
qRB
	Folder.Init( RawFolder );
	common_::NormalizeFolderName( Delimiter_, false, Folder );

	muaima::Select( Folder, Console_ );

	if ( ( Status = PurgeResponses_( &PendingMessage_, qRP ) ) == sOK )
		Success = true;
	else
		PendingStatus_ = Status;
qRR
qRT
qRE
	return Success;
}

bso::sBool muaima::rSession::GetMailAmount(
	const str::dString &RawFolder,
	bso::sUInt &Amount,
	qRPN )
{
	bso::sBool Success = false;
qRH
	str::wString Folder;
	eResponseCode Code = rc_None;
	rResponseDriver_ Driver;
qRB
	Folder.Init( RawFolder );
	common_::NormalizeFolderName( Delimiter_, false, Folder );

	muaima::Select( Folder, Console_ );

	while ( ( ( Code = Console_.GetPendingResponseCode() ) != rc_None ) && ( Code != rcExists ) )
		Console_.SkipResponse();

	if ( Code == rc_None ) {
		PendingStatus_ = Console_.GetStatus();
		RetrieveMessage_( &PendingMessage_ );
	} else {
		Driver.Init(Console_.GetResponseDriver(), dNone );
		Amount = common_::GetNumber( Driver );
		Console_.SkipRemainingReponses();
		Success = true;
	}
qRR
qRT
qRE
	return Success;
}



namespace rfc822_{
	#define C( part  ) case rp##part : return item::nRFC822##part; break
	inline item::eName Convert( eRFC822Part Part )
	{
		switch ( Part ) {
		case rpAll:
			return item::nRFC822;
		C( Size );
		C( Header );
		C( Text );
		default:
			qRGnr();
			break;
		}

		return item::n_Undefined;	// To avoid a warning.
	}
#undef C
}

bso::sBool muaima::rSession::GetRFC822(
	eRFC822Part Part,
	eFlavor Flavor,
	const str::dString &RawFolder,
	bso::sUInt Number,
	rRFC822 &RFC822,
	qRPN )
{
	return GetFetchItem_( rfc822_::Convert( Part ), Flavor, RawFolder, Number, RFC822, qRP );
}

bso::sBool muaima::rSession::GetUID(
	const str::dString &Folder,
	bso::sUInt Number,
	class rUID &UID,
	qRPN )
{
	return GetFetchItem_( item::nUID, muaima::fRegular, Folder, Number, UID, qRP );
}

namespace {
	void FillAutomats_( void )
	{
		item_::FillAutomat();
	}
}

qGCTOR( muaima )
{
	FillAutomats_();
	CurrentFolder.Init();
}

