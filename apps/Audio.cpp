#include <tools/logger/Stdout.h>
#include <audio/Mixer.h>

#include <vector>
#include <cmath>
#include <climits>

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
	
	Sound * note1 = new Sound( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	fillSound( note1, 262, 2 );
	
	Sound * note2 = new Sound( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	fillSound( note2, 330, 1 );
	
	note1->mix( note2, 400, 1.0f );
	Mixer::get()->add( "note1", note1 );
	
	delete note1;
	delete note2;

	
	Mixer::get()->play( "note1" );
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

