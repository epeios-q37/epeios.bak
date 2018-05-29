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

#ifndef XPP__INC
# define XPP__INC

# define XPP_NAME		"XPP"

# if defined( E_DEBUG ) && !defined( XPP_NODBG )
#  define XPP_DBG
# endif

// XML PrepProcessor

# include "err.h"
# include "flw.h"
# include "xml.h"
# include "fdr.h"
# include "xtf.h"
# include "flx.h"
# include "flf.h"
# include "crptgr.h"
# include "cdgb64.h"
# include "fnm.h"
# include "stkbch.h"


# ifdef XPP_PREPROCESSOR_DEFAULT_NAMESPACE
#  define XPP__PREPROCESSOR_DEFAULT_NAMESPACE XPP_PREPROCESSOR_DEFAULT_NAMESPACE
#else
# define XPP__PREPROCESSOR_DEFAULT_NAMESPACE	"xpp"
#endif

/*************/
/**** OLD ****/
/*************/

namespace xpp {

	typedef bso::uint__ level__;	// Imbrication level.

	#define XPP_LEVEL_MAX	BSO_UINT_MAX

	// NOTA : Si modifi�, modifier 'GetTranslation()' en cons�quent, ainsi que le contenu du ficher 'xpp.xlcl'.
	enum status__ {
		sOK = xml::sOK,
		sNoTagsAllowedHere = xml::s_amount,
		sUnexpectedValue,
		sUnknownDirective,
		sUnexpectedAttribute,
		sMissingNameAttribute,
		sEmptyNameAttributeValue,
		sMissingSelectOrHRefAttribute,
		sEmptySelectOrHRefAttributeValue,
		sMissingSelectAttribute,
		sEmptySelectAttributeValue,
		sMissingValueAttribute,
		sMissingValueOrValueOfAttribute,
		sMissingNameAndValueAttributes,
		sMissingSelectAndValueAttributes,
		sMustBeEmpty,
		sTooManyTags,
		sUnableToOpenFile,
		sUnknownMacro,
		sBadAttributeDefinitionSyntax,
		sBadCypherKey,
		sMissingCypherKey,
		sMissingKeyOrFormatAttribute,
		sEmptyResult,
		sCDataNestingForbidden,
		sUnknownVariable,

		s_amount,
		s_Undefined,
		s_Pending,
	};

	inline status__ _Convert( xml::status__ Status )
	{
		return (status__)Status;
	}

	const char *GetLabel( status__ Status );

	struct coords___
	{
	public:
		xtf::pos__ Position;
		fnm::name___ FileName;
		void reset( bso::bool__ P = true )
		{
			Position.reset( P  );
			FileName.reset( P );
		}
		coords___( void )
		{
			reset( false );

			Init();
		}
		coords___(
			const xtf::pos__ &Position,
			const fnm::name___ &FileName = fnm::name___() )
		{
			reset( false );

			this->Position = Position;
			this->FileName.Init( FileName );
		}
		~coords___( void )
		{
			reset();
		}
		void Init( void )
		{
			Position.Init();
			FileName.Init();
		}
	};

	struct context___ {
		status__ Status;
		coords___ Coordinates;
		void reset( bso::bool__ P = true )
		{
			Status = s_Undefined;

			Coordinates.reset( P  );
		}
		context___( void )
		{
			reset( false );

			Init();
		}
		context___(
			status__ Status,
			const coords___ &Coordinates )
		{
			reset( false );

			this->Status = Status;
			this->Coordinates = Coordinates;
		}
		~context___( void )
		{
			reset();
		}
		void Init( void )
		{
			Status = s_Undefined;

			Coordinates.Init();
		}
	};

	void GetMeaning(
		const coords___ &Coordinates,
		lcl::meaning_ &Meaning );

	void GetMeaning(
		const context___ &Context,
		lcl::meaning_ &Meaning );

	struct _qualified_preprocessor_directives___ {
		str::string NamespaceWithSeparator;
		str::string DefineTag_;
		str::string ExpandTag;
		str::string SetTag;
		str::string IfeqTag;
		str::string BlocTag;
		str::string CDataTag;
		str::string CypherTag;
		str::string AttributeAttribute;	//'<tag xpp:attribute="..." ...>'//
		str::string XMLNS;	// <... xmlns:xpp="..." ...> ('xpp' ou ce qui a �t� choisi par l'utilisateur ...).
		void reset( bso::bool__ P = true )
		{
			NamespaceWithSeparator.reset( P );

			DefineTag_.reset( P );
			ExpandTag.reset( P );
			SetTag.reset( P );
			IfeqTag.reset( P );
			BlocTag.reset( P );
			CDataTag.reset( P );
			AttributeAttribute.reset( P );
			XMLNS.reset( P );
		}
		_qualified_preprocessor_directives___( void )
		{
			reset( false );
		}
		~_qualified_preprocessor_directives___( void )
		{
			reset();
		}
		void Init( const str::string_ &Namespace );
	};

