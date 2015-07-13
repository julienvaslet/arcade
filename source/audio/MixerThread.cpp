#include <audio/MixerThread.h>
#include <unistd.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace audio
{
	MixerThread::MixerThread() : Thread()
	{
	}
	
	MixerThread::~MixerThread()
	{
	}
	
	int MixerThread::life()
	{
		while( this->running )
		{
			Logger::get() << "Hello from MixerThread." << Logger::endl;
			usleep( 500000 );
		}
		
		return 0;
	}
	
	void MixerThread::stop()
	{
		this->running = false;
	}
}

