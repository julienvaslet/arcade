#include <opengl/BitmapFont.h>
#include <opengl/ArrayBufferObject.h>
#include <opengl/ElementArrayBufferObject.h>
#include <opengl/Screen.h>
#include <SDL2/SDL_image.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	Program * BitmapFont::program = NULL;
	unsigned int BitmapFont::instances = 0;
	
	BitmapFont::BitmapFont( const string& filename, unsigned int characterWidth, unsigned int characterHeight ) : Font(filename), texture(NULL), charactersByLine(0), characterWidth(characterWidth), characterHeight(characterHeight), relativeCharacterWidth(0.0f), relativeCharacterHeight(0.0f)
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
			int pitch = surface->pitch / surface->w;
			
			this->charactersByLine = surface->w / this->characterWidth;
			this->relativeCharacterWidth = static_cast<float>( this->characterWidth ) / static_cast<float>( surface->w );
			this->relativeCharacterHeight = static_cast<float>( this->characterHeight ) / static_cast<float>( surface->h );
			
			// Texture should be rotated because of SDL loading functions
			vector<unsigned char> pixels( surface->h * surface->w * pitch, '\0' );
			
			for( int j = 0 ; j < surface->h ; j++ )
			{
				for( int i = 0 ; i < surface->w ; i++ )
				{
					for( int p = 0 ; p < pitch ; p++ )
					{
						pixels[ j * surface->w * pitch + i * pitch + p ] = ( *( static_cast<unsigned char *>(surface->pixels) + (surface->h - j) * surface->w * pitch + i * pitch + p ) );
					}
				}

			}
						
			this->texture->setData( &(pixels[0]), surface->w, surface->h, GL_RGB );

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

		vector<Point2D> m_points;
		m_points.push_back( Point2D( 200.0f, 150.0f ) );
		m_points.push_back( Point2D( 600.0f, 150.0f ) );
		m_points.push_back( Point2D( 600.0f, 550.0f ) );
		m_points.push_back( Point2D( 200.0f, 550.0f ) );
		
		char c = 'P';
		
		float x = static_cast<float>( c % this->charactersByLine );
		float y = static_cast<float>( c / this->charactersByLine );
		
		vector<Point2D> m_texcoords;
		m_texcoords.push_back( Point2D( x * this->relativeCharacterWidth, 1.0f - ((y + 1) * this->relativeCharacterHeight) ) );
		m_texcoords.push_back( Point2D( (x + 1.0f) * this->relativeCharacterWidth, 1.0f - ((y + 1) * this->relativeCharacterHeight) ) );
		m_texcoords.push_back( Point2D( (x + 1.0f) * this->relativeCharacterWidth, 1.0f - (y * this->relativeCharacterHeight) ) );
		m_texcoords.push_back( Point2D( x * this->relativeCharacterWidth, 1.0f - (y * this->relativeCharacterHeight) ) );
	
		vector<unsigned int> m_indices;
		m_indices.push_back( 0 );
		m_indices.push_back( 1 );
		m_indices.push_back( 2 );
		m_indices.push_back( 3 );
	
		ArrayBufferObject * vbo = new ArrayBufferObject();
		vbo->setData( m_points );
	
		ArrayBufferObject * tbo = new ArrayBufferObject();
		tbo->setData( m_texcoords );
	
		ElementArrayBufferObject * ibo = new ElementArrayBufferObject();
		ibo->setData( m_indices );
		
		BitmapFont::program->sendUniform( "window", static_cast<float>( Screen::get()->getWidth() ), static_cast<float>( Screen::get()->getHeight() ) );
		BitmapFont::program->sendAttributePointer( "a_Vertex", vbo, 2 );
		BitmapFont::program->sendAttributePointer( "a_TexCoord", tbo, 2 );
		BitmapFont::program->sendUniform( "texture", *(this->texture), 0 );

		ibo->draw( OpenGL::Quads );
		
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

