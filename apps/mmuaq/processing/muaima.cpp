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

namespace {
	inline const str::dString &GetAlphanumMinus_(
		flw::sIFlow &Flow,
		str::dString &AlphaNum )
	{
		flw::sByte Byte = 0;
		while ( !Flow.EndOfFlow() && ( isalnum( Byte = Flow.View() ) || ( Byte == '-' ) ) )
			AlphaNum.Append( Flow.Get() );

		return AlphaNum;
	}
}

#define C( name )	case rc##name : return #name; break
 
const char *muaima::GetLabel( eResponseCode Code )
{
	switch ( Code ) {
	C( OK );
	C( No );
	C( Bad );
	C( PreAuth);
	C( Bye );
	C( Alert );
	C( BadCharSet );
	C( Capability );
	C( Parse );
	C( PermanentFlags );
	case rcReadOnly:
		return "Read-Only";
		break;
	case rcReadWrite:
		return "Read-Write";
		break;
	C( TryCreate );
	C( UIDNext );
	C( UIDValidity );
	C( Unseen );
	C( Unavailable );
	C( AuthenticationFailed );
	C( AuthorizationFailed );
	C( Expired );
	C( PrivacyRequired );
	C( ContactAdmin );
	C( NoPerm );
	C( InUse );
	C( ExpungeIssued );
	C( Corruption );
	C( ServerBug );
	C( ClientBug );
	C( CanNot );
	C( Limit );
	C( OverQuota );
	C( AlreadyExists );
	C( NonExistent );
	C( List );
	C( LSub );
	C( Status );
	C( Search );
	C( Flags );
	C( Exists );
	C( Recent );
	C( Expunge );
	C( Fetch );
	default:
		qRFwk();
		break;
	}
 
	return NULL;	// To avoid a warning.
}

#undef C

namespace code_ {
	namespace _ {
		stsfsm::wAutomat Automat;

		const str::dString &GetUCLabel(
			eResponseCode Code,
			str::dString &Label )
		{
			Label.Append( GetLabel( Code ) );

			str::ToUpper( Label );

			return Label;
		}

		eResponseCode GetCode( const str::dString &Pattern )
		{
			return stsfsm::GetId( Pattern, Automat, rc_Undefined, rc_amount );
		}
	}

	void FillAutomat( void )
	{
		_::Automat.Init();
		stsfsm::Fill<eResponseCode>( _::Automat, rc_amount, _::GetUCLabel );
	}

	eResponseCode Get(
		flw::sIFlow &Flow,
		str::dString &Before )
	{
		eResponseCode Code = rc_Undefined;
	qRH
		str::wString Pattern;
		flw::sByte Byte = 0;
	qRB
		if ( isdigit( Flow.View() ) ) {
			GetAlphanumMinus_( Flow, Before );

			if ( Flow.Get() != ' ' )
				qRGnr();
		}

		Pattern.Init();

		GetAlphanumMinus_( Flow, Pattern );

		Code = _::GetCode( Pattern );

		if ( Flow.View() == ' ' )
			Flow.Skip();
	qRR
	qRT
	qRE
		return Code;
	}
}

eResponseCode muaima::rConsole::GetPendingResponseCode( void )
{
	eResponseCode Code = rc_Undefined;
qRH
	flw::sIFlow &Flow = IFlow_;
	str::wString PendingData, Tag;
qRB
	PendingData.Init();

	if ( PendingCode_ != rc_Undefined ) {
		if ( PendingCodeIsStatus_ ) {
			Code = rc_None;
			PendingCodeIsStatus_ = false;
		} else {
			Code = PendingCode_;
			PendingCode_ = rc_Undefined;
		}
		ResponseDriver_.Init( Flow, PendingData, dCRLF );
	} else {
		if ( Flow.View() == '*' ) {
			Flow.Skip();

			if ( NoTaggedStatusResponse_ ) {
				PendingCodeIsStatus_ = true;
				NoTaggedStatusResponse_ = false;
			}
		} else {
			Tag.Init();
			GetAlphanumMinus_( Flow, Tag );
			PendingCodeIsStatus_ = true;
		}

		if ( Flow.Get() != ' ' )
			qRGnr();

		Code = code_::Get( Flow, PendingData );

		if ( Flow.View() == '[' ) {
			PendingCode_ = Code;
			if ( PendingData.Amount() != 0 )
				qRGnr();
			Flow.Skip();
			Code = code_::Get( Flow, PendingData );
			ResponseDriver_.Init( Flow, PendingData, dBracket );
		} else {
			if ( PendingCodeIsStatus_ ) {
				PendingCode_ = Code;
				Code = rc_None;
				PendingCodeIsStatus_ = false;
			}

			ResponseDriver_.Init( Flow, PendingData, dCRLF );
		}
	}
qRR
qRT
qRE
	return Code;
}

#define C( name )	case s##name : return #name; break
const char *muaima::GetLabel( eStatus Status )
{
	switch ( Status ) {
	C( OK );
	C( NO );
	C( BAD );
	default:
		qRFwk();
		break;
	}
 
	return NULL;	// To avoid a warning.
}
#undef C


