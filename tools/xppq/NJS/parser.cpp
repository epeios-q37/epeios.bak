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
				Locale.GetTranslation( Meaning, "", Content.Error );
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
		flw::sDressedIFlow<> IFlow_;
		xtf::sIFlow XFlow_;
		xml::rParser Parser_;
		rContent_ Content_;
		qRMV( sclnjs::rCallback, C_, Callback_ );
		common::sRelay Relay_;
		bso::sBool First_;
	public:
		txf::rOFlow OFlow;
		str::wString Error;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, IFlow_, XFlow_, Parser_, Content_, Callback_, Relay_, First_, Error );
		}
		qCVDTOR( rRack_ );
		void Init( sclnjs::rCallback &Callback )
		{
			Callback_ = &Callback;
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
				C_().Launch( 0, Content_.Tag, Content_.Attribute, Content_.Error );
				XFlow_.UndelyingFlow().IDriver().ITake( tht::GetTID() );
				XFlow_.Dismiss();	// To avoid locker owner problem on destruction.
				return true;
			} else if ( Content_.Token == xml::t_Processed ) {
				XFlow_.UndelyingFlow().IDriver().ITake( tht::GetTID() );
				XFlow_.Dismiss();	// To avoid locker owner problem on destruction.
				return true;
			} else {
				int Token = 0;
				// Must match 'xppq.js'.
				switch ( Content_.Token ) {
				case xml::tStartTag:
					Token = 1;
					break;
				case xml::tAttribute:
					Token = 2;
					break;
				case xml::tValue:
					Token = 3;
					break;
				case xml::tEndTag:
					Token = 4;
					break;
				default:
					qRGnr();
					break;
				}

				C_().Launch( Token, Content_.Tag, Content_.Attribute, Content_.Value );
			}

			return false;
		}
	};

	typedef n4njs::cAsync cAsync_;

	class rRackAsyncCallback_
	: public rRack_,
  	  public cAsync_ {
	protected:
		void N4NJSWork( void ) override
		{
			return rRack_::Read();
		}
		n4njs::eBehavior N4NJSAfter( void ) override
		{
			if ( rRack_::SendToCallback() )
				return n4njs::bExitAndDelete;
			else
				return n4njs::bRelaunch;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rRack_::reset( P );
		}
		qCVDTOR( rRackAsyncCallback_ );
		void Init( sclnjs::rCallback &Callback )
		{
			rRack_::Init( Callback );
		}
	};
#if 1
	void OnReadable_( sclnjs::sCaller &Caller )
	{
	qRH
		sclnjs::rRStream This;
		sclnjs::rBuffer Chunk;
	qRB
		This.Init();
		Caller.GetArgument( This );

		rRack_ &Rack = *(rRack_ *)This.Get( "_rack" );

		Chunk.Init();

		if ( This.Read( Chunk ) )
			Rack.OFlow << Chunk;
		else
			Rack.OFlow.Commit();
	qRR
	qRT
	qRE
	}
#else
	void OnData_( const v8q::sFunctionInfos &Infos )
	{
		qRFH
			nodeq::sRStream This;
		nodeq::sBuffer Chunk;
		qRFB
			This.Init( Infos.This() );

		Chunk.Init();
		v8q::Get( Infos, Chunk );

		rRack_ &Rack = *nodeq::sExternal<rRack_>( This.Get( "_rack" ) ).Value();

		Rack.OFlow << Chunk;
		qRFR
			qRFT
			qRFE( sclnjs::ErrFinal() )
	}

	void OnEnd_( const v8q::sFunctionInfos &Infos )
	{
		qRFH
			nodeq::sRStream This;
		qRFB
			This.Init( Infos.This() );

		rRack_ &Rack = *nodeq::sExternal<rRack_>( This.Get( "_rack" ) ).Value();

		Rack.OFlow.Commit();
		qRFR
			qRFT
			qRFE( sclnjs::ErrFinal() )
	}

#endif
}

void parser::OnReadable( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream This;
	sclnjs::rBuffer Chunk;
qRB
	tol::Init( This, Chunk );
	Caller.GetArgument( This, Chunk );

	rRack_ &Rack = *(rRack_ *)This.Get( "_rack" );

	if ( Chunk.IsNull() )
		Rack.OFlow.Commit();
	else
		Rack.OFlow << Chunk;
qRR
qRT
qRE
}


void parser::Parse( sclnjs::sCaller &Caller )
{
qRH
	sclnjs::rRStream Source;
	sclnjs::rCallback Callback;
	rRackAsyncCallback_ *Rack = NULL;
	sclnjs::rAsyncLauncher Launcher;
qRB
	Rack = new rRackAsyncCallback_;

	if ( Rack == NULL )
		qRGnr();

	tol::Init( Source, Callback );
	Caller.GetArgument( Source, Callback );
	Rack->Init( Callback );

	Source.Set( "_rack", &Rack );

#if 1
//	Source.OnReadable( OnReadable_ );
// The 'readable' event is implemented in the JS file. 
# else // Doesn't always work. Sometimes, 'onend' event is not launched...
	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );
#endif

	Launcher.Init();
	Launcher.Launch( *Rack );

	Rack = NULL;
qRR
qRT
	if ( Rack != NULL )
		delete Rack;
qRE
}
