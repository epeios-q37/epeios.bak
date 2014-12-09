/* DON'T MODIFY : computer-generated file ! */
/*
	API from : esketchbkd (Version) Build Dec  7 2014 08:46:16 - MSVC_IA-32

	This file was generated using eGetAPI 20140119 (Build Nov 19 2014 07:28:00 MSVC_IA-32)
*/

#ifndef ESKETCH__INC
# define ESKETCH__INC

# define ESKETCH_TARGET "esketch"
# define ESKETCH_API_VERSION "1"

# include "fblfrd.h"
# include "cpe.h"

namespace esketch {

	namespace {
		typedef fblfrd::frontend_depot__ _frontend_depot__;
	}

	class statics___
	: public _frontend_depot__
	{
	private:
		fblfrd::object__ _ID;
		fblfrd::command__ _Commands[1];
	public:
		void reset( bso::bool__ P = true )
		{
			_frontend_depot__::reset( P );
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		E_CVDTOR( statics___ )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8__ Parameters[] = {
				0, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = FBLFRD_MASTER_OBJECT;
			CommandsDetails.Init();

			CommandDetail.Init();
			CommandDetail.Name = "SKTTest";;
			CommandDetail.Casts.Append( Parameters + 0, 1 );
			CommandsDetails.Append( CommandDetail );


			Commands.Init();
			this->Frontend().GetCommands( FBLFRD_MASTER_TYPE, CommandsDetails, Commands );
			Commands.Recall( 0, 1, _Commands );
		}
		fblovl::reply__ SKTTest( void ) const
		{
			Frontend().PushHeader( _ID, Commands()[0] );

			Frontend().EndOfInParameters();


			return Frontend().Handle();
		}
		const fblfrd::command__ *Commands( void ) const
		{
			return _Commands;
		}
	};

	class skt_myobject_common__
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
		E_CVDTOR( skt_myobject_common__ )
		void Init( fblfrd::frontend___ &Frontend )
		{
			fblfrd::commands_details CommandsDetails;
			fblfrd::command_detail CommandDetail;
			fblfrd::id16s Commands;

			fblfrd::id8__ Parameters[] = {
				21, 0, 21, 
				0, 
			};

			_frontend_depot__::Init( Frontend );

			_ID = this->Frontend().GetType( str::string( "myobject" ) );

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

	class skt_myobject___
	{
	private:
		fblfrd::object__ _ID;
		skt_myobject_common__ *Common_;
	public:
		void reset( bso::bool__ P = true )
		{
			if ( P == true )
				if ( ( Common_ != NULL ) && ( _ID != FBLFRD_UNDEFINED_OBJECT ) )
					Common_->RemoveObject( _ID );

			Common_ = NULL;
			_ID = FBLFRD_UNDEFINED_OBJECT;
		}
		skt_myobject___( void )
		{
			reset( false );
		}
		~skt_myobject___( void )
		{
			reset();
		}
		fblfrd::object__ ID( void ) const
		{
			return _ID;
		}
		void Init( skt_myobject_common__ &Common )
		{
			Common_ = &Common;

			_ID = Common_->GetNewObject();
		}
		fblovl::reply__ ToUC( 
			const fblfrd::string_ &In1,
			fblfrd::string_ &Out1 ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[0] );
			Common_->Frontend().StringIn( In1 );

			Common_->Frontend().EndOfInParameters();

			Common_->Frontend().StringOut( Out1 );

			return Common_->Frontend().Handle();
		}
		fblovl::reply__ Test( void ) const
		{
			Common_->Frontend().PushHeader( _ID, Common_->Commands()[1] );

			Common_->Frontend().EndOfInParameters();


			return Common_->Frontend().Handle();
		}
	};

}

#endif //#ifndef esketch__INC