namespace status_ {
	namespace _ {
		stsfsm::wAutomat Automat;

		eStatus GetStatus( const str::dString &Pattern )
		{
			return stsfsm::GetId( Pattern, Automat, s_Undefined, s_amount );
		}
	}

	void FillAutomat( void )
	{
		_::Automat.Init();
		stsfsm::Fill<eStatus>( _::Automat, s_amount, GetLabel );
	}

	eStatus Get( flw::sIFlow &Flow )
	{
		eStatus Status = s_Undefined;
	qRH
		str::wString Pattern;
	qRB
		Pattern.Init();

		GetAlphanumMinus_( Flow, Pattern );

		Status = _::GetStatus( Pattern );
	qRR
	qRT
	qRE
		return Status;
	}
}

namespace _ {
	qENUM( Command_ ) {
		cLogin,
		cLogout,	// NOT the counter-part of 'Login' ; closes the connection.
		cCapability,
		cSelect,
		cList,
		cLSub,
		cFetch,
		c_amount,
		c_Undefined
	};

#define C( name )\
	case c##name:\
		return #name;\
		break

	const char *GetLabel_( eCommand_ Command )
	{
		switch ( Command ) {
		C( Login);
		C( Logout );
		C( Capability );
		C( Select );
		C( List );
		C( LSub );
		C( Fetch );
		default:
			qRGnr();
			break;
		}

		return NULL;	// To avoid a warning.
	}

	void SendCommand(
		const str::dString &Tag,
		eCommand_ Command,
		txf::sOFlow &Flow )
	{
	qRH
		str::wString CommandString;
	qRB
		CommandString.Init( GetLabel_( Command ) );

		str::ToUpper( CommandString );

		Flow << Tag << ' ' << CommandString;
	qRR
	qRT
	qRE
	}

	void SendCFLR( txf::sOFlow &Flow )
	{
		 Flow << "\r\n";

		 Flow.Commit();
	}
}

void muaima::Connect( rConsole &Console )
{
	Console.ReportUntaggedStatusResponse();
}

void muaima::Login(
	const str::dString &Username,
	const str::dString &Password,
	rConsole &Console )
{
	::_::SendCommand( Console.GetNextTag(), ::_::cLogin, Console.OFlow() );

	Console.OFlow() << ' ' << Username << ' ' << Password;

	::_::SendCFLR( Console.OFlow() );
}

void muaima::Logout( rConsole &Console )
{
	::_::SendCommand( Console.GetNextTag(), ::_::cLogout, Console.OFlow() );
	::_::SendCFLR( Console.OFlow() );
}

void muaima::Capability( rConsole &Console )
{
	::_::SendCommand( Console.GetNextTag(), ::_::cCapability, Console.OFlow() );
	::_::SendCFLR( Console.OFlow() );
}

void muaima::Select(
	const str::dString &Mailbox,
	rConsole &Console )
{
	::_::SendCommand( Console.GetNextTag(), ::_::cSelect, Console.OFlow() );
	Console.OFlow() << ' ' << Mailbox;
	::_::SendCFLR( Console.OFlow() );
}

void muaima::List(
	const str::dString &Reference,
	const str::dString &Mailbox,
	rConsole &Console )
{
	::_::SendCommand( Console.GetNextTag(), ::_::cList, Console.OFlow() );
	Console.OFlow() << " \"" << Reference << "\" \"" << Mailbox << '"';
	::_::SendCFLR( Console.OFlow() );
}

void muaima::LSub(
	const str::dString &Reference,
	const str::dString &Mailbox,
	rConsole &Console )
{
	::_::SendCommand( Console.GetNextTag(), ::_::cLSub, Console.OFlow() );
	Console.OFlow() << " \"" << Reference << "\" \"" << Mailbox << '"';
	::_::SendCFLR( Console.OFlow() );
}

void muaima::Fetch(
	const str::dString &Sequence,
	const str::dString &Items,
	rConsole &Console )
{
	::_::SendCommand(Console.GetNextTag(), ::_::cFetch, Console.OFlow() );
	Console.OFlow() << ' ' << Sequence << ' ' << Items;
	::_::SendCFLR( Console.OFlow() );
}

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
		return "RFC.822";
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

item::eName muaima::item::rConsole::Get( void )
{
	eName Name = n_Undefined;
qRH
	str::wString Pattern;
qRB
	if ( Flow_.Get() == ')' )
		Name = n_None;
	else {
		tol::Init( Pattern );
		str::ToUpper( Pattern );

		Name = item_::GetName( Pattern );

		if ( ( Name == nBody ) && ( Flow_.View() == '[' ) )
			Name = nBodyWithSection;
		else if ( Flow_.Get() != ' ' )
			qRGnr();
	}
qRR
qRT
qRE
	return Name;
}

