/*
	'lcl.cpp' by Claude SIMON (http://zeusw.org/).

	'lcl' is part of the Epeios framework.

    The Epeios framework is free software: you can redistribute it and/or
	modify it under the terms of the GNU General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Epeios framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Epeios framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#define LCL__COMPILATION

#include "lcl.h"

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

using namespace lcl;

static inline vrow__ NewValue_(
	const str::string_ &Value,
	values_ &Values )
{
	vrow__ Row = Values.New();

	Values( Row ).Init( Value );

	Values.Flush();

	return Row;
}

static inline brow__ NewBasic_(
	vrow__ Value,
	bso::bool__ ToTranslate,
	_basics_ &Basics )
{
	brow__ Row = Basics.New();

	Basics( Row ).Init( ToTranslate );

	Basics( Row ).S_.Value = Value;

	Basics.Flush();

	return Row;
}

static inline brow__ NewBasic_(
	const str::string_ &Value,
	bso::bool__ ToTranslate,
	_core_ &Core )
{
	return NewBasic_( NewValue_( Value, Core.Values ), ToTranslate, Core.Basics );
}

static inline brow__ NewBasic_(
	const _basic_ &SourceBasic,
	const _core_ &SourceCore,
	_core_ &NewCore );	// Pré-déclaration

static void Duplicate_(
	const brows_ &Tags,
	const _core_ &SourceCore,
	_basic_ &Basic,
	_core_ &NewCore )
{
	ctn::E_CMITEMt( _basic_, brow__ ) SourceBasic;
	sdr::row__ Row = Tags.First();

	SourceBasic.Init( SourceCore.Basics );

	while ( Row != E_NIL ) {
		Basic.Tags.Append( NewBasic_( SourceBasic( Tags( Row ) ), SourceCore, NewCore ) );

		Row = Tags.Next( Row );
	}
}

static inline brow__ NewBasic_(
	const _basic_ &SourceBasic,
	const _core_ &SourceCore,
	_core_ &NewCore )
{
	brow__ Row = E_NIL;
ERRProlog
	ctn::E_CMITEMt( str::string_, vrow__ ) Value;
	_basic Basic;
ERRBegin
	Value.Init( SourceCore.Values );
	Row = NewBasic_( Value( SourceBasic.S_.Value ), SourceBasic.ToTranslate(), NewCore );

	Basic.Init( NewCore.Basics( Row ).ToTranslate() );
	Basic = NewCore.Basics( Row );
	NewCore.Basics.Flush();

	Duplicate_( SourceBasic.Tags, SourceCore, Basic, NewCore );

	NewCore.Basics( Row ) = Basic;
	NewCore.Basics.Flush();
ERRErr
ERREnd
ERREpilog
	return Row;
}

static inline brow__ NewBasic_(
	const meaning_ &Meaning,
	_core_ &Core )
{
	ctn::E_CMITEMt( _basic_, brow__ ) Basic;

	return NewBasic_( Meaning.GetBasic( Basic ), Meaning.Core, Core );
}

void lcl::meaning_::SetValue( const str::string_ &Value )
{
#if 1
	vrow__ Row = NewValue_( Value, Core.Values );
	_Basic().S_.Value = Row;
#else
	_Basic().S_.Value = NewValue_( Value, Core.Values );
#endif

	Core.Basics.Flush();
}

void lcl::meaning_::AddTag( const str::string_ &Value )
{
#if 1
	brow__ Row = NewBasic_( Value, false, Core );
	_Basic().Tags.Append( Row );
#else
	_Basic().Tags.Append( NewBasic_( Value, false, Core ) );
#endif

	Core.Basics.Flush();
}

void lcl::meaning_::AddTag( const meaning_ &Meaning )
{
#if 1
	brow__ Row = NewBasic_( Meaning, Core );
	_Basic().Tags.Append( Row );
#else
	_Basic().Tags.Append( NewBasic_( Meaning, Core ) );
#endif

	Core.Basics.Flush();
}

void lcl::locale_::_GetCorrespondingLabels(
	const strings_ &Labels,
	strings_ &Wordings ) const
{
ERRProlog
	ctn::E_CMITEM( str::string_ ) Label;
	str::string Wording;
	sdr::row__ Row = E_NIL;
	str::string Path;
ERRBegin
	Label.Init( Labels );

	Row = Labels.First();

	while ( Row != E_NIL ) {
		Path.Init( "Languages/Language[label=\"" );
		Path.Append( Label( Row ) );
		Path.Append( "\"]/@Wording" );

		Wording.Init();
		if ( Registry.GetValue( Path, Wording ) )
			Wordings.Append( Wording );
		else
			Wordings.Append( Label( Row ) );

		Row = Labels.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

bso::bool__ lcl::locale_::_GetTranslationFollowingLanguageThenMessage(
	const str::string_ &Text,
	const char *Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]/Translation[message=\"" );
	Path.Append( Text );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ lcl::locale_::_GetTranslationFollowingMessageThenLanguage(
	const str::string_ &Text,
	const char *Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[message=\"" );
	Path.Append( Text );
	Path.Append( "\"]/Translation[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ lcl::locale_::_GetTranslationFollowingLanguageThenText(
	const str::string_ &Text,
	const char *Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]/Translation[text=\"" );
	Path.Append( Text );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

bso::bool__ lcl::locale_::_GetTranslationFollowingTextThenLanguage(
	const str::string_ &Text,
	const char *Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;
ERRProlog
	str::string Path;
ERRBegin
	Path.Init( "Translations[text=\"" );
	Path.Append( Text );
	Path.Append( "\"]/Translation[language=\"" );
	Path.Append( Language );
	Path.Append( "\"]" );

	Found = Registry.GetValue( Path, Translation );
ERRErr
ERREnd
ERREpilog
	return Found;
}

void lcl::locale_::_GetTranslation(
	const str::string_ &Text,
	const char *Language,
	str::string_ &Translation ) const
{
	bso::bool__ Found = false;

	if ( !( Found = _GetTranslationFollowingLanguageThenText( Text, Language, Translation ) ) )
		if ( !( Found = _GetTranslationFollowingTextThenLanguage( Text, Language, Translation ) ) )
			if ( !( Found = _GetTranslationFollowingLanguageThenMessage( Text, Language, Translation ) ) )	// Pour des raisons de compatibilité ascendante.
				Found = _GetTranslationFollowingMessageThenLanguage( Text, Language, Translation );	// Pour des raisons de compatibilité ascendante.

	if ( !Found ) {
		Translation.Append( Text );
		Translation.Append( LCL_TAG_MARKER_S "0" );	// Lorsque pas de traduction trouvée, on rajoute le 'marker' remplacé par l'ensemble des paramètres.
	}
}


void lcl::locale_::GetLanguages(
	strings_ &Labels,
	strings_ &Wordings ) const
{
	sdr::row__ PathErrorRow = E_NIL;

	Registry.GetValues( str::string( "Languages/Language/@label" ), Labels, &PathErrorRow );

	_GetCorrespondingLabels( Labels, Wordings );

	if ( PathErrorRow != E_NIL )
		ERRFwk();
}

static const str::string_ &GetTranslation_(
	const _basic_ &Basic,
	const _core_ &Core,
	const char *Language,
	const locale_ &Locale,
	str::string_ &Translation );	// Pré-déclaration.

static void GetTags_(
	const brows_ &InputTags,
	const _core_ &Core,
	const char *Language,
	const locale_ &Locale,
	str::strings_ &Tags )
{
ERRProlog
	ctn::E_CMITEMt( _basic_, brow__ ) Basic;
	sdr::row__ Row = E_NIL;
	str::string Translation;
ERRBegin
	Row = InputTags.First();

	Basic.Init( Core.Basics );

	while ( Row != E_NIL ) {
		Translation.Init();

		GetTranslation_( Basic( InputTags( Row ) ), Core, Language, Locale, Translation );
		Tags.Append( Translation );

		Row = InputTags.Next( Row );
	}
ERRErr
ERREnd
ERREpilog
}

static const str::string_ &GetTranslation_(
	const _basic_ &Basic,
	const _core_ &Core,
	const char *Language,
	const locale_ &Locale,
	str::string_ &Translation )
{
ERRProlog
	str::strings Tags;
	str::string Value, Intermediate;
	TOL_CBUFFER___ Buffer;
ERRBegin
	if ( Basic.S_.Value == E_NIL )
		ERRReturn;

	Value.Init();
	Core.Values.Recall( Basic.S_.Value, Value );

	Intermediate.Init();
	if ( Basic.GetToTranslate() )
		Locale.GetTranslation( Value.Convert( Buffer ), Language, Intermediate );
	else
		Intermediate = Value;

	Tags.Init();
	GetTags_( Basic.Tags, Core, Language, Locale, Tags );

	str::ReplaceTags( Intermediate, Tags, LCL_TAG_MARKER_C );

	Translation = Intermediate;
ERRErr
ERREnd
ERREpilog
	return Translation;
}

const str::string_  &lcl::locale_::GetTranslation(
	const meaning_ &Meaning,
	const char *Language,
	str::string_ &Translation ) const
{
	ctn::E_CMITEMt( _basic_, brow__ ) Basic;

	return GetTranslation_( Meaning.GetBasic( Basic ), Meaning.Core, Language, *this, Translation );
}



/*

const char *lcl::locale_::GetTranslation(
	const char *Text,
	const char *Language,
	STR_BUFFER___ &Buffer ) const
{
ERRProlog
	str::string Translation;
ERRBegin
	Translation.Init();

	GetTranslation( Text, Language, Translation );

	Translation.Convert( Buffer );
ERRErr
ERREnd
ERREpilog
	return Buffer;
}
*/

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class lclpersonnalization
{
public:
	lclpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~lclpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

static lclpersonnalization Tutor;
