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

// eXtensible Markup Language

#ifndef XML_INC_
# define XML_INC_

# define XML_NAME		"XML"

# if defined( E_DEBUG ) && !defined( XML_NODBG )
#  define XML_DBG
# endif

#include "err.h"
#include "flw.h"
#include "str.h"
#include "xtf.h"
#include "stkctn.h"
#include "ctn.h"
#include "cpe.h"

/***************/
/***** New *****/
/***************/

// Prdclaration.
namespace lcl {
	class meaning_;
}

namespace xml {
	typedef str::string_	name_;
	typedef str::string		name;

	typedef str::string_	value_;
	typedef str::string		value;

		// Code d'erreur 'retourn' par 'Parse()'.
	qENUM(Status) {
		sOK,
		s_FirstXTFError,
		eUnsupportedEncoding = s_FirstXTFError + xtf::eUnsupportedEncoding,
		eUnexpectedEncoding = s_FirstXTFError + xtf::eUnexpectedEncoding,
		eEncodingDiscrepancy = s_FirstXTFError + xtf::eEncodingDiscrepancy,
		s_FirstNonXTFError,
		sUnexpectedEOF = s_FirstNonXTFError,
		sUnknownEntity,
		sMissingEqualSign,
		sBadAttributeValueDelimiter,
		sUnexpectedCharacter,
		sMismatchedTag,
		s_amount,
		s_Undefined,
	};


	// Output layout.
	qENUM( Layout ) {
		lCompact,	// All of one line.
		oCompact = lCompact,	// Old
		lIndent,	// Several line with indentation.
		oIndent = lIndent,	// Old
		l_amount,
		l_Undefined,
		o_Undefined = l_Undefined	// Old
	};

	// Special characters handling ('<', '&', ...).
	qENUM( SpecialCharHandling ) {
		schReplace,	// Replaced with their entities.
		schKeep,	// Not replaced, except the '"' in the attribute values.
		sch_amount,
		sch_Undefined,
		sch_Default = schReplace
	};

	enum _encoding__ {
		eISO_8859_1,
		eUTF_8,
		e_amount,
		e_Undefined,
		e_Default = eUTF_8,
		e_None,
	};

	inline const char *Label( _encoding__ Encoding )
	{
		switch ( Encoding ) {
		case eISO_8859_1:
			return "iso-8859-1";
			break;
		case eUTF_8:
			return "utf-8";
			break;
		case e_None:
			return NULL;
			break;
		default:qRFwk();
			break;
		}
		return NULL;	// Pour viter un 'Warning'.
	}

	class fEncoding
	{
	private:
		const char *_EncodingString;
	public:
		fEncoding( void )
		{
			_EncodingString = Label( e_Default );
		}
		fEncoding( _encoding__ Encoding )
		{
			_EncodingString = Label( Encoding );
		}
		fEncoding( const char *Encoding )
		{
			_EncodingString = Encoding;
		}
		const char *EncodingString( void ) const
		{
			return _EncodingString;
		}
	};

	E_TMIMIC__( stkctn::sRow, sMark );

	qCDEF( sMark, Undefined, (sMark)qNIL );

	inline bso::bool__ WriteXMLHeader(
		txf::text_oflow__ &OFlow,
		const char *Encoding )
	{
		if ( Encoding != NULL ) {
			OFlow << "<?xml version=\"1.0\" encoding=\"" << Encoding << "\"?>";
			return true;
		} else
			return false;
	}

	inline bso::bool__ WriteXMLHeader(
		txf::text_oflow__ &OFlow,
		_encoding__ Encoding )
	{
		return WriteXMLHeader( OFlow, Label( Encoding ) );
	}


	inline bso::bool__ WriteXMLHeader(
		txf::text_oflow__ &OFlow,
		fEncoding Encoding )
	{
		return WriteXMLHeader( OFlow, Encoding.EncodingString() );
	}

	class parser___;

	typedef parser___ rParser;

	typedef bso::sU8 sLevel;

