/* DON'T MODIFY : computer-generated file ! */
/*
	API from : orgnzqbkd 20160128 Build Mar 20 2016 12:29:00 - Win32;MSC 1800;IA-32

	This file was generated using barq 20160313 (Build Mar 16 2016 15:57:53 Win32;MSC 1800;IA-32)
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
		fblfrd::command__ _Commands[3];
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
				21, 21, 0, 2, 
				0, 9, 22, 22, 
				0, 18, 22, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = FBLFRD_MASTER_OBJECT;
			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "OGZLogin";;
			CommandDetail.Casts.Append( Parameters + 0, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetNumbers";;
			CommandDetail.Casts.Append( Parameters + 4, 4 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "OGZGetTypes";;
			CommandDetail.Casts.Append( Parameters + 8, 3 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( FBLFRD_MASTER_TYPE, CommandsDetails, Commands );
			Commands.Recall( 0, 3, _Commands );
		}
		fblovl::reply__ OGZLogin( 
			const fblfrd::string_ &In1,
			const fblfrd::string_ &In2,
			fblfrd::boolean__ &Out1 ) const
		{
			Frontend().PushHeader( _ID, Commands()[0] );
			Frontend().StringIn( In1 );
			Frontend().StringIn( In2 );

			Frontend().EndOfInParameters();

			Frontend().BooleanOut( Out1 );

			return Frontend().Handle();
		}
		fblovl::reply__ OGZGetNumbers( 
			fblfrd::id8s_ &Out1,
			fblfrd::strings_ &Out2,
			fblfrd::strings_ &Out3 ) const
		{
			Frontend().PushHeader( _ID, Commands()[1] );

			Frontend().EndOfInParameters();

			Frontend().Id8sOut( Out1 );
			Frontend().StringsOut( Out2 );
			Frontend().StringsOut( Out3 );

			return Frontend().Handle();
		}
		fblovl::reply__ OGZGetTypes( 
			fblfrd::ids_ &Out1,
			fblfrd::strings_ &Out2 ) const
		{
			Frontend().PushHeader( _ID, Commands()[2] );

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

	class fOGZRecordCommon
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
		E_CVDTOR( fOGZRecordCommon )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8__ Parameters[] = {
				17, 0, 
				17, 8, 21, 21, 0, 17, 
				0, 18, 18, 9, 22, 22, 
				0, 18, 23, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = this->Frontend().GetType( str::string( "Record" ) );

			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "EditRecord";;
			CommandDetail.Casts.Append( Parameters + 0, 2 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "CreateField";;
			CommandDetail.Casts.Append( Parameters + 2, 6 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetFieldsColumns";;
			CommandDetail.Casts.Append( Parameters + 8, 6 );
			CommandsDetails.Append( CommandDetail );

			CommandDetail.Init();
			CommandDetail.Name = "GetFieldsData";;
			CommandDetail.Casts.Append( Parameters + 14, 3 );
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

	class rOGZRecord
	{
	private:
		fblfrd::object__ _ID;
		fOGZRecordCommon *Common_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P == true )
				if ( ( Common_ != NULL ) && ( _ID != FBLFRD_UNDEFINED_OBJECT ) )
					Common_->RemoveObject( _ID );

			Common_ = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		rOGZRecord( void )
		{
			reset( false );
		}
		~rOGZRecord( void )
		{
			reset();
		}
		fblfrd::object__ ID( void ) const
		{
			return _ID;
		}
		void Init( fOGZRecordCommon &Common )
		{
			Common_ = &Common;

			_ID = Common_->GetNewObject();
		}
		fblovl::reply__ EditRecord( 
			const fblfrd::id__ &In1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );
			Common_->Frontend().IdIn( In1 );

			Common_->Frontend().EndOfInParameters();


			return Common_->Frontend().Handle();
		}
		fblovl::reply__ CreateField( 
			const fblfrd::id__ &In1,
			const fblfrd::id8__ &In2,
			const fblfrd::string_ &In3,
			const fblfrd::string_ &In4,
			fblfrd::id__ &Out1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[1] );
			Common_->Frontend().IdIn( In1 );
			Common_->Frontend().Id8In( In2 );
			Common_->Frontend().StringIn( In3 );
			Common_->Frontend().StringIn( In4 );

			Common_->Frontend().EndOfInParameters();

			Common_->Frontend().IdOut( Out1 );

			return Common_->Frontend().Handle();
		}
		fblovl::reply__ GetFieldsColumns( 
			fblfrd::ids_ &Out1,
			fblfrd::ids_ &Out2,
			fblfrd::id8s_ &Out3,
			fblfrd::strings_ &Out4,
			fblfrd::strings_ &Out5 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[2] );

			Common_->Frontend().EndOfInParameters();

			Common_->Frontend().IdsOut( Out1 );
			Common_->Frontend().IdsOut( Out2 );
			Common_->Frontend().Id8sOut( Out3 );
			Common_->Frontend().StringsOut( Out4 );
			Common_->Frontend().StringsOut( Out5 );

			return Common_->Frontend().Handle();
		}
		fblovl::reply__ GetFieldsData( 
			fblfrd::ids_ &Out1,
			fblfrd::xstrings_ &Out2 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[3] );

			Common_->Frontend().EndOfInParameters();

			Common_->Frontend().IdsOut( Out1 );
			Common_->Frontend().XStringsOut( Out2 );

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
				if ( ( Common_ != NULL ) && ( _ID != FBLFRD_UNDEFINED_OBJECT ) )
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