void muaima::rSession::RetrieveMessage_( void )
{
qRH
	flx::rStringODriver Driver;
qRB
	Message_.Init();
	Driver.Init( Message_, fdr::ts_Default );

	fdr::Copy(Console_.GetResponseDriver(), Driver );
qRR
qRE
qRT
}

eStatus muaima::rSession::HandleStatus_( qRPN )
{
	eStatus Status = Console_.GetStatus() ;

	switch ( Status ) {
	case sOK:
		Console_.SkipResponse();
		break;
	case sNO:
	case sBAD:
		RetrieveMessage_();
		break;
	default:
		qRGnr();
		break;
	}

	return Status;
}

eStatus muaima::rSession::HandleResponses_(
	cResponse_ &ReponseCallback,
	qRPN )
{
	eResponseCode Code = rc_Undefined;

	while ( ( Code = Console_.GetPendingResponseCode()) != rc_None )
		ReponseCallback.OnResponse( Code, Console_.GetResponseDriver() );

	return HandleStatus_( qRP );
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
			Driver.Drain();
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
	} NOPResponseCallback_;
}

eStatus muaima::rSession::Connect_(
	const str::dString &Username,
	const str::dString &Password,
	qRPN )
{
	eStatus Status = s_Undefined;

	Connect( Console_ );
	Status = HandleResponses_( NOPResponseCallback_, qRP );

	if ( Status == sOK ) {
		Login( Username, Password, Console_ );
		Status = HandleResponses_( NOPResponseCallback_, qRP );
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
		str::dString &Delimiter,
		str::dString &Name )
	{
	qRH
		rValueDriver_ ValueDriver;
	qRB
		ValueDriver.Init( Driver );
		Get_( ValueDriver, Attributes);

		ValueDriver.Init( Driver );
		Get_( ValueDriver, Delimiter);

		ValueDriver.Init( Driver );
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
			if ( Delimiter.Amount() != 0 )
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
		str::wString Delimiter;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Delimiter );
		}
		qCVDTOR( sResponseCallback_ );
		void Init( void )
		{
			tol::Init( Delimiter );
		}
	};
}

eStatus muaima::rSession::FetchHierarchyDelimiter_( qRPN )
{
	eStatus Status = s_Undefined;
qRH
	fetch_hierarchy_delimiter_::sResponseCallback_ ResponseCallback;
qRB
	if ( Delimiter_ != 0 )
		qRGnr();

	ResponseCallback.Init();

	muaima::List( str::wString(""), str::wString(""), Console_ );
	Status = HandleResponses_( ResponseCallback, qRP );

	if ( ResponseCallback.Delimiter.Amount() != 1 )
		qRGnr();

	Delimiter_ = ResponseCallback.Delimiter( ResponseCallback.Delimiter.First() );
qRR
qRT
qRE
	return Status;
}

eStatus muaima::rSession::Disconnect_( qRPN )
{
	Logout( Console_ );
	return HandleResponses_( NOPResponseCallback_, qRP );
}

namespace list_ {
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

	class sResponseCallback
	: public cResponse_
	{
	private:
		qRMV( cList, C_, Callback_ );
		bso::sChar Delimiter_;
	protected:
		virtual void MUAIMAOnResponse(
			eResponseCode Code,
			fdr::rIDriver &Driver ) override
		{
		qRH
			str::wString Dummy, Child;
		qRB
			if ( Code != rcList )
				qRGnr();

			tol::Init( Dummy, Child );
			list_lsub_answer_::Get( Driver, Dummy, Dummy, Child );

			Crop_( Delimiter_, Child );

			C_().OnMailbox( Child );
		qRR
		qRT
		qRE
		}
	public:
		void reset( bso::sBool P = true )
		{
			Delimiter_ = 0;
		}
		qCVDTOR( sResponseCallback );
		void Init(
			cList &Callback,
			bso::sChar Delimiter )
		{
			Callback_ = &Callback;
			Delimiter_ = Delimiter;
		}
	};
}


eStatus muaima::rSession::GetFolders(
	const str::dString &Folder,
	cList &Callback,
		qRPN )
{
	eStatus Status = s_Undefined;
qRH
	str::wString Reference;
	list_::sResponseCallback ResponseCallback;
qRB
	if ( Delimiter_ == 0 ) {
		qRGnr();
	}

	Reference.Init( Folder );

	if ( ( Reference.Amount() != 0) && ( Reference( Reference.Last() ) != '/' ) )
		Reference.Append('/' );
	
	Reference.Replace( '/', Delimiter_ );

	ResponseCallback.Init( Callback, Delimiter_ );
	muaima::List( Reference, str::wString( "%" ), Console_ );
	Status = HandleResponses_( ResponseCallback, qRP );
qRR
qRT
qRE
	return Status;
}

namespace {
	void FillAutomats_( void )
	{
		code_::FillAutomat();
		status_::FillAutomat();
		item_::FillAutomat();
	}
}

qGCTOR( muaima )
{
	FillAutomats_();
	NOPResponseCallback_.Init();
}