	qENUM( Delimiter )
	{
		dAPOS,
		dQUOT,
		d_amount,
		d_Undefined,
		d_Default
	};

	namespace delimiter {
		qCDEF( char, sAPOS, '\'' );
		qCDEF( char, sQUOT, '"' );
		qCDEF( char, sDefault, sQUOT );
		qCDEF( char, sUndefined, sQUOT );
	}

	inline eDelimiter Convert( bso::sChar Delimiter )
	{
		switch ( Delimiter ) {
		case delimiter::sAPOS:
			return dAPOS;
			break;
		case delimiter::sQUOT:
			return dQUOT;
			break;
		default:
			qRFwk();
			break;
		}

		return d_Undefined;	// To avoid a warning.
	}

	class rWriter
	{
	private:
		stkctn::qMCSTACKwl( name_ ) Tags_;
		txf::text_oflow__ *Flow_;
		bso::bool__ OpeningTagInProgress_;
		bso::bool__ TagValueInProgress_;
		eLayout Outfit_;
		eSpecialCharHandling SpecialCharHandling_;
		sLevel LevelsToIgnore_;	// Tags which level (beginning at 1) are inferior to this value are ignored.
									// With a value of '1', the root tag is ignored.
		bso::bool__ AlwaysCommit_;	// Fait un 'commit' aprs chaque criture. Utile pour le dboguage d'application.
		bso::sChar AttributeDelimiter_;
		void CloseAllTags_( void );
		void Indent_( bso::size__ Amount ) const;
		txf::text_oflow__ &F_( void ) const
		{
			if ( Flow_ == NULL )
				qRFwk();

			if ( LevelsToIgnore_ >= ( Tags_.Amount() + ( OpeningTagInProgress_ ? 0 : 1 ) ) )
				return txf::WVoid;
			else
				return *Flow_;
		}
		flw::oflow__ &RF_( void ) const
		{
			return F_().Flow();
		}
		void Commit_( void )
		{
			if ( AlwaysCommit_ )
				F_().Commit();
		}
		bso::sChar GetAttributeDelimiter_( eDelimiter Delimiter ) const
		{
			switch ( Delimiter ) {
			case dAPOS:
				return delimiter::sAPOS;
				break;
			case dQUOT:
				return delimiter::sQUOT;
				break;
			case d_Default:
				if ( strchr( "\"'", AttributeDelimiter_ ) == NULL )
					qRFwk();
				break;
			default:
				qRFwk();
				break;
			}

			return AttributeDelimiter_;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P )
				CloseAllTags_();

			OpeningTagInProgress_ = false;
			TagValueInProgress_ = false;

