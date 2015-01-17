#include <opengl/BitmapFont.h>
#include <opengl/TexturedRectangle.h>
#include <game/Resource.h>
#include <sstream>

using namespace game;
using namespace data::image;
using namespace std;

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

namespace opengl
{
	Program * BitmapFont::program = NULL;
	unsigned int BitmapFont::instances = 0;
	
	BitmapFont::BitmapFont( const string& filename, unsigned int characterWidth, unsigned int characterHeight, unsigned int marginWidth, unsigned marginHeight ) : Font(BitmapFont::getFontNameFromPath(filename)), vertices(NULL), textureCoordinates(NULL), indices(NULL), texture(NULL), charactersByLine(0), characterWidth(characterWidth), characterHeight(characterHeight), marginWidth(marginWidth), marginHeight(marginHeight)
	{
		BitmapFont::instances++;
		
		if( BitmapFont::program == NULL )
		{
			BitmapFont::program = new Program();
			#ifdef __PI__
			BitmapFont::program->loadVertexShaderFile( "data/shaders/BitmapFont.es.vs" );
			BitmapFont::program->loadFragmentShaderFile( "data/shaders/BitmapFont.es.fs" );
			#else
			BitmapFont::program->loadVertexShaderFile( "data/shaders/BitmapFont.vs" );
			BitmapFont::program->loadFragmentShaderFile( "data/shaders/BitmapFont.fs" );
			#endif
			BitmapFont::program->link( true );
		}
		
		this->vertices = new ArrayBufferObject();
		this->textureCoordinates = new ArrayBufferObject();
		this->indices = new ElementArrayBufferObject();
		
		stringstream sFontResourceName;
		sFontResourceName << "font." << BitmapFont::getFontNameFromPath( filename );
		string fontResourceName = sFontResourceName.str();
				
		Resource::loadTexture2D( fontResourceName, filename );
		this->texture = static_cast<Texture2D *>( Resource::get( fontResourceName ) );
	
		if( this->texture != NULL )
		{
			this->texture->use();
			
			this->charactersByLine = this->texture->getWidth() / this->characterWidth;
			this->texture->setFiltering( GL_LINEAR, GL_LINEAR );
		}
		#ifdef DEBUG1
		else
		{
			Logger::get() << "[BitmapFont#" << BitmapFont::getFontNameFromPath( filename ) << "] Unable to load texture file \"" << filename << "\"." << Logger::endl;
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
			this->texture->free();
		
		BitmapFont::instances--;
		
		if( BitmapFont::instances == 0 )
		{
			delete BitmapFont::program;
			BitmapFont::program = NULL;
		}
	}
	
	void BitmapFont::render( const Point2D& origin, const string& text, const Color& color, float size ) const
	{
		if( size == 0.0f ) size = 1.0f;
		
		if( this->texture != NULL )
		{
			vector<Point3D> vertices;
			vector<Point2D> textureCoordinates;
			vector<unsigned short int> indices;

			TexturedRectangle rectangle( (this->characterWidth - (2.0f * this->marginWidth)) * size, (this->characterHeight - (2.0f * this->marginHeight)) * size, this->texture );
			rectangle.getOrigin().moveTo( origin.getX(), origin.getY(), 0.0f );
		
			if( text.length() > 0 )
			{
				for( unsigned int i = 0 ; i < text.length() ; i++ )
				{
					if( text[i] == '\n' )
					{
						rectangle.getOrigin().moveTo( origin.getX(), rectangle.getOrigin().getY() + rectangle.getHeight(), 0.0f );
						continue;
					}
				
					float x = static_cast<float>( text[i] % this->charactersByLine );
					float y = static_cast<float>( text[i] / this->charactersByLine );
					
					rectangle.getTile()->setView( (x * this->characterWidth) + this->marginWidth, (y * this->characterHeight) + this->marginHeight, rectangle.getWidth(), rectangle.getHeight() );
					rectangle.prepareRendering( vertices, textureCoordinates, indices );
					rectangle.getOrigin().moveBy( rectangle.getWidth(), 0.0f, 0.0f );
				}
				
				TexturedRectangle::render( vertices, textureCoordinates, indices, this->texture, 0 );

				/*this->vertices->setData( m_points );
				this->textureCoordinates->setData( m_texcoords );
				this->indices->setData( m_indices );

				BitmapFont::program->sendUniform( "texture0", *(this->texture), 0 );
				BitmapFont::program->sendUniform( "window", static_cast<float>( Screen::get()->getWidth() ), static_cast<float>( Screen::get()->getHeight() ) );
				BitmapFont::program->sendUniform( "color", color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha() );
				BitmapFont::program->sendAttributePointer( "a_Vertex", this->vertices, 2 );
				BitmapFont::program->sendAttributePointer( "a_TexCoord", this->textureCoordinates, 2 );

				this->indices->draw( OpenGL::Triangles );*/
			}
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

