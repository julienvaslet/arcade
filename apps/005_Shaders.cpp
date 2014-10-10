#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <tools/logger/Stdout.h>

#include <opengl/Screen.h>
#include <opengl/OpenGL.h>
#include <opengl/Camera.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>
#include <opengl/Point3D.h>
#include <opengl/Color.h>
#include <opengl/Matrix.h>
#include <opengl/Program.h>

#include <GL/glu.h>

using namespace opengl;
using namespace std;
using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize( "005 - Shaders" ) )
	{
		Logger::get() << "Unable to initialize screen. Exiting.\n";
		return 1;
	}
	
	
	/* Should check if shader's extensions are available.
	if( !OpenGL::hasExtension( ) )
	{
		cout << "OpenGL .. extensions is not available. Exiting." << endl;
	}*/
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	bool useShaders = true;
	
	float yRotation = 0.0f;
	unsigned int lastRotateTicks = 0;
	
	bool cameraOrtho = false;
	bool cameraPerspective = true;
	bool cameraFrustum = false;
	
	// Shaders initialization
	Program * program = NULL;
	
	Program * program1 = new Program();
	program1->loadVertexShaderFile( "data/shaders/vertex.vs" );
	program1->loadFragmentShaderFile( "data/shaders/fragment.fs" );
	program1->link( true );
	program1->use( true );
	
	Program * program2 = new Program();
	program2->loadVertexShaderFile( "data/shaders/vertex_nocolor.vs" );
	program2->loadFragmentShaderFile( "data/shaders/fragment.fs" );
	program2->link( true );
	program2->use( true );

	program = program2;

	// Generating points
	vector<Point3D> m_points;
	m_points.push_back( Point3D( -0.5f, 0.0f, 0.5f ) );
	m_points.push_back( Point3D( 0.5f, 0.0f, 0.5f ) );
	m_points.push_back( Point3D( 0.5f, 1.0f, 0.5f ) );
	m_points.push_back( Point3D( -0.5f, 1.0f, 0.5f ) );
	m_points.push_back( Point3D( -0.5f, 1.0f, -0.5f ) );
	m_points.push_back( Point3D( -0.5f, 0.0f, -0.5f ) );
	m_points.push_back( Point3D( 0.5f, 0.0f, -0.5f ) );
	m_points.push_back( Point3D( 0.5f, 1.0f, -0.5f ) );
	
	// Generating indexes
	vector<unsigned int> m_indices;
	
	// Front face
	m_indices.push_back( 0 );
	m_indices.push_back( 1 );
	m_indices.push_back( 2 );
	m_indices.push_back( 0 );
	m_indices.push_back( 2 );
	m_indices.push_back( 3 );
	
	// Top face
	m_indices.push_back( 3 );
	m_indices.push_back( 2 );
	m_indices.push_back( 4 );
	m_indices.push_back( 2 );
	m_indices.push_back( 7 );
	m_indices.push_back( 4 );
	
	// Right face
	m_indices.push_back( 1 );
	m_indices.push_back( 6 );
	m_indices.push_back( 2 );
	m_indices.push_back( 6 );
	m_indices.push_back( 7 );
	m_indices.push_back( 2 );
	
	// Left face
	m_indices.push_back( 0 );
	m_indices.push_back( 3 );
	m_indices.push_back( 4 );
	m_indices.push_back( 4 );
	m_indices.push_back( 5 );
	m_indices.push_back( 0 );
	
	// Back face
	m_indices.push_back( 4 );
	m_indices.push_back( 6 );
	m_indices.push_back( 5 );
	m_indices.push_back( 4 );
	m_indices.push_back( 7 );
	m_indices.push_back( 6 );
	
	// Bottom face
	m_indices.push_back( 0 );
	m_indices.push_back( 5 );
	m_indices.push_back( 6 );
	m_indices.push_back( 6 );
	m_indices.push_back( 1 );
	m_indices.push_back( 0 );
	
	// Generating colors
	vector<Color> m_colors;
	m_colors.push_back( Color( "ff0000" ) );
	m_colors.push_back( Color( "00ff00" ) );
	m_colors.push_back( Color( "0000ff" ) );
	m_colors.push_back( Color( "ffff00" ) );
	m_colors.push_back( Color( "00ffff" ) );
	m_colors.push_back( Color( "ff00ff" ) );
	m_colors.push_back( Color( "f0f0f0" ) );
	m_colors.push_back( Color( "ffffff" ) );
	
	ArrayBufferObject * vbo = new ArrayBufferObject();
	vbo->setData( m_points );
	
	ArrayBufferObject * cbo = new ArrayBufferObject();
	cbo->setData( m_colors );
	
	ElementArrayBufferObject * ibo = new ElementArrayBufferObject();
	ibo->setData( m_indices );
	
	Camera camera;
	camera.getEye().moveTo( 0.0f, 1.25f, 4.0f );
	camera.getCenter().moveTo( 0.0f, 0.0f, 0.0f );
	
	glEnable( GL_DEPTH_TEST );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	
	cout << "Press [ENTER] to switch between shaders." << endl;
	cout << "Press [SPACE] to switch between rendering with or without shaders." << endl;
	cout << "Press [F1], [F2], [F3] to switch between, respectively, perspective view, orthogonal view and frustum view." << endl;
	cout << "Press arrow keys to move the box." << endl;

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
		            if( lastEvent.key.keysym.sym == SDLK_ESCAPE )
						running = false;
		                
		            else if( lastEvent.key.keysym.sym == SDLK_RETURN )
		            {
		            	if( program == program1 )
		            	{
		            		cout << "Using shader with white color." << endl;
		            		program = program2;
		            	}
		            	else
		            	{
		            		cout << "Using shader with dynamic color." << endl;
		            		program = program1;
		            	}
		            	
		            	program->use();
		            }
		                
		            else if( lastEvent.key.keysym.sym == SDLK_SPACE )
		            {
		            	useShaders = !useShaders;
		            	
		            	if( useShaders )
		            	{
		            		cout << "Using shaders." << endl;
		            		program->use();
		            	}
		            	else
		            	{
		            		cout << "Stop using shaders." << endl;
		            		glUseProgram( 0 );
		            	}
		            }
		            else if( lastEvent.key.keysym.sym == SDLK_F1 )
		            {
		            	cout << "Using perspective view" << endl;
						cameraOrtho = false;
						cameraPerspective = true;
						cameraFrustum = false;
		            }
		            else if( lastEvent.key.keysym.sym == SDLK_F2 )
		            {
		            	cout << "Using orthogonal view" << endl;
						cameraOrtho = true;
						cameraPerspective = false;
						cameraFrustum = false;
		            }
		            else if( lastEvent.key.keysym.sym == SDLK_F3 )
		            {
		            	cout << "Using frustum view" << endl;
						cameraOrtho = false;
						cameraPerspective = false;
						cameraFrustum = true;
		            }
		            else if( lastEvent.key.keysym.sym == SDLK_LEFT )
		            {
		            	camera.getCenter().moveBy( -0.1f, 0.0f, 0.0f );
		            	
		            	if( camera.getCenter().getX() < -5.0f )
		            		camera.getCenter().setX( -5.0f );
		            }
		            else if( lastEvent.key.keysym.sym == SDLK_RIGHT ) 
		            {
		            	camera.getCenter().moveBy( 0.1f, 0.0f, 0.0f );
		            	
		            	if( camera.getCenter().getX() > 5.0f )
		            		camera.getCenter().setX( 5.0f );
		            }
		            else if( lastEvent.key.keysym.sym == SDLK_DOWN ) 
		            {
		            	camera.getCenter().moveBy( 0.0f, -0.1f, 0.0f );
		            	
		            	if( camera.getCenter().getY() < -2.0f )
		            		camera.getCenter().setY( -2.0f );
		            }
		            else if( lastEvent.key.keysym.sym == SDLK_UP )
		            {
		            	camera.getCenter().moveBy( 0.0f, 0.1f, 0.0f );
		            	
		            	if( camera.getCenter().getY() > 5.0f )
		            		camera.getCenter().setY( 5.0f );
		            }

					break;
				}
			}
		}
		
		unsigned int ticks = SDL_GetTicks();
		
		if( ticks - lastRotateTicks > 20 )
		{
			yRotation = static_cast<float>( ( static_cast<int>( yRotation ) + 1 ) % 360 );
			lastRotateTicks = ticks;
		}
		
		if( ticks - lastDrawTicks > 15 )
		{
			Screen::get()->clear();
			
			if( cameraPerspective )
			{
				camera.setPerspective( 45.0f, 800.0f / 600.0f, 1.0f, 100.0f );
			}
			else if( cameraOrtho )
			{
				camera.setOrthogonal( -2.5f, 2.5f, -1.88f, 1.88f, 1.0f, 100.0f );
			}
			else if( cameraFrustum )
			{
				camera.setFrustum( -1.25f, 1.25f, -0.89f, 0.89f, 1.0f, 100.0f );
			}
			
			glMatrixMode( GL_MODELVIEW );
			camera.look();

			glMultMatrixf( Matrix::translation( camera.getCenter().getX(), camera.getCenter().getY(), camera.getCenter().getZ() ).get() );
			glMultMatrixf( Matrix::rotationY( yRotation ).get() );
			
			if( useShaders )
			{
				program->sendProjectionMatrix( "projection_matrix" );
				program->sendModelviewMatrix( "modelview_matrix" );
				
				program->sendVertexPointer( "a_Vertex", vbo );
				
				if( program == program1 )
					program->sendColorPointer( "a_Color", cbo );
				
				ibo->draw();
		
				glMultMatrixf( Matrix::translation( -1 * camera.getCenter().getX(), -1 * camera.getCenter().getY(), -1 * camera.getCenter().getZ() ).get() );
				glPushMatrix();
				glMultMatrixf( Matrix::translation( 2.0f, 0.0f, 1.0f ).get() );
				
				program->sendModelviewMatrix( "modelview_matrix" );
				
				ibo->draw( 0, 24 );
		
				glPopMatrix();
				glMultMatrixf( Matrix::translation( -2.0f, 0.0f, -1.0f ).get() );
				
				program->sendModelviewMatrix( "modelview_matrix" );
				
				ibo->draw( 0, 12 );
			}
			else
			{
				glColor3f( 1.0f, 1.0f, 1.0f );
				
				cbo->bindColorPointer();
				vbo->bindVertexPointer();
		
				// Render with Vertex Arrays
				glEnableClientState( GL_COLOR_ARRAY );
				glEnableClientState( GL_VERTEX_ARRAY );
		
				ibo->draw();
		
				glPushMatrix();
				glMultMatrixf( Matrix::translation( 2.0f, 0.0f, 1.0f ).get() );
				
				ibo->draw( 0, 24 );
		
				glPopMatrix();
				glMultMatrixf( Matrix::translation( -2.0f, 0.0f, -1.0f ).get() );
				
				ibo->draw( 0, 12 );
		
				glDisableClientState( GL_VERTEX_ARRAY );
				glDisableClientState( GL_COLOR_ARRAY );
			}
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}
	
	program = NULL;
	delete program1;
	delete program2;

	delete ibo;
	delete cbo;
	delete vbo;
	Screen::destroy();
	
	Logger::destroy();
	
	return 0;
}