	E_ROW( _rrow__ );	// Repository row.

	class _repository_
	{
	private:
		_rrow__ _Locate( const str::string_ &Name ) const
		{
			_rrow__ Row = Names.First();

			while ( ( Row!= qNIL ) && ( Names( Row ) != Name ) )
				Row = Names.Next( Row );

			return Row;
		}
	public:
		struct s {
			ctn::E_MCONTAINERt_( str::string_, _rrow__ )::s Names;
			bch::E_BUNCHt_( xtf::pos__, _rrow__ )::s Positions;
			ctn::E_MCONTAINERt_( str::string_, _rrow__ )::s FileNames;
			ctn::E_MCONTAINERt_( str::string_, _rrow__ )::s Contents;
		};
		ctn::E_MCONTAINERt_( str::string_, _rrow__ ) Names;
		bch::E_BUNCHt_( xtf::pos__, _rrow__ ) Positions;
		ctn::E_MCONTAINERt_( str::string_, _rrow__ ) FileNames;
		ctn::E_MCONTAINERt_( str::string_, _rrow__ ) Contents;
		_repository_( s &S )
		: Names( S.Names ),
		  Positions( S.Positions ),
		  FileNames( S.FileNames ),
		  Contents( S.Contents )
		{}
		void reset( bso::bool__ P = true )
		{
			Names.reset( P );
			Positions.reset( P );
			FileNames.reset( P );
			Contents.reset( P );
		}
		void plug( qASd *AS )
		{
			Names.plug( AS );
			Positions.plug( AS );
			FileNames.plug( AS );
			Contents.plug( AS );
		}
		_repository_ &operator =( const _repository_ &R )
		{
			Names = R.Names;
			Positions = R.Positions;
			FileNames = R.FileNames;
			Contents = R.Contents;

			return *this;
		}
		void Init( void )
		{
			Names.Init();
			Positions.Init();
			FileNames.Init();
			Contents.Init();
		}
		bso::bool__ Delete( const str::string_ &Name )
		{
			_rrow__ Row = _Locate( Name );

			if ( Row != qNIL ) {
				Names.Remove( Row );
				Positions.Remove( Row );
				FileNames.Remove( Row );
				Contents.Remove( Row );

				return true;
			} else
				return false;
		}
		bso::bool__ Store(
			const str::string_ &Name,
			xtf::pos__ Position,
			const fnm::name___ &FileName,
			const str::string_ &Content )
		{
			bso::bool__ AlreadyExists = false;
		qRH
			str::string Buffer;
			_rrow__ Row = qNIL;
		qRB
			AlreadyExists = Delete( Name );

			Row = Names.Append( Name );

			if ( Row != Positions.Append( Position ) )
				qRFwk();

			Buffer.Init();

			if ( Row != FileNames.Append(FileName.UTF8( Buffer ) ) )
				qRFwk();

			if ( Row != Contents.Append( Content ) )
				qRFwk();
		qRR
		qRT
		qRE
			return AlreadyExists;
		}
		bso::bool__ Get(
			const str::string_ &Name,
			xtf::pos__ &Position,
			str::string_ &FileName,
			str::string_ &Content ) const
		{
			_rrow__ Row = _Locate( Name );

			if ( Row == qNIL )
				return false;

			Position = Positions.Get( Row );

			FileNames.Recall( Row, FileName );

			Contents.Recall( Row, Content );

			return true;
		}
	};

	E_AUTO( _repository )

	E_ROW( _vrow__ );	// Variable row.

