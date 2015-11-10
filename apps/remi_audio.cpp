#include <tools/logger/Stdout.h>

#include <opengl/Screen.h>
#include <audio/Mixer.h>
#include <audio/Song.h>
#include <audio/instrument/Sine.h>
#include <audio/instrument/SawTooth.h>
#include <audio/instrument/Square.h>
#include <audio/instrument/Triangle.h>
#include <audio/instrument/Silence.h>

#include <vector>

using namespace tools::logger;
using namespace audio;
using namespace audio::instrument;
using namespace std;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	// Not needed when Screen is used.
	#ifdef __NO_X_WINDOW__
	bcm_host_init();
	#else
	SDL_Init( SDL_INIT_AUDIO );
	#endif
	
	new Mixer( 44100, 1, 1024 );

	Song * song = new Song( 140, Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	Song * song2 = new Song( 140, Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	
	Sine sine( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	SawTooth sawtooth( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	Square square( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	Triangle triangle( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	Silence silence( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );

	song->mixNote( sine, Note::getFrequency( 'B', false, 2 ), Note::DoubleCroche, false );
	song->mixNote( silence, Note::getFrequency( 'B', false, 3 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'B', false, 2 ), Note::DoubleCroche, false );
	song->mixNote( silence, Note::getFrequency( 'B', false, 3 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'C', true, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'D', false, 3 ), Note::Croche, false );
	song->mixNote( silence, Note::getFrequency( 'B', false, 3 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'C', true, 4 ), Note::Noire, true );
	song->mixNote( sine, Note::getFrequency( 'C', true, 4 ), Note::Croche, false );
	song->mixNote( sine, Note::getFrequency( 'B', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'G', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'F', true, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'D', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'C', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'C', true, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'F', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'B', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'D', false, 4 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'B', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'G', true, 4 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'D', false, 4 ), Note::Noire, true );
	song->mixNote( silence, Note::getFrequency( 'B', false, 3 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'C', true, 4 ), Note::Croche, false );
	song->mixNote( silence, Note::getFrequency( 'B', false, 3 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'C', false, 4 ), Note::Croche, true );
	song->mixNote( sine, Note::getFrequency( 'F', false, 2 ), Note::Croche, true );
	song->mixNote( sine, Note::getFrequency( 'B', false, 3 ), Note::Noire, false );
	song->mixNote( silence, Note::getFrequency( 'B', false, 3 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'G', true, 3 ), Note::DoubleCroche, true );
	song->mixNote( silence, Note::getFrequency( 'B', false, 5 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'F', true, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'F', false, 3 ), Note::Noire, true );
	song->mixNote( sine, Note::getFrequency( 'D', false, 4 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'G', true, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'F', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'B', false, 2 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'F', false, 2 ), Note::Croche, true );
	song->mixNote( sine, Note::getFrequency( 'G', false, 2 ), Note::Croche, true );
	song->mixNote( sine, Note::getFrequency( 'F', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'B', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'F', false, 3 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'G', false, 2 ), Note::DoubleCroche, true );
	song->mixNote( sine, Note::getFrequency( 'C', true, 4 ), Note::Blanche, true );
	
	//----------------------
	
	/*song->mixNote( sawtooth, Note::getFrequency( 'A', false, 1 ), Note::Noire, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 2 ), Note::Croche, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 1 ), Note::Noire, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 2 ), Note::Croche, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 2 ), Note::Croche, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 2 ), Note::Croche, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 1 ), Note::Noire, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 2 ), Note::Croche, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 1 ), Note::Noire, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 2 ), Note::Croche, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 2 ), Note::Croche, false );
	song->mixNote( sawtooth, Note::getFrequency( 'A', false, 2 ), Note::Croche, false );
	
	
	song2->mixNote( sine, Note::getFrequency( 'A', false, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'B', false, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'C', true, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'D', false, 4 ), Note::Croche, false );	
	song2->mixNote( sine, Note::getFrequency( 'E', false, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'F', true, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'G', true, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'G', true, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'F', true, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'E', false, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'D', false, 4 ), Note::Croche, false );	
	song2->mixNote( sine, Note::getFrequency( 'C', true, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'B', false, 4 ), Note::Croche, false );
	song2->mixNote( sine, Note::getFrequency( 'A', false, 4 ), Note::Croche, false );

	song->rawMix( song2 );
	delete song2;*/
	
	//----------------------
	
	/*song->mixNote( sine, Note::getFrequency( 'E', false, 3 ), Note::Croche, false );
	song->mixNote( silence, Note::getFrequency( 'E', false, 3 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'E', false, 3 ), Note::Croche, false );
	song->mixNote( silence, Note::getFrequency( 'E', false, 3 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'E', false, 3 ), Note::Croche, false );
	
	song->mixNote( sine, Note::getFrequency( 'B', false, 3 ), Note::Croche, false );
	song->mixNote( sine, Note::getFrequency( 'A', false, 3 ), Note::Croche, false );
	song->mixNote( sine, Note::getFrequency( 'B', false, 3 ), Note::Croche, false );
	song->mixNote( silence, Note::getFrequency( 'E', false, 3 ), Note::Croche, false );
	
	song->mixNote( sine, Note::getFrequency( 'G', false, 3 ), Note::Croche, false );
	song->mixNote( silence, Note::getFrequency( 'E', false, 3 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'G', false, 3 ), Note::Croche, false );
	song->mixNote( silence, Note::getFrequency( 'E', false, 3 ), Note::DoubleCroche, false );
	song->mixNote( sine, Note::getFrequency( 'G', false, 3 ), Note::Croche, false );
	
	song->mixNote( sine, Note::getFrequency( 'A', false, 3 ), Note::Croche, false );
	song->mixNote( sine, Note::getFrequency( 'B', false, 3 ), Note::Croche, false );
	song->mixNote( sine, Note::getFrequency( 'A', false, 3 ), Note::Croche, false );
	song->mixNote( silence, Note::getFrequency( 'E', false, 3 ), Note::Croche, false );
	
	song2->mixNote( sawtooth, Note::getFrequency( 'E', false, 2 ), Note::Noire, true );
	song2->mixNote( silence, Note::getFrequency( 'E', false, 2 ), Note::Croche );
	song2->mixNote( sawtooth, Note::getFrequency( 'C', false, 2 ), Note::Noire, true );
	song2->mixNote( silence, Note::getFrequency( 'E', false, 2 ), Note::Croche );
	
	song2->mixNote( sawtooth, Note::getFrequency( 'E', false, 2 ), Note::Noire, true );
	song2->mixNote( silence, Note::getFrequency( 'E', false, 2 ), Note::Croche );
	song2->mixNote( sawtooth, Note::getFrequency( 'G', false, 2 ), Note::Noire, true );
	song2->mixNote( silence, Note::getFrequency( 'E', false, 2 ), Note::Croche );

	song->rawMix( song2 );
	delete song2;*/
	
	// ------------
	
	Mixer::get()->add( "song", song );
	delete song;
	
	Mixer::get()->setRepeat( "song", true, 2 );
	Mixer::get()->play( "song" );
	while( Mixer::get()->isPlaying() );
		
	Mixer::destroy();
	
	// Not needed when Screen is used.
	#ifdef __NO_X_WINDOW__
	
	#else
	SDL_Quit();
	#endif
	
	Logger::destroy();
	
	return 0;
}


