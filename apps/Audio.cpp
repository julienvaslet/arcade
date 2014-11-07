#include <tools/logger/Stdout.h>

#include <audio/Mixer.h>
#include <audio/instrument/Sine.h>

#include <vector>

using namespace tools::logger;
using namespace audio;
using namespace std;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	SDL_Init( SDL_INIT_AUDIO );
	
	new Mixer( 44100, 1, 512 );
	
	vector<float> notes;
	notes.push_back( instrument::Note::getFrequency( 'C', false, 4 ) );
	notes.push_back( instrument::Note::getFrequency( 'C', false, 4 ) );
	notes.push_back( instrument::Note::getFrequency( 'C', false, 4 ) );
	notes.push_back( instrument::Note::getFrequency( 'D', false, 4 ) );
	notes.push_back( instrument::Note::getFrequency( 'E', false, 4 ) );
	notes.push_back( instrument::Note::getFrequency( 'D', false, 4 ) );
	notes.push_back( instrument::Note::getFrequency( 'C', false, 4 ) );
	notes.push_back( instrument::Note::getFrequency( 'E', false, 4 ) );
	notes.push_back( instrument::Note::getFrequency( 'D', false, 4 ) );
	notes.push_back( instrument::Note::getFrequency( 'D', false, 4 ) );
	notes.push_back( instrument::Note::getFrequency( 'C', false, 4 ) );
	
	Sound * song = new Sound( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	
	instrument::Sine * sine = new instrument::Sine( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	unsigned int time = 0;
	unsigned int noteDuration = 500;
	
	for( vector<float>::iterator it = notes.begin() ; it != notes.end() ; it++ )
	{
		Sound * note = sine->generate( *it, noteDuration, 0.9f );
		song->mix( note, time, 1.0f );
		time += noteDuration;
		delete note;
	}
	
	delete sine;
	
	Mixer::get()->add( "song", song );
	delete song;
	
	Mixer::get()->play( "song" );
	while( Mixer::get()->isPlaying() );
	
	Mixer::destroy();
	
	SDL_Quit();
	
	Logger::destroy();
	
	return 0;
}

