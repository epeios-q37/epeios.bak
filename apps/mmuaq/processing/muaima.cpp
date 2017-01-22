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
	case nRFC822_Header:
		return "RFC822.Header";
		break;
	case nRFC822_Size:
		return "RFC822.Size";
		break;
	case nRFC822_Text:
		return "RFC822.Text";
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
		flw::sIFlow &Flow,
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

eStatus muaima::rSession::HandleResponses_(
	cResponse_ &ReponseCallback,
	str::dString *Message,
	qRPN )
{
	eResponseCode Code = rc_Undefined;
	eStatus Status = s_Undefined;

	while ( ( Code = Console_.GetPendingResponseCode()) != rc_None )
		ReponseCallback.OnResponse( Code, Console_.GetResponseDriver() );

	Status = Console_.GetStatus();

	RetrieveMessage_( Message );

	return Status;
}

namespace {
	class sNOPResponseCallback_
	: public cResponse_
	{
	protected:
		virtual void MUAIMAOnResponse(
			eResponseCode Code,
			fdr::rIDriver &Driver ) override
		{
			fdr::Purge( Driver );
		}
	public:
		void reset( bso::sBool = true )
		{
			// Standardization.
		}
		qCVDTOR( sNOPResponseCallback_ );
		void Init( void )
		{
			// Standardization.
		}
	} PurgeResponseCallback_;
}

eStatus muaima::rSession::Connect_(
	const str::dString &Username,
	const str::dString &Password,
	str::dString *Message,
	qRPN )
{
	eStatus Status = s_Undefined;

	Connect( Console_ );
	Status = HandleResponses_( PurgeResponseCallback_, Message, qRP );

	if ( Status == sOK ) {
		Login( Username, Password, Console_ );
		Status = HandleResponses_( PurgeResponseCallback_, Message, qRP );
	}

	return Status;
}

namespace {
	void Get_(
		flw::sIFlow &Flow,
		bso::sByte Separator,
		str::dString &Value )
	{
		flw::sByte Byte = 0;

		while ( ( Byte = Flow.Get() ) != Separator )
			Value.Append( Byte );
	}

	void Get_(
		fdr::rIDriver &Driver,
		str::dString &Value )
	{
	qRH
		flw::sDressedIFlow<> Flow;
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
		flw::sIFlow &Flow,
		str::dString &Attributes,
		str::dString &Delimiter,
		str::dString &Name )
	{
		flw::sByte Byte = 0;

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
		fdr::rIDriver &Driver,
		str::dString &Attributes,
		bso::sByte &Delimiter,
		str::dString &Name )
	{
	qRH
		rValueDriver_ ValueDriver;
		str::wString RawDelimiter;
	qRB
		ValueDriver.Init( Driver, dNone, NULL );
		Get_( ValueDriver, Attributes);

		ValueDriver.Init( Driver, dNone, NULL );
		RawDelimiter.Init();
		Get_( ValueDriver, RawDelimiter);

		if ( RawDelimiter.Amount() == 1 )
			Delimiter = RawDelimiter( RawDelimiter.First() );
		else
			qRGnr();

		ValueDriver.Init( Driver, dNone, NULL );
		Get_( ValueDriver, Name);
	qRR
	qRT
	qRE
	}
}

namespace fetch_hierarchy_delimiter_ {
	class sResponseCallback_
	: public cResponse_
	{
	protected:
		virtual void MUAIMAOnResponse(
			eResponseCode Code,
			fdr::rIDriver &Driver ) override
		{
		qRH
			str::wString Dummy;
		qRB
			if ( Delimiter != 0 )
				qRGnr();

			if ( Code != rcList )
				qRGnr();

			Dummy.Init();
			list_lsub_answer_::Get( Driver, Dummy, Delimiter, Dummy );
		qRR
		qRT
		qRE
		}
	public:
		bso::sByte Delimiter;
		void reset( bso::sBool P = true )
		{
			Delimiter = 0;
		}
		qCVDTOR( sResponseCallback_ );
		void Init( void )
		{
			Delimiter = 0;
		}
	};
}

