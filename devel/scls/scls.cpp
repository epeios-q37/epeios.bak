/*
	Copyright (C) 1999 Claude SIMON (http://q37.info/contact/).

	This file is part of the Epeios framework.

	The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	The Epeios framework is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with the Epeios framework.  If not, see <http://www.gnu.org/licenses/>
*/

#define SCLS_COMPILATION_

#include "scls.h"

using namespace scls;

void scls::prolog::WriteLayout(
	sclf::rFrontend &Frontend,
	xml::rWriter &Writer)
{
	sclf::WritePresetFeatures( Frontend.Language(), Writer );
}

static sclf::ePresetType GetPresetType_( sclx::sProxy &Proxy )
{
	sclf::ePresetType Type = sclf::pt_Undefined;
qRH
	str::string RawType;
qRB
	RawType.Init();
	Type = sclf::GetPresetType(Proxy.GetValue(prolog::PresetId, RawType));
qRR
qRT
qRE
	return Type;
}

void scls::prolog::HandlePresetSwitching( sclx::sProxy & Proxy )
{
	switch ( GetPresetType_( Proxy ) ) {
	case sclf::ptNone:
		Proxy.AddClass( prolog::ProjectFormId, "hide" );
		Proxy.AddClass( prolog::SetupFormId, "hide" );
		Proxy.AddClass( prolog::BorderId, "fieldset-vanish" );
		break;
	case sclf::ptSetup:
		Proxy.AddClass( prolog::ProjectFormId, "hide" );
		Proxy.RemoveClass( prolog::SetupFormId, "hide" );
		Proxy.RemoveClass( prolog::BorderId, "fieldset-vanish" );
		break;
	case sclf::ptProject:
		Proxy.RemoveClass( prolog::ProjectFormId, "hide" );
		Proxy.AddClass( prolog::SetupFormId, "hide" );
		Proxy.RemoveClass( prolog::BorderId, "fieldset-vanish" );
		break;
	default:
		qRFwk();
		break;
	}
}

void scls::prolog::DisplaySelectedProjectFilename(
	sclx::sProxy &Proxy,
	const str::dString &Id )
{
qRH
	str::wString Buffer;
	str::wString FileName;
	xdhcmn::digest Args;
	xdhcmn::retriever__ Retriever;
qRB
	tol::Init(Args, Buffer);
	xdhcmn::Split( Proxy.GetValue( Id, Buffer ), Args );

	Retriever.Init( Args );

	FileName.Init();

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FileName );

	if ( FileName.Amount() != 0 )
		Proxy.SetValue( ProjectId, FileName );
qRR
qRT
qRE
}

sclf::ePresetType scls::prolog::GetPresetFeatures(
	sclx::sProxy &Proxy,
	str::string_ &Feature )
{
	sclf::ePresetType Type = sclf::pt_Undefined;
qRH
	str::wString Buffer;
qRB
	Buffer.Init();

	switch ( Type = GetPresetType_( Proxy ) ) {
	case sclf::ptNone:
		break;
	case sclf::ptSetup:
		Feature.Append( Proxy.GetValue( SetupId, Buffer ) );
		break;
	case sclf::ptProject:
		Feature.Append( Proxy.GetValue( ProjectId, Buffer ) );
		break;
	default:
		qRFwk();
		break;
	}
qRR
qRT
qRE
	return Type;
}

bso::sBool scls::prolog::LoadPreset( sclx::sProxy &Proxy )
{
	bso::sBool ReturnValue = false;
qRH
	str::string PresetFeature;
qRB
	PresetFeature.Init();
	ReturnValue = sclf::LoadPreset( prolog::GetPresetFeatures( Proxy, PresetFeature ), PresetFeature, Proxy.Info() );
qRR
qRT
qRE
	return ReturnValue;
}

#define C( name ) case bv##name : return #name

const char *scls::login::GetLabel( eBackendVisibility Visibility )
{
	switch ( Visibility ) {
		C( Show );
		C( Hide );
	default:
		qRFwk();
		break;
	}

	return NULL;	// To avoid a warning.
}

#undef C

sclf::eLogin scls::login::WriteLayout(
	sclf::rFrontend &Frontend,
	xml::rWriter &Writer)
{
	sclf::WriteBackendsFeatures(Frontend.Language(), Writer);

	return sclf::WriteLoginFeatures( Writer );
}

namespace {
	const str::dString &GetType_(
		sclx::sProxy &Proxy,
		str::dString &Type )
	{
		return Proxy.GetValue( login::BackendTypeId, Type );
	}

	qENUM( BackendType_ )
	{
		// Below both types are special backend types.
		btNone,			// No backend.
		btPredefined,	// Predefined backend.
		// Below backend types are plugins.
		btEmbedded,
		btStraight,
		btProxy,
		bt_amount,
		bt_Undefined
	};

	stsfsm::wAutomat TypeAutomat_;

#define C( name )	case bt##name : return #name; break

	const char *GetLabel_( eBackendType_ Type )
	{
		switch ( Type ) {
		case btNone:
			return sclf::NoneBackendType;
			break;
		case btPredefined:
			return sclf::PredefinedBackendType;
			break;
			C( Embedded );
			C( Straight );
			C( Proxy );
		default:
			qRFwk();
			break;
		}

		return NULL;	// To avoid a warning.
	}

#undef C

	void FillTypeAutomat_( void )
	{
		TypeAutomat_.Init();
		stsfsm::Fill<eBackendType_>( TypeAutomat_, bt_amount, GetLabel_ );
	}

