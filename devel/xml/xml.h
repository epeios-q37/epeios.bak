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

#ifndef XML__INC
# define XML__INC

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

# define XML_UNDEFINED_MARK	((xml::mark__)qNIL)

namespace xml {
	typedef str::string_	name_;
	typedef str::string		name;

	typedef str::string_	value_;
	typedef str::string		value;

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

	E_TMIMIC__( stkctn::sRow, mark__ );

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

	class dWriter
	{
	private:
		void _CloseAllTags( void );
		void _Indent( bso::size__ Amount ) const;
		void _Commit( void )
		{
			if ( S_.AlwaysCommit )
				S_.Flow->Commit();
		}
		txf::text_oflow__ &F_( void ) const
		{
			if ( S_.Flow == NULL )
				qRFwk();

			return *S_.Flow;
		}
		flw::oflow__ &RF_( void ) const
		{
			return F_().Flow();
		}
	public:
		struct s {
			stkctn::qMCSTACKdl( name_ )::s Tags;
			txf::text_oflow__ *Flow;
			bso::bool__ TagNameInProgress;
			bso::bool__ TagValueInProgress;
			eLayout Outfit;
			eSpecialCharHandling SpecialCharHandling;
			bso::bool__ Ignore;
			bso::bool__ AlwaysCommit;	// Fait un 'commit' aprs chaque criture. Utile pour le dboguage d'application.
		} &S_;
		stkctn::qMCSTACKdl( name_ ) Tags;
		dWriter( s &S )
		: S_( S ),
		  Tags( S.Tags )
		{}
		void reset( bso::bool__ P = true )
		{
			if ( P )
				_CloseAllTags();

			S_.TagNameInProgress = false;
			S_.TagValueInProgress = false;

			Tags.reset( P );
			S_.Flow = NULL;
			S_.Outfit = o_Undefined;
			S_.SpecialCharHandling = sch_Undefined;
			S_.Ignore = false;
			S_.AlwaysCommit = false;
		}
		void plug( qASd *AS )
		{
			Tags.plug( AS );
		}
		dWriter &operator =( const dWriter &W )
		{
			Tags = W.Tags;

			S_.TagNameInProgress = W.S_.TagNameInProgress;
			S_.TagValueInProgress = W.S_.TagValueInProgress;
			S_.Flow = W.S_.Flow;
			S_.Outfit = W.S_.Outfit;
			S_.SpecialCharHandling = W.S_.SpecialCharHandling;
			S_.Ignore = W.S_.Ignore;
			S_.AlwaysCommit = W.S_.AlwaysCommit;

			return *this;
		}
		void Init(
			txf::text_oflow__ &Flow,
			eLayout Outfit,
			fEncoding Encoding,
			eSpecialCharHandling SpecialCharHandling = sch_Default )
		{
			reset();

			Tags.Init();
			S_.Flow = &Flow;
			S_.Outfit = Outfit;
			S_.SpecialCharHandling = SpecialCharHandling;

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
		mark__ GetMark( void ) const
		{
			return Tags.Last();
		}
		mark__ PushTag( const name_ &Name )	// Valeur retourne : voir 'PopTag(...)'.
		{
			mark__ Mark = XML_UNDEFINED_MARK;

			if ( S_.TagNameInProgress ) {
				*S_.Flow << '>';

				if ( S_.Outfit == oIndent )
					*S_.Flow << txf::nl;
			}

			if ( S_.Outfit == oIndent )
				_Indent( Tags.Amount() );

			*S_.Flow << '<' << Name;
			Mark = Tags.Push( Name );
			S_.TagNameInProgress = true;
			S_.TagValueInProgress = false;

			_Commit();

			return Mark;
		}
		mark__ PushTag( const char *Name )	// Valeur retourne : voir 'PopTag(...)'.
		{
			return PushTag( name( Name ) );
		}
		void PutRawValue( flw::sRFlow &Flow );
		void PutValue( flw::sRFlow &Flow );
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
			flw::sRFlow &Flow );
		void PutAttribute(
			const name_ &Name,
			flw::sRFlow &Flow );
		void PutAttribute(
			const name &Name,
			flw::sRFlow &Flow )
		{
			PutAttribute( *Name, Flow );
		}
		void PutRawAttribute(
			const name_ &Name,
			const value_ &Value );
		void PutAttribute(
			const name_ &Name,
			const value_ &Value );
		void PutAttribute(
			const value_ &Value,
			const name &Name )
		{
			PutAttribute( Value, *Name );
		}
		void PutAttribute(
			const value &Value,
			const name &Name )
		{
			PutAttribute( *Value, *Name );
		}
		void PutAttribute(
			const value &Value,
			const name_ &Name )
		{
			PutAttribute( *Value, Name );
		}
		void PutAttribute(
			const value_ &Value,
			const char *Name )
		{
			PutAttribute( Value, name( Name ) );
		}
		void PutAttribute(
			const value &Value,
			const char *Name )
		{
			PutAttribute( *Value, Name );
		}
		void PutAttribute(
			const char *Name,
			const char *Value )
		{
			PutAttribute( name( Name ), value( Value ) );
		}
		void PutAttribute(
			const char *Name,
			const value_ &Value )
		{
			PutAttribute( name( Name ), Value );
		}
		void PutAttribute(
			const char *Name,
			const value &Value )
		{
			PutAttribute( Name, *Value );
		}
		template <typename s, typename i> void PutAttribute(
			const s &Name,
			i Value )
		{
			bso::integer_buffer__ IBuffer;

			PutAttribute( Name, bso::Convert( Value, IBuffer ) );
		}
		// No attribute is added if 'Value' == 'UndefinedValue'.
		template <typename s, typename i> void PutAttribute(
			const s &Name,
			i Value,
			i UndefinedValue )
		{
			if ( Value != UndefinedValue )
				PutAttribute( Name, Value );
		}
		void PutCData( flw::sRFlow &Flow );
		void PutCData( const str::string_ &Value );
		void PutCData( const char *Value )
		{
			PutCData( value( Value ) ) ;
		}
		mark__ PopTag( mark__ Mark = XML_UNDEFINED_MARK );	// 'Mark', si utilis, est retourn par un 'Push(...)' et permet de contrler si l'on se retrouve bien au mme niveau que le 'Push(...)' en question.
		void Rewind( mark__ Mark );	// Dsemile l'arbre jusqu'au niveau de 'Mark'.
		txf::text_oflow__ &GetFlow( void )
		{
			return *S_.Flow;
		}
		void AlwayCommit( bso::bool__ Value = true )
		{
			S_.AlwaysCommit = Value;
		}
		bso::sBool Put( rParser &Parser );
		// Ident and put the content of 'XFlow' (reparse it).
		bso::sBool Put( xtf::extended_text_iflow__ &XFlow );
		// Ident and put the content of 'XML' (reparse it).
		bso::sBool Put( const str::dString &XML );
		qRODISCLOSEd( eLayout, Outfit )
		qRODISCLOSEd( eSpecialCharHandling, SpecialCharHandling )
	};

