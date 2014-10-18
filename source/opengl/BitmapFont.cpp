#include <opengl/BitmapFont.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>
#include <SDL2/SDL_image.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	Program * BitmapFont::program = NULL;
	unsigned int BitmapFont::instances = 0;
	
	BitmapFont::BitmapFont( const string& filename, unsigned int characterWidth, unsigned int characterHeight ) : Font(filename), texture(NULL), characterWidth(characterWidth), characterHeight(characterHeight), textureWidth(0), textureHeight(0)
	{
		BitmapFont::instances++;
		
		if( BitmapFont::program == NULL )
		{
			BitmapFont::program = new Program();
			BitmapFont::program->loadVertexShaderFile( "data/shaders/BitmapFont.vs" );
			BitmapFont::program->loadFragmentShaderFile( "data/shaders/BitmapFont.fs" );
			BitmapFont::program->link( true );
		}
		
		this->texture = new Texture2D();
		this->texture->bind();
		
		#ifdef DEBUG0
		Logger::get() << "[BitmapFont] Loading file \"" << filename << "\"..." << Logger::endl;
		#endif
		
		SDL_Surface * surface = IMG_Load( filename.c_str() );
	
		if( surface != NULL )
		{
			this->textureWidth = surface->w;
			this->textureHeight = surface->h;
			
			this->texture->setData( static_cast<unsigned char *>( surface->pixels ), surface->w, surface->h, GL_RGB );
			SDL_FreeSurface( surface );
		}
		#ifdef DEBUG1
		else
		{
			Logger::get() << "[BitmapFont] Unable to load texture file \"" << filename << "\": " << IMG_GetError() << Logger::endl;
		}
		#endif
	}
	
	BitmapFont::~BitmapFont()
	{
		if( this->texture != NULL )
			delete this->texture;
		
		BitmapFont::instances--;
		
		if( BitmapFont::instances == 0 )
			delete BitmapFont::program;
	}
	
	void BitmapFont::render( const Point2D& origin, const string& text ) const
	{
		BitmapFont::program->use( true );
		
		vector<Point2D> m_texcoords;
		m_texcoords.push_back( Point2D( 1.0f, 1.0f ) );
		m_texcoords.push_back( Point2D( 0.0f, 1.0f ) );
		m_texcoords.push_back( Point2D( 0.0f, 0.0f ) );
		m_texcoords.push_back( Point2D( 1.0f, 0.0f ) );

		vector<Point3D> m_points;
		m_points.push_back( Point3D( -1.0f, -1.0f, 0.5f ) );
		m_points.push_back( Point3D( 1.0f, -1.0f, 0.5f ) );
		m_points.push_back( Point3D( 1.0f, 1.0f, 0.5f ) );
		m_points.push_back( Point3D( -1.0f, 1.0f, 0.5f ) );
	
		vector<unsigned int> m_indices;
		m_indices.push_back( 0 );
		m_indices.push_back( 1 );
		m_indices.push_back( 2 );
		m_indices.push_back( 0 );
		m_indices.push_back( 2 );
		m_indices.push_back( 3 );
	
		ArrayBufferObject * vbo = new ArrayBufferObject();
		vbo->setData( m_points );
	
		ArrayBufferObject * tbo = new ArrayBufferObject();
		tbo->setData( m_texcoords );
	
		ElementArrayBufferObject * ibo = new ElementArrayBufferObject();
		ibo->setData( m_indices );
		
		BitmapFont::program->sendProjectionMatrix( "projection_matrix" );
		BitmapFont::program->sendModelviewMatrix( "modelview_matrix" );
		
		BitmapFont::program->sendVertexPointer( "a_Vertex", vbo );
		//BitmapFont::program->sendColorPointer( "a_Color", cbo );
		BitmapFont::program->sendUniform( "texture0", *(this->texture), 0 );
		BitmapFont::program->sendTextureCoordinatesPointer( "a_TexCoord0", tbo );

		ibo->draw();
		
		delete vbo;
		delete tbo;
		delete ibo;
	}
	
	void BitmapFont::renderSize( Point2D& origin, const string& text ) const
	{
		origin.moveBy( 0, this->characterHeight );
		
		for( unsigned int i = 0 ; i < text.length() ; i++ )
		{
			if( text[i] == '\n' )
				origin.moveBy( 0, this->characterHeight );
			else
				origin.moveBy( this->characterWidth, 0 );
		}
	}
	
	unsigned int BitmapFont::renderWidth( const string& text ) const
	{
		Point2D origin( 0, 0 );
		this->renderSize( origin, text );
		return origin.getX();
	}
	
	unsigned int BitmapFont::renderHeight( const string& text ) const
	{
		Point2D origin( 0, 0 );
		this->renderSize( origin, text );
		return origin.getY();
	}
}