			Tags_.reset( P );
			Flow_ = NULL;
			Outfit_ = o_Undefined;
			SpecialCharHandling_ = sch_Undefined;
			LevelsToIgnore_ = 0;
			AlwaysCommit_ = false;
			AttributeDelimiter_ = delimiter::sUndefined;
		}
		qCDTOR( rWriter );
		void Init(
			txf::text_oflow__ &Flow,
			eLayout Outfit,
			fEncoding Encoding,
			sLevel LevelsToIgnore = 0,
			eSpecialCharHandling SpecialCharHandling = sch_Default )
		{
			reset();

			Tags_.Init();
			Flow_ = &Flow;
			Outfit_ = Outfit;
			LevelsToIgnore_ = LevelsToIgnore;
			SpecialCharHandling_ = SpecialCharHandling;
			AttributeDelimiter_ = delimiter::sDefault;

			if ( WriteXMLHeader( Flow, Encoding ) )
				switch ( Outfit ) {
				case oIndent:
					Flow << txf::nl;
					break;
				case oCompact:
					break;
				default:
					qRFwk();
					break;
			}
		}
		void Init(
			txf::text_oflow__ &Flow,
			eLayout Outfit,
			fEncoding Encoding,
			eSpecialCharHandling SpecialCharHandling )
		{
			return Init( Flow, Outfit, Encoding, 0, SpecialCharHandling );

		}
		sMark GetMark( void ) const
		{
			return Tags_.Last();
		}
		sMark PushTag( const name_ &Name )	// See 'PopTag(...)' for the returned value.
		{
			if ( OpeningTagInProgress_ ) {
				F_() << '>';

				if ( Outfit_ == oIndent )
					F_() << txf::nl;
			}

			sMark Mark = Tags_.Push( Name );

			OpeningTagInProgress_ = true;
			TagValueInProgress_ = false;

			if ( Outfit_ == oIndent )
				Indent_( Tags_.Amount() - 1 );

			F_() << '<' << Name;

			Commit_();

			return Mark;
		}
		sMark PushTag( const char *Name )	// See 'PopTag(...)' for the returned value.
		{
			return PushTag( name( Name ) );
		}
		void PutRawValue( flw::rRFlow &Flow );
		void PutValue( flw::rRFlow &Flow );
		void PuRawValue( const value_ &Value );
		void PutValue( const value_ &Value );
		void PutValue( const char *Value )
		{
			PutValue( value( Value ) );
		}
		void PutValue(
			const value_ &Value,
			const name_ &Name )
		{
			PushTag( Name );

			PutValue( Value );

			PopTag();
		}
		void PutValue(
			const value &Value,
			const name_ &Name )
		{
			PutValue( *Value, Name );
		}
		void PutValue(
			const value_ &Value,
			const name &Name )
		{
			PutValue( Value, *Name );
		}
		void PutValue(
			const value &Value,
			const name &Name )
		{
			PutValue( *Value, *Name );
		}
		void PutValue(
			const value_ &Value,
			const char *Name )
		{
			PutValue( Value, name( Name ) );
		}
		void PutValue(
			const value &Value,
			const char *Name )
		{
			PutValue( *Value, Name );
		}
		void PutValue(
			const char *Value,
			const char *Name )
		{
			PutValue( value( Value ), name( Name ) );
		}
		template <typename s, typename i> void PutValue(
			i Value,
			const s &Name )
		{
			bso::integer_buffer__ IBuffer;

			PutValue( bso::Convert( Value, IBuffer ), Name );
		}
		template <typename s, typename i> void PutValue(
			i Value,
			const s &Name,
			i UndefinedValue )
		{
			if ( Value == UndefinedValue )
				PutValue( "", Name );
			else
				PutValue( Value, Name );
		}
		void PutRawAttribute(
			const name_ &Name,
			flw::rRFlow &Flow,
			eDelimiter Delimiter = d_Default );
		void PutAttribute(
			const name_ &Name,
			flw::rRFlow &Flow,
			eDelimiter Delimiter = d_Default );
		void PutAttribute(
			const name &Name,
			flw::rRFlow &Flow )
		{
			PutAttribute( *Name, Flow );
		}
		void PutRawAttribute(
			const name_ &Name,
			const value_ &Value,
			eDelimiter Delimiter = d_Default );
		void PutAttribute(
			const name_ &Name,
			const value_ &Value,
			eDelimiter Delimiter = d_Default );
		void PutAttribute(
			const value_ &Value,
			const name &Name,
			eDelimiter Delimiter = d_Default )
		{
			PutAttribute( Value, *Name, Delimiter );
		}
		void PutAttribute(
			const value &Value,
			const name &Name,
			eDelimiter Delimiter = d_Default )
		{
			PutAttribute( *Value, *Name, Delimiter );
		}
		void PutAttribute(
			const value &Value,
			const name_ &Name,
			eDelimiter Delimiter = d_Default )
		{
			PutAttribute( *Value, Name, Delimiter );
		}
		void PutAttribute(
			const value_ &Value,
			const char *Name,
			eDelimiter Delimiter = d_Default )
		{
			PutAttribute( Value, name( Name ), Delimiter );
		}
		void PutAttribute(
			const value &Value,
			const char *Name,
			eDelimiter Delimiter = d_Default )
		{
			PutAttribute( *Value, Name, Delimiter );
		}
		void PutAttribute(
			const char *Name,
			const char *Value,
			eDelimiter Delimiter = d_Default )
		{
			PutAttribute( name( Name ), value( Value ), Delimiter );
		}
		void PutAttribute(
			const char *Name,
			const value_ &Value,
			eDelimiter Delimiter = d_Default )
		{
			PutAttribute( name( Name ), Value, Delimiter );
		}
		void PutAttribute(
			const char *Name,
			const value &Value,
			eDelimiter Delimiter = d_Default )
		{
			PutAttribute( Name, *Value, Delimiter );
		}
		template <typename s, typename i> void PutAttribute(
			const s &Name,
			i Value,
			eDelimiter Delimiter = d_Default )
		{
			bso::integer_buffer__ IBuffer;

			PutAttribute( Name, bso::Convert( Value, IBuffer ), Delimiter );
		}
		// No attribute is added if 'Value' == 'UndefinedValue'.
		template <typename s, typename i> void PutAttribute(
			const s &Name,
			i Value,
			i UndefinedValue,
			eDelimiter Delimiter = d_Default )
		{
			if ( Value != UndefinedValue )
				PutAttribute( Name, Value, Delimiter );
		}
		void PutCData( flw::rRFlow &Flow );
		void PutCData( const str::string_ &Value );
		void PutCData( const char *Value )
		{
			PutCData( value( Value ) ) ;
		}
		sMark PopTag(
			bso::sBool NoSelfClosing = false,	// Forces '<tag></tag>' instead of '<tag/>' for empty tag.
			sMark Mark = Undefined );	// 'Mark', is used,is returned by a 'Push(...)', and is used to control if we are at the same level as the concerned 'Push(...)'.
		sMark PopTag(
			sMark Mark,
			bso::sBool NoSelfClosing = false)
		{
			return PopTag(NoSelfClosing, Mark);
		}
		void Rewind( sMark Mark );	// Unwind to 'Mark' level.
		txf::text_oflow__ &GetFlow( void )
		{
			return *Flow_;
		}
		void AlwayCommit( bso::bool__ Value = true )
		{
			AlwaysCommit_ = Value;
		}
		eStatus Put(rParser &Parser);
		// Indent and put the content of 'XFlow' (reparse it).
		eStatus Put(xtf::extended_text_iflow__ &XFlow);
		// Indent and put the content of 'XML' (reparse it).
		eStatus Put(
			const str::dString &XML,
			xtf::sPos &ErrorPosition);	// Only valid when returned valus != 'sOK'.
		qRODISCLOSEr( eLayout, Outfit )
		qRODISCLOSEr( eSpecialCharHandling, SpecialCharHandling )
	};
}

