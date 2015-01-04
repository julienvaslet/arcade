#include <SDL2/SDL.h>

#include <opengl/Screen.h>
#include <tools/logger/Stdout.h>
#include <opengl/Camera.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>
#include <opengl/Point3D.h>
#include <opengl/Color.h>
#include <opengl/Matrix.h>
#include <opengl/Program.h>

#include <controller/Controller.h>

using namespace opengl;
using namespace tools::logger;
using namespace controller;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	bool running = true;
	
	if( !Screen::initialize() )
	{
		cout << "Unable to initialize screen. Exiting." << endl;
		return 1;
	}
	
	Program * program = new Program();
	program->loadVertexShaderFile( "data/shaders/vertex.es.vs" );
	program->loadFragmentShaderFile( "data/shaders/fragment.es.fs" );
	program->link( true );
	program->use( true );
	
	vector<Point3D> m_points;
	m_points.push_back( Point3D( -1.0f, 0.0f, 0.0f ) );
	m_points.push_back( Point3D( 1.0f, 0.0f, 0.0f ) );
	m_points.push_back( Point3D( 0.0f, 1.0f, 0.0f ) );
	
	vector<unsigned short int> m_indices;
	m_indices.push_back( 0 );
	m_indices.push_back( 1 );
	m_indices.push_back( 2 );
	
	vector<Color> m_colors;
	m_colors.push_back( Color( "ff0000" ) );
	m_colors.push_back( Color( "ff0000" ) );
	m_colors.push_back( Color( "ff0000" ) );
	
	ArrayBufferObject * vbo = new ArrayBufferObject();
	vbo->setData( m_points );

	ArrayBufferObject * cbo = new ArrayBufferObject();
	cbo->setData( m_colors );

	ElementArrayBufferObject * ibo = new ElementArrayBufferObject();
	ibo->setData( m_indices );
	
	Camera camera;
	camera.getEye().moveTo( 0.0f, 0.0f, 4.0f );
	camera.getCenter().moveTo( 0.0f, 0.0f, 0.0f );
	
	// Load joysticks
	Controller::open( "Joystick1" );
	Controller::open( "Joystick2" );
	
	if( Controller::getControllersCount() == 0 )
		Controller::scan();
	
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	
	Matrix projection = Matrix::perspective( 45.0f, 800.0f / 600.0f, 1.0f, 100.0f );
	Matrix modelview = Matrix::lookAt( camera.getEye().getX(), camera.getEye().getY(), camera.getEye().getZ(), camera.getCenter().getX(), camera.getCenter().getY(), camera.getCenter().getZ(), camera.getUp().getX(), camera.getUp().getY(), camera.getUp().getZ() );
	Matrix translation = Matrix::translation( camera.getCenter().getX(), camera.getCenter().getY(), camera.getCenter().getZ() );
	modelview.multiply( translation );
	
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
				
				case SDL_JOYBUTTONDOWN:
				{
					if( lastEvent.jbutton.state == SDL_PRESSED )
					{
						m_colors.clear();
				
						switch( lastEvent.jbutton.button )
						{
							case 2:
							{
								for( int i = 0 ; i < 3 ; i++ )
									m_colors.push_back( Color( "ffff00" ) );

								break;
							}
							
							case 1:
							{
								for( int i = 0 ; i < 3 ; i++ )
									m_colors.push_back( Color( "888888" ) );

								break;
							}
							
							case 0:
							{
								for( int i = 0 ; i < 3 ; i++ )
									m_colors.push_back( Color( "0000ff" ) );

								break;
							}
							
							default:
							case 3:
							{
								for( int i = 0 ; i < 3 ; i++ )
									m_colors.push_back( Color( "ff0000" ) );

								break;
							}
						}
						
						cbo->setData( m_colors );
					}
					
					//break;
				}
				
				case SDL_JOYBUTTONUP:
				case SDL_JOYAXISMOTION:
				case SDL_JOYDEVICEREMOVED:
				{
					Controller::handleEvent( &lastEvent );
					
					break;
				}
				
				case SDL_JOYDEVICEADDED:
				{
					Controller::scan();
					break;
				}
				
				case SDL_KEYDOWN:
				{
		            if( lastEvent.key.keysym.sym == SDLK_ESCAPE )
						running = false;
					else if( lastEvent.key.keysym.sym == SDLK_SPACE )
					{
						m_colors.clear();
						
						for( int i = 0 ; i < 3 ; i++ )
							m_colors.push_back( Color( "ffff00" ) );
							
						cbo->setData( m_colors );
					}
						
					break;
				}
			}
		}

		unsigned int ticks = SDL_GetTicks();
		
		if( ticks - lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			program->sendUniform( "projection_matrix", projection, false );
			program->sendUniform( "modelview_matrix", modelview, false );
			program->sendVertexPointer( "a_Vertex", vbo );
			program->sendColorPointer( "a_Color", cbo );
	
			ibo->draw();
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}
	
	glUseProgram( 0 );
	delete program;
	delete vbo;
	delete cbo;
	delete ibo;

	Controller::destroy();
	Screen::destroy();
	Logger::destroy();
	
	return 0;
}
