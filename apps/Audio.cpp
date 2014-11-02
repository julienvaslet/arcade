#include <tools/logger/Stdout.h>
#include <audio/Mixer.h>

#include <vector>
#include <cmath>

using namespace tools::logger;
using namespace audio;
using namespace std;

void fillSound( Sound * sound, unsigned int frequency, unsigned int duration );

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	new Mixer( 44100, 1, 512 );
	
	Sound * note = new Sound( Mixer::get()->getSamplingFrequency(), Mixer::get()->getChannels() );
	fillSound( note, 220, 2 );
	
	Mixer::get()->add( "note", note );
	delete note;
	
	Mixer::get()->play( "note" );
	while( Mixer::get()->isPlaying() );
	
	Mixer::destroy();
	Logger::destroy();
	
	return 0;
}

void fillSound( Sound * sound, unsigned int frequency, unsigned int duration )
{
	vector<int> noteData;
	double period = static_cast<double>( sound->getFrequency() ) / static_cast<double>( frequency );

	for( unsigned int i = 0 ; i < sound->getFrequency() * duration ; i++ )
	{
		unsigned iDegrees = 360.0 * ((i % static_cast<int>(period)) / period);
		int value = static_cast<int>( sin( ((iDegrees % 360) / 360.0) * 2.0 * M_PI ) * 127.0 );
		
		for( unsigned short int c = 0 ; c < sound->getChannels() ; c++ )
			noteData.push_back( value );
	}
	
	sound->setData( noteData );
	
	Logger::get() << "[Audio] Sound is initialized (" << frequency << " Hz, " << sound->getDuration() << "s)." << Logger::endl;
}