/***************/
/***** Old *****/
/***************/

namespace xml {
	using xtf::pos__;

	typedef eStatus status__;

	const char *GetLabel( status__ Status );

	void GetMeaning(
		status__ Status,
		const pos__ &Position,	// Obtenu de 'xtf::extended_text_iflow__'.
		lcl::meaning_ &Meaning );

	inline bso::bool__ IsEncodingRelatedError( status__ Status )
	{
		return ( Status >= s_FirstXTFError ) && ( Status < s_FirstNonXTFError );
	}

	class dump_ {
	public:
		struct s {
			str::string_::s Data;
			pos__ Position;
		} &S_;
		str::string_ Data;
		dump_( s &S )
		: S_( S ),
		  Data( S.Data )
		{
		}
		void reset( bso::bool__ P = true )
		{
			Data.reset( P );

			S_.Position.reset( P );
		}
		void plug( str::sHook &Hook )
		{
			Data.plug( Hook );
		}
		void plug( qASd *AS )
		{
			Data.plug( AS );
		}
		dump_ &operator =( const dump_ &D )
		{
			Data = D.Data;

			S_.Position = D.S_.Position;

			return *this;
		}
		void Init( void )
		{
			Data.Init();

			S_.Position.Init();
		}
		void Set( pos__ Position )
		{
			S_.Position = Position;
		}
		void Append( const dump_ &Dump )
		{
			if ( Data.Amount() == 0 )
				this->operator =( Dump );
			else
				Data.Append( Dump.Data );
		}
		void TrimHeadingSpaces(void)
		{
		  sdr::sRow Row = Data.First();

      while ( ( Row != qNIL ) && isspace(Data(Row)) )
        Row = Data.Next(Row);

      if ( Row != qNIL )
        Data.Crop(Row, Data.Last());
		}
    void PurgeData( void )
		{
			Init();
		}
		E_RWDISCLOSE_( pos__, Position );
	};

