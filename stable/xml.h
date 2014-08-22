/*
	'xml.h' by Claude SIMON (http://zeusw.org/).

	'xml' is part of the Epeios framework.

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

#ifndef XML__INC
# define XML__INC

# define XML_NAME		"XML"

# if defined( E_DEBUG ) && !defined( XML_NODBG )
#  define XML_DBG
# endif

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/

// eXtensible Markup Language

#include "err.h"
#include "flw.h"
#include "str.h"
#include "xtf.h"
#include "stk.h"
#include "ctn.h"
#include "cpe.h"

// Prédéclaration.
namespace lcl {
	class meaning_;
}

namespace xml {
	using xtf::pos__;

	// Code d'erreur 'retourné' par 'Parse()'.
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
		void plug( sdr::E_SDRIVER__ &SD )
		{
			Data.plug( SD );
		}
		void plug( ags::E_ASTORAGE_ &AS )
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
		flw::datum__ Get( xtf::utf__ &UTFOut )
		{
			xtf::utf__ UTF;

			UTF.Init();

			if ( Dump.Data.Amount() == 0 )
				Dump.Set( _Flow->Position() );

			flw::datum__ C = _Flow->Get( UTF );

			Dump.Data.Append( (const bso::char__ *)UTF.Data, UTF.Size );

			if ( &UTFOut != NULL )
				UTFOut = UTF;

			return C;
		}
		flw::datum__ Get_( void )
		{
			return Get( *(xtf::utf__ *)NULL );
		}
		flw::datum__ View( xtf::utf__ &UTFOut )
		{
			xtf::utf__ UTF;

			UTF.Init();

			flw::datum__  C = _Flow->View( UTF );

			if ( &UTFOut != NULL )
				UTFOut = UTF;

			return C;
		}
		flw::datum__ View_( void )
		{
			return View( *(xtf::utf__ *)NULL );
		}
		bso::bool__ EndOfFlow( void )
		{
			xtf::error__ Error = xtf::e_NoError;
			bso::bool__ EOFlow = _Flow->EndOfFlow( Error );

			if ( EOFlow && ( Error != xtf::e_NoError ) )
				ERRFree();

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
		t_Processed,	// Tout le flux XML a été traité.
		t_Error,	// Erreur dans l'analyse du flux XML; voir 'Status'.
		t_Undefined
	};

#ifdef TF
#	define XML__TF_BUFFER	TF
#	undef TF
#endif

#define TF( name )	tf##name = ( 1 << t##name )

	// Permet de n'avoir à traiter que certains 'token's.
	enum token_flag__
	{
		tfNone = 0,	// Pour n'obtenir que 'tError' et 'tProcessed', qui ne peuvent être ignorés.
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
	// Traitement des entités ('&...;').
	enum entities_handling__ {
		ehReplace,	// Les 'entity's sont remplacés par leur caractère correspondant.
		ehKeep,		// Les 'entity's sont gardés tel quel.
		eh_amount,
		eh_Undefined,
		eh_Default = ehReplace,
	};

	class parser___
	{
	private:
		_context__ _Context;
		token__ _Token;
		stk::E_MCSTACK( str::string_ ) _Tags;
		bso::bool__ _EmptyTag;	// A 'true' pour '<tag/>', sinon à 'false'.
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
			str::string_ &Value,	// Contient la valeur d'une balise ('tag') our d'un attribut, en fonction de la valeur retournée ('tTag' ou 'tAttribute').
			xml::dump_ &Dump,
			status__ &Status,
			int TokenToReport = tfAll )	// 'Status' initialisé seulement si valeur retournée == 'tError'.
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
		callback__ &Callback );
	// Si valeur retournée == 'false', 'Flow.Line()' et 'Flow.Column()' est positionné là où il y a l'erreur.

	// Transformation des caractères spéciaux, comm '<' qui devient '&lt;'.
	void TransformUsingEntities(
		str::string_ &Target,
		bso::bool__ DelimiterOnly );	// Si à true', seul les délimiteurs de valeur d'attributs (''' et '"') sont modifiés.

	inline void TransformUsingEntities(
		const str::string_ &Source,
		bso::bool__ DelimiterOnly,
		str::string_ &Target )	// Conversion des caractères spéciaux, comme '<' qui devient '&lt;'.
	{
		Target = Source;

		TransformUsingEntities( Target, DelimiterOnly );
	}


	typedef str::string_	name_;
	typedef str::string		name;

	typedef str::string_	value_;
	typedef str::string		value;

	// Mise en forme de la sortie.
	enum outfit__ {
		oCompact,	// Tout sur une seule ligne.
		oIndent,	// Indenté.
		o_amount,
		o_Undefined
	};	// 

	// Traitement des caractères spéciaux ('<', '&', ...).
	enum special_char_handling__ {
		schReplace,	// Sont remplacés par les 'entity's dédiés.
		schKeep,	// Ne sont pas remplacés. Cependant, les '"' sont tout de même remplacé per leur 'entity' correspondante dans les valeurs d'attributs.
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
		default:ERRPrm();
			break;
		}
		return NULL;	// Pour éviter un 'Warning'.
	}

	class encoding__
	{
	private:
		const char *_EncodingString;
	public:
		encoding__( void )
		{
			_EncodingString = Label( e_Default );
		}
		encoding__( _encoding__ Encoding )
		{
			_EncodingString = Label( Encoding );
		}
		encoding__( const char *Encoding )
		{
			_EncodingString = Encoding;
		}
		const char *EncodingString( void ) const
		{
			return _EncodingString;
		}
	};

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
		encoding__ Encoding )
	{
		return WriteXMLHeader( OFlow, Encoding.EncodingString() );
	}

	E_TMIMIC__( stk::row__, mark__ );

# define XML_UNDEFINED_MARK	((xml::mark__)E_NIL)


	class writer_
	{
	private:
		void _CloseAllTags( void );
		void _Indent( bso::size__ Amount ) const;
		void _Commit( void )
		{
			if ( S_.AlwaysCommit )
				S_.Flow->Commit();
		}
	public:
		struct s {
			stk::E_MCSTACK_( name_ )::s Tags;
			txf::text_oflow__ *Flow;
			bso::bool__ TagNameInProgress;
			bso::bool__ TagValueInProgress;
			outfit__ Outfit;
			special_char_handling__ SpecialCharHandling;
			bso::bool__ Ignore;
			bso::bool__ AlwaysCommit;	// Fait un 'commit' aprés chaque écriture. Utile pour le déboguage d'application.
		} &S_;
		stk::E_MCSTACK_( name_ ) Tags;
		writer_( s &S )
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
		void plug( ags::E_ASTORAGE_ &AS )
		{
			Tags.plug( AS );
		}
		writer_ &operator =( const writer_ &W )
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
			outfit__ Outfit,
			encoding__ Encoding,
			special_char_handling__ SpecialCharHandling = sch_Default )
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
					ERRPrm();
					break;
			}
		}
		mark__ GetMark( void ) const
		{
			return Tags.Last();
		}
		mark__ PushTag( const name_ &Name )	// Valeur retournée : voir 'PopTag(...)'.
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
		mark__ PushTag( const char *Name )	// Valeur retournée : voir 'PopTag(...)'.
		{
			return PushTag( name( Name ) );
		}
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
			const value_ &Value,
			const char *Name )
		{
			PutValue( Value, name( Name ) );
		}
		void PutValue(
			const char *Value,
			const char *Name )
		{
			PutValue( value( Value ), name( Name ) );
		}
		void PutAttribute(
			const name_ &Name,
			const value_ &Value );
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
		void PutCData( const str::string_ &Value );
		void PutCData( const char *Value )
		{
			PutCData( value( Value ) ) ;
		}
		mark__ PopTag( mark__ Mark = XML_UNDEFINED_MARK );	// 'Mark', si utilisé, est retourné par un 'Push(...)' et permet de contrôler si l'on se retrouve bien au même niveau que le 'Push(...)' en question.
		void Rewind( mark__ Mark );	// Désemile l'arbre jusqu'au niveau de 'Mark'.
		txf::text_oflow__ &GetFlow( void )
		{
			return *S_.Flow;
		}
		void AlwayCommit( bso::bool__ Value = true )
		{
			S_.AlwaysCommit = Value;
		}
		E_RODISCLOSE_( outfit__, Outfit )
		E_RODISCLOSE_( special_char_handling__, SpecialCharHandling )
	};

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

				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

#endif
