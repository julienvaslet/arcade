#include <opengl/BitmapFont.h>
#include <opengl/Screen.h>
#include <SDL2/SDL_image.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	Program * BitmapFont::program = NULL;
	unsigned int BitmapFont::instances = 0;
	
	BitmapFont::BitmapFont( const string& filename, unsigned int characterWidth, unsigned int characterHeight, unsigned int marginWidth, unsigned marginHeight ) : Font(BitmapFont::getFontNameFromPath(filename)), vertices(NULL), textureCoordinates(NULL), indices(NULL), texture(NULL), charactersByLine(0), characterWidth(characterWidth), characterHeight(characterHeight), marginWidth(marginWidth), marginHeight(marginHeight), relativeCharacterWidth(0.0f), relativeCharacterHeight(0.0f)
	{
		BitmapFont::instances++;
		
		if( BitmapFont::program == NULL )
		{
			BitmapFont::program = new Program();
			BitmapFont::program->loadVertexShaderFile( "data/shaders/BitmapFont.vs" );
			BitmapFont::program->loadFragmentShaderFile( "data/shaders/BitmapFont.fs" );
			BitmapFont::program->link( true );
		}
		
		this->vertices = new ArrayBufferObject();
		this->textureCoordinates = new ArrayBufferObject();
		this->indices = new ElementArrayBufferObject();
		
		this->texture = new Texture2D();
		this->texture->bind();
		
		#ifdef DEBUG0
		Logger::get() << "[BitmapFont#" << BitmapFont::getFontNameFromPath( filename ) << "] Loading file \"" << filename << "\"..." << Logger::endl;
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
			this->texture->setFiltering( GL_LINEAR, GL_LINEAR );

			SDL_FreeSurface( surface );
		}
		#ifdef DEBUG1
		else
		{
			Logger::get() << "[BitmapFont#" << BitmapFont::getFontNameFromPath( filename ) << "] Unable to load texture file \"" << filename << "\": " << IMG_GetError() << Logger::endl;
		}
		#endif
	}
	
	BitmapFont::~BitmapFont()
	{
		if( this->vertices != NULL )
			delete this->vertices;
			
		if( this->textureCoordinates != NULL )
			delete this->textureCoordinates;
			
		if( this->indices != NULL )
			delete this->indices;
			
		if( this->texture != NULL )
			delete this->texture;
		
		BitmapFont::instances--;
		
		if( BitmapFont::instances == 0 )
			delete BitmapFont::program;
	}
	
	void BitmapFont::render( const Point2D& origin, const string& text, float size ) const
	{
		if( size == 0.0f ) size = 1.0f;
		
		vector<Point2D> m_points;
		vector<Point2D> m_texcoords;
		vector<unsigned int> m_indices;

		Point2D point(origin);
		
		if( text.length() > 0 )
		{
			BitmapFont::program->use( true );

			for( unsigned int i = 0, j = 0 ; i < text.length() ; i++, j+=4 )
			{
				if( text[i] == '\n' )
				{
					point.moveTo( origin.getX(), point.getY() - (this->characterHeight - (2.0f * this->marginHeight)) * size );
					j -= 4;
					continue;
				}
				
				float x = static_cast<float>( text[i] % this->charactersByLine );
				float y = static_cast<float>( text[i] / this->charactersByLine );
				float dx = static_cast<float>( this->marginWidth ) / static_cast<float>( this->characterWidth );
				float dy = static_cast<float>( this->marginHeight ) / static_cast<float>( this->characterHeight );

				// Points
				m_points.push_back( point );
				point.moveBy( (this->characterWidth - (2.0f * this->marginWidth)) * size, 0 );
				m_points.push_back( point );
				point.moveBy( 0, (this->characterHeight - (2.0f * this->marginHeight)) * size );
				m_points.push_back( point );
				point.moveBy( -1.0f * (this->characterWidth - (2.0f * this->marginWidth)) * size, 0 );
				m_points.push_back( point );
				
				// Move to the next character position
				point.moveBy( (this->characterWidth - (2.0f * this->marginWidth)) * size, -1.0f * (this->characterHeight - (2.0f * this->marginHeight)) * size );
		
				// Texture Coordinates
				m_texcoords.push_back( Point2D( (x + dx) * this->relativeCharacterWidth, 1.0f - ((y + 1.0f - dy) * this->relativeCharacterHeight) ) );
				m_texcoords.push_back( Point2D( (x + (1.0f - dx)) * this->relativeCharacterWidth, 1.0f - ((y + 1.0f - dy) * this->relativeCharacterHeight) ) );
				m_texcoords.push_back( Point2D( (x + (1.0f - dx)) * this->relativeCharacterWidth, 1.0f - ((y + dy) * this->relativeCharacterHeight) ) );
				m_texcoords.push_back( Point2D( (x + dx) * this->relativeCharacterWidth, 1.0f - ((y + dy) * this->relativeCharacterHeight) ) );
	
				// Indices
				m_indices.push_back( j );
				m_indices.push_back( j + 1 );
				m_indices.push_back( j + 2 );
				m_indices.push_back( j );
				m_indices.push_back( j + 2 );
				m_indices.push_back( j + 3 );
			}

			this->vertices->setData( m_points );
			this->textureCoordinates->setData( m_texcoords );
			this->indices->setData( m_indices );
	
			BitmapFont::program->sendUniform( "window", static_cast<float>( Screen::get()->getWidth() ), static_cast<float>( Screen::get()->getHeight() ) );
			BitmapFont::program->sendAttributePointer( "a_Vertex", this->vertices, 2 );
			BitmapFont::program->sendAttributePointer( "a_TexCoord", this->textureCoordinates, 2 );
			BitmapFont::program->sendUniform( "texture", *(this->texture), 0 );

			this->indices->draw( OpenGL::Triangles );
		}
	}
	
	void BitmapFont::renderSize( Point2D& origin, const string& text, float size ) const
	{
		origin.moveBy( 0, (this->characterHeight - (2.0f * this->marginHeight)) * size );
		
		for( unsigned int i = 0 ; i < text.length() ; i++ )
		{
			if( text[i] == '\n' )
				origin.moveBy( 0, (this->characterHeight - (2.0f * this->marginHeight)) * size );
			else
				origin.moveBy( (this->characterWidth - (2.0f * this->marginWidth)) * size, 0 );
		}
	}
	
	unsigned int BitmapFont::renderWidth( const string& text, float size ) const
	{
		Point2D origin( 0, 0 );
		this->renderSize( origin, text, size );
		return origin.getX();
	}
	
	unsigned int BitmapFont::renderHeight( const string& text, float size ) const
	{
		Point2D origin( 0, 0 );
		this->renderSize( origin, text, size );
		return origin.getY();
	}
	
	string BitmapFont::getFontNameFromPath( const string& filename )
	{
		unsigned int lastSlash = filename.find_last_of("/");
		if( lastSlash == string::npos ) lastSlash = 0;
		
		unsigned int lastDot = filename.find_last_of(".");
		if( lastDot == string::npos ) lastDot = filename.length();
		
		return filename.substr( lastSlash + 1, lastDot - lastSlash - 1);
	}
}

