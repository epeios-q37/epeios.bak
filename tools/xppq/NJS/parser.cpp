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
					Attribute_.Append( Attribute );
					Value_.Append( Value );

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
		struct rData {
			tht::rBlocker Blocker;
			fdr::rIDriver *IDriver;
			common_::rContent *Content;
			void reset( bso::sBool P = true )
			{
				tol::reset( P , Blocker, IDriver, Content );
			}
			qCDTOR( rData );
			void Init( void )
			{
				reset();
			}
		};

		void Routine( void *UP )
		{
		qRFH
			xml::rParser Parser;
			rData &Data = *(rData *)UP;
			fdr::rIDriver &IDriver = *Data.IDriver;
			common_::rContent &Content  = *Data.Content;
			flw::sDressedIFlow<> IFlow;
			xtf::sIFlow XFlow;
			common_::eToken Token = xml::t_Undefined;
		qRFB
			Data.Blocker.Unblock();

			IFlow.Init( IDriver );
			XFlow.Init( IFlow, utf::f_Guess );

			Parser.Init( XFlow, xml::eh_Default );

			while ( ( Token = Parser.Parse( xml::tfObvious ) ) != xml::t_Processed ) {
				switch ( Token ) {
				case xml::t_Error:
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
		qRFR
		qRFT
			XFlow.Dismiss();	// Avoid lock owner problem when destroying rack.
		qRFE( scln::ErrFinal() )
		}
	}

	typedef common::sFRelay sFRelay_;

	class sUpstreamRack_
	{
	private:
		sFRelay_ Relay_;
	public:
		txf::rOFlow OFlow;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Relay_, OFlow );
		}
		qCDTOR( sUpstreamRack_ );
		void Init( void )
		{
			tol::Init( Relay_ );
			OFlow.Init( Relay_.Out );
		}
		fdr::rIDriver &IDriver( void )
		{
			return Relay_.In;
		}
	};

	class sRack_ {
	public:
		sUpstreamRack_ Upstream;
		common_::rContent Content;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Upstream, Content );
		}
		qCDTOR( sRack_ );
		void Init( void )
		{
		qRH
			process_::rData Data;
		qRB
			Upstream.Init();
			Content.Init();

//			int i = 0; while ( i == 0 );

			Data.Init();

			Data.Blocker.Init();
			Data.IDriver = &Upstream.IDriver();
			Data.Content = &Content;

			mtk::Launch( process_::Routine, &Data );
			Data.Blocker.Wait();
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
			Callback.Launch( Tag, Attribute, Value );
			tol::Init( Tag, Attribute, Value );
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
		cio::COut << __LOC__ << tht::GetTID() << txf::nl << txf::commit;

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

		delete v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void OnRead_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This;
	qRFB
		This.Init( Infos.This() );
		sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();
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



