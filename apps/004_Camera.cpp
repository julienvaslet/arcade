#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#define GL_GLEXT_PROTOTYPES
#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/Point.h>
#include <opengl/Color.h>
#include <opengl/Matrix.h>

#include <GL/glu.h>

using namespace opengl;
using namespace std;

int main( int argc, char ** argv )
{
	if( !Screen::initialize( "004 - Camera" ) )
	{
		cout << "Unable to initialize screen. Exiting." << endl;
		return 1;
	}
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	bool customMatrix = false;

	// Generating points
	vector<Point> m_points;
	m_points.push_back( Point( -0.5f, -0.5f, -2.0f ) );
	m_points.push_back( Point( 0.5f, -0.5f, -2.0f ) );
	m_points.push_back( Point( 0.0f, 0.5f, -2.0f ) );
	
	// Generating colors
	vector<Color> m_colors;
	m_colors.push_back( Color( "ff0000" ) );
	m_colors.push_back( Color( "00ff00" ) );
	m_colors.push_back( Color( "0000ff" ) );
	
	ArrayBufferObject * vbo = new ArrayBufferObject();
	vbo->setData( m_points );
	
	ArrayBufferObject * cbo = new ArrayBufferObject();
	cbo->setData( m_colors );
	
	Matrix projectionMatrix = Matrix::perspective( 45.0f, 800.0f / 600.0f, 1.0f, 100.0f );

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
				
				case SDL_KEYDOWN:
				{
		            if (lastEvent.key.keysym.sym == SDLK_ESCAPE)
		                running = false;
		                
		            else if( lastEvent.key.keysym.sym == SDLK_SPACE )
		            {
		            	customMatrix = !customMatrix;
		            	
		            	if( customMatrix )
		            		cout << "Using custom matrices." << endl;
		            	else
		            		cout << "Using OpenGL matrices." << endl;
		            }

					break;
				}
			}
		}
		
		unsigned int ticks = SDL_GetTicks();
		
		if( ticks - lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			glMatrixMode( GL_PROJECTION );
			
			if( customMatrix )
			{
				glLoadMatrixf( projectionMatrix.get() );
			}
			else
			{
				glLoadIdentity();
				gluPerspective( 45.0f, 800.0f / 600.0f, 1.0f, 100.0f );
			}
			
			glMatrixMode( GL_MODELVIEW );
			
			glBindBuffer( GL_ARRAY_BUFFER, cbo->getBuffer() );
			glColorPointer( 4, GL_FLOAT, 0, 0 );
			
			glBindBuffer( GL_ARRAY_BUFFER, vbo->getBuffer() );
			glVertexPointer( 3, GL_FLOAT, 0, 0 );
			
			// Render with Vertex Arrays
			glEnableClientState( GL_VERTEX_ARRAY );
			glEnableClientState( GL_COLOR_ARRAY );
			glDrawArrays( GL_TRIANGLES, 0, 3 );
			glDisableClientState( GL_COLOR_ARRAY );
			glDisableClientState( GL_VERTEX_ARRAY );
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}

	delete cbo;
	delete vbo;
	Screen::destroy();
	
	return 0;
}