	E_AUTO( dump )

	class _flow___ {
	private:
		xtf::extended_text_iflow__ *_Flow;
	public:
		dump Dump;
		void reset( bso::bool__ P = true )
		{
			Dump.reset( P );
			_Flow = NULL;
		}
		_flow___( void )
		{
			reset( false );
		}
		~_flow___( void )
		{
			reset();
		}
		flw::byte__ Get( xtf::utf__ &UTF )
		{
			if ( Dump.Data.Amount() == 0 )
				Dump.Set( _Flow->Position() );

			flw::byte__ C = _Flow->Get( UTF );

			Dump.Data.Append( (const bso::char__ *)UTF.Data, UTF.Size );

			return C;
		}
		flw::byte__ Get( void )
		{
			xtf::utf__ Dummy;
			Dummy.Init();

			return Get( Dummy );
		}
		flw::byte__ View( xtf::utf__ &UTF )
		{
			flw::byte__  C = _Flow->View( UTF );

			return C;
		}
		flw::byte__ View( void )
		{
			xtf::utf__ Dummy;
			Dummy.Init();

			return View( Dummy );
		}
		bso::bool__ EndOfFlow( void )
		{
			xtf::error__ Error = xtf::e_NoError;
			bso::bool__ EOFlow = _Flow->EndOfFlow( Error );

			if ( EOFlow && ( Error != xtf::e_NoError ) )
				qRFree();

			return EOFlow;
		}
		void TrimHeadingSpaces(void)
		{
		  Dump.TrimHeadingSpaces();
		}
		void Purge( void )
		{
			Dump.PurgeData();
		}
		void Init( xtf::extended_text_iflow__ &Flow )
		{
			reset();

			_Flow = &Flow;

			Dump.Init();
		}
		const xtf::pos__ &GetCurrentPosition( void ) const
		{
			return _Flow->Position();
		}
		xtf::extended_text_iflow__ &Flow( void )
		{
			return *_Flow;
		}
		const xtf::extended_text_iflow__ &Flow( void ) const
		{
			return *_Flow;
		}
	};

	enum _context__ {
		cHeaderExpected,
		cTagExpected,
		cTagConfirmed,
		cOpeningTag,
		cClosingTag,
		cAttribute,
		cValueExpected,
		c_amount,
		c_Undefined
	};

	enum token__
	{
		tProcessingInstruction,
		tStartTag,
		tStartTagClosed,
		tAttribute,
		tSpecialAttribute,	// 'xmlns[:...]', par exemple.
		tValue,
		tEndTag,
		tComment,
		tCData,
		t_amount,
		t_Processed,	// Tout le flux XML a t trait.
		t_Error,	// Erreur dans l'analyse du flux XML; voir 'Status'.
		t_Undefined
	};

#ifdef TF
#	define XML__TF_BUFFER	TF
#	undef TF
#endif

#define TF( name )	tf##name = ( 1 << t##name )

