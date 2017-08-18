/*
	Copyright (C) 2007-2017 Claude SIMON (http://q37.info/contact/).

	This file is part of xppq.

	xppq is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	xppq is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with xppq. If not, see <http://www.gnu.org/licenses/>.
*/

#include "parser.h"

#include "common.h"

#include "uvq.h"
#include "nodeq.h"

#include "sclnjs.h"

#include "cio.h"
#include "mtk.h"
#include "tht.h"
#include "lcl.h"
#include "xml.h"

namespace {
	namespace {
		class rContent_
		{
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
			Parser.Flow().UndelyingFlow().IDriver().ITake(tht::GetTID() );	// Between calls, the thread is not the same.

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

	class rRack_
	{
	private:
		flw::sDressedIFlow<> IFlow_;
		xtf::sIFlow XFlow_;
		xml::rParser Parser_;
		rContent_ Content_;
		v8::Persistent<v8::Function> Function_;
		common::sRelay Relay_;
		bso::sBool First_;
	public:
		txf::rOFlow OFlow;
		str::wString Error;
		void reset( bso::sBool P = true )
		{
			if ( P )
				Function_.Reset();

			tol::reset( P , IFlow_, XFlow_, Parser_, Content_, Relay_, First_, Error );
		}
		qCVDTOR( rRack_ );
		void Init( v8q::sFunction &Function )
		{
			Function_.Reset(nodeq::GetIsolate(), Function.Core() );
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
			nodeq::sFunction Function(v8::Local<v8::Function>::New( nodeq::GetIsolate(), Function_ ) );
			nodeq::sNumber Token;

			if ( Content_.Token == xml::t_Error ) {
				Token.Init( 0 );
				Function.Launch( Token, Content_.Tag, Content_.Attribute, Content_.Error );
				XFlow_.UndelyingFlow().IDriver().ITake(tht::GetTID() );
				XFlow_.Dismiss();	// To avoid locker owner problem on destruction.
				return true;
			} else if ( Content_.Token == xml::t_Processed ) {
				XFlow_.UndelyingFlow().IDriver().ITake(tht::GetTID() );
				XFlow_.Dismiss();	// To avoid locker owner problem on destruction.
				return true;
			} else {
				// Must match 'xppq.js'.
				switch ( Content_.Token ) {
				case xml::tStartTag:
					Token.Init( 1 );
					break;
				case xml::tAttribute:
					Token.Init( 2 );
					break;
				case xml::tValue:
					Token.Init( 3 );
					break;
				case xml::tEndTag:
					Token.Init( 4 );
					break;
				default:
					qRGnr();
					break;
				}

				Function.Launch( Token, Content_.Tag, Content_.Attribute, Content_.Value );
			}

			return false;
		}
	};

	typedef uvq::cASync cASync_;

	class rRackASyncCallback_
	: public rRack_, 
	  public cASync_
	{
	protected:
		void UVQWork( void ) override
		{
			return rRack_::Read();
		}
		uvq::eBehavior UVQAfter( void ) override
		{
			if ( rRack_::SendToCallback() )
				return uvq::bExitAndDelete;
			else
				return uvq::bRelaunch;
		}
	public:
		void reset( bso::sBool P = true )
		{
			rRack_::reset( P );
		}
		qCVDTOR( rRackASyncCallback_ );
		void Init( v8q::sFunction &Function )
		{
			rRack_::Init( Function );
		}
	};
#if 1
	void OnReadable_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		nodeq::sRStream This;
		nodeq::sBuffer Chunk;
	qRFB
		This.Init(Infos.This() );

		rRack_ &Rack = *nodeq::sExternal<rRack_>( This.Get( "_rack" ) ).Value();

		Chunk.Init();

		if ( This.Read( Chunk )  )
			Rack.OFlow << Chunk;
		else
			Rack.OFlow.Commit();
	qRFR
	qRFT
	qRFE( sclnjs::ErrFinal() )
	}
#else
	void OnData_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		nodeq::sRStream This;
		nodeq::sBuffer Chunk;
	qRFB
		This.Init(Infos.This() );

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
		This.Init(Infos.This() );

		rRack_ &Rack = *nodeq::sExternal<rRack_>( This.Get( "_rack" ) ).Value();

		Rack.OFlow.Commit();
	qRFR
	qRFT
	qRFE( sclnjs::ErrFinal() )
	}

#endif
}

void parser::Parse( const sclnjs::sArguments &Arguments )
{
qRH
	nodeq::sRStream Source;
	nodeq::sFunction Callback;
	rRackASyncCallback_ *Rack = NULL;
qRB
	Rack = new rRackASyncCallback_;

	if ( Rack == NULL )
		qRGnr();

	tol::Init( Source, Callback );
	Arguments.Get( Source, Callback );
	Rack->Init( Callback );

	Source.Set( "_rack", nodeq::sExternal<rRack_>( Rack ) );

#if 1
	Source.OnReadable( OnReadable_ );
# else // Doesn't always work. Sometimes, 'onend' event is not launched...
	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );
#endif

	uvq::Launch( *Rack );

	Rack = NULL;
qRR
qRT
	if ( Rack != NULL )
		delete Rack;
qRE
}



