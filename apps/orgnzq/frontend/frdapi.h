/* DON'T MODIFY : computer-generated file ! */
/*
	API from : orgnzqbkd 20160615 Build Jun 17 2016 11:41:18 - Win32;MSC 1800;IA-32

	This file was generated using barq 20160313 (Build Jun  5 2016 10:29:06 Win32;MSC 1800;IA-32)
*/

#ifndef ORGNZQ__INC
# define ORGNZQ__INC

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
		fblfrd::command__ _Commands[11];
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
				0, 22, 26, 26, 
				0, 21, 
				21, 1, 0, 21, 
				21, 0, 22, 22, 13, 26, 26, 
				21, 0, 22, 22, 22, 27, 
				21, 1, 0, 
				0, 22, 26, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = FBLFRD_MASTER_OBJECT;
			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "LoadSetupOfId";;
			CommandDetail.Casts.Append( Parameters + 0, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "LoadSetupContent";;
			CommandDetail.Casts.Append( Parameters + 2, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZLogin";;
			CommandDetail.Casts.Append( Parameters + 4, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetNumbers";;
			CommandDetail.Casts.Append( Parameters + 8, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetTypes";;
			CommandDetail.Casts.Append( Parameters + 11, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZCreateRecord";;
			CommandDetail.Casts.Append( Parameters + 15, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZCreateField";;
			CommandDetail.Casts.Append( Parameters + 17, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetRecordColumns";;
			CommandDetail.Casts.Append( Parameters + 21, 7 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetRecordFields";;
			CommandDetail.Casts.Append( Parameters + 28, 6 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZUpdateField";;
			CommandDetail.Casts.Append( Parameters + 34, 3 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetRecords";;
			CommandDetail.Casts.Append( Parameters + 37, 3 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( FBLFRD_MASTER_TYPE, CommandsDetails, Commands );
			Commands.Recall( 0, 11, _Commands );
		}
		fblovl::reply__ LoadSetupOfId( 
			const fblfrd::string_ &In1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[0] );
			Frontend().StringIn( In1 );

			Frontend().EndOfInParameters();


			return Frontend().Handle();
		}
		fblovl::reply__ LoadSetupContent( 
			const fblfrd::string_ &In1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[1] );
			Frontend().StringIn( In1 );

			Frontend().EndOfInParameters();


			return Frontend().Handle();
		}
		fblovl::reply__ OGZLogin( 
			const fblfrd::string_ &In1,
			const fblfrd::string_ &In2,
			fblfrd::boolean__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[2] );
			Frontend().StringIn( In1 );
			Frontend().StringIn( In2 );

			Frontend().EndOfInParameters();

			Frontend().BooleanOut( Out1 );

			return Frontend().Handle();
		}
		fblovl::reply__ OGZGetNumbers( 
			fblfrd::id8s_ &Out1,
			fblfrd::strings_ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[3] );

			Frontend().EndOfInParameters();

			Frontend().Id8sOut( Out1 );
			Frontend().StringsOut( Out2 );

			return Frontend().Handle();
		}
		fblovl::reply__ OGZGetTypes( 
			fblfrd::ids_ &Out1,
			fblfrd::strings_ &Out2,
			fblfrd::strings_ &Out3 ) const
		{
			Frontend().PushHeader( _ID, Commands()[4] );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().StringsOut( Out2 );
			Frontend().StringsOut( Out3 );

			return Frontend().Handle();
		}
		fblovl::reply__ OGZCreateRecord( 
			fblfrd::id__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[5] );

			Frontend().EndOfInParameters();

			Frontend().IdOut( Out1 );

			return Frontend().Handle();
		}
		fblovl::reply__ OGZCreateField( 
			const fblfrd::id__ &In1,
			const fblfrd::object__ &In2,
			fblfrd::id__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[6] );
			Frontend().IdIn( In1 );
			Frontend().ObjectIn( In2 );

			Frontend().EndOfInParameters();

			Frontend().IdOut( Out1 );

			return Frontend().Handle();
		}
		fblovl::reply__ OGZGetRecordColumns( 
			const fblfrd::id__ &In1,
			fblfrd::ids_ &Out1,
			fblfrd::ids_ &Out2,
			fblfrd::id8s_ &Out3,
			fblfrd::strings_ &Out4,
			fblfrd::strings_ &Out5 ) const
		{
			Frontend().PushHeader( _ID, Commands()[7] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().IdsOut( Out2 );
			Frontend().Id8sOut( Out3 );
			Frontend().StringsOut( Out4 );
			Frontend().StringsOut( Out5 );

			return Frontend().Handle();
		}
		fblovl::reply__ OGZGetRecordFields( 
			const fblfrd::id__ &In1,
			fblfrd::ids_ &Out1,
			fblfrd::ids_ &Out2,
			fblfrd::ids_ &Out3,
			fblfrd::xstrings_ &Out4 ) const
		{
			Frontend().PushHeader( _ID, Commands()[8] );
			Frontend().IdIn( In1 );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().IdsOut( Out2 );
			Frontend().IdsOut( Out3 );
			Frontend().XStringsOut( Out4 );

			return Frontend().Handle();
		}
		fblovl::reply__ OGZUpdateField( 
			const fblfrd::id__ &In1,
			const fblfrd::object__ &In2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[9] );
			Frontend().IdIn( In1 );
			Frontend().ObjectIn( In2 );

			Frontend().EndOfInParameters();


			return Frontend().Handle();
		}
		fblovl::reply__ OGZGetRecords( 
			fblfrd::ids_ &Out1,
			fblfrd::strings_ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[10] );

			Frontend().EndOfInParameters();

			Frontend().IdsOut( Out1 );
			Frontend().StringsOut( Out2 );

			return Frontend().Handle();
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
			CommandDetail.Name = "Initialize";;
			CommandDetail.Casts.Append( Parameters + 0, 1 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Define";;
			CommandDetail.Casts.Append( Parameters + 1, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Update";;
			CommandDetail.Casts.Append( Parameters + 3, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Get";;
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
		fblfrd::command__ _Commands[3];
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
				21, 0, 
				0, 21, 12, 22, 26, 
				21, 25, 0, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = this->Frontend().GetType( str::string( "Field" ) );

			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "Define";;
			CommandDetail.Casts.Append( Parameters + 0, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "Get";;
			CommandDetail.Casts.Append( Parameters + 2, 5 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "UpdateEntry";;
			CommandDetail.Casts.Append( Parameters + 7, 3 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( _ID, CommandsDetails, Commands );
			Commands.Recall( 0, 3, _Commands );
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
		fblovl::reply__ Initialize( void ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );

			Common_->Frontend().EndOfInParameters();


			return Common_->Frontend().Handle();
		}
		fblovl::reply__ Define( 
			const fblfrd::id__ &In1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[1] );
			Common_->Frontend().IdIn( In1 );

			Common_->Frontend().EndOfInParameters();


			return Common_->Frontend().Handle();
		}
		fblovl::reply__ Update( 
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


			return Common_->Frontend().Handle();
		}
		fblovl::reply__ Get( 
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

			return Common_->Frontend().Handle();
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
		fblovl::reply__ Define( 
			const fblfrd::id__ &In1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );
			Common_->Frontend().IdIn( In1 );

			Common_->Frontend().EndOfInParameters();


			return Common_->Frontend().Handle();
		}
		fblovl::reply__ Get( 
			fblfrd::id__ &Out1,
			fblfrd::id8__ &Out2,
			fblfrd::ids_ &Out3,
			fblfrd::strings_ &Out4 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[1] );

			Common_->Frontend().EndOfInParameters();

			Common_->Frontend().IdOut( Out1 );
			Common_->Frontend().Id8Out( Out2 );
			Common_->Frontend().IdsOut( Out3 );
			Common_->Frontend().StringsOut( Out4 );

			return Common_->Frontend().Handle();
		}
		fblovl::reply__ UpdateEntry( 
			const fblfrd::id__ &In1,
			const fblfrd::string_ &In2 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[2] );
			Common_->Frontend().IdIn( In1 );
			Common_->Frontend().StringIn( In2 );

			Common_->Frontend().EndOfInParameters();


			return Common_->Frontend().Handle();
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
		fblovl::reply__ Test( void ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );

			Common_->Frontend().EndOfInParameters();


			return Common_->Frontend().Handle();
		}
	};

}

#endif //#ifndef orgnzq__INC
