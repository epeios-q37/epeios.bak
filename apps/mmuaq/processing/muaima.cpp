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

void muaima::rDriverBase_::HandleFreeContext_(
	fdr::sByte Byte,
	flw::sIFlow &Flow,
	fdr::sByte *Buffer,
	fdr::sSize &Amount )
{
	bso::sBool Put = true;	// If 'true', 'Byte' is put in 'Buffer'.

	switch ( Byte ) {
	case '"':
		if ( Delimiter_ == dNone ) {
			Delimiter_ = dQuote;
			Put = false;
		} else if ( Delimiter_ == dQuote ) {
			Context_ = _::cEOF;
			Put = false;
		} else
			Context_ = _::cQuoted;
		break;
	case '(':
		if ( Delimiter_ == dNone ) {
			Delimiter_ = dParenthesis;
			Put = false;
		} else if ( Level_ == bso::U8Max )
			qRLmt();
		else 
			Level_++;
		// We stay in the same context.
		break;
	case ')':
		if ( Level_ == 0 ) {
			if ( Delimiter_ != dParenthesis )
				qRGnr();

			Context_ = _::cEOF;
			Put = false;
		} else
			Level_--;

		// We stay in the same context.
		break;
	case '[':	// Can happen with 'FETCH' 'BODY[HEADER]' reponse.
		if ( Delimiter_ == dBracket )
			qRGnr();
		break;
	case ']':
		if ( Delimiter_ == dBracket ) {
			Context_ = _::cEOF;
			Put = false;
		}
		break;		
	case '{':
		if ( Delimiter_ == dNone ) {
			Delimiter_ = dBracket;
			Put = false;
		}

		Context_ = _::cLiteral;
		break;
	case '}':
		qRGnr();
		break;
	case '\r':
		if ( Flow.View() != '\n' )
			qRGnr();

		if ( ( Delimiter_ == dCRLF ) || ( Delimiter_ == dNone ) ) {	// Can sometimes occur when directly called from 'HandleFreeContext_(...)'.
			Context_ = _::cEOF;
			Put = false;
		} else 
			Force_ = 1;
		break;
	case '\n':	// Should have be already skipped.
		qRGnr();
		break;
	case 0:		// Not allowed
		qRGnr();
		break;
	default:
		break;
	}

	if ( Put )
		Buffer[Amount++] = Byte;
}

bso::sBool muaima::rDriverBase_::HandleQuotedContext_(
	fdr::sByte Byte,
	flw::sIFlow &Flow,
	fdr::sByte *Buffer,
	fdr::sSize &Amount )
{
	switch ( Byte ) {
	case '\\':
		Byte = Flow.View();
		if ( ( Byte != '\\' ) && ( Byte != '"' ) )
			qRGnr();

		if ( Delimiter_ == dQuote ) {
			Buffer[Amount++] = Flow.Get();
		} else {
			Buffer[Amount++] = '\\';
			Force_ = 1;
		}
		break;
	case '"':
		if ( Delimiter_ == dQuote )
			Context_ = _::cEOF;
		else {
			Buffer[Amount++] = Byte;
			Context_ = _::cFree;
		}
		break;
	default:
		Buffer[Amount++]=Byte;
		break;
	}

	return true;
}

void muaima::rDriverBase_::HandleLiteralContext_(
	fdr::sByte Byte,
	flw::sIFlow &Flow,
	fdr::sByte *Buffer,
	fdr::sSize &Amount )
{
	if ( Byte == '}' ) {
		if ( Delimiter_ != dLiteral )
			Force_ += 2;	// To include the 'CRLF'.
		Context_ = _::cFree;
	} else if ( !isdigit(Byte) ) {
		qRGnr();
	} else if ( Force_ < ( ( bso::SizeMax / 10 ) -1 ) ) {
		Force_ = Force_ * 10 + ( Byte - '0' );
	} else
		qRLmt();

	if ( Delimiter_ != dLiteral )
		Buffer[Amount++] = Byte;
}

void muaima::rDriverBase_::HandleContext_(
	flw::sIFlow &Flow,
	fdr::sByte *Buffer,
	fdr::sSize &Amount )
{
	bso::sBool HandleSPCRLF = true;

	switch ( Context_ ) {
	case _::cFree:
		HandleFreeContext_( Flow.Get(), Flow, Buffer, Amount );
		break;
	case _::cQuoted:
		HandleQuotedContext_( Flow.Get(), Flow, Buffer, Amount );
		break;
	case _::cLiteral:
		HandleLiteralContext_( Flow.Get(), Flow, Buffer, Amount );
		break;
	case _::cEOF:
		HandleSPCRLF = false;
		break;
	default:
		qRGnr();
		break;
	}

	if ( Context_ == _::cEOF ) {
		if ( HandleSPCRLF && !Flow.EndOfFlow() ) {
			switch ( Flow.Get() ) {
			case ' ':
				break;
			case '\r':
				if ( Flow.Get() != '\n' )
					qRGnr();
				break;
			case '\n':	// '\r' was already eaten.
				break;
			default:	// All above space chars are already be eaten.
				break;
			}
		}
	}
}

