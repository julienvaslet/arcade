#include <tools/logger/Stdout.h>

#include <audio/Mixer.h>
#include <audio/instrument/Sine.h>

#include <vector>

using namespace tools::logger;
using namespace audio;
using namespace std;

void fillSound( Sound * sound, unsigned int frequency, unsigned int duration );

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	SDL_Init( SDL_INIT_AUDIO );
	
	new Mixer( 44100, 1, 512 );
	
	vector<float> notes;
	notes.push_back( NOTE_C4 );
	notes.push_back( NOTE_C4 );
	notes.push_back( NOTE_C4 );
	notes.push_back( NOTE_D4 );
	notes.push_back( NOTE_E4 );
	notes.push_back( NOTE_D4 );
	notes.push_back( NOTE_C4 );
	notes.push_back( NOTE_E4 );
	notes.push_back( NOTE_D4 );
	notes.push_back( NOTE_D4 );
	notes.push_back( NOTE_C4 );
	
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

void fillSound( Sound * sound, unsigned int frequency, unsigned int duration )
{
	vector<int> noteData;
	double period = static_cast<double>( sound->getFrequency() ) / static_cast<double>( frequency );
	
	unsigned int i = 0;

	for( i = 0 ; i < sound->getFrequency() * duration ; i++ )
	{
		unsigned iDegrees = 360.0 * ((i % static_cast<int>(period)) / period);
		int value = sin( ((iDegrees % 360) / 360.0) * 2.0 * M_PI ) * INT_MAX * 0.9;
		
		for( unsigned short int c = 0 ; c < sound->getChannels() ; c++ )
			noteData.push_back( value );
	}
	
	sound->setData( noteData );
	
	Logger::get() << "[Audio] Sound is initialized (" << frequency << " Hz, " << sound->getDuration() << "s)." << Logger::endl;
}