	// Permet de n'avoir  traiter que certains 'token's.
	enum token_flag__
	{
		tfNone = 0,	// Pour n'obtenir que 'tError' et 'tProcessed', qui ne peuvent tre ignors.
		TF( ProcessingInstruction ),
		TF( StartTag ),
		TF( StartTagClosed ),
		TF( Attribute ),
		TF( SpecialAttribute ),
		TF( Value ),
		TF( EndTag ),
		TF( Comment ),
		TF( CData ),
		tfAll = ( ( 1 << t_amount ) - 1 ),
		tfAllButUseless = tfAll & ~( tfComment | tfSpecialAttribute | tfCData ),
		tfObvious = tfStartTag | tfAttribute | tfValue | tfEndTag,
		tfAllAttributes = tfAttribute | tfSpecialAttribute,
	};

#undef TF

#ifdef XML__TF_BUFFER
#	define	TF	XML__TF_BUFFER
#endif
	// Traitement des entits ('&...;').
	enum entities_handling__ {
		ehReplace,	// Les 'entity's sont remplacs par leur caractre correspondant.
		ehKeep,		// Les 'entity's sont gards tel quel.
		eh_amount,
		eh_Undefined,
		eh_Default = ehReplace,
	};

	class parser___
	{
	private:
		_context__ _Context;
		token__ _Token;
		stkctn::qMCSTACKwl( str::dString ) _Tags;
		bso::bool__ SelfClosing_;	// A 'true' pour '<tag/>', sinon  'false'.
		_flow___ _Flow;
		str::string _TagName;
		str::string AttributeName_;
		bso::sChar AttributeDelimiter_;
		str::string _Value;
		status__ _Status;
		entities_handling__ _EntitiesHandling;
		void PartialReset_(bso::sBool P)
		{
			_Context = c_Undefined;
			_Token = t_Undefined;
			SelfClosing_ = false;

			_TagName.reset( P );
			AttributeName_.reset( P );
			AttributeDelimiter_ = delimiter::sUndefined;
			_Value.reset( P );
			_Status = s_Undefined;

			_Tags.reset( P );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			PartialReset_(P);

			_Flow.reset( P );
			_EntitiesHandling = eh_Undefined;
		}
		parser___( void )
		{
			reset( false );
		}
		virtual ~parser___( void )
		{
			reset();
		}
		void Init(
			xtf::extended_text_iflow__ &Flow,
			entities_handling__ EntitiesHandling )
		{
			reset();

			_Tags.Init();
			_Flow.Init( Flow );
			_Context = cHeaderExpected;

			_TagName.Init();
			AttributeName_.Init();

			_Value.Init();

			_EntitiesHandling = EntitiesHandling;
		}
		// If input data contains more then one root, allows to process next tree.
		void Reset(void)
		{
			if ( _Token != t_Processed )
				qRFwk();

			PartialReset_(true);
			tol::Init(_Tags, _TagName, AttributeName_, _Value);

			_Context = cHeaderExpected;
		}
		token__  Parse( int TokenToReport = tfAll );
		token__ Parse(
			str::string_ &TagName,
			str::string_ &AttributeName,
			bso::sChar &AttributeDelimiter,
			str::string_ &Value,	// Contient la valeur d'une balise ('tag') our d'un attribut, en fonction de la valeur retourne ('tTag' ou 'tAttribute').
			xml::dump_ &Dump,
			status__ &Status,
			int TokenToReport = tfAll )	// 'Status' initialis seulement si valeur retourne == 'tError'.
		{
			token__ Token = Parse( TokenToReport );

			TagName = _TagName;
			AttributeName = AttributeName_;
			AttributeDelimiter = AttributeDelimiter_;
			Value = _Value;
			Status = _Status;

			return Token;
		}
		void Skip( void );	// Saute le contenu de la balise en cours.
		const xtf::pos__ &DumpPosition( void ) const
		{
			return _Flow.Dump.Position();
		}
		const str::string_ &DumpData( void ) const
		{
			return _Flow.Dump.Data;
		}
		void TrimDumpDataHeadingSpaces(void)
		{
		  _Flow.Dump.TrimHeadingSpaces();
		}
		void PurgeDumpData( void )
		{
			_Flow.Dump.PurgeData();
		}
		E_RODISCLOSE__( str::string_, TagName );
		qRODISCLOSEr( str::string_, AttributeName );
		qRODISCLOSEr( bso::sChar, AttributeDelimiter );
		E_RODISCLOSE__( str::string_, Value );
		E_RODISCLOSE__( status__, Status );
		E_RODISCLOSE__( token__, Token );
		E_RODISCLOSE__( entities_handling__, EntitiesHandling );
		qRODISCLOSEs(bso::sBool, SelfClosing);
		const xtf::pos__ &GetCurrentPosition( void ) const
		{
			return _Flow.GetCurrentPosition();
		}
		xtf::extended_text_iflow__ &Flow( void )
		{
			return _Flow.Flow();
		}
		const xtf::extended_text_iflow__ &Flow( void ) const
		{
			return _Flow.Flow();
		}
		utf::format__ GetFormat( void ) const
		{
			return Flow().Format();
		}
		bso::bool__ SetFormat( utf::format__ Format )
		{
			return Flow().SetFormat( Format );
		}
	};