	class _variables
	{
	private:
		_vrow__ _Locate( const str::string_ &Name ) const
		{
			_vrow__ Row = Names.First();

			while ( ( Row != qNIL ) && ( Names( Row ) != Name ) )
				Row = Names.Next( Row );

			return Row;
		}
	public:
		void reset( bso::bool__ P = true )
		{
			Names.reset( P );
			Values.reset( P );
		}
		_variables( void )
		{
			reset( false );
		}
		~_variables( void )
		{
			reset();
		}
		ctn::E_MCONTAINERt( str::string_, _vrow__ ) Names, Values;
		void Init( const str::dString &BinPath )
		{
			Names.Init();
			Values.Init();
			Set( str::wString( "_BinPath" ), BinPath );
		}
		void Set(
			const str::string_ &Name,
			const str::string_ &Value )
		{
			_vrow__ Row = _Locate( Name );

			if ( Row == qNIL ) {
				Row = Names.Append( Name );
				if ( Row != Values.Append( Value ) )
				qRFwk();
			} else {
				Values( Row ) = Value;
				Values.Flush();
			}
		}
		bso::bool__ GetValue(
			const str::string_ &Name,
			str::string_ &Value ) const
		{
			_vrow__ Row = _Locate( Name );

			if ( Row != qNIL ) {
				Value.Append( Values( Row ) );
			}

			return Row != qNIL;
		}
		bso::bool__ Exists( const str::string_ &Name ) const
		{
			return _Locate( Name ) != qNIL;
		}
		bso::bool__ IsEqual(
			const str::string_ &Name,
			const str::string_ &Value ) const
		{
			_vrow__ Row = _Locate( Name );

			if ( Row == qNIL )
				qRFwk();

			return Values( Row ) == Value;
		}
	};

	typedef _variables	_variables_;

	typedef bch::E_BUNCH_( bso::char__ ) substitution_markers_;
	E_AUTO( substitution_markers );

	inline bso::sBool GetValue_(
		const _variables &Variables,
		const str::dString &Name,
		const fnm::name___ &SelfPath,
		str::dString &Value )
	{
		if ( !Variables.GetValue( Name, Value ) ) {
			if ( Name == "_SelfPath" ) {
				SelfPath.UTF8( Value );
				return true;
			} else
				return false;
		} else
			return true;
	}

