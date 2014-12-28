#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <opengl/ArrayBufferObject.h>
#include <tools/logger/Stdout.h>

#include <GL/glu.h>

using namespace opengl;
using namespace std;
using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize() )
	{
		cout << "Unable to initialize screen. Exiting." << endl;
		return 1;
	}
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	
	cout << "Validating buffer usages..." << endl;
	cout << "GL_STREAM_DRAW : " << ( BufferObject::getUsage( BufferObject::Stream, BufferObject::Draw ) == GL_STREAM_DRAW ? "OK" : "KO" ) << endl;
	cout << "GL_STREAM_READ : " << ( BufferObject::getUsage( BufferObject::Stream, BufferObject::Read ) == GL_STREAM_READ ? "OK" : "KO" ) << endl;
	cout << "GL_STREAM_COPY : " << ( BufferObject::getUsage( BufferObject::Stream, BufferObject::Copy ) == GL_STREAM_COPY ? "OK" : "KO" ) << endl;
	cout << "GL_STATIC_DRAW : " << ( BufferObject::getUsage( BufferObject::Static, BufferObject::Draw ) == GL_STATIC_DRAW ? "OK" : "KO" ) << endl;
	cout << "GL_STATIC_READ : " << ( BufferObject::getUsage( BufferObject::Static, BufferObject::Read ) == GL_STATIC_READ ? "OK" : "KO" ) << endl;
	cout << "GL_STATIC_COPY : " << ( BufferObject::getUsage( BufferObject::Static, BufferObject::Copy ) == GL_STATIC_COPY ? "OK" : "KO" ) << endl;
	cout << "GL_DYNAMIC_DRAW : " << ( BufferObject::getUsage( BufferObject::Dynamic, BufferObject::Draw ) == GL_DYNAMIC_DRAW ? "OK" : "KO" ) << endl;
	cout << "GL_DYNAMIC_READ : " << ( BufferObject::getUsage( BufferObject::Dynamic, BufferObject::Read ) == GL_DYNAMIC_READ ? "OK" : "KO" ) << endl;
	cout << "GL_DYNAMIC_COPY : " << ( BufferObject::getUsage( BufferObject::Dynamic, BufferObject::Copy ) == GL_DYNAMIC_COPY ? "OK" : "KO" ) << endl;
	
	// Generating points
	vector<float> m_vertices;
	m_vertices.push_back( -0.5f );
	m_vertices.push_back( -0.5f );
	m_vertices.push_back( -2.0f );
	m_vertices.push_back( 0.5f );
	m_vertices.push_back( -0.5f );
	m_vertices.push_back( -2.0f );
	m_vertices.push_back( 0.0f );
	m_vertices.push_back( 0.5f );
	m_vertices.push_back( -2.0f );
	
	// Generating colors
	vector<float> m_colors;
	m_colors.push_back( 1.0f );
	m_colors.push_back( 0.0f );
	m_colors.push_back( 0.0f );
	m_colors.push_back( 0.0f );
	m_colors.push_back( 1.0f );
	m_colors.push_back( 0.0f );
	m_colors.push_back( 0.0f );
	m_colors.push_back( 0.0f );
	m_colors.push_back( 1.0f );
	
	ArrayBufferObject * vbo = new ArrayBufferObject();
	vbo->setData( m_vertices );
	
	ArrayBufferObject * cbo = new ArrayBufferObject();
	cbo->setData( m_colors );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	gluPerspective( 45.0f, 800.0f / 600.0f, 1.0f, 100.0f );
	glMatrixMode( GL_MODELVIEW );

	while( running )
	{
		while( SDL_PollEvent( &lastEvent ) )
		{
			switch( lastEvent.type )
			{
				case SDL_QUIT:
				{
					running = false;
					break;
				}
			}
		}
		
		unsigned int ticks = SDL_GetTicks();
		
		if( ticks - lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			glBindBuffer( GL_ARRAY_BUFFER, cbo->getBuffer() );
			glColorPointer( 3, GL_FLOAT, 0, 0 );
			
			glBindBuffer( GL_ARRAY_BUFFER, vbo->getBuffer() );
			glVertexPointer( 3, GL_FLOAT, 0, 0 );
			
			// Render with Vertex Arrays
			glEnableClientState( GL_VERTEX_ARRAY );
			glEnableClientState( GL_COLOR_ARRAY );
			glDrawArrays( GL_TRIANGLES, 0, 3 );
			glDisableClientState( GL_COLOR_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );
			
			// OpenGL direct
			/*glBegin( GL_TRIANGLES );
			glColor3f( 1.0f, 1.0f, 1.0f );
			glVertex3f( -0.5f, -0.5f, -2.0f );
			glVertex3f( 0.5f, -0.5f, -2.0f );
			glVertex3f( 0.0f, 0.5f, -2.0f );
			glEnd();*/
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}

	delete cbo;
	delete vbo;
	Screen::destroy();
	
	Logger::destroy();
	
	return 0;
}