	eBackendType_ GetType_( const str::dString &Pattern )
	{
		return stsfsm::GetId( Pattern, TypeAutomat_, bt_Undefined, bt_amount );
	}

	eBackendType_ GetType_( sclx::sProxy &Proxy )
	{
		eBackendType_ Type = bt_Undefined;
	qRH;
		str::wString Pattern;
	qRB;
		Pattern.Init();

		Type = GetType_( GetType_( Proxy, Pattern ) );
	qRR;
	qRT;
	qRE;
		return Type;
	}

#define A( name )	Ids.Append(login::name##BackendId)

	void SetIds_( str::dStrings &Ids )
	{
		A( Predefined );
		A( Remote );
		A( Proxyfied );
		A( Embedded );
	}

#undef A

	void SetClasses_( str::dStrings &Classes )
	{
		Classes.Append("hide");
		Classes.Append("hide");
		Classes.Append("hide");
		Classes.Append("hide");
	}

	void HideAll_( sclx::sProxy &Proxy )
	{
	qRH;
		str::wStrings Ids, Classes;
	qRB;
		tol::Init( Ids, Classes );

		SetIds_( Ids );
		SetClasses_( Classes );

		Proxy.AddClasses( Ids, Classes );
	qRR;
	qRT;
	qRE;
	}
}

#define S( name )	Proxy.RemoveClass( login::name##BackendId, "hide" )

void scls::login::HandleBackendTypeSwitching(
  sclx::sProxy &Proxy,
  eBackendVisibility Visibility)
{
	HideAll_( Proxy );

	switch ( GetType_( Proxy ) ) {
	case btNone:
		// Nothing to do ; all forms remain hidden.
		break;
	case btPredefined:
		S( Predefined );
		break;
	case btEmbedded:
		S( Embedded );
		break;
	case btStraight:
		S( Remote );
		break;
	case btProxy:
		S( Proxyfied );
		break;
	default:
		qRFwk();
		break;
	}

	switch ( Visibility ) {
	case bvHide:
		Proxy.AddClass(BackendId, "hide");
		break;
	case bvShow:
		Proxy.RemoveClass(BackendId, "hide");
		break;
	default:
		qRFwk();
		break;
	}
}

#undef S

namespace {
	void NormalizeParameters_(
		str::dString &Parameters,
		const str::dString DefaultPort )
	{
		if ( Parameters.Search( ':' ) == qNIL ) {
			if ( DefaultPort.Amount() ) {
				Parameters.Append( ':' );
				Parameters.Append( DefaultPort );
			}
		}
	}
}

namespace straight_ {
	namespace {
		rgstry::rEntry DefaultPort_( "@DefaultStraightPort", sclf::BackendParametersRegistryEntry );
	}

	void Normalize( str::dString &Parameters )
	{
	qRH
		str::wString Port;
	qRB
		Port.Init();
		sclm::OGetValue( DefaultPort_, Port );

		NormalizeParameters_( Parameters, Port );
	qRR
	qRT
	qRE
	}
}

namespace proxy_ {
	namespace {
		rgstry::rEntry DefaultPort_( "@DefaultProxyPort", sclf::BackendParametersRegistryEntry );
	}

	void Normalize( str::dString &Parameters )
	{
	qRH
		str::wString Port;
	qRB
		Port.Init();
		sclm::OGetValue( DefaultPort_, Port );

		NormalizeParameters_( Parameters, Port );
	qRR
	qRT
	qRE
	}
}

void scls::login::GetBackendFeatures(
	sclx::sProxy &Proxy,
	sclf::rBackendFeatures &Features )
{
qRH;
	eBackendType_ Type = bt_Undefined;
	str::wString Buffer;
	str::string Parameters;
qRB;
	tol::Init(Parameters, Buffer);

	switch ( Type = GetType_( Proxy ) ) {
	case btNone:
		break;
	case btPredefined:
		Parameters.Append( Proxy.GetValue( PredefinedBackendId, Buffer ) );
		break;
	case btEmbedded:
		Parameters.Append( Proxy.GetValue( EmbeddedBackendId, Buffer ) );
		break;
	case btStraight:
		Parameters.Append( Proxy.GetValue( RemoteBackendId, Buffer ) );
		straight_::Normalize( Parameters );
		break;
	case btProxy:
		Parameters.Append( Proxy.GetValue( ProxyfiedBackendId, Buffer ) );
		proxy_::Normalize( Parameters );
		break;
	default:
		qRFwk();
		break;
	}

	sclf::SetBackendFeatures( str::wString( GetLabel_( Type ) ), Parameters, Features );
qRR;
qRT;
qRE;
}

void scls::login::DisplaySelectedEmbeddedBackendFilename(
	sclx::sProxy &Proxy,
	const str::dString &Id )
{
qRH
	str::wString Buffer;
	str::string FileName;
	xdhcmn::digest Args;
	xdhcmn::retriever__ Retriever;
qRB
	tol::Init(Args, Buffer);
	xdhcmn::Split( str::string( Proxy.GetValue( Id, Buffer ) ), Args );

	Retriever.Init( Args );

	FileName.Init();

	if ( Retriever.Availability() != strmrg::aNone )
		Retriever.GetString( FileName );

	if ( FileName.Amount() != 0 )
		Proxy.SetValue( EmbeddedBackendId, FileName );
qRR
qRT
qRE
}

qGCTOR( scls )
{
	FillTypeAutomat_();
}