	class _extended_parser___
	{
	private:
		flf::file_iflow___ _FFlow;
		str::string _MacroContent;
		flx::E_STRING_IFLOW__ _SFlow;
		cdgb64::decoding_iflow___ _Decoder;
		crptgr::decrypt_iflow___ _Decrypter;
		xtf::extended_text_iflow__ _XFlow;
		xml::parser___ _Parser;
		_repository_ &_Repository;
		_variables_ &_Variables;
		_qualified_preprocessor_directives___ &_Directives;
		fnm::name___ _LocalizedFileName;	// Si le 'parser' sert � l'inclusion d'un fichier ('<xpp:expand href="...">), contient le nom du fichier inclut.
		fnm::name___ _Directory;
		str::string _CypherKey;
		bso::bool__ Preserve_;	// If at true, this means that the 'preserve' attribute in 'bloc' tag should be handled, NOT that we have to preserve
											// the preprocessor directives (that is the goal of below parameter).
		level__ PreservationLevel_;
		substitution_markers SubstitutionMarkers_;
		bso::bool__ _IgnorePreprocessingInstruction;
		bso::bool__ _AttributeDefinitionInProgress;
		bso::uint__ _CDataNesting;
		bso::sBool GetVariableValue_(
			const str::dString &Name,
			str::dString &Value )
		{
			return GetValue_( _Variables, Name, _Directory, Value );
		}
		status__ _HandleDefineDirective( _extended_parser___ *&Parser );
		status__ _InitWithFile(
			const fnm::name___ &FileName,
			const fnm::name___ &Directory,
			const str::string_ &CypherKey,
			bso::bool__ Preserve,
			bso::char__ SubstitutionMarker,
			utf::format__ Format );
		status__ _InitWithContent(
			const str::string_ &Content,
			const fnm::name___ &NameOfTheCurrentFile,
			const xtf::pos__ &Position,
			const fnm::name___ &Directory,
			const str::string_ &CypherKey,
			bso::bool__ Preserve,
			bso::char__ SubstitutionMarker,
			utf::format__ Format );
		status__ _InitCypher(
			flw::iflow__ &Flow,
			const fnm::name___ &FileName,
			const xtf::pos__ &Position,
			const fnm::name___ &Directory,
			const str::string_ &CypherKey,
			bso::bool__ Preserve,
			bso::char__ SubstitutionMarker,
			utf::format__ Format );
		status__ _HandleMacroExpand(
			const str::string_ &MacroName,
			_extended_parser___ *&Parser );
		status__ _HandleFileExpand(
			const str::string_ &FileName,
			_extended_parser___ *&Parser );
		status__ _HandleVariableExpand(
			const str::string_ &VariableName,
			_extended_parser___ *&Parser );
		status__ _HandleExpandDirective( _extended_parser___ *&Parser );
		status__ _HandleSetDirective( _extended_parser___ *&Parser );
		status__ _HandleIfeqDirective( _extended_parser___ *&Parser );
		status__ _HandleCypherDecryption(
			const str::string_ &MacroName,
			_extended_parser___ *&Parser );
		status__ _HandleCypherOverride(
			const str::string_ &CypherKey,
			_extended_parser___ *&Parser );
		status__ _HandleCypherDirective( _extended_parser___ *&Parser );
		bso::char__ SubstitutionMarker_( void ) const
		{
			if ( ( PreservationLevel_ != 0 ) || ( SubstitutionMarkers_.Amount() == 0 ) )
				return 0;
			else
				return SubstitutionMarkers_.Top();
		}
		status__ HandleAtributeValueSubstitution_(
			const str::string_ &Source,
			bso::char__ Marker,
			str::string_ &Data );
		status__ _HandleAttributeDirective(
			const str::string_ &Parameters,
			_extended_parser___ *&Parser,
			str::string_ &Data );
		status__ _HandlePreprocessorDirective(
			int Directive,
			_extended_parser___ *&Parser );
	public:
		void reset( bso::bool__ P = true )
		{
			//_Repository.reset( P );
			//_Tags.reset( P );
			_MacroContent.reset( P );
			_FFlow.reset( P );
			_SFlow.reset( P );
			_Decoder.reset( P );
			_Decrypter.reset( P );
			_XFlow.reset( P );
			_LocalizedFileName.reset( P );
			_Directory.reset( P );
			_CypherKey.reset( P );
			Preserve_ = false;
			PreservationLevel_ = 0;
			SubstitutionMarkers_.reset( P );
			_Parser.reset( P );
			_IgnorePreprocessingInstruction = false;
			_AttributeDefinitionInProgress = false;
			_CDataNesting = 0;
		}
		_extended_parser___(
			_repository_ &Repository,
			_variables_ &Variables,
			_qualified_preprocessor_directives___ &Directives )
		: _Repository( Repository ),
		  _Variables( Variables ),
		  _Directives( Directives )
		{
			reset( false );
		}
		~_extended_parser___( void )
		{
			reset();
		}
		status__ Init(
			xtf::extended_text_iflow__ &XFlow,
			const fnm::name___ &LocalizedFileName,	// Si 'XFlow' est rattach� � un fichier, le nom de ce fichier (utile pour la gestion d'erreurs).
			const fnm::name___ &Directory,
			const str::string_ &CypherKey,
			bso::bool__ Preserve,
			bso::char__ SubstitutionMarker )
		{
			// _Repository.Init();
			// _Tags.Init();

			_Parser.Init( XFlow, xml::ehKeep );
			_LocalizedFileName.Init( LocalizedFileName );
			_Directory.Init( Directory );
			_CypherKey.Init( CypherKey );
			_IgnorePreprocessingInstruction = false;
			_AttributeDefinitionInProgress = false;
			_CDataNesting = 0;
			Preserve_= Preserve;
			PreservationLevel_ = 0;
			SubstitutionMarkers_.Init();
			SubstitutionMarkers_.Push( SubstitutionMarker );

			return sOK;
		}
		status__ Handle(
			_extended_parser___ *&Parser,
			str::string_ &Data );
		xtf::extended_text_iflow__ &Flow( void )
		{
			return _Parser.Flow();
		}
		const xtf::extended_text_iflow__ &Flow( void ) const
		{
			return _Parser.Flow();
		}
		const str::string_ &DumpData_( void ) const
		{
			return _Parser.DumpData();
		}
		const xtf::pos__ &DumpPosition( void ) const
		{
			return _Parser.DumpPosition();
		}
		const xtf::pos__ &Position( void ) const
		{
			return _Parser.Flow().Position();
		}
		utf::format__ GetFormat( void ) const
		{
			return _Parser.GetFormat();
		}
		bso::bool__ SetFormat( utf::format__ Format )
		{
			return _Parser.SetFormat( Format );
		}
		E_RODISCLOSE__( fnm::name___, LocalizedFileName );
	};

