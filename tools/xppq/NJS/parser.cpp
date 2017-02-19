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
			typedef common::cASync cASync_;
		}

		class cASyncCallback
		: public cASync_
		{
		private:
			fdr::rIDriver *IDriver_;
			common_::rContent *Content_;
		protected:
			bso::sBool COMMONProcess( void ) override
			{
				bso::sBool Terminate = false;
			qRH
				xml::rParser Parser;
				fdr::rIDriver &IDriver = *IDriver_;
				common_::rContent &Content  = *Content_;
				flw::sDressedIFlow<> IFlow;
				xtf::sIFlow XFlow;
				common_::eToken Token = xml::t_Undefined;
				lcl::wMeaning Meaning;
				str::wString Translation;
				lcl::locale Locale;
			qRB
				IFlow.Init( IDriver );
				XFlow.Init( IFlow, utf::f_Guess );

				Parser.Init( XFlow, xml::eh_Default );

				while ( ( Token = Parser.Parse( xml::tfAllButUseless ) ) != xml::t_Processed ) {
					switch ( Token ) {
					case xml::t_Error:
						Meaning.Init();
						xml::GetMeaning( Parser.GetStatus(), XFlow.Position(), Meaning );
						Translation.Init();
						Locale.Init();
						Locale.GetTranslation(Meaning, "fr", Translation );
						cio::CErr << Translation << txf::nl << txf::commit;
						qRGnr();
						break;
					default:
						while ( !Content.P_Put( Parser.Token(), Parser.TagName(), Parser.AttributeName(), Parser.Value() ) )
							tht::Defer();
						break;
					}
				}

				while ( !Content.P_Put( Parser.Token(), Parser.TagName(), Parser.AttributeName(), Parser.Value() ) )
					tht::Defer();

				Terminate = true;
			qRR
			qRT
				XFlow.Dismiss();	// Avoid lock owner problem when destroying rack.
			qRE
				return Terminate;
			}
			void COMMONDisclose(void) override {}
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P , IDriver_, Content_ );
			}
			qCVDTOR( cASyncCallback);
			void Init(
				fdr::rIDriver &IDriver,
				common_::rContent &Content )
			{
				IDriver_ = &IDriver;
				Content_ = &Content;
			}
		};
	}

	typedef common::sFRelay sFRelay_;

	class sRack_ {
	private:
		process_::cASyncCallback Callback_;
	public:
		common::sUpstreamRack Upstream;
		common_::rContent Content;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Callback_, Upstream, Content );
		}
		qCDTOR( sRack_ );
		void Init( void )
		{
		qRH
		qRB
			Upstream.Init();
			Content.Init();

			Callback_.Init( Upstream.IDriver(), Content );

			common::HandleASync( Callback_ );
		qRR
		qRT
		qRE
		}
	};

	bso::sBool CallCallback_(
		common_::rContent &Content,
		v8qnjs::sFunction &Callback )
	{
		bso::sBool Done = false;
	qRH
		str::wString Tag, Attribute, Value;
		common_::eToken Token = xml::t_Undefined;
	qRB
		tol::Init( Tag, Attribute, Value );

		while ( ( ( Token = Content.P_Get(Tag, Attribute, Value)) != xml::t_Undefined) && (Token != xml::t_Processed ) ) {  
			if ( Token != xml::t_Undefined ) {
				Callback.Launch( Tag, Attribute, Value );
				tol::Init( Tag, Attribute, Value );
			}
			tht::Defer();
		}

		Done = Token == xml::t_Processed; 
	qRR
	qRT
	qRE
		return !Done;
	}


	void OnData_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This;
		v8qnjs::sFunction Callback;
		v8qnjs::sBuffer Chunk;
	qRFB
		This.Init(Infos.This() );

		Chunk.Init();
		v8q::Get( Infos, Chunk );

		Callback.Init( This.Get("_callback" ) );
		
		sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();

		Rack.Upstream.OFlow << Chunk;

		CallCallback_( Rack.Content, Callback );
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void OnEnd_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This;
		v8qnjs::sFunction Callback;
	qRFB
		This.Init(Infos.This() );
		Callback.Init( This.Get("_callback" ) );

		sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();

		Rack.Upstream.OFlow.Commit();

		while ( CallCallback_( Rack.Content, Callback ) );

		// delete v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}
}


void parser::Parse( const v8q::sArguments &Arguments )
{
qRFH
	v8qnjs::sRStream Source;
	v8qnjs::sFunction Callback;
	sRack_ *Rack = NULL;
qRFB
	Rack = new sRack_;

	if ( Rack == NULL )
		qRGnr();

	tol::Init( Source, Callback );
	Arguments.Get( Source, Callback );

	Rack->Init();

	Source.Set( "_rack", v8qnjs::sExternal<sRack_>( Rack ) );
	Source.Set( "_callback", Callback );

	Source.OnData( OnData_ );
	Source.OnEnd( OnEnd_ );

	Rack = NULL;
qRFR
qRFT
	if ( Rack != NULL )
		delete Rack;
qRFE( scln::ErrFinal() )
}