eStatus muaima::rSession::FetchHierarchyDelimiter_(
	str::dString *Message,
	qRPN )
{
	eStatus Status = s_Undefined;
qRH
	fetch_hierarchy_delimiter_::sResponseCallback_ ResponseCallback;
qRB
	if ( Delimiter_ != 0 )
		qRGnr();

	ResponseCallback.Init();

	muaima::List( str::wString(""), str::wString(""), Console_ );
	Status = HandleResponses_( ResponseCallback, Message, qRP );

	Delimiter_ = ResponseCallback.Delimiter;
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
	return HandleResponses_( PurgeResponseCallback_, Message, qRP );
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

	bso::sUInt GetNumber( fdr::rIDriver &Driver )
	{
		bso::sUInt Result = 0;
	qRH
		flw::sDressedIFlow<> Flow;
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
		fdr::rIDriver &Driver,
		str::dString &String )
	{
	qRH
		flw::sDressedIFlow<> Flow;
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

void muaima::rSession::GetFolders(
	const str::dString &Folder,
	rFolders &Folders )
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
}

namespace get_mail_ {
	bso::sUInt GetSequence( fdr::rIDriver &Driver )
	{
		bso::sUInt Sequence = 0;
	qRH
		rValueDriver_ ValueDriver;
	qRB
		ValueDriver.Init( Driver, dNone, NULL );
		Sequence = common_::GetNumber( ValueDriver );
	qRR
	qRT
	qRE
		return Sequence;
	}

	void SearchRFC822Value( fdr::rIDriver &Driver )
	{
	qRH
		str::wString Name;
		bso::sBool Continue = true;
		rValueDriver_ ValueDriver;
	qRB
		while ( Continue ) {
			Name.Init();
			ValueDriver.Init( Driver, dNone, NULL );
			common_::GetString(ValueDriver, Name );

			if ( item_::GetName( Name ) == item_::nRFC822 )
				Continue = false;
			else {
				ValueDriver.Init( Driver, dNone, NULL );
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

void muaima::get_mail_::rRack::GetValue_( void )
{
qRH
	str::wString Name;
qRB
	::get_mail_::GetSequence( FetchValueDriver_ );

	ItemsValueCallback_.Init( FetchValueDriver_ );
	ItemsValueDriver_.Init( FetchValueDriver_, dNone, &ItemsValueCallback_ );
	
	::get_mail_::SearchRFC822Value( ItemsValueDriver_ );
qRR
qRT
qRE

}

eStatus muaima::rSession::GetMail(
	const str::dString &RawFolder,
	bso::sUInt Number,
	qRPN )
{
	eStatus Status = s_Undefined;
qRH
	str::wString Folder;
	eResponseCode Code = rc_None;
	bso::bInteger Buffer;
qRB
	Folder.Init( RawFolder );
	common_::NormalizeFolderName( Delimiter_, false, Folder );

	Select( Folder, Console_ );

	Status = Console_.GetStatus();

	if ( Status == sOK ) {
		Fetch( muaima::f_Default, str::wString( bso::Convert( Number, Buffer ) ), str::wString( item::GetLabel( item::nRFC822 ) ), Console_ );

		while ( ( ( Code = Console_.GetPendingResponseCode() ) != rc_None ) && ( Code != rcFetch ) )
			Console_.SkipResponse();

		if ( Code == rc_None ) {
			Status = Console_.GetStatus();
			if ( Status == sOK )
				qRGnr();
		} else {
			GetMailRack_.Init( Console_ );
			ValueDriver_ = GetMailRack_();
		}
	}
qRR
qRT
qRE
	return Status;
}

void muaima::rSession::GetMail(
	const str::dString &RawFolder,
	bso::sUInt Number,
	rMail &Mail )
{
qRH
	str::wString Folder;
	eResponseCode Code = rc_None;
	bso::bInteger Buffer;
	eStatus Status = s_Undefined;
qRB
	Folder.Init( RawFolder );
	common_::NormalizeFolderName( Delimiter_, false, Folder );

	Select( Folder, Console_ );

	Status = Console_.GetStatus();

	if ( Status == sOK ) {
		Fetch( muaima::f_Default, str::wString( bso::Convert( Number, Buffer ) ), str::wString( item::GetLabel( item::nRFC822 ) ), Console_ );

		while ( ( ( Code = Console_.GetPendingResponseCode() ) != rc_None ) && ( Code != rcFetch ) )
			Console_.SkipResponse();

		if ( Code == rc_None ) {
			Status = Console_.GetStatus();
			if ( Status == sOK )
				qRGnr();
		} else {
			GetMailRack_.Init( Console_ );
			ValueDriver_ = GetMailRack_();
		}
	}
qRR
qRT
qRE
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
	PurgeResponseCallback_.Init();
}

