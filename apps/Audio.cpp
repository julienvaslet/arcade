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
	
	SDL_Init( SDL_INIT_AUDIO );
	
	new Mixer( 44100, 1, 512 );
	
	Song * song = new Song( 160, Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	Sine sine( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	
	song->mixNote( sine, Note::getFrequency( 'C', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'C', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'C', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'D', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'E', false, 4 ), Note::Blanche );
	song->mixNote( sine, Note::getFrequency( 'D', false, 4 ), Note::Blanche );
	song->mixNote( sine, Note::getFrequency( 'C', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'E', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'D', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'D', false, 4 ), Note::Noire );
	song->mixNote( sine, Note::getFrequency( 'C', false, 4 ), Note::Blanche );
	
	Mixer::get()->add( "song", song );
	delete song;
	
	Mixer::get()->play( "song" );
	while( Mixer::get()->isPlaying() );
	
	Mixer::destroy();
	
	SDL_Quit();
	
	Logger::destroy();
	
	return 0;
}