	qW( Writer )
}

/***************/
/***** Old *****/
/***************/

namespace xml {
	using xtf::pos__;

	// Code d'erreur 'retourn' par 'Parse()'.
	enum status__ {
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
		sEmptyTagName,
		sMismatchedTag,
		s_amount,
		s_Undefined,
	};

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
		bso::bool__ _EmptyTag;	// A 'true' pour '<tag/>', sinon  'false'.
		_flow___ _Flow;
		str::string _TagName;
		str::string _AttributeName;
		str::string _Value;
		status__ _Status;
		entities_handling__ _EntitiesHandling;
	public:
		void reset( bso::bool__ P = true )
		{
			_Context = c_Undefined;
			_Token = t_Undefined;
			_EmptyTag = false;
			_Flow.reset( P );

			_TagName.reset( P );
			_AttributeName.reset( P );
			_Value.reset( P );
			_Status = s_Undefined;
			_EntitiesHandling = eh_Undefined;

			_Tags.reset( P );
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
			_AttributeName.Init();
			_Value.Init();

			_EntitiesHandling = EntitiesHandling;
		}
		token__  Parse( int TokenToReport = tfAll );
		token__ Parse(
			str::string_ &TagName,
			str::string_ &AttributeName,
			str::string_ &Value,	// Contient la valeur d'une balise ('tag') our d'un attribut, en fonction de la valeur retourne ('tTag' ou 'tAttribute').
			xml::dump_ &Dump,
			status__ &Status,
			int TokenToReport = tfAll )	// 'Status' initialis seulement si valeur retourne == 'tError'.
		{
			token__ Token = Parse( TokenToReport );

			TagName = _TagName;
			AttributeName = _AttributeName;
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
		void PurgeDumpData( void )
		{
			_Flow.Dump.PurgeData();
		}
		E_RODISCLOSE__( str::string_, TagName );
		E_RODISCLOSE__( str::string_, AttributeName );
		E_RODISCLOSE__( str::string_, Value );
		E_RODISCLOSE__( status__, Status );
		E_RODISCLOSE__( token__, Token );
		E_RODISCLOSE__( entities_handling__, EntitiesHandling );
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
			const str::string_ &Value,
			const dump_ &Dump ) = 0;
		virtual bso::bool__ XMLSpecialAttribute(
			const str::string_ &TagName,
			const str::string_ &Name,
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

	typedef dWriter writer_;

	E_AUTO( writer )

	inline void WriteXSLDeclaration(
		const str::string_ &XSLHRef,
		txf::text_oflow__ &OFlow )
	{
		OFlow << "<?xml-stylesheet type=\"text/xsl\" href=\"" << XSLHRef << "\"?>";
	}

	template <typename i> void PutValue(
		i Value,
		xml::writer_ &Writer )
	{
		bso::integer_buffer__ IBuffer;

		Writer.PutValue( bso::Convert( Value, IBuffer ) );
	}

	template <typename s, typename i> void PutValue(
		i Value,
		const s &Name,
		xml::writer_ &Writer )
	{
		bso::integer_buffer__ IBuffer;

		Writer.PutValue( bso::Convert( Value, IBuffer ), Name );
	}

	template <typename s, typename i> void PutAttribute(
		const s &Name,
		i Value,
		xml::writer_ &Writer )
	{
		bso::integer_buffer__ IBuffer;

		Writer.PutAttribute( Name, bso::Convert( Value, IBuffer ) );
	}
}

#endif
