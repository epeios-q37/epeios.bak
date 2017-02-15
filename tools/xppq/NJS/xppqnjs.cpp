// hello.cc
#include <node.h>

#include "cio.h"
#include "scln.h"
#include "v8qnjs.h"
#include "xpp.h"
#include "mtk.h"

namespace stream_ {
	namespace process_ {
		struct rData {
			tht::rBlocker Blocker;
			fdr::rIDriver *IDriver;
			txf::sOFlow *OFlow;
			void reset( bso::sBool P = true )
			{
				tol::reset( P , Blocker, IDriver, OFlow );
			}
			qCDTOR( rData );
			void Init( void )
			{
				reset();
			}
		};

		void Routine( void *UP )
		{
		qRFH
			flw::sDressedIFlow<> IFlow;
			xtf::sIFlow XFlow;
			rData &Data = *(rData *)UP;
			fdr::rIDriver &IDriver = *Data.IDriver;
			txf::sOFlow &OFlow = *Data.OFlow;
		qRFB
			Data.Blocker.Unblock();

			IFlow.Init( IDriver );
			XFlow.Init( IFlow, utf::f_Guess );

			if ( xpp::Process( XFlow, xpp::criterions___( "" ), xml::oIndent, OFlow ) != xpp::sOK )
				qRGnr();

		qRFR
		qRFT
			XFlow.Dismiss();	// Avoid lock owner problem when destroying rack.
			OFlow.Commit();	
		qRFE( scln::ErrFinal() )
		}
	}

	namespace read_ {
		class rCache {
		private:
			tht::rLocker Locker_;
			fdr::sByte Buffer_[100];
			fdr::sSize Amount_;
			bso::sBool Drained_;
			fdr::sSize Size_( void )
			{
				return sizeof( Buffer_ );
			}
		public:
			void reset( bso::sBool P = true )
			{
				tol::reset( P, Locker_, Drained_ );

				Amount_ = 0;
			}
			qCDTOR( rCache );
			void Init( void )
			{
				Locker_.Init();
				Amount_ = 0;
				Drained_ = false;
			}
			fdr::sSize Read(
				fdr::sSize Amount,
				fdr::sByte *Buffer )
			{
			qRH
				tht::rLockerHandler Locker;
			qRB
				Locker.Init( Locker_ );

				if ( Amount_ != 0 ) {
					if ( Amount > Amount_ )
						Amount = Amount_;

					memcpy( Buffer, Buffer_, Amount );

					Amount_ -= Amount;

					if ( Amount_ != 0 )
						memcpy( Buffer_, Buffer_ + Amount, Amount_ );
				} else
					Amount = 0;
			qRR
			qRT
			qRE
				return Amount;
			}
			fdr::sSize Write(
				fdr::sByte *Buffer,
				fdr::sSize Amount )
			{
			qRH
				tht::rLockerHandler Locker;
			qRB
				Locker.Init( Locker_ );

				if ( Drained_ )
					qRGnr();

				if ( ( Amount == 0) || (Buffer == NULL) ) {
					Drained_ = true;
					Amount = 0;
				} else {
					if ( Amount > ( Size_() - Amount_ ) )
						Amount = Size_() - Amount_;

					if ( Amount != 0 ) {
						memcpy( Buffer_ + Amount_, Buffer, Amount );

						Amount_ += Amount;
					}
				}
			qRR
			qRT
			qRE
				return Amount;
			}
			qRODISCLOSEr( bso::sBool, Drained );
		};
		
		struct rData {
			tht::rBlocker Blocker;
			flw::sIFlow *Flow;
			rCache *Cache;
			void reset( bso::sBool P = true )
			{
				tol::reset( P , Blocker, Flow, Cache );
			}
			qCDTOR( rData );
			void Init( void )
			{
				reset();
			}
		};

		void Routine( void *UP )
		{
		qRFH
			fdr::sSize Amount = 0, Handled = 0;
			fdr::sByte Buffer[100];
			txf::rOFlow COut;
			rData &Data = *( rData *)UP;
			flw::sIFlow &Flow = *Data.Flow;
			rCache &Cache = *Data.Cache;
		qRFB
			COut.Init( cio::GetOutDriver() );

			Data.Blocker.Unblock();

			while ( !Flow.EndOfFlow() ) {
				Handled = 0;
				Amount = Flow.ReadUpTo( sizeof( Buffer ), Buffer );

				while ( Amount != Handled ) {
					Handled += Cache.Write( Buffer + Handled, Amount - Handled );
					tht::Defer();
				}
			}

		qRFR
		qRFT
			Flow.Dismiss();	// Avoid lock owner problem when destroying rack.
			Cache.Write( NULL, 0 );
		qRFE( scln::ErrFinal() )
		}
	}

