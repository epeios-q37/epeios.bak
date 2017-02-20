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

#include "v8qnjs.h"

#include "scln.h"

#include "cio.h"
#include "mtk.h"
#include "tht.h"
#include "lcl.h"
#include "xml.h"

namespace {
	namespace common_ {
		namespace {
			typedef xml::token__ eToken;
		}

		typedef common::cASync cASync;

		class rContent
		{
		private:
			tht::rLocker Locker_;
			eToken Token_;
			str::wString
				Tag_,
				Attribute_,
				Value_;
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Locker_, Tag_, Attribute_, Value_ );

				Token_ = xml::t_Undefined;
			}
			qCDTOR( rContent );
			void Init( void )
			{
				tol::Init( Locker_, Tag_, Attribute_, Value_ );
				Token_ = xml::t_Undefined;
			}
			bso::sBool P_Put(
				eToken Token,
				const str::dString &Tag,
				const str::dString &Attribute,
				const str::dString &Value )
			{
				bso::sBool Done = false;
			qRH
				tht::rLockerHandler Locker;
			qRB
				Locker.Init( Locker_ );

				if ( Token_ == xml::t_Processed )
					qRGnr();

				if ( Token_ == xml::t_Undefined ) {
					Token_ = Token;

					Tag_ = Tag;
					Attribute_ = Attribute;
					Value_ = Value;

					Done = true;
				}
			qRR
			qRT
			qRE
				return Done;
			}
			eToken P_Get(
				str::dString &Tag,
				str::dString &Attribute,
				str::dString &Value )
			{
				eToken Token = xml::t_Undefined;
			qRH
				tht::rLockerHandler Locker;
			qRB
				Locker.Init( Locker_ );

				if ( Token_ != xml::t_Undefined ) {
					Token = Token_;
					Tag.Append( Tag_ );
					Attribute.Append( Attribute_ );
					Value.Append( Value_ );

					if ( Token_ != xml::t_Processed )
						Token_ = xml::t_Undefined;
				}
			qRR
			qRT
			qRE
				return Token;
			}
		};
	}

	namespace process_ {
		namespace {
			class rContent_
			{
			public:
				common_::eToken Token;
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
				case xml::t_Processed:
					Terminate = true;
					break;
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

		class cASyncCallback
		: public common_::cASync
		{
		private:
			xml::rParser Parser_;
			flw::sDressedIFlow<> IFlow_;
			xtf::sIFlow XFlow_;
			rContent_ Content_;
			v8::Persistent<v8::Function> Function_;
			bso::sBool First_;
		protected:
			bso::sBool COMMONProcess( void ) override
			{
				if ( First_ ) {
					XFlow_.Init( IFlow_, utf::f_Guess );
					Parser_.Init( XFlow_, xml::eh_Default );

					First_ = false;
				}

				return Process_( Parser_, Content_ );
			}
			void COMMONDisclose(void) override
			{
				v8qnjs::sFunction Function(v8::Local<v8::Function>::New( v8qnjs::GetIsolate(), Function_ ) );

				if ( Content_.Token != xml::t_Undefined ) {
					Function.Launch( Content_.Tag, Content_.Attribute, Content_.Value );
				}
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

	class sAWaitCallback
	: public common_::cASync
	{
	private:
		tht::rBlocker *Blocker_;
	protected:
		bso::sBool COMMONProcess( void ) override
		{
			Blocker_->Init();
			Blocker_->Wait();

			return true;
		}
		void COMMONDisclose(void) override {}
	public:
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Blocker_ );
		}
		qCVDTOR( sAWaitCallback );
		void Init( tht::rBlocker &Blocker )
		{
			Blocker_ = &Blocker;
		}
	};

	class sRack_ {
	private:
		process_::cASyncCallback Callback_;
	public:
		common::sFRelay Relay;
		common_::rContent Content;
		txf::rOFlow OFlow;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Callback_, Relay, Content );
		}
		qCDTOR( sRack_ );
		void Init( v8qnjs::sFunction &Function )
		{
		qRH
		qRB
			Relay.Init();
			Content.Init();
#if 1
			Callback_.Init( Relay.In, Function );
			common::HandleASync( Callback_, false );
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



