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

// Frontend/Backend Layout FRrontenD

#ifndef FBLFRD_INC_
# define FBLFRD_INC_

# define FBLFRD_NAME		"FBLFRD"

# if defined( E_DEBUG ) && !defined( FBLFRD_NODBG )
#  define FBLFRD_DBG
# endif

# include "err.h"
# include "flw.h"

# include "fbltyp.h"
# include "fblcst.h"
# include "fblovl.h"
# include "fblcmd.h"
# include "fblfph.h"
# include "fblfrp.h"
# include "fblfep.h"

# define FBLFRD_UNDEFINED_CAST		255
# define FBLFRD_UNDEFINED_TYPE		65535
# define FBLFRD_UNDEFINED_OBJECT		65535
# define FBLFRD_UNDEFINED_COMMAND	65535

# define FBLFRD_UNDEFINED_ID		FBLTYP_UNDEFINED_ID
# define FBLFRD_UNDEFINED_ID32		FBLTYP_UNDEFINED_ID32
# define FBLFRD_UNDEFINED_ID16		FBLTYP_UNDEFINED_ID16
# define FBLFRD_UNDEFINED_ID8		FBLTYP_UNDEFINED_ID8

# define FBLFRD_MASTER_OBJECT	FBLFRD_UNDEFINED_OBJECT
# define FBLFRD_MASTER_TYPE		FBLFRD_UNDEFINED_TYPE
# define FBLFRD_MASTER_COMMAND	FBLFRD_UNDEFINED_COMMAND

