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

#define EOFT	if ( Flow.EndOfFlow() ) qRGnr();

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

#define C( name )	case c##name : return #name; break
 
const char *muaima::GetLabel( eCode Code )
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
	case cReadOnly:
		return "Read-Only";
		break;
	case cReadWrite:
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
			eCode Code,
			str::dString &Label )
		{
			Label.Append( GetLabel( Code ) );

			str::ToUpper( Label );

			return Label;
		}

		eCode GetCode( const str::dString &Pattern )
		{
			return stsfsm::GetId( Pattern, Automat, c_Undefined, c_amount );
		}
	}

	void FillAutomat( void )
	{
		_::Automat.Init();
		stsfsm::Fill<eCode>( _::Automat, c_amount, _::GetUCLabel );
	}

	eCode Get(
		flw::sIFlow &Flow,
		str::dString &Before )
	{
		eCode Code = c_Undefined;
	qRH
		str::wString Pattern;
		flw::sByte Byte = 0;
	qRB
		EOFT;

		if ( isdigit( Flow.View() ) ) {
			GetAlphanumMinus_( Flow, Before );

			if ( Flow.Get() != ' ' )
				qRGnr();
		}

		Pattern.Init();

		GetAlphanumMinus_( Flow, Pattern );

		Code = _::GetCode( Pattern );

		EOFT;

		if ( Flow.View() == ' ' )
			Flow.Skip();
	qRR
	qRT
	qRE
		return Code;
	}
}

eCode muaima::rConsole::GetCode( void )
{
	eCode Code = c_Undefined;
qRH
	flw::sIFlow &Flow = IFlow_;
	str::wString PendingData, Tag;
qRB
	PendingData.Init();

	if ( PendingCode_ != c_Undefined ) {
		if ( PendingCodeIsStatus_ ) {
			Code = c_None;
			PendingCodeIsStatus_ = false;
		} else {
			Code = PendingCode_;
			PendingCode_ = c_Undefined;
		}
		ResponseDriver_.Init( Flow, PendingData, false );
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

		EOFT;

		if ( Flow.Get() != ' ' )
			qRGnr();

		Code = code_::Get( Flow, PendingData );

		EOFT;

		if ( Flow.View() == '[' ) {
			PendingCode_ = Code;
			if ( PendingData.Amount() != 0 )
				qRGnr();
			Flow.Skip();
			Code = code_::Get( Flow, PendingData );
			ResponseDriver_.Init( Flow, PendingData, true );
		} else {
			if ( PendingCodeIsStatus_ ) {
				PendingCode_ = Code;
				Code = c_None;
				PendingCodeIsStatus_ = false;
			}

			ResponseDriver_.Init( Flow, PendingData, false );
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
		EOFT;

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

eStatus muaima::GetCompletionStatus( rConsole &Console )
{
	return Console.GetPendingStatus();
}


eStatus muaima::Connect( rConsole &Console )
{
	Console.ReportUntaggedStatusResponse();

	return s_Undefined;
}

eStatus muaima::Login(
	const str::dString &Username,
	const str::dString &Password,
	rConsole &Console )
{
	_::SendCommand(Console.GetNextTag(), _::cLogin, Console.OFlow() );

	Console.OFlow() << ' ' << Username << ' ' << Password;

	_::SendCFLR( Console.OFlow() );

	return s_Undefined;
}

eStatus muaima::Logout( rConsole &Console )
{
	_::SendCommand( Console.GetNextTag(), _::cLogout, Console.OFlow() );
	_::SendCFLR( Console.OFlow() );

	return s_Undefined;
}

eStatus muaima::Capability( rConsole &Console )
{
	_::SendCommand( Console.GetNextTag(), _::cCapability, Console.OFlow() );
	_::SendCFLR( Console.OFlow() );

	return s_Undefined;
}

eStatus muaima::Select(
	const str::dString &Mailbox,
	rConsole &Console )
{
	_::SendCommand( Console.GetNextTag(), _::cSelect, Console.OFlow() );
	Console.OFlow() << ' ' << Mailbox;
	_::SendCFLR( Console.OFlow() );

	return s_Undefined;
}

eStatus muaima::List(
	const str::dString &Reference,
	const str::dString &Mailbox,
	rConsole &Console )
{
	_::SendCommand( Console.GetNextTag(), _::cList, Console.OFlow() );
	Console.OFlow() << " \"" << Reference << "\" \"" << Mailbox << '"';
	_::SendCFLR( Console.OFlow() );

	return s_Undefined;
}

eStatus muaima::LSub(
	const str::dString &Reference,
	const str::dString &Mailbox,
	rConsole &Console )
{
	_::SendCommand( Console.GetNextTag(), _::cLSub, Console.OFlow() );
	Console.OFlow() << " \"" << Reference << "\" \"" << Mailbox << '"';
	_::SendCFLR( Console.OFlow() );

	return s_Undefined;
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

eStatus muaima::rSession::HandleCompletion_(
	eStatus Status,
	qRPN )
{
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

eStatus muaima::rSession::HandlePending_(
	cResponse_ &ReponseCallback,
	qRPN )
{
	eCode Code = c_Undefined;

	while ( (Code = Console_.GetCode()) != c_None )
		ReponseCallback.OnReponse(Code, Console_.GetResponseDriver() );

	return sOK;
}

eStatus muaima::rSession::Handle_(
	eStatus Status,
	cResponse_ &ReponseCallback,
	qRPN )
{
	if ( Status == s_Pending ) {
		if ( HandlePending_( ReponseCallback, qRP ) == sOK )
			Status = HandleCompletion_( GetCompletionStatus( Console_ ), qRP );
	} else
		Status = HandleCompletion_( GetCompletionStatus( Console_ ), qRP );

	return Status;
}

namespace {
	class sNOPReponseCallback_
	: public cResponse_
	{
	protected:
		virtual void MUAIMAOnResponse(
			eCode Code,
			fdr::rIDriver &Driver ) override
		{
			Driver.Drain();
		}
	public:
		void reset( bso::sBool = true )
		{
			// Standardization.
		}
		qCVDTOR( sNOPReponseCallback_ );
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

	Status = Handle_( Connect( Console_ ), NOPResponseCallback_, qRP );

	if ( Status == sOK )
		Status = Handle_( Login( Username, Password, Console_ ), NOPResponseCallback_, qRP );

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
}

namespace list_answer_  {	// Also for 'LSUB'.
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
	}

	void Get(
		fdr::rIDriver &Driver,
		str::dString &Attributes,
		str::dString &Delimiter,
		str::dString &Name )
	{
	qRH
		flw::sDressedIFlow<> Flow;
	qRB
		Flow.Init( Driver );

		Get( Flow, Attributes, Delimiter, Name );
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
			eCode Code,
			fdr::rIDriver &Driver ) override
		{
		qRH
			str::wString Dummy;
		qRB
			if ( Delimiter.Amount() != 0 )
				qRGnr();

			list_answer_::Get( Driver, Dummy, Delimiter, Dummy );
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

	Status = Handle_( List( str::wString(""), str::wString("" ), Console_ ), ResponseCallback, qRP );

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
	return Handle_( Logout( Console_ ), NOPResponseCallback_, qRP );
}




namespace {
	void FillAutomats_( void )
	{
		code_::FillAutomat();
		status_::FillAutomat();
	}
}

qGCTOR( muaima )
{
	FillAutomats_();
	NOPResponseCallback_.Init();
}

