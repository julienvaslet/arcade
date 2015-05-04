#include <tools/logger/Stdout.h>

#include <audio/Mixer.h>
#include <audio/Song.h>
#include <audio/instrument/Sine.h>

#include <vector>

using namespace tools::logger;
using namespace audio;
using namespace audio::instrument;
using namespace std;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	/*SDL_Init( SDL_INIT_AUDIO );
	
	new Mixer( 44100, 1, 1024 );

	Song * song = new Song( 90, Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	Sine sine( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	
	song->mixNote( sine, Note::getFrequency( 'A', false, 3 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'D', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'E', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'F', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'G', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'A', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'A', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'A', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'A', true, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'A', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'G', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'G', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'G', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'G', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'A', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'G', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'F', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'F', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'F', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'F', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'E', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'F', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'G', false, 4 ), Note::Croche );
	song->mixNote( sine, Note::getFrequency( 'E', false, 4 ), Note::Noire );
	
	//song->mixNote( sine, Note::getFrequency( 'A', false, 3 ), Note::Noire );
	//song->mixNote( sine, Note::getFrequency( 'B', false, 3 ), Note::Croche );
	//song->mixNote( sine, Note::getFrequency( 'C', false, 4 ), Note::Croche, true );
	//song->mixNote( sine, Note::getFrequency( 'E', false, 4 ), Note::Croche, true );
	//song->mixNote( sine, Note::getFrequency( 'D', true, 4 ), Note::Noire );
	//song->mixNote( sine, Note::getFrequency( 'F', false, 4 ), Note::Croche );
	//song->mixNote( sine, Note::getFrequency( 'E', false, 4 ), Note::Croche, true );
	//song->mixNote( sine, Note::getFrequency( 'C', false, 4 ), Note::Croche, true );
	
	Mixer::get()->add( "song", song );
	delete song;
	
	Mixer::get()->setRepeat( "song", true, 5 );
	
	//for( float pitch = 1.00f ; pitch <= 1.20f ; pitch += 0.05f )
	for( float pitch = 1.00f ; pitch <= 1.00f ; pitch += 0.05f )
	{
		Mixer::get()->setPitch( "song", pitch );
		Mixer::get()->play( "song" );
		while( Mixer::get()->isPlaying() );
	}
		
	Mixer::destroy();
	
	SDL_Quit();*/
	
	Logger::destroy();
	
	return 0;
}