	struct rASync_ {
	private:
		flx::rASync<> Core_;
	public:
		flx::rIASync In;
		flx::rOASync Out;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, Core_, In, Out );
		}
		qCDTOR( rASync_ );
		void Init( void )
		{
			Core_.Init();
			In.Init( Core_ );
			Out.Init( Core_ );
		}
	};

	class sRack_ {
	private:
		rASync_
			UpStream_,	// Before processing.
			DownStream_; // After processing.
		txf::rOFlow OFlow_;	// Downstream.
		flx::rIOMonitor Monitor_;
		txf::rOFlow FlowI_, FlowO_;
		flx::sMarkers Markers_;
	public:
		txf::rOFlow OFlow;	// Upstream.
		flw::sDressedIFlow<> IFlow;	// Downstream.
		read_::rCache Cache;
		void reset( bso::sBool P = true )
		{
			tol::reset( P, UpStream_, DownStream_, OFlow_, OFlow, IFlow, Cache );
		}
		qCDTOR( sRack_ );
		void Init( void )
		{
		qRH
			process_::rData Data;
		qRB
			UpStream_.Init();
			OFlow.Init( UpStream_.Out );

			DownStream_.Init();
			FlowI_.Init( cio::GetOutDriver() );
			FlowO_.Init( cio::GetOutDriver() );
			Monitor_.Init( DownStream_.In, DownStream_.Out, Markers_, FlowI_, FlowO_ );

			Markers_.Async = true;
			Markers_.In.Before = "\n >>>>>>>>>>>>>>>>>I\n";
			Markers_.In.After = "\n <<<<<<<<<<<<<<<<<I\n";
			Markers_.Out.Before = "\n >>>>>>>>>>>>>>>>>O\n";
			Markers_.Out.After = "\n <<<<<<<<<<<<<<<<<O\n";

	//		int i = 0; while ( i == 0 );
	#if 0
			IFlow.Init( Monitor_ );
			OFlow_.Init( Monitor_ );
	#else
			IFlow.Init( DownStream_.In );
			OFlow_.Init( DownStream_.Out );
	#endif

			Cache.Init();

			Data.Init();

			Data.Blocker.Init();
			Data.IDriver = &UpStream_.In;
			Data.OFlow = &OFlow_;

			mtk::Launch( process_::Routine, &Data );
			Data.Blocker.Wait();
		qRR
		qRT
		qRE
		}
	};

	bso::sBool Fill_(
		read_::rCache &Cache,
		v8qnjs::sRStream &Stream,
		v8::Local<v8::Value> RawFunction )
	{
	qRH
		fdr::sByte Buffer[100];
		fdr::sSize Amount = 0;
		v8qnjs::sBuffer Chunk;
		v8qnjs::sFunction Function;
	qRB
		Function.Init( RawFunction );

		while ( ( Amount = Cache.Read( sizeof( Buffer ) - 1, Buffer ) ) != 0 ) {
			Buffer[Amount] = 0;
			Chunk.Init( (const char *)Buffer, Function );
			Stream.Push( Chunk );
		}
	qRR
	qRT
	qRE
		return !Cache.Drained();
	}

	void OnData_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This, Dest;
		v8qnjs::sBuffer Chunk;
		fdr::sSize Amount = 0;
	qRFB
		This.Init(Infos.This() );

		Chunk.Init();
		v8q::Get( Infos, Chunk );

		Dest.Init( This.Get("_dest" ) );
		
		sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();

		Rack.OFlow << Chunk;

		Fill_( Rack.Cache, Dest, This.Get( "_func" ) );
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void OnEnd_( const v8q::sFunctionInfos &Infos )
	{
	qRFH
		v8qnjs::sRStream This, Dest;
	qRFB
		This.Init(Infos.This() );
		sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();

		Dest.Init( This.Get("_dest" ) );

		Rack.OFlow.Commit();

		while ( Fill_( Rack.Cache, Dest, This.Get( "_func" ) ) );

		delete v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void OnRead_( const v8q::sFunctionInfos &Infos )
	{
		static bso::sBool Handled = false;
	qRFH
		v8qnjs::sRStream This;
		read_::rData Data;
	qRFB
		if ( !Handled ) {
			This.Init( Infos.This() );
			sRack_ &Rack = *v8qnjs::sExternal<sRack_>( This.Get( "_rack" ) ).Value();

			Data.Init();
			Data.Blocker.Init();
			Data.Flow = &Rack.IFlow;
			Data.Cache = &Rack.Cache;

			mtk::Launch( read_::Routine, &Data );

			Data.Blocker.Wait();

			Handled = true;
		}
	qRFR
	qRFT
	qRFE( scln::ErrFinal() )
	}

	void Set( const v8q::sArguments &Arguments )
	{
	qRFH
		v8qnjs::sRStream Source, This;
		v8qnjs::sHelper Helper;
		sRack_ *Rack = NULL;
	qRFB
		Rack = new sRack_;

		if ( Rack == NULL )
			qRGnr();

		tol::Init( Source, This, Helper );
		Arguments.Get( Source, This, Helper );

		Rack->Init();

		Source.Set( "_rack", v8qnjs::sExternal<sRack_>( Rack ) );
		Source.Set( "_dest", This );
		Source.Set("_func", v8q::sFunction( Helper.Get("from" ) ).Core() ),
		This.Set( "_rack", v8qnjs::sExternal<sRack_>( Rack ) );

		Source.OnData( OnData_ );
		Source.OnEnd( OnEnd_ );
		This.OnRead( OnRead_ );

		Rack = NULL;
	qRFR
	qRFT
		if ( Rack != NULL )
			delete Rack;
	qRFE( scln::ErrFinal() )
	}
}

void scln::SCLNRegister( scln::sRegistrar &Registrar )
{
	cio::Initialize(cio::GetConsoleSet() );
	Registrar.Register( stream_::Set );
}

SCLN_MODULE( xppq );