# define FBLFRD__T( name, type, bunch, container )\
	E_TYPEDEF( type, name##__ );\
	typedef bch::E_BUNCH_( name##__ )	name##s_;\
	E_AUTO( name##s )\
	typedef ctn::E_XMCONTAINER_( name##s_ )	x##name##s_;\
	E_AUTO( x##name##s )\
	inline bunch &_( name##s_ &O )\
	{\
		return *(bunch *)&O;\
	}\
	inline const bunch &_( const name##s_ &O )\
	{\
		return *(bunch *)&O;\
	}\
	inline container &_( x##name##s_ &O )\
	{\
		return *(container *)&O;\
	}\
	inline const container &_( const x##name##s_ &O )\
	{\
		return *(container *)&O;\
	}

# define FBLFRD_T32( name )	FBLFRD__T( name, fblfrd::id32__, fblfrd::ids32_, fblfrd::xids32_ )
# define FBLFRD_T16( name )	FBLFRD__T( name, fblfrd::id16__, fblfrd::ids16_, fblfrd::xids16_ )
# define FBLFRD_T8( name )	FBLFRD__T( name, fblfrd::id8__, fblfrd::ids8_, fblfrd::xids8_ )

# define FBLFRD_M( name, type )\
	void name##In( const fbltyp::type &O )\
	{\
		_In( fblcst::c##name, &O );\
	}\
	void name##Out( fbltyp::type &O )\
	{\
		_Out( fblcst::c##name, &O );\
	}\

/*************/
/**** OLD ****/
/*************/

namespace fblfrd {
	using namespace fblovl;

	using namespace fbltyp;
		typedef fbltyp::id16__ command__;
		typedef fbltyp::id16__ type__;

	class cReporting
	{
	protected:
		virtual void FBLFRDReport(
			fblovl::reply__ Reply,
			const str::dString &Message ) = 0;
	public:
		qCALLBACK(Reporting)
		void Report(
			fblovl::reply__ Reply,
			const str::dString &Message )
		{
			FBLFRDReport( Reply, Message );
		}
	};

	struct compatibility_informations__
	{
		const char
			*BackendLabel,
			*APIVersion;
		void reset( bso::bool__ = true )
		{
			BackendLabel = APIVersion = "";	// Bien une chane vide, et pas 'NULL'.
		}
		compatibility_informations__( void )
		{
			reset( false );
		}
		compatibility_informations__(
			const char *BackendLabel,
			const char *APIVersion )
		{
			Init( BackendLabel, APIVersion );
		}
		~compatibility_informations__( void )
		{
			reset();
		}
		void Init(
			const char *BackendLabel,
			const char *APIVersion )
		{
			this->BackendLabel = BackendLabel;
			this->APIVersion = APIVersion;
		}
	};

	class incompatibility_informations_
	{
	public:
		struct s {
			str::string_::s
				Message,
				URL;
		};
		str::string_
			Message,
			URL;
		incompatibility_informations_( s &S )
		: Message( S.Message ),
			URL( S.URL )
		{}
		void reset( bso::bool__ P = true )
		{
			Message.reset( P );
			URL.reset( P );
		}
		void plug( qASd *AS )
		{
			Message.plug( AS );
			URL.plug( AS );
		}
		incompatibility_informations_ &operator =( const incompatibility_informations_ &II )
		{
			Message = II.Message;
			URL = II.URL;

			return *this;
		}
		void Init( void )
		{
			Message.Init();
			URL.Init();
		}
	};

	E_AUTO( incompatibility_informations );

	class cFrontend
	{
	protected:
		virtual void FBLFRDOnConnection( void ) = 0;
		virtual void FBLFRDOnDisconnection( void ) = 0;
	public:
		qCALLBACK( Frontend );
		void OnConnection( void )
		{
			return FBLFRDOnConnection();
		}
		void OnDisconnection( void )
		{
			return FBLFRDOnDisconnection();
		}
	};

	class frontend___
	{
	private:
		qRMV( flw::ioflow__, C_, Channel_ );
		qRMV( cFrontend, F_, FrontendCallback_ )
		qRMV( fblfph::callbacks__, P_, ParametersCallback_ );
		cReporting *_ReportingCallback;
		flw::iflow__ *_FlowInParameter;	// Contient, s'il y en a un,  le pointeur sur le 'Flow' en paramtre d'entre.
		bso::bool__ _FlowOutParameter;	// Signale s'il y a un paramtre flow dans les paramtres de sortie.
		bso::bool__ _DismissPending;
		str::wString CodeKey_;	// Cl� de cryptage des codes pour le 'Ping' et le 'Crash'.
		void _PreProcess( void )
		{
			P_().PreProcess();
		}
		void _In(
			fblcst::cast__ Cast,
			const void *Pointer )
		{
			if ( _DismissPending )
				qRFwk();

			C_().Put( Cast );
			P_().In( Cast, Pointer, C_() );
		}
		void _Out(
			fblcst::cast__ Cast,
			void *Pointer )
		{
			if ( _DismissPending )
				qRFwk();

			C_().Put( Cast );
			P_().Out( C_(), Cast, Pointer );
		}
		bso::sBool FlowIn_(
			bso::bool__ FirstCall,
			flw::iflow__ &Flow )
		{
			bso::sBool Success = false;
		qRH
		qRB
			if ( _DismissPending )
				qRFwk();

			P_().FlowIn( FirstCall, Flow, C_() );

			Success = true;
		qRR
			ERRRst();
		qRT
		qRE
			return Success;
		}
		void _FlowOut( flw::iflow__ *&Flow )
		{
			if ( _DismissPending )
				qRFwk();

			C_().Put( fblcst::cFlow );
			P_().FlowOut( C_(), Flow );
		}
		void _PostProcess( flw::ioflow__ &Flow )
		{
			P_().PostProcess( Flow );
		}
		void _ReportError(
			fblovl::reply__ Reply,
			const str::dString &Message )
		{
			if ( _ReportingCallback == NULL )
				qRFwk();

			_ReportingCallback->Report( Reply, Message );
		}
		id16__ Commands_[fblcmd::c_amount];
		char Message_[256];
		bso::bool__ TestBackendCasts_( void );
		command__ GetBackendDefaultCommand_( void );
		void GetGetCommandsCommand_( command__ DefaultCommand );
		void GetBackendCommands_( void );
		void RetrieveBackendCommands_( void )
		{
			command__ DefaultCommand;

			if ( !TestBackendCasts_() )
				qRFwk();

			DefaultCommand = GetBackendDefaultCommand_();

			GetGetCommandsCommand_( DefaultCommand );

			GetBackendCommands_();
		}
		void Internal_( fblcmd::command Command )
		{
			PushHeader( FBLFRD_MASTER_OBJECT, Commands_[Command] );
		}
		void TestInput_( fblcst::cast__ Cast )
		{
			C_().Put( Cast );
		}
		fblovl::reply__ Send_( void )
		{
			fblovl::reply__ Reply = fblovl::r_Undefined;
		qRH
		qRB
			if ( Channel_ != NULL ) {
				if ( _FlowInParameter != NULL ) {
					if ( !FlowIn_(false, *_FlowInParameter) ) {
						Channel_ = NULL;
						Reply = fblovl::rDisconnected;
					}

					_FlowInParameter = NULL;
				}

				if ( Reply == fblovl::r_Undefined ) {
					C_().Commit();

					if ( C_().EndOfFlow() ) {
						Channel_ = NULL;
						Reply = fblovl::rDisconnected;
						Message_[0] = 0;
					} else {
						if ( ( Reply = (fblovl::reply__)C_().Get() ) != fblovl::rOK ) {
							if ( Reply >= fblovl::r_amount )
								qRFwk();

							if ( ( !flw::GetString( C_(), Message_, sizeof( Message_ ) ) ) )
								strcpy( Message_ + sizeof( Message_ ) - 7, " [...]" );
						}
					}
				}
			} else
				Reply = fblovl::rDisconnected;
		qRR
			Reply = fblovl::rDisconnected;
			Channel_ = NULL;
		qRT
		qRE
			return Reply;
		}
		bso::bool__ _TestCompatibility(
			const char *Language,
			const compatibility_informations__ &CompatibilityInformations,
			flw::ioflow__ &Flow,
			incompatibility_informations_ &IncompatibilityInformations );
	public:
		void reset( bool P = true )
		{
			if ( P ) {
				if ( Channel_ != NULL )
					Disconnect();
			}

			ParametersCallback_ = NULL;
			FrontendCallback_ = NULL;
			_ReportingCallback = NULL;
			Channel_ = NULL;
			_FlowInParameter = NULL;
			_FlowOutParameter = false;
			_DismissPending = false;
			CodeKey_.reset( P );
			Message_[0] = 0;
		}
		E_CVDTOR( frontend___ );
		void Init(
			const str::dString &CodeKey,
			cFrontend &FrontendCallback,
			cReporting &ReportingCallback )
		{

			if ( Channel_ != NULL )
				Disconnect();

			Channel_ = NULL;
			ParametersCallback_ = NULL;
			FrontendCallback_ = &FrontendCallback;
			_ReportingCallback = &ReportingCallback;
			_FlowInParameter = NULL;
			_FlowOutParameter = false;
			_DismissPending = false;
			CodeKey_.Init( CodeKey );
		}
		bso::bool__ Connect(
			const char *Language,
			flw::ioflow__ &Channel,
			fblfph::callbacks__ *ParametersCallback,	// If == NULL, no backend is used, and this object is only used to know that there is no backend connection.
			const compatibility_informations__ &CompatibilityInformations,
			incompatibility_informations_ &IncompatibilityInformations )
		{
			bso::bool__ Success = true;
		qRH
		qRB
			if ( Channel_ != NULL )
				qRFwk();

			if ( ParametersCallback_ != NULL )
				qRFwk();

			ParametersCallback_ = ParametersCallback;

			if ( ParametersCallback_ != NULL ) {
				Channel_ = &Channel;


				if ( !_TestCompatibility( Language, CompatibilityInformations, C_(), IncompatibilityInformations ) )
					Success = false;
				else {
					RetrieveBackendCommands_();

					F_().OnConnection();
				}
			}
		qRR
			Channel_ = NULL;	// Pour viter toute future tentative de communication avec le backend.
		qRT
		qRE
			return Success;
		}
		void Disconnect( void )
		{
			F_().OnDisconnection();

			Internal_( fblcmd::cDisconnect );

			EndOfInParameters();

			Handle();

			Channel_ = NULL;

			ParametersCallback_ = NULL;
		}
		//f Add header with object 'Object' and command 'Command'.
		void PushHeader(
			object__ Object,
			command__ Command )
		{
			_PreProcess();

			flw::Put( Object, C_() );
			flw::Put( Command, C_() );
		}
		//f Put 'Object'.
		FBLFRD_M( Object, object__)
		FBLFRD_M( Boolean, boolean__ )
		FBLFRD_M( Booleans, booleans_ )
		FBLFRD_M( SInt, sint__ )
		FBLFRD_M( SInt, sints_ )
		FBLFRD_M( UInt, uint__ )
		FBLFRD_M( UInts, uints_ )
		FBLFRD_M( SBig, sbig__ )
		FBLFRD_M( SBig, sbigs_ )
		FBLFRD_M( UBig, ubig__ )
		FBLFRD_M( UBigs, ubigs_ )
		FBLFRD_M( Id, id__ )
		FBLFRD_M( Ids, ids_ )
		FBLFRD_M( XIds, xids_ )
		FBLFRD_M( Id8, id8__ )
		FBLFRD_M( Id8s, id8s_ )
		FBLFRD_M( XId8s, xid8s_ )
		FBLFRD_M( Id16, id16__ )
		FBLFRD_M( Id16s, id16s_ )
		FBLFRD_M( XId16s, xid16s_ )
		FBLFRD_M( Id32, id32__ )
		FBLFRD_M( Id32s, id32s_ )
		FBLFRD_M( XId32s, xid32s_ )
		FBLFRD_M( Char, char__ )
		FBLFRD_M( String, string_ )
		FBLFRD_M( Strings, strings_ )
		FBLFRD_M( XStrings, xstrings_ )
		FBLFRD_M( Byte, byte__ )
		FBLFRD_M( Binary, binary_ )
		FBLFRD_M( Binaries, binaries_ )
		FBLFRD_M( Items, items_ )
		FBLFRD_M( Item8s, item8s_ )
		FBLFRD_M( Item16s, item16s_ )
		FBLFRD_M( Item32s, item32s_ )
		FBLFRD_M( XItem8s, xitem8s_ )
		FBLFRD_M( XItem16s, xitem16s_ )
		FBLFRD_M( XItem32s, xitem32s_ )
		FBLFRD_M( CommandsDetails, commands_details_ )
		FBLFRD_M( ObjectsReferences, objects_references_ )
		void FlowIn( flw::iflow__ &Flow )
		{
			if ( _FlowInParameter != NULL)
				qRFwk();

			C_().Put( fblcst::cFlow );

			if ( !FlowIn_( true, Flow ) )
				qRFwk();

			_FlowInParameter = &Flow;

			// Le passage du contenu du paramtre se fera ultrieurement, en fin de requte.
		}
		// Lorsque tout le contenu de ce 'Flow' est lu, appeler 'Dismiss()'.
		void FlowOut( flw::iflow__ *&Flow )
		{
			if ( _FlowOutParameter )
				qRFwk();

			_FlowOut( Flow );

			_FlowOutParameter = true;
		}
		void EndOfInParameters( void )
		{
			C_().Put( 0 );	// End of request
		}
		//f Send the request.
		void Handle( void )
		{
			fblovl::reply__  Reply = Send_();

			if ( Reply == fblovl::rOK )
				_PostProcess( C_() );

			if ( Reply != fblovl::rDisconnected ) {

				if ( C_().Get() != fblcst::cEnd )
					qRFwk();

				if ( !_FlowOutParameter )
					C_().Dismiss();
				else {
					if ( _DismissPending )
						qRFwk();

					_DismissPending= true;
					_FlowOutParameter = false;
				}

				_FlowOutParameter = false;
			}

			if ( Reply != rOK )
				_ReportError( Reply, str::wString(Message_));
		}
		void DismissFlow( void )	// To call only when the flow as a out parameter was entirely red ('EndOfFlow()' retuns 'true').
		{
			if ( _FlowOutParameter )
				qRFwk();

			if ( !_DismissPending )
				qRFwk();

			C_().Dismiss();

			_DismissPending = false;
		}
		//f Return the explanation messag, if any.
		const char *GetMessage( void ) const
		{
			return Message_;
		}
		//f Return the channel used to handle the request as input flow.
		flw::iflow__ &Input( void )
		{
			return C_();
		}
		//f Return the channel used to handle the request as ouput flow.
		flw::oflow__ &Output( void )
		{
			return C_();
		}
		//f Throw an user error, for testing purpose.
		void ThrowERRFwk( void )
		{
			Internal_( fblcmd::cThrowERRFwk );

			EndOfInParameters();

			Handle();
		}
		//f Throw an intentional error, for testing purpose.
		void ThrowERRFree( void )
		{
			Internal_( fblcmd::cThrowERRFree );

			EndOfInParameters();

			Handle();
		}
		// Test la notification avec retournant le message 'Message'.
		void TestNotification( const string_ &Message )
		{
			Internal_( fblcmd::cTestNotification );

			StringIn( Message );

			EndOfInParameters();

			return Handle();	// NOTA : Always to 'true'.
		}
		//f Return the id of a new object of type 'Type'.
		object__ GetNewObject( type__ Type )
		{
			object__ O;

			Internal_( fblcmd::cGetNewObject );

			Id16In( Type );

			EndOfInParameters();

			ObjectOut( O );

			Handle();

			return O;
		}
		//f Return the type id. of type 'TypeName'.
		type__ GetType( const string_ &TypeName )
		{
			type__ Type;

			Internal_( fblcmd::cGetType );

			StringIn( TypeName );

			EndOfInParameters();

			Id16Out( Type );

			Handle();

			return Type;
		}
#if 0
		//f Put in 'RawMessages' all the raw messages from backend.
		void GetRawMessages( strings_ &RawMessages )
		{
			Internal_( fblcmd::cGetRawMessages );

			EndOfInParameters();

			StringsOut( RawMessages );

			_Handle();
		}
#endif
		//f Return the command of object type 'Type', named 'Name' with parameter 'Parameters'.
		command__ GetCommand(
			type__ Type,
			const string_ &Name,
			const id8s_ &Parameters )
		{
			command__ Command;

			Internal_( fblcmd::cGetCommand );

			Id16In( Type );
			StringIn( Name );
			Id8sIn( Parameters );

			EndOfInParameters();

			Id16Out( Command );

			Handle();

			return Command;
		}
		/*f Put in 'Commands' commands from object of type 'Type',
		whith names 'Names' and casts 'Casts'. */
		void GetCommands(
			type__ Type,
			const commands_details_ &CommandDetails,
			id16s_ &Commands )
		{
			Internal_( fblcmd::cGetCommands );

			Id16In( Type );
			CommandsDetailsIn( CommandDetails );

			EndOfInParameters();

			Id16sOut( Commands );

			Handle();
		}
		//f Remove object 'Object'.
		void RemoveObject( object__ Object )
		{
			Internal_( fblcmd::cRemoveObject );

			ObjectIn( Object );

			EndOfInParameters();

			Handle();
		}
		void About(
			string_ &ProtocolVersion,
			string_ &BackendLabel,
			string_ &APIVersion,
			string_ &Backend,
			string_ &BackendCopyright,
			string_ &Software )
		{
			Internal_( fblcmd::cAbout );

			EndOfInParameters();

			StringOut( ProtocolVersion );
			StringOut( BackendLabel );
			StringOut( APIVersion );
			StringOut( Backend );
			StringOut( BackendCopyright );
			StringOut( Software );

			Handle();
		}
		/// (watchdog testing purpose) To verify if the the 'backend' is still responding. If 'Code' is correct, the backend doesn't return.
		void Ping( const str::dString &Code );
		/// Crashes the backend (watchdog testing purpose), if 'Code' is correct.
		void Crash( const str::dString &Code );
		//f Disconnection.
		bso::bool__ IsConnected( void ) const
		{
			return Channel_ != NULL;
		}
		//f Put in 'TypeXItems' the types prefix, name and id.
		void GetTypesIDAndPrefixAndName( xitem16s_ &TypeXItems )
		{
			Internal_( fblcmd::cGetTypesIDAndPrefixAndName );

			EndOfInParameters();

			XItem16sOut( TypeXItems );

			Handle();
		}
		//f Put in 'Items' the commands name and id of an object of type 'Type'.
		void GetCommandsIDAndName(
			type__ Type,
			item16s_ &CommandItems )
		{
			Internal_( fblcmd::cGetCommandsIDAndName );

			Id16In( Type );

			EndOfInParameters();

			Item16sOut( CommandItems );

			Handle();
		}
   //f Return the current language.
		const string_ &GetLanguage( string_ &Language )
        {
			Internal_( fblcmd::cGetLanguage );

			EndOfInParameters();

			StringOut( Language );

			Handle();

			return Language;
        }
         //f Set language to language of id 'Language';
		void SetLanguage( const string_ &Language )
		{
			Internal_( fblcmd::cSetLanguage );

			StringIn( Language );

			EndOfInParameters();

			Handle();
		}
  		//f Put in 'Parameters' parameters of command 'Command' from object type 'Type'.
		void GetParameters(
			type__ Type,
			command__ Command,
			id8s_ &Parameters )
		{
			Internal_( fblcmd::cGetParameters );
			Id16In( Type );
			Id16In( Command );

			EndOfInParameters();

			Id8sOut( Parameters );

			Handle();
		}
	};

	class frontend_depot__
	{
	private:
		frontend___ *_Frontend;
	public:
		void reset( bso::bool__ = true )
		{
			_Frontend = NULL;
		}
		E_CDTOR( frontend_depot__ );
		void Init( frontend___ &Frontend )
		{
			_Frontend = &Frontend;
		}
		frontend___ &Frontend( void ) const
		{
			if ( _Frontend == NULL )
				qRFwk();

			return *_Frontend;
		}
	};


	class universal_frontend___
	: public frontend___
	{
	private:
		fblfrp::remote_callbacks___ _RemoteCallbacks;
		fblfep::embedded_callbacks__ _EmbeddedCallbacks;
	public:
		void reset( bso::bool__ P = true )
		{
			frontend___::reset( P );
			_RemoteCallbacks.reset( P );
			_EmbeddedCallbacks.reset( P );
		}
		E_CVDTOR( universal_frontend___ );
		bso::bool__ Connect(
			const char *Language,
			flw::ioflow__ &Flow,
			fblovl::eMode Mode,
			const compatibility_informations__ &CompatibilityInformations,
			incompatibility_informations_ &IncompatibilityInformations )
		{
			fblfph::callbacks__ *Callbacks = NULL;

			switch ( Mode ) {
			case fblovl::mNone:
				break;
			case fblovl::mSerialized:
				_RemoteCallbacks.Init();
				Callbacks = &_RemoteCallbacks;
				break;
			case fblovl::mReferenced:
				_EmbeddedCallbacks.Init();
				Callbacks = &_EmbeddedCallbacks;
				break;
			default:
				qRFwk();
				break;
			}

			return frontend___::Connect( Language, Flow, Callbacks, CompatibilityInformations, IncompatibilityInformations  );
		}
		void Disconnect( void )
		{
			if ( IsConnected() ) {
				frontend___::Disconnect();

				_RemoteCallbacks.reset();
				_EmbeddedCallbacks.reset();
			}
		}
	};
}

/*************/
/**** NEW ****/
/*************/

namespace fblfrd {
	typedef compatibility_informations__ sCompatibilityInformations;

	typedef incompatibility_informations_ dIncompatibilityInformations;
	qW( IncompatibilityInformations );

	/*
	class sDefaultReportingCallback
	: public cReporting
	{
	protected:
		virtual void FBLFRDReport(
			fblovl::reply__ Reply,
			const char *Message ) override;
	public:
		void reset( bso::bool__ P = true )
		{
			cReporting::reset( P );
		}
		E_CVDTOR( sDefaultReportingCallback );
		void Init( void)
		{
			cReporting::Init();
		}
	};
	*/
}

#endif
