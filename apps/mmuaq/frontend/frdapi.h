/* DON'T MODIFY : computer-generated file ! */
/*
	API from : mmuaqbkd V20161124 Build Nov 25 2016 10:06:59 - Win32;MSC 1800;IA-32

	This file was generated using barq 20161027 (Build Nov 13 2016 17:38:12 Win32;MSC 1800;IA-32)
*/

#ifndef MMUAQ__INC
# define MMUAQ__INC

# define MMUAQ_TARGET "mmuaq"
# define MMUAQ_API_VERSION "1"

# include "fblfrd.h"
# include "cpe.h"

namespace mmuaq {

	namespace {
		typedef fblfrd::frontend_depot__ _frontend_depot__;
	}

	class fStatics
	: public _frontend_depot__
	{
	private:
		fblfrd::object__ _ID;
		fblfrd::command__ _Commands[16];
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend_depot__::reset( P );
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( fStatics )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8__ Parameters[] = {
				25, 0, 
				25, 0, 
				0, 
				25, 25, 0, 2, 
				0, 22, 26, 
				21, 0, 25, 25, 25, 
				21, 25, 25, 25, 2, 25, 0, 21, 
				0, 13, 
				0, 21, 21, 
				21, 0, 22, 
				22, 0, 26, 
				21, 25, 0, 
				21, 0, 22, 26, 
				21, 0, 25, 
				21, 21, 0, 
				21, 21, 0, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = FBLFRD_MASTER_OBJECT;
			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "LoadSetupOfId_1";;
			CommandDetail.Casts.Append( Parameters + 0, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "LoadSetupContent_1";;
			CommandDetail.Casts.Append( Parameters + 2, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUATest_1";;
			CommandDetail.Casts.Append( Parameters + 4, 1 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUALogin_1";;
			CommandDetail.Casts.Append( Parameters + 5, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetAgents_1";;
			CommandDetail.Casts.Append( Parameters + 9, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetAgent_1";;
			CommandDetail.Casts.Append( Parameters + 12, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAUpdateAgent_1";;
			CommandDetail.Casts.Append( Parameters + 17, 8 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetFields_1";;
			CommandDetail.Casts.Append( Parameters + 25, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetRootAndInboxFolders_1";;
			CommandDetail.Casts.Append( Parameters + 27, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetFolders_1";;
			CommandDetail.Casts.Append( Parameters + 30, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetFoldersNames_1";;
			CommandDetail.Casts.Append( Parameters + 33, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUARenameFolder_1";;
			CommandDetail.Casts.Append( Parameters + 36, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetMailsFields_1";;
			CommandDetail.Casts.Append( Parameters + 39, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetMail_1";;
			CommandDetail.Casts.Append( Parameters + 43, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAMoveMailTo_1";;
			CommandDetail.Casts.Append( Parameters + 46, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAMoveFolderTo_1";;
			CommandDetail.Casts.Append( Parameters + 49, 3 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( FBLFRD_MASTER_TYPE, CommandsDetails, Commands );
			Commands.Recall( 0, 16, _Commands );
		}
		void LoadSetupOfId_1( 
			const fblfrd::string_ &In1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[0] );
			Frontend().StringIn( In1 );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		void LoadSetupContent_1( 
			const fblfrd::string_ &In1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[1] );
			Frontend().StringIn( In1 );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		void MUATest_1( void ) const
		{
			Frontend().PushHeader( _ID, Commands()[2] );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		void MUALogin_1( 
			const fblfrd::string_ &In1,
			const fblfrd::string_ &In2,
			fblfrd::boolean__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[3] );
			Frontend().StringIn( In1 );
			Frontend().StringIn( In2 );

			Frontend().EndOfInParameters();

			Frontend().BooleanOut( Out1 );

			Frontend().Handle();
		}
		void MUAGetAgents_1( 
			fblfrd::ids_ &Out1,
			fblfrd::strings_ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[4] );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().StringsOut( Out2 );

			Frontend().Handle();
		}
		void MUAGetAgent_1( 
			const fblfrd::id__ &In1,
			fblfrd::string_ &Out1,
			fblfrd::string_ &Out2,
			fblfrd::string_ &Out3 ) const
		{
			Frontend().PushHeader( _ID, Commands()[5] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().StringOut( Out1 );
			Frontend().StringOut( Out2 );
			Frontend().StringOut( Out3 );

			Frontend().Handle();
		}
		void MUAUpdateAgent_1( 
			const fblfrd::id__ &In1,
			const fblfrd::string_ &In2,
			const fblfrd::string_ &In3,
			const fblfrd::string_ &In4,
			const fblfrd::boolean__ &In5,
			const fblfrd::string_ &In6,
			fblfrd::id__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[6] );
			Frontend().IdIn( In1 );
			Frontend().StringIn( In2 );
			Frontend().StringIn( In3 );
			Frontend().StringIn( In4 );
			Frontend().BooleanIn( In5 );
			Frontend().StringIn( In6 );

			Frontend().EndOfInParameters();

			Frontend().IdOut( Out1 );

			Frontend().Handle();
		}
		void MUAGetFields_1( 
			fblfrd::id8s_ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[7] );

			Frontend().EndOfInParameters();

			Frontend().Id8sOut( Out1 );

			Frontend().Handle();
		}
		void MUAGetRootAndInboxFolders_1( 
			fblfrd::id__ &Out1,
			fblfrd::id__ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[8] );

			Frontend().EndOfInParameters();

			Frontend().IdOut( Out1 );
			Frontend().IdOut( Out2 );

			Frontend().Handle();
		}
		void MUAGetFolders_1( 
			const fblfrd::id__ &In1,
			fblfrd::ids_ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[9] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );

			Frontend().Handle();
		}
		void MUAGetFoldersNames_1( 
			const fblfrd::ids_ &In1,
			fblfrd::strings_ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[10] );
			Frontend().IdsIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().StringsOut( Out1 );

			Frontend().Handle();
		}
		void MUARenameFolder_1( 
			const fblfrd::id__ &In1,
			const fblfrd::string_ &In2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[11] );
			Frontend().IdIn( In1 );
			Frontend().StringIn( In2 );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		void MUAGetMailsFields_1( 
			const fblfrd::id__ &In1,
			fblfrd::ids_ &Out1,
			fblfrd::strings_ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[12] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().StringsOut( Out2 );

			Frontend().Handle();
		}
		void MUAGetMail_1( 
			const fblfrd::id__ &In1,
			fblfrd::string_ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[13] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().StringOut( Out1 );

			Frontend().Handle();
		}
		void MUAMoveMailTo_1( 
			const fblfrd::id__ &In1,
			const fblfrd::id__ &In2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[14] );
			Frontend().IdIn( In1 );
			Frontend().IdIn( In2 );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		void MUAMoveFolderTo_1( 
			const fblfrd::id__ &In1,
			const fblfrd::id__ &In2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[15] );
			Frontend().IdIn( In1 );
			Frontend().IdIn( In2 );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		const fblfrd::command__ *Commands( void ) const
		{
			return _Commands;
		}
	};

	class fMUAMyObjectCommon
	: public _frontend_depot__
	{
	private:
		fblfrd::id16__ _ID;
		fblfrd::command__ _Commands[2];
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend_depot__::reset( P );
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( fMUAMyObjectCommon )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8__ Parameters[] = {
				25, 0, 25, 
				0, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = this->Frontend().GetType( str::string( "MyObject" ) );

			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "ToUC";;
			CommandDetail.Casts.Append( Parameters + 0, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Test";;
			CommandDetail.Casts.Append( Parameters + 3, 1 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( _ID, CommandsDetails, Commands );
			Commands.Recall( 0, 2, _Commands );
		}
		fblfrd::object__ GetNewObject( void )
		{
			return Frontend().GetNewObject( _ID );
		}
		void RemoveObject( fblfrd::object__ Object )
		{
			Frontend().RemoveObject( Object );
		}
		const fblfrd::command__ *Commands( void ) const
		{
			return _Commands;
		}
	};

	class rMUAMyObject
	{
	private:
		fblfrd::object__ _ID;
		fMUAMyObjectCommon *Common_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P == true )
				if ( ( Common_ != NULL ) && ( _ID != FBLFRD_UNDEFINED_OBJECT ) && Common_->Frontend().IsConnected() )
					Common_->RemoveObject( _ID );

			Common_ = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		rMUAMyObject( void )
		{
			reset( false );
		}
		~rMUAMyObject( void )
		{
			reset();
		}
		fblfrd::object__ ID( void ) const
		{
			return _ID;
		}
		void Init( fMUAMyObjectCommon &Common )
		{
			Common_ = &Common;

			_ID = Common_->GetNewObject();
		}
		void ToUC( 
			const fblfrd::string_ &In1,
			fblfrd::string_ &Out1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );
			Common_->Frontend().StringIn( In1 );

			Common_->Frontend().EndOfInParameters();

			Common_->Frontend().StringOut( Out1 );

			Common_->Frontend().Handle();
		}
		void Test( void ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[1] );

			Common_->Frontend().EndOfInParameters();


			Common_->Frontend().Handle();
		}
	};

}

#endif //#ifndef mmuaq__INC