	struct callback__
	{
		virtual bso::bool__ XMLProcessingInstruction( const dump_ &Dump ) = 0;
		virtual bso::bool__ XMLStartTag(
			const str::string_ &Name,
			const dump_ &Dump ) = 0;
		virtual bso::bool__ XMLStartTagClosed(
			const str::string_ &Name,
			const dump_ &Dump )
		{
			return true;
		}
		virtual bso::bool__ XMLAttribute(
			const str::string_ &TagName,
			const str::string_ &Name,
			bso::sChar AttributeDelimiter,
			const str::string_ &Value,
			const dump_ &Dump ) = 0;
		virtual bso::bool__ XMLSpecialAttribute(
			const str::string_ &TagName,
			const str::string_ &Name,
			bso::sChar AttributeDelimiter,
			const str::string_ &Value,
			const dump_ &Dump )
		{
			return true;
		}
		virtual bso::bool__ XMLValue(
			const str::string_ &TagName,
			const str::string_ &Value,
			const dump_ &Dump ) = 0;
		virtual bso::bool__ XMLEndTag(
			const str::string_ &Name,
			const dump_ &Dump ) = 0;
		virtual bso::bool__ XMLComment(
			const str::string_ &Value,
			const dump_ &Dump )
		{
			return true;
		}
		void Init( void )
		{}
	};

	status__ Parse(
		xtf::extended_text_iflow__ &Flow,
		entities_handling__ EntitiesHandling,
		callback__ &Callback,
		qRPD );

	// Transformation des caractres spciaux, comm '<' qui devient '&lt;'.
	void TransformUsingEntities(
		const str::dString &Source,
		str::dString &Target );

	/*
	inline void TransformUsingEntities(
		const str::string_ &Source,
		str::string_ &Target )	// Conversion des caractres spciaux, comme '<' qui devient '&lt;'.
	{
		Target = Source;

		TransformUsingEntities( Target );
	}
	*/

	typedef eLayout outfit__;

	typedef eSpecialCharHandling special_char_handling__;

	typedef fEncoding encoding__;

	inline void WriteXSLDeclaration(
		const str::string_ &XSLHRef,
		txf::text_oflow__ &OFlow )
	{
		OFlow << "<?xml-stylesheet type=\"text/xsl\" href=\"" << XSLHRef << "\"?>";
	}

	template <typename i> void PutValue(
		i Value,
		xml::rWriter &Writer )
	{
		bso::integer_buffer__ IBuffer;

		Writer.PutValue( bso::Convert( Value, IBuffer ) );
	}

	template <typename s, typename i> void PutValue(
		i Value,
		const s &Name,
		xml::rWriter &Writer )
	{
		bso::integer_buffer__ IBuffer;

		Writer.PutValue( bso::Convert( Value, IBuffer ), Name );
	}

	template <typename s, typename i> void PutAttribute(
		const s &Name,
		i Value,
		xml::rWriter &Writer )
	{
		bso::integer_buffer__ IBuffer;

		Writer.PutAttribute( Name, bso::Convert( Value, IBuffer ) );
	}
}

#endif
