#ifndef __AUDIO__MIXER_THREAD
#define __AUDIO__MIXER_THREAD	1

#include <tools/thread/Thread.h>

using namespace tools::thread;

namespace audio
{
	class MixerThread : public Thread
	{
		protected:
			virtual int life();
		
		public:
			MixerThread();
			virtual ~MixerThread();
			void stop();
	};
}

#endif
