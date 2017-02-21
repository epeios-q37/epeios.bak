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
#include "v8qnjs.h"

#include "scln.h"

#include "cio.h"
#include "mtk.h"
#include "tht.h"
#include "lcl.h"
#include "xml.h"

namespace {
	namespace process_ {
		namespace {
			class rContent_
			{
			public:
				xml::token__ Token;
				str::wString
					Tag,
					Attribute,
					Value;
				void reset( bso::sBool P = true )
				{
					tol::reset( P, Tag, Attribute, Value );
					Token = xml::t_Undefined;
				}
				qCDTOR( rContent_ );
				void Init( void )
				{
					Token = xml::t_Undefined;

					tol::Init( Tag, Attribute, Value );
				}
			};

			bso::sBool Process_(
				xml::parser___ &Parser,
				rContent_ &Content )
			{
				bso::sBool Terminate = false;
			qRH
				lcl::wMeaning Meaning;
				str::wString Translation;
				lcl::locale Locale;
			qRB
				Parser.Flow().UndelyingFlow().IDriver().ITake(tht::GetTID() );	// Between calls, the thread is not the same.
				switch ( Parser.Parse( xml::tfAllButUseless ) ) {
				case xml::t_Error:
					Meaning.Init();
					xml::GetMeaning( Parser.GetStatus(), Parser.Flow().Position(), Meaning );
					Translation.Init();
					Locale.Init();
					Locale.GetTranslation(Meaning, "fr", Translation );
					cio::CErr << Translation << txf::nl << txf::commit;
					qRGnr();
					break;
				default:
					Content.Init();
					Content.Token = Parser.Token();
					Content.Tag = Parser.TagName();
					Content.Attribute = Parser.AttributeName();
					Content.Value = Parser.Value();
					break;
				}
			qRR
			qRT
			qRE
				return Terminate;
			}
		}

		typedef uvq::cASync cASync;

		class cASyncCallback
		: public cASync
		{
		private:
			xml::rParser Parser_;
			flw::sDressedIFlow<> IFlow_;
			xtf::sIFlow XFlow_;
			rContent_ Content_;
			v8::Persistent<v8::Function> Function_;
			bso::sBool First_;
		protected:
			void UVQWork( void ) override
			{
				if ( First_ ) {
					XFlow_.Init( IFlow_, utf::f_Guess );
					Parser_.Init( XFlow_, xml::eh_Default );

					First_ = false;
				}

				Process_( Parser_, Content_ );
			}
			uvq::eBehavior UVQAfter( void ) override
			{
				uvq::eBehavior Behavior = uvq::b_Undefined;
				v8qnjs::sFunction Function(v8::Local<v8::Function>::New( v8qnjs::GetIsolate(), Function_ ) );

				switch ( Content_.Token ) {
				case xml::t_Processed:
					Behavior = uvq::bExitOnly;
					break;
				default:
					Function.Launch( Content_.Tag, Content_.Attribute, Content_.Value );
					Behavior = uvq::bRelaunch;
					break;
				}

				return Behavior;
			}
		public:
			void reset( bso::sBool P = true )
			{
				if ( P )
					Function_.Reset();
				tol::reset( P , Parser_, IFlow_, XFlow_, Content_, First_ );
			}
			qCVDTOR( cASyncCallback);
			void Init(
				fdr::rIDriver &IDriver,
				 v8q::sFunction &Function )
			{
				Function_.Reset(v8qnjs::GetIsolate(), Function.Core() );
				tol::Init( Content_ );
				IFlow_.Init( IDriver );
				First_ = true;
				// Will be made asynchronously, as it blocks.
				/*
				XFlow_.Init( IFlow_, utf::f_Guess );
				Parser_.Init( XFlow_, xml::eh_Default );
				*/

			}
		};
	}

	typedef common::sFRelay sFRelay_;

	class sRack_ {
	private:
		process_::cASyncCallback Callback_;
	public:
		common::sFRelay Relay;
		txf::rOFlow OFlow;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Callback_, Relay );
		}
		qCDTOR( sRack_ );
		void Init( v8qnjs::sFunction &Function )
		{
		qRH
		qRB
			Relay.Init();
#if 1
			Callback_.Init( Relay.In, Function );
			uvq::Launch( Callback_ );
			OFlow.Init( Relay.Out );
#else
			WaitCallback_.Init( Blocker_ );	// Initialize 'Blocker_'.
			common::HandleASync( WaitCallback_, false );
			process_::ASyncProcess( Upstream.IDriver(), Content, Blocker_ );
#endif
		
		qRR
		qRT
		qRE
		}
	};

	void OnData_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This;
		v8qnjs::sBuffer Chunk;
	qRFB
		This.Init(Infos.This() );

		Chunk.Init();
		v8q::Get( Infos, Chunk );
		
		sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();

		Rack.OFlow << Chunk;
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void OnEnd_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This;
	qRFB
		This.Init(Infos.This() );

		sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();

		Rack.OFlow.Commit();

		// delete v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}
}

void Parse_( const v8q::sArguments &Arguments )
{
qRH
	v8qnjs::sRStream Source;
	v8qnjs::sFunction Callback;
	sRack_ *Rack = NULL;
qRB
	Rack = new sRack_;

	if ( Rack == NULL )
		qRGnr();

	tol::Init( Source, Callback );
	Arguments.Get( Source, Callback );
	Rack->Init( Callback );

	Source.Set( "_rack", v8qnjs::sExternal<sRack_>( Rack ) );

	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );

	Rack = NULL;
qRR
qRT
	if ( Rack != NULL )
		delete Rack;
qRE
}


void parser::Parse( const v8q::sArguments &Arguments )
{
qRFH
qRFB
	Parse_( Arguments );
qRFR
qRFT
qRFE( scln::ErrFinal() )
}



