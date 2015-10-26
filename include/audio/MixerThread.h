#ifndef __AUDIO__MIXER_THREAD
#define __AUDIO__MIXER_THREAD	1

#include <tools/thread/Thread.h>

namespace audio
{
	class MixerThread;
}

#include <audio/Mixer.h>

using namespace tools::thread;

namespace audio
{
	class MixerThread : public Thread
	{
		protected:
			Mixer * mixer;
		
			virtual int life();
		
		public:
			MixerThread( Mixer * mixer );
			virtual ~MixerThread();
			void stop();
	};
}

#endif
