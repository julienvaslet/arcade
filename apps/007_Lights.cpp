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
#include <opengl/Point2D.h>
#include <opengl/Color.h>
#include <opengl/Matrix.h>
#include <opengl/Program.h>
#include <opengl/Texture2D.h>

#include <GL/glu.h>

using namespace opengl;
using namespace std;
using namespace tools::logger;

int main( int argc, char ** argv )
{
	// Initialize standard-output logger
	new Stdout( "stdout", true );
	
	if( !Screen::initialize( "007 - Lights" ) )
	{
		Logger::get() << "Unable to initialize screen. Exiting.\n";
		return 1;
	}
	
	bool running = true;
	SDL_Event lastEvent;
	unsigned int lastDrawTicks = 0;
	
	float yRotation = 0.0f;
	unsigned int lastRotateTicks = 0;
	
	bool cameraOrtho = false;
	bool cameraPerspective = true;
	bool cameraFrustum = false;
	
	// Shaders initialization
	Program * program = NULL;
	
	Program * program2 = new Program();
	program2->loadVertexShaderFile( "data/shaders/vertex_texture.vs" );
	program2->loadFragmentShaderFile( "data/shaders/fragment_texture.fs" );
	program2->link( true );
	program2->use( true );
	
	Program * program1 = new Program();
	program1->loadVertexShaderFile( "data/shaders/vertex_lights.vs" );
	program1->loadFragmentShaderFile( "data/shaders/fragment_lights.fs" );
	program1->link( true );
	program1->use( true );

	program = program1;
	
	// Texture initialization
	GLubyte textureData[16] = {
		255, 0, 0, 255,
		0, 255, 0, 255,
		0, 0, 255, 255,
		255, 255, 0, 255 };
		
	Texture2D * texture = new Texture2D();
	texture->bind();
	texture->setData( textureData, 2, 2, GL_RGBA );

	vector<Point2D> m_texcoords;
	m_texcoords.push_back( Point2D( 0.0f, 0.0f ) );
	m_texcoords.push_back( Point2D( 0.0f, 1.0f ) );
	m_texcoords.push_back( Point2D( 1.0f, 1.0f ) );
	m_texcoords.push_back( Point2D( 1.0f, 0.0f ) );
	m_texcoords.push_back( Point2D( 1.0f, 1.0f ) );
	m_texcoords.push_back( Point2D( 0.0f, 1.0f ) );
	m_texcoords.push_back( Point2D( 0.0f, 0.0f ) );
	m_texcoords.push_back( Point2D( 1.0f, 0.0f ) );

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
	
	ArrayBufferObject * tbo = new ArrayBufferObject();
	tbo->setData( m_texcoords );
	
	ElementArrayBufferObject * ibo = new ElementArrayBufferObject();
	ibo->setData( m_indices );
	
	Camera camera;
	camera.getEye().moveTo( 0.0f, 1.25f, 4.0f );
	camera.getCenter().moveTo( 0.0f, 0.0f, 0.0f );
	
	glEnable( GL_DEPTH_TEST );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
	cout << "Press [ENTER] to switch between shaders (texture or no texture)." << endl;
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
		            		cout << "Using shader without lights." << endl;
		            		program = program2;	
							glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		            	}
		            	else
		            	{
		            		cout << "Using shader with lights." << endl;
		            		program = program1;
							glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		            	}
		            	
		            	program->use();
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
			
			program->sendProjectionMatrix( "projection_matrix" );
			program->sendModelviewMatrix( "modelview_matrix" );
			
			program->sendVertexPointer( "a_Vertex", vbo );
			
			if( program == program1 )
			{
				program->sendColorPointer( "a_Color", cbo );
			}
			else
			{
				program->sendUniform( "texture0", *texture, 0 );
				program->sendTextureCoordinatesPointer( "a_TexCoord0", tbo );
			}
			
			ibo->draw();
	
			glMultMatrixf( Matrix::translation( -1 * camera.getCenter().getX(), -1 * camera.getCenter().getY(), -1 * camera.getCenter().getZ() ).get() );
			glPushMatrix();
			glMultMatrixf( Matrix::translation( 2.0f, 0.0f, 1.0f ).get() );
			
			program->sendModelviewMatrix( "modelview_matrix" );
			
			ibo->draw( 24 );
	
			glPopMatrix();
			glMultMatrixf( Matrix::translation( -2.0f, 0.0f, -1.0f ).get() );
			
			program->sendModelviewMatrix( "modelview_matrix" );
			
			ibo->draw( 12 );
			
			Screen::get()->render();
			
			lastDrawTicks = ticks;
		}
	}
	
	program = NULL;
	delete program1;
	delete program2;
	
	delete texture;

	delete ibo;
	delete cbo;
	delete vbo;
	delete tbo;
	Screen::destroy();
	
	Logger::destroy();
	
	return 0;
}
