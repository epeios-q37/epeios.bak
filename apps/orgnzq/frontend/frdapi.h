/* DON'T MODIFY : computer-generated file ! */
/*
	API from : orgnzqbkd 20171029 Build Nov  7 2017 10:10:09 - Win64;MSC 1911;AMD64

	This file was generated using barq 20171107 (Build Nov  7 2017 10:45:46 Win64;MSC 1911;AMD64)
*/

#ifndef ORGNZQ_INC_
# define ORGNZQ_INC_

# define ORGNZQ_TARGET "orgnzq"
# define ORGNZQ_API_VERSION "1"

# include "fblfrd.h"
# include "cpe.h"

namespace orgnzq {

	namespace {
		typedef fblfrd::frontend_depot__ _frontend_depot__;
	}

	class fStatics
	: public _frontend_depot__
	{
	private:
		fblfrd::object__ _ID;
		fblfrd::command__ _Commands[13];
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
				25, 25, 0, 2, 
				0, 13, 26, 
				0, 22, 26, 
				21, 0, 22, 22, 13, 26, 26, 
				21, 0, 22, 22, 22, 27, 
				22, 22, 0, 26, 
				0, 21, 
				21, 1, 1, 0, 21, 
				21, 1, 0, 2, 2, 
				0, 22, 26, 
				21, 21, 21, 0, 
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
			CommandDetail.Name = "OGZLogin_1";;
			CommandDetail.Casts.Append( Parameters + 4, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetNumbers_1";;
			CommandDetail.Casts.Append( Parameters + 8, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetTypes_1";;
			CommandDetail.Casts.Append( Parameters + 11, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetRecordColumns_1";;
			CommandDetail.Casts.Append( Parameters + 14, 7 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetRecordFields_1";;
			CommandDetail.Casts.Append( Parameters + 21, 6 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetEntries_1";;
			CommandDetail.Casts.Append( Parameters + 27, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZCreateRecord_1";;
			CommandDetail.Casts.Append( Parameters + 31, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZCreateField_1";;
			CommandDetail.Casts.Append( Parameters + 33, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZUpdateField_1";;
			CommandDetail.Casts.Append( Parameters + 38, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetRecords_1";;
			CommandDetail.Casts.Append( Parameters + 43, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZMoveField_1";;
			CommandDetail.Casts.Append( Parameters + 46, 4 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( FBLFRD_MASTER_TYPE, CommandsDetails, Commands );
			Commands.Recall( 0, 13, _Commands );
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
		void OGZLogin_1( 
			const fblfrd::string_ &In1,
			const fblfrd::string_ &In2,
			fblfrd::boolean__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[2] );
			Frontend().StringIn( In1 );
			Frontend().StringIn( In2 );

			Frontend().EndOfInParameters();

			Frontend().BooleanOut( Out1 );

			Frontend().Handle();
		}
		void OGZGetNumbers_1( 
			fblfrd::id8s_ &Out1,
			fblfrd::strings_ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[3] );

			Frontend().EndOfInParameters();

			Frontend().Id8sOut( Out1 );
			Frontend().StringsOut( Out2 );

			Frontend().Handle();
		}
		void OGZGetTypes_1( 
			fblfrd::ids_ &Out1,
			fblfrd::strings_ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[4] );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().StringsOut( Out2 );

			Frontend().Handle();
		}
		void OGZGetRecordColumns_1( 
			const fblfrd::id__ &In1,
			fblfrd::ids_ &Out1,
			fblfrd::ids_ &Out2,
			fblfrd::id8s_ &Out3,
			fblfrd::strings_ &Out4,
			fblfrd::strings_ &Out5 ) const
		{
			Frontend().PushHeader( _ID, Commands()[5] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().IdsOut( Out2 );
			Frontend().Id8sOut( Out3 );
			Frontend().StringsOut( Out4 );
			Frontend().StringsOut( Out5 );

			Frontend().Handle();
		}
		void OGZGetRecordFields_1( 
			const fblfrd::id__ &In1,
			fblfrd::ids_ &Out1,
			fblfrd::ids_ &Out2,
			fblfrd::ids_ &Out3,
			fblfrd::xstrings_ &Out4 ) const
		{
			Frontend().PushHeader( _ID, Commands()[6] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().IdsOut( Out2 );
			Frontend().IdsOut( Out3 );
			Frontend().XStringsOut( Out4 );

			Frontend().Handle();
		}
		void OGZGetEntries_1( 
			const fblfrd::ids_ &In1,
			const fblfrd::ids_ &In2,
			fblfrd::strings_ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[7] );
			Frontend().IdsIn( In1 );
			Frontend().IdsIn( In2 );

			Frontend().EndOfInParameters();

			Frontend().StringsOut( Out1 );

			Frontend().Handle();
		}
		void OGZCreateRecord_1( 
			fblfrd::id__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[8] );

			Frontend().EndOfInParameters();

			Frontend().IdOut( Out1 );

			Frontend().Handle();
		}
		void OGZCreateField_1( 
			const fblfrd::id__ &In1,
			const fblfrd::object__ &In2,
			const fblfrd::object__ &In3,
			fblfrd::id__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[9] );
			Frontend().IdIn( In1 );
			Frontend().ObjectIn( In2 );
			Frontend().ObjectIn( In3 );

			Frontend().EndOfInParameters();

			Frontend().IdOut( Out1 );

			Frontend().Handle();
		}
		void OGZUpdateField_1( 
			const fblfrd::id__ &In1,
			const fblfrd::object__ &In2,
			fblfrd::boolean__ &Out1,
			fblfrd::boolean__ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[10] );
			Frontend().IdIn( In1 );
			Frontend().ObjectIn( In2 );

			Frontend().EndOfInParameters();

			Frontend().BooleanOut( Out1 );
			Frontend().BooleanOut( Out2 );

			Frontend().Handle();
		}
		void OGZGetRecords_1( 
			fblfrd::ids_ &Out1,
			fblfrd::strings_ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[11] );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().StringsOut( Out2 );

			Frontend().Handle();
		}
		void OGZMoveField_1( 
			const fblfrd::id__ &In1,
			const fblfrd::id__ &In2,
			const fblfrd::id__ &In3 ) const
		{
			Frontend().PushHeader( _ID, Commands()[12] );
			Frontend().IdIn( In1 );
			Frontend().IdIn( In2 );
			Frontend().IdIn( In3 );

			Frontend().EndOfInParameters();


			Frontend().Handle();
		}
		const fblfrd::command__ *Commands( void ) const
		{
			return _Commands;
		}
	};

	class fOGZColumnCommon
	: public _frontend_depot__
	{
	private:
		fblfrd::id16__ _ID;
		fblfrd::command__ _Commands[4];
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend_depot__::reset( P );
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( fOGZColumnCommon )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8__ Parameters[] = {
				0, 
				21, 0, 
				21, 12, 25, 25, 0, 
				0, 21, 12, 25, 25, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = this->Frontend().GetType( str::string( "Column" ) );

			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "New_1";;
			CommandDetail.Casts.Append( Parameters + 0, 1 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Fill_1";;
			CommandDetail.Casts.Append( Parameters + 1, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Update_1";;
			CommandDetail.Casts.Append( Parameters + 3, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Get_1";;
			CommandDetail.Casts.Append( Parameters + 8, 5 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( _ID, CommandsDetails, Commands );
			Commands.Recall( 0, 4, _Commands );
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

	class fOGZFieldCommon
	: public _frontend_depot__
	{
	private:
		fblfrd::id16__ _ID;
		fblfrd::command__ _Commands[5];
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend_depot__::reset( P );
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( fOGZFieldCommon )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8__ Parameters[] = {
				1, 0, 
				21, 0, 
				0, 21, 12, 22, 26, 
				21, 25, 0, 2, 
				21, 21, 0, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = this->Frontend().GetType( str::string( "Field" ) );

			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "New_1";;
			CommandDetail.Casts.Append( Parameters + 0, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Fill_1";;
			CommandDetail.Casts.Append( Parameters + 2, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Get_1";;
			CommandDetail.Casts.Append( Parameters + 4, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "UpdateEntry_1";;
			CommandDetail.Casts.Append( Parameters + 9, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "MoveEntry_1";;
			CommandDetail.Casts.Append( Parameters + 13, 3 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( _ID, CommandsDetails, Commands );
			Commands.Recall( 0, 5, _Commands );
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

	class fOGZMyObjectCommon
	: public _frontend_depot__
	{
	private:
		fblfrd::id16__ _ID;
		fblfrd::command__ _Commands[1];
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend_depot__::reset( P );
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( fOGZMyObjectCommon )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8__ Parameters[] = {
				0, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = this->Frontend().GetType( str::string( "MyObject" ) );

			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "Test";;
			CommandDetail.Casts.Append( Parameters + 0, 1 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( _ID, CommandsDetails, Commands );
			Commands.Recall( 0, 1, _Commands );
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

	class rOGZColumn
	{
	private:
		fblfrd::object__ _ID;
		fOGZColumnCommon *Common_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P == true )
				if ( ( Common_ != NULL ) && ( _ID != FBLFRD_UNDEFINED_OBJECT ) && Common_->Frontend().IsConnected() )
					Common_->RemoveObject( _ID );

			Common_ = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		rOGZColumn( void )
		{
			reset( false );
		}
		~rOGZColumn( void )
		{
			reset();
		}
		fblfrd::object__ ID( void ) const
		{
			return _ID;
		}
		void Init( fOGZColumnCommon &Common )
		{
			Common_ = &Common;

			_ID = Common_->GetNewObject();
		}
		void New_1( void ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );

			Common_->Frontend().EndOfInParameters();


			Common_->Frontend().Handle();
		}
		void Fill_1( 
			const fblfrd::id__ &In1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[1] );
			Common_->Frontend().IdIn( In1 );

			Common_->Frontend().EndOfInParameters();


			Common_->Frontend().Handle();
		}
		void Update_1( 
			const fblfrd::id__ &In1,
			const fblfrd::id8__ &In2,
			const fblfrd::string_ &In3,
			const fblfrd::string_ &In4 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[2] );
			Common_->Frontend().IdIn( In1 );
			Common_->Frontend().Id8In( In2 );
			Common_->Frontend().StringIn( In3 );
			Common_->Frontend().StringIn( In4 );

			Common_->Frontend().EndOfInParameters();


			Common_->Frontend().Handle();
		}
		void Get_1( 
			fblfrd::id__ &Out1,
			fblfrd::id8__ &Out2,
			fblfrd::string_ &Out3,
			fblfrd::string_ &Out4 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[3] );

			Common_->Frontend().EndOfInParameters();

			Common_->Frontend().IdOut( Out1 );
			Common_->Frontend().Id8Out( Out2 );
			Common_->Frontend().StringOut( Out3 );
			Common_->Frontend().StringOut( Out4 );

			Common_->Frontend().Handle();
		}
	};

	class rOGZField
	{
	private:
		fblfrd::object__ _ID;
		fOGZFieldCommon *Common_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P == true )
				if ( ( Common_ != NULL ) && ( _ID != FBLFRD_UNDEFINED_OBJECT ) && Common_->Frontend().IsConnected() )
					Common_->RemoveObject( _ID );

			Common_ = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		rOGZField( void )
		{
			reset( false );
		}
		~rOGZField( void )
		{
			reset();
		}
		fblfrd::object__ ID( void ) const
		{
			return _ID;
		}
		void Init( fOGZFieldCommon &Common )
		{
			Common_ = &Common;

			_ID = Common_->GetNewObject();
		}
		void New_1( 
			const fblfrd::object__ &In1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );
			Common_->Frontend().ObjectIn( In1 );

			Common_->Frontend().EndOfInParameters();


			Common_->Frontend().Handle();
		}
		void Fill_1( 
			const fblfrd::id__ &In1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[1] );
			Common_->Frontend().IdIn( In1 );

			Common_->Frontend().EndOfInParameters();


			Common_->Frontend().Handle();
		}
		void Get_1( 
			fblfrd::id__ &Out1,
			fblfrd::id8__ &Out2,
			fblfrd::ids_ &Out3,
			fblfrd::strings_ &Out4 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[2] );

			Common_->Frontend().EndOfInParameters();

			Common_->Frontend().IdOut( Out1 );
			Common_->Frontend().Id8Out( Out2 );
			Common_->Frontend().IdsOut( Out3 );
			Common_->Frontend().StringsOut( Out4 );

			Common_->Frontend().Handle();
		}
		void UpdateEntry_1( 
			const fblfrd::id__ &In1,
			const fblfrd::string_ &In2,
			fblfrd::boolean__ &Out1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[3] );
			Common_->Frontend().IdIn( In1 );
			Common_->Frontend().StringIn( In2 );

			Common_->Frontend().EndOfInParameters();

			Common_->Frontend().BooleanOut( Out1 );

			Common_->Frontend().Handle();
		}
		void MoveEntry_1( 
			const fblfrd::id__ &In1,
			const fblfrd::id__ &In2 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[4] );
			Common_->Frontend().IdIn( In1 );
			Common_->Frontend().IdIn( In2 );

			Common_->Frontend().EndOfInParameters();


			Common_->Frontend().Handle();
		}
	};

	class rOGZMyObject
	{
	private:
		fblfrd::object__ _ID;
		fOGZMyObjectCommon *Common_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P == true )
				if ( ( Common_ != NULL ) && ( _ID != FBLFRD_UNDEFINED_OBJECT ) && Common_->Frontend().IsConnected() )
					Common_->RemoveObject( _ID );

			Common_ = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		rOGZMyObject( void )
		{
			reset( false );
		}
		~rOGZMyObject( void )
		{
			reset();
		}
		fblfrd::object__ ID( void ) const
		{
			return _ID;
		}
		void Init( fOGZMyObjectCommon &Common )
		{
			Common_ = &Common;

			_ID = Common_->GetNewObject();
		}
		void Test( void ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );

			Common_->Frontend().EndOfInParameters();


			Common_->Frontend().Handle();
		}
	};

}

#endif //#ifndef orgnzq_INC_
