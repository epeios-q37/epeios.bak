/*
	Copyright (C) 2016 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of XPPq.

    XPPq is free software: you can redistribute it and/or modify it
    under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    XPPq is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with XPPq.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "parser.h"

#include "common.h"

using namespace parser;

namespace {
	qCDEF( char *, Id_, "_q37RackParser" );
}

namespace {
	namespace {
		class rContent_ {
		public:
			xml::token__ Token;
			str::wString
				Tag,
				Attribute,
				Value,
				Error;
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Tag, Attribute, Value, Error );
				Token = xml::t_Undefined;
			}
			qCDTOR( rContent_ );
			void Init( void )
			{
				Token = xml::t_Undefined;

				tol::Init( Tag, Attribute, Value, Error );
			}
		};

		void Process_(
			xml::parser___ &Parser,
			rContent_ &Content )
		{
		qRH
			lcl::wMeaning Meaning;
			lcl::locale Locale;
		qRB
			Parser.Flow().UndelyingFlow().IDriver().ITake( tht::GetTID() );	// Between calls, the thread is not the same.

			Content.Init();

			switch ( Content.Token = Parser.Parse( xml::tfObvious ) ) {
			case xml::t_Error:
				Meaning.Init();
				xml::GetMeaning( Parser.GetStatus(), Parser.Flow().Position(), Meaning );
				Locale.Init();
				sclmisc::GetBaseTranslation( Meaning, Content.Error );
				break;
			default:
				Content.Tag = Parser.TagName();
				Content.Attribute = Parser.AttributeName();
				Content.Value = Parser.Value();
				break;
			}
		qRR
		qRT
		qRE
		}
	}

	class rRack_ {
	private:
		flw::sDressedRFlow<> IFlow_;
		xtf::sIFlow XFlow_;
		xml::rParser Parser_;
		rContent_ Content_;
		qRMV( sclnjs::rCallback, C_, Callback_ );
		common::sRelay Relay_;
		bso::sBool First_;
	public:
		txf::rWFlow OFlow;
		str::wString Error;
		void reset( bso::sBool P = true )
		{
			if ( P ) {
				if ( Callback_ != NULL )
					delete Callback_;
			}

			tol::reset( P, IFlow_, XFlow_, Parser_, Content_, Callback_, Relay_, First_, Error );
		}
		qCVDTOR( rRack_ );
		void Init( sclnjs::rCallback *Callback )
		{
			Callback_ = Callback;
			tol::Init( Relay_, Content_, Error );
			OFlow.Init( Relay_.Out );
			IFlow_.Init( Relay_.In );
			tol::Init( Content_ );
			First_ = true;
			// Will be made asynchronously, as it blocks.
			/*
			XFlow_.Init( IFlow_, utf::f_Guess );
			Parser_.Init( XFlow_, xml::eh_Default );
			*/
		}
		void Read( void )
		{
			if ( First_ ) {
				XFlow_.Init( IFlow_, utf::f_Guess );
				Parser_.Init( XFlow_, xml::eh_Default );

				First_ = false;
			}

			Process_( Parser_, Content_ );
		}
		bso::sBool SendToCallback( void )
		{
			if ( Content_.Token == xml::t_Error ) {
				C_().VoidLaunch( 0, Content_.Tag, Content_.Attribute, Content_.Error );
				XFlow_.UndelyingFlow().IDriver().ITake( tht::GetTID() );
				XFlow_.Dismiss();	// To avoid locker owner problem on destruction.
				return true;
			} else if ( Content_.Token == xml::t_Processed ) {
				C_().VoidLaunch( 1, Content_.Tag, Content_.Attribute, Content_.Value );
				XFlow_.UndelyingFlow().IDriver().ITake( tht::GetTID() );
				XFlow_.Dismiss();	// To avoid locker owner problem on destruction.
				return true;
			} else {
				int Token = 0;
				// Must match declaration in 'XPPq.js'.
				switch ( Content_.Token ) {
				case xml::tStartTag:
					Token = 2;
					break;
				case xml::tAttribute:
					Token = 3;
					break;
				case xml::tValue:
					Token = 4;
					break;
				case xml::tEndTag:
					Token = 5;
					break;
				default:
					qRGnr();
					break;
				}

				C_().VoidLaunch( Token, Content_.Tag, Content_.Attribute, Content_.Value );
			}

			return false;
		}
	};

	typedef sclnjs::cAsync cAsync_;

	class rRackAsyncCallback_
	: public rRack_,
  	  public cAsync_
	{
	protected:
		void SCLNJSWork( void ) override
		{
			return rRack_::Read();
		}
		sclnjs::eBehavior SCLNJSAfter( void ) override
		{
			if ( rRack_::SendToCallback() )
				return sclnjs::bExitAndDelete;
			else
				return sclnjs::bRelaunch;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rRack_::reset( P );
		}
		qCVDTOR( rRackAsyncCallback_ );
		void Init( sclnjs::rCallback *Callback )
		{
			rRack_::Init( Callback );
		}
	};
}

SCLNJS_F( parser::OnData )
{
qRH
	sclnjs::rRStream This;
	sclnjs::rBuffer Chunk;
qRB
	tol::Init( This, Chunk );
	Caller.GetArgument( This, Chunk );

	rRack_ &Rack = *(rRack_ *)This.Get( Id_ );

	Rack.OFlow << Chunk;
qRR
qRT
qRE
}

SCLNJS_F( parser::OnEnd )
{
qRH
	sclnjs::rRStream This;
qRB
	tol::Init( This );
	Caller.GetArgument( This );

	rRack_ &Rack = *(rRack_ *)This.Get( Id_ );

	Rack.OFlow.Commit();
qRR
qRT
qRE
}

SCLNJS_F( parser::Parse )
{
qRH
	sclnjs::rRStream Source;
	sclnjs::rCallback *Callback = NULL;
	rRackAsyncCallback_ *Rack = NULL;
qRB
	Rack = new rRackAsyncCallback_;

	if ( Rack == NULL )
		qRGnr();

	Callback = new sclnjs::rCallback;

	if ( Callback == NULL )
		qRGnr();

	tol::Init( Source, *Callback );
	Caller.GetArgument( Source, *Callback );
	Rack->Init( Callback );

	Source.Set( Id_, Rack );

#if 1
//	Source.OnReadable( OnReadable_ );
// The 'readable' event is implemented in the JS file. 
# else // Doesn't always work. Sometimes, 'onend' event is not launched...
	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );
#endif

	sclnjs::Launch( *Rack );
qRR
	if ( Rack != NULL )
		delete Rack;

	if ( Callback != NULL )
		delete Callback;
qRT
qRE
}