	typedef stkbch::qBSTACKdl(_extended_parser___ *) _xparser_stack_;
	E_AUTO( _xparser_stack );

	typedef fdr::iflow_driver___<> _iflow_driver___;

	inline _extended_parser___ *NewParser(
		_repository_ &Repository,
		_variables_ &Variables,
		_qualified_preprocessor_directives___ &Directives )
	{
		_extended_parser___ *Parser = new _extended_parser___( Repository, Variables, Directives );

		if ( Parser == NULL )
			qRAlc();

		return Parser;
	}

	struct criterions___
	{
		fnm::name___
			Directory;
		str::string
			CypherKey,
			Namespace;
		bso::bool__ Preserve;
		bso::char__ SubstitutionTag;
		void reset( bso::bool__ P = true )
		{
			Directory.reset( P);
			CypherKey.reset( P );
			Namespace.reset( P );
			Preserve = false;
			SubstitutionTag = 0;
		}
		~criterions___( void )
		{
			reset();
		}
		criterions___( 
			const fnm::name___ &Directory,
			const str::string_ &CypherKey = str::string() ,
			const str::string_ &Namespace = str::string(),
			bso::bool__ Preserve = false,
			bso::char__ SubstitutionTag = 0 )
		{
			reset( false );

			Init( Directory, CypherKey, Namespace, Preserve, SubstitutionTag );
		}
		void Init( 
			const fnm::name___ &Directory,
			const str::string_ &CypherKey = str::string() ,
			const str::string_ &Namespace = str::string(),
			bso::bool__ Preserve = false,
			bso::char__ SubstitutionTag = 0 )
		{
			this->Directory.Init( Directory );
			this->CypherKey.Init( CypherKey );
			this->Namespace.Init( Namespace );
			this->Preserve = Preserve;
			this->SubstitutionTag = SubstitutionTag;
		}
		bso::bool__ IsNamespaceDefined( void ) const
		{
			return Namespace.Amount() != 0;
		}
	};

	class _preprocessing_iflow_driver___
	: public _iflow_driver___
	{
	private:
		status__ _Status;
		_qualified_preprocessor_directives___ _Directives;
		_repository _Repository;
		_variables _Variables;
		str::string _Data;
		sdr::size__ _Position;	// Position du premier caract�re non lu dans le '_Data'.
		_xparser_stack _Parsers;
		_extended_parser___ *_CurrentParser;
		void _DeleteParsers( void );
		_extended_parser___ &_Parser( void )
		{
#ifdef XPP_DBG
			if ( _CurrentParser == NULL )
				qRFwk();
#endif
			return *_CurrentParser;
		}
		const _extended_parser___ &_Parser( void ) const
		{
#ifdef XPP_DBG
			if ( _CurrentParser == NULL )
				qRFwk();
#endif
			return *_CurrentParser;
		}
	protected:
		virtual sdr::size__ FDRRead(
			sdr::size__ Maximum,
			sdr::byte__ *Buffer ) override;
		virtual void FDRDismiss( bso::sBool Unlock ) override
		{}
		virtual fdr::sTID FDRITake( fdr::sTID Owner ) override
		{
			return _Parser().Flow().UndelyingFlow().Take( Owner );
		}
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P ) {
				_DeleteParsers();
			}

