/* DON'T MODIFY : computer-generated file ! */
/*
	API from : mmuaqbkd V20170127 Build Jan 29 2017 11:04:34 - Win32;MSC 1800;IA-32

	This file was generated using barq 20161027 (Build Dec 21 2016 17:42:27 Win32;MSC 1800;IA-32)
*/

#ifndef MMUAQ_INC_
# define MMUAQ_INC_

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
		fblfrd::command__ _Commands[19];
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
				0, 
				0, 22, 26, 3, 
				21, 0, 25, 2, 25, 25, 2, 
				21, 25, 2, 25, 25, 2, 25, 0, 21, 
				21, 0, 
				0, 13, 
				0, 21, 21, 
				21, 0, 22, 
				22, 0, 26, 
				21, 25, 0, 21, 
				21, 25, 0, 
				21, 0, 22, 26, 22, 
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
			CommandDetail.Name = "MUACheck_1";;
			CommandDetail.Casts.Append( Parameters + 9, 1 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetAgents_1";;
			CommandDetail.Casts.Append( Parameters + 10, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetAgent_1";;
			CommandDetail.Casts.Append( Parameters + 14, 7 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAUpdateAgent_1";;
			CommandDetail.Casts.Append( Parameters + 21, 9 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUARemoveAgent_1";;
			CommandDetail.Casts.Append( Parameters + 30, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetFields_1";;
			CommandDetail.Casts.Append( Parameters + 32, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetRootAndInboxFolders_1";;
			CommandDetail.Casts.Append( Parameters + 34, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetFolders_1";;
			CommandDetail.Casts.Append( Parameters + 37, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetFoldersNames_1";;
			CommandDetail.Casts.Append( Parameters + 40, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUACreateFolder_1";;
			CommandDetail.Casts.Append( Parameters + 43, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAUpdateFolder_1";;
			CommandDetail.Casts.Append( Parameters + 47, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetMailsFields_1";;
			CommandDetail.Casts.Append( Parameters + 50, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAGetMail_1";;
			CommandDetail.Casts.Append( Parameters + 55, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAMoveMailTo_1";;
			CommandDetail.Casts.Append( Parameters + 58, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MUAMoveFolderTo_1";;
			CommandDetail.Casts.Append( Parameters + 61, 3 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( FBLFRD_MASTER_TYPE, CommandsDetails, Commands );
			Commands.Recall( 0, 19, _Commands );
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
		void MUACheck_1( void ) const
		{
			Frontend().PushHeader( _ID, Commands()[4] );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		void MUAGetAgents_1( 
			fblfrd::ids_ &Out1,
			fblfrd::strings_ &Out2,
			fblfrd::booleans_ &Out3 ) const
		{
			Frontend().PushHeader( _ID, Commands()[5] );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().StringsOut( Out2 );
			Frontend().BooleansOut( Out3 );

			Frontend().Handle();
		}
		void MUAGetAgent_1( 
			const fblfrd::id__ &In1,
			fblfrd::string_ &Out1,
			fblfrd::boolean__ &Out2,
			fblfrd::string_ &Out3,
			fblfrd::string_ &Out4,
			fblfrd::boolean__ &Out5 ) const
		{
			Frontend().PushHeader( _ID, Commands()[6] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().StringOut( Out1 );
			Frontend().BooleanOut( Out2 );
			Frontend().StringOut( Out3 );
			Frontend().StringOut( Out4 );
			Frontend().BooleanOut( Out5 );

			Frontend().Handle();
		}
		void MUAUpdateAgent_1( 
			const fblfrd::id__ &In1,
			const fblfrd::string_ &In2,
			const fblfrd::boolean__ &In3,
			const fblfrd::string_ &In4,
			const fblfrd::string_ &In5,
			const fblfrd::boolean__ &In6,
			const fblfrd::string_ &In7,
			fblfrd::id__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[7] );
			Frontend().IdIn( In1 );
			Frontend().StringIn( In2 );
			Frontend().BooleanIn( In3 );
			Frontend().StringIn( In4 );
			Frontend().StringIn( In5 );
			Frontend().BooleanIn( In6 );
			Frontend().StringIn( In7 );

			Frontend().EndOfInParameters();

			Frontend().IdOut( Out1 );

			Frontend().Handle();
		}
		void MUARemoveAgent_1( 
			const fblfrd::id__ &In1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[8] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		void MUAGetFields_1( 
			fblfrd::id8s_ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[9] );

			Frontend().EndOfInParameters();

			Frontend().Id8sOut( Out1 );

			Frontend().Handle();
		}
		void MUAGetRootAndInboxFolders_1( 
			fblfrd::id__ &Out1,
			fblfrd::id__ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[10] );

			Frontend().EndOfInParameters();

			Frontend().IdOut( Out1 );
			Frontend().IdOut( Out2 );

			Frontend().Handle();
		}
		void MUAGetFolders_1( 
			const fblfrd::id__ &In1,
			fblfrd::ids_ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[11] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );

			Frontend().Handle();
		}
		void MUAGetFoldersNames_1( 
			const fblfrd::ids_ &In1,
			fblfrd::strings_ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[12] );
			Frontend().IdsIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().StringsOut( Out1 );

			Frontend().Handle();
		}
		void MUACreateFolder_1( 
			const fblfrd::id__ &In1,
			const fblfrd::string_ &In2,
			fblfrd::id__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[13] );
			Frontend().IdIn( In1 );
			Frontend().StringIn( In2 );

			Frontend().EndOfInParameters();

			Frontend().IdOut( Out1 );

			Frontend().Handle();
		}
		void MUAUpdateFolder_1( 
			const fblfrd::id__ &In1,
			const fblfrd::string_ &In2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[14] );
			Frontend().IdIn( In1 );
			Frontend().StringIn( In2 );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		void MUAGetMailsFields_1( 
			const fblfrd::id__ &In1,
			fblfrd::ids_ &Out1,
			fblfrd::strings_ &Out2,
			fblfrd::ids_ &Out3 ) const
		{
			Frontend().PushHeader( _ID, Commands()[15] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().StringsOut( Out2 );
			Frontend().IdsOut( Out3 );

			Frontend().Handle();
		}
		void MUAGetMail_1( 
			const fblfrd::id__ &In1,
			fblfrd::string_ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[16] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().StringOut( Out1 );

			Frontend().Handle();
		}
		void MUAMoveMailTo_1( 
			const fblfrd::id__ &In1,
			const fblfrd::id__ &In2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[17] );
			Frontend().IdIn( In1 );
			Frontend().IdIn( In2 );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		void MUAMoveFolderTo_1( 
			const fblfrd::id__ &In1,
			const fblfrd::id__ &In2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[18] );
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

#endif //#ifndef mmuaq_INC_