fdr::sSize muaima::rDriverBase_::Read(
	fdr::sSize Maximum,
	fdr::sByte *Buffer )
{
	fdr::sSize Amount = 0;

	flw::sIFlow &Flow = F_();

	if ( Maximum < 2 )	// Due to the 'Pending' handling, that must be place in the 'Buffer' to put at least chars.
		qRVct();

	while ( Amount < Maximum ) {
		if ( !Pending_.IsEmpty() ) {
			if ( Maximum > ( Amount + 1 ) ) { // To ensure that there is place in 'Buffer' to put a ' '.
				Amount += Pending_.ReadUpTo( Maximum - Amount - 1, Buffer + Amount );

				if ( Pending_.IsEmpty() )
					if ( Flow.View() != '\r' )
						Buffer[Amount++] = ' ';
			} else
				Maximum = Amount;	// To exit the loop.
		} else if ( ( Force_ != 0 ) && ( Context_ != _::cLiteral ) ) {
			bso::sSize PonctualAmount = Flow.ReadUpTo( Force_ > Maximum - Amount ? Maximum - Amount : Force_, Buffer + Amount );

			Amount += PonctualAmount;

			Force_ -= PonctualAmount;
		} else {
			HandleContext_( Flow, Buffer, Amount );
		}

		if ( ( Context_ == _::cEOF ) || Flow.EndOfFlow() ) {
			Context_ = _::cEOF;
			Maximum = Amount;	// To exit the loop.
		}
	}

	return Amount;
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

eStatus muaima::rSession::HandleStatus_(
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
		if ( qRPT )
			qRGnr();
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
		ValueDriver.Init( Driver, NULL );
		Get_( ValueDriver, Attributes);

		ValueDriver.Init( Driver, NULL );
		Get_( ValueDriver, Delimiter);

		ValueDriver.Init( Driver, NULL );
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
			if ( Result < ( bso::UIntMax / 10 - 1 ) )
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

	class sResponseCallback
	: public cResponse_
	{
	private:
		qRMV( cFolders, C_, Callback_ );
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

			C_().OnFolder( Child );
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
			cFolders &Callback,
			bso::sChar Delimiter )
		{
			Callback_ = &Callback;
			Delimiter_ = Delimiter;
		}
	};
}

eStatus muaima::rSession::GetFolders(
	const str::dString &Folder,
	cFolders &Callback,
	qRPN )
{
	eStatus Status = s_Undefined;
qRH
	str::wString Reference;
	get_folders_::sResponseCallback ResponseCallback;
qRB
	Reference.Init( Folder );

	common_::NormalizeFolderName( Delimiter_, true, Reference );

	ResponseCallback.Init( Callback, Delimiter_ );
	muaima::List( Reference, str::wString( "%" ), Console_ );
	Status = HandleResponses_( ResponseCallback, qRP );
qRR
qRT
qRE
	return Status;
}

namespace get_mail_ {
	class sResponseCallback
	: public cResponse_
	{
	protected:
		virtual void MUAIMAOnResponse(
			eResponseCode Code,
			fdr::rIDriver &Driver ) override
		{
			if ( Code == rcExists ) {
				if ( Amount != 0 )
					qRGnr();

				Amount = common_::GetNumber( Driver );
			} else
				fdr::Copy( Driver, flx::VoidOFlowDriver );
		}
	public:
		bso::sUInt Amount;
		void reset( bso::sBool P = true )
		{
			Amount = 0;
		}
		qCVDTOR( sResponseCallback );
		void Init( void )
		{
			Amount = 0;
		}
	};

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

	Status = HandleStatus_( Console_.SkipRemainingReponses(), qRP );

	if ( Status == sOK ) {
		Fetch( str::wString( bso::Convert( Number, Buffer ) ), str::wString( item::GetLabel( item::nRFC822 ) ), Console_ );

		while ( ( ( Code = Console_.GetPendingResponseCode() ) != rc_None ) && ( Code != rcFetch ) )
			Console_.SkipResponse();

		if ( Code == rc_None ) {
			Status = HandleStatus_( qRP );
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