			_Repository.reset( P );
			_Variables.reset( P );
			_Directives.reset();
			_Data.reset( P );
			_Position = 0;
			_iflow_driver___::reset( P );
			_Parsers.reset( P );
			_CurrentParser = NULL;
			_Status = s_Undefined;
		}
		_preprocessing_iflow_driver___( void )
		{
			reset( false );
		}
		virtual ~_preprocessing_iflow_driver___( void )
		{
			reset();
		}
		void Init(
			xtf::extended_text_iflow__ &XFlow,
			fdr::thread_safety__ ThreadSafety,
			const criterions___ &Criterions )
		{
		qRH
			str::wString Buffer;
		qRB
			_DeleteParsers();
			_Repository.Init();

			Buffer.Init();
			_Variables.Init( Criterions.Directory.UTF8( Buffer ) );
# if 0	// A priori �quivalent � ce qu'il y a dans le '#else', mais VC++ 10 d�truit 'Criterions.Namespace' quand 'Criterions.IsNamespaceDefined()' est vrai. Fonctionne avec 'g++4'.
			_Directives.Init( Criterions.IsNamespaceDefined() ? Criterions.Namespace : str::string( XPP__PREPROCESSOR_DEFAULT_NAMESPACE ) );
# else
			if ( Criterions.IsNamespaceDefined() )
				_Directives.Init( Criterions.Namespace );
			else
				_Directives.Init( str::string( XPP__PREPROCESSOR_DEFAULT_NAMESPACE ) );
# endif
			_Data.Init();
			_Position = 0;
			_iflow_driver___::Init( ThreadSafety );
			_CurrentParser = NewParser( _Repository, _Variables, _Directives );
			_Parsers.Init();
			if ( _Parser().Init( XFlow, str::string(), Criterions.Directory, Criterions.CypherKey, Criterions.Preserve, Criterions.SubstitutionTag ) != sOK )
				qRFwk();
			_Status = sOK;
		qRR
		qRT
		qRE

		}
		status__ Status( void ) const
		{
			return _Status;
		}
		const context___ &GetContext( context___ &Context ) const
		{
			Context.Coordinates = coords___( _Parser().Position(), _Parser().LocalizedFileName() );
			Context.Status = _Status;

			return Context;
		}
	};

	typedef flw::standalone_iflow__<> _iflow__;

	class preprocessing_iflow___
	: public _iflow__
	{
	private:
		_preprocessing_iflow_driver___ _FlowDriver;
	public:
		void reset( bso::bool__ P = true )
		{
			_FlowDriver.reset( P );
			_iflow__::reset( P );
		}
		preprocessing_iflow___( void )
		{
			reset( false );
		}
		~preprocessing_iflow___( void )
		{
			reset( true );
		}
		void Init(
			xtf::extended_text_iflow__ &XFlow,
			const criterions___ &Criterions )
		{
			_FlowDriver.Init( XFlow, fdr::tsDisabled, Criterions );
			_iflow__::Init( _FlowDriver );
		}
		const context___ &GetContext( context___ &Context ) const
		{
			return _FlowDriver.GetContext( Context );
		}
		status__ Status( void ) const
		{
			return _FlowDriver.Status();
		}
	};

	// Lorsqu'une erreur s'est produite; information stock�es dans 'PFlow'.
	inline void GetMeaning(
		const preprocessing_iflow___ &PFlow,
		lcl::meaning_ &Meaning )
	{
	qRH
		context___ Context;
	qRB
		Context.Init();

		GetMeaning( PFlow.GetContext( Context ), Meaning );
	qRR
	qRT
	qRE
	}

	status__ Encrypt(
		const str::string_ &Namespace,
		flw::iflow__ &IFlow,
		utf::format__ Format,
		xml::writer_ &Writer,
		context___ &Context );

	status__ Encrypt(
		const str::string_ &Namespace,
		flw::iflow__ &IFlow,
		xml::outfit__ Outfit,
		utf::format__ Format,
		txf::text_oflow__ &OFlow,
		context___ &Context );

	status__ Process(
		xtf::extended_text_iflow__ &XFlow,
		const criterions___ &Criterions,
		xml::writer_ &Writer,
		context___ &Context );

	status__ Process(
		xtf::extended_text_iflow__ &XFlow,
		const criterions___ &Criterions,
		xml::outfit__ Outfit,
		txf::text_oflow__ &OFlow,
		context___ &Context );

	inline status__ Process(
		xtf::extended_text_iflow__ &XFlow,
		const criterions___ &Criterions,
		xml::writer_ &Writer )
	{
		status__ Status = s_Undefined;
	qRH
		context___ Context;
	qRB
		Context.Init();

		Status = Process( XFlow, Criterions, Writer, Context );
	qRR
	qRT
	qRE
		return Status;
	}

	inline status__ Process(
		xtf::extended_text_iflow__ &XFlow,
		const criterions___ &Criterions,
		xml::outfit__ Outfit,
		txf::text_oflow__ &OFlow )
	{
		status__ Status = s_Undefined;
	qRH
		context___ Context;
	qRB
		Context.Init();

		Status =  Process( XFlow, Criterions, Outfit, OFlow, Context );
	qRR
	qRT
	qRE
		return Status;
	}

	void Process(
		const str::string_ &In,
		xml::outfit__ Outfit,
		str::string_ &Out,
		const criterions___ &Criterions );
}

/*************/
/**** NEW ****/
/*************/

namespace xpp {
	typedef preprocessing_iflow___ rIFlow;
	typedef criterions___ rCriterions;
}


#endif
