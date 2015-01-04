#include <blockgame/Block.h>
#include <tools/logger/Logger.h>

#include <SDL2/SDL.h>
#include <data/image/Image.h>

using namespace opengl;
using namespace tools::logger;

#define BLOCKGAME_TEXTURE_FILENAME	"data/blockgame/block.tga"

namespace blockgame
{
	unsigned int Block::instances = 0;
	Program * Block::program = NULL;
	Texture2D * Block::texture = NULL;
	ArrayBufferObject * Block::vertices = NULL;
	ArrayBufferObject * Block::textureCoordinates = NULL;
	ArrayBufferObject * Block::colors = NULL;
	ElementArrayBufferObject * Block::indices = NULL;
	
	Block::Block( const Point2D& position, const Color& color ) : position(position), color(color)
	{
		Block::instances++;
		
		if( Block::program == NULL )
		{
			Block::program = new Program();
			#ifdef __PI__
			Block::program->loadVertexShaderFile( "data/blockgame/block.es.vs" );
			Block::program->loadFragmentShaderFile( "data/blockgame/block.es.fs" );
			#else
			Block::program->loadVertexShaderFile( "data/blockgame/block.vs" );
			Block::program->loadFragmentShaderFile( "data/blockgame/block.fs" );
			#endif
			Block::program->link( true );
		}
		
		if( Block::vertices == NULL )
			Block::vertices = new ArrayBufferObject();
			
		if( Block::textureCoordinates == NULL )
			Block::textureCoordinates = new ArrayBufferObject();
			
		if( Block::colors == NULL )
			Block::colors = new ArrayBufferObject();
			
		if( Block::indices == NULL )
			Block::indices = new ElementArrayBufferObject();
		
		if( Block::texture == NULL )
		{
			Block::texture = new Texture2D();
			Block::texture->bind();
		
			#ifdef DEBUG0
			Logger::get() << "[Block] Loading file \"" << BLOCKGAME_TEXTURE_FILENAME << "\"" << Logger::endl;
			#endif
		
			Image * image = Image::load( BLOCKGAME_TEXTURE_FILENAME );
	
			if( image != NULL )
			{	
				Block::texture->setData( *image );
				Block::texture->setFiltering( GL_LINEAR, GL_LINEAR );

				delete image;
			}
			#ifdef DEBUG1
			else
			{
				Logger::get() << "[Block] Unable to load texture file \"" << BLOCKGAME_TEXTURE_FILENAME << "\"." << Logger::endl;
			}
			#endif
		}
	}
	
	Block::~Block()
	{
		Block::instances--;
		
		if( Block::instances == 0 )
		{
			if( Block::program != NULL )
			{
				delete Block::program;
				Block::program = NULL;
			}
			
			if( Block::vertices != NULL )
			{
				delete Block::vertices;
				Block::vertices = NULL;
			}
			
			if( Block::textureCoordinates != NULL )
			{
				delete Block::textureCoordinates;
				Block::textureCoordinates = NULL;
			}
			
			if( Block::colors != NULL )
			{
				delete Block::colors;
				Block::colors = NULL;
			}
			
			if( Block::indices != NULL )
			{
				delete Block::indices;
				Block::indices = NULL;
			}
			
			if( Block::texture != NULL )
			{
				delete Block::texture;
				Block::texture = NULL;
			}
		}
	}
	
	Point2D& Block::getPosition()
	{
		return this->position;
	}
	
	void Block::prepareRendering( vector<Point3D>& vPoints, vector<Point2D>& vTexCoords, vector<Color>& vColors, vector<unsigned short int>& vIndices ) const
	{
		unsigned short int j = static_cast<unsigned short int>( vPoints.size() );
		 
		// Points
		vPoints.push_back( Point3D( 3.0f * this->position.getX(), 3.0f * this->position.getY(), 0.0f ) );
		vPoints.push_back( Point3D( 3.0f * this->position.getX(), 3.0f * (this->position.getY() + 1.0f), 0.0f ) );
		vPoints.push_back( Point3D( 3.0f * (this->position.getX() + 1.0f), 3.0f * (this->position.getY() + 1.0f), 0.0f ) );
		vPoints.push_back( Point3D( 3.0f * (this->position.getX() + 1.0f), 3.0f * this->position.getY(), 0.0f ) );
		
		// Texture Coordinates
		vTexCoords.push_back( Point2D( 0.0f, 0.0f ) );
		vTexCoords.push_back( Point2D( 0.0f, 1.0f ) );
		vTexCoords.push_back( Point2D( 1.0f, 1.0f ) );
		vTexCoords.push_back( Point2D( 1.0f, 0.0f ) );
		
		// Colors
		vColors.push_back( this->color );
		vColors.push_back( this->color );
		vColors.push_back( this->color );
		vColors.push_back( this->color );

		// Indices
		vIndices.push_back( j );
		vIndices.push_back( j + 1 );
		vIndices.push_back( j + 2 );
		vIndices.push_back( j );
		vIndices.push_back( j + 2 );
		vIndices.push_back( j + 3 );
	}
	
	void Block::renderBlocks( Matrix& projection, Matrix& modelview, vector<Point3D>& vPoints, vector<Point2D>& vTexCoords, vector<Color>& vColors, vector<unsigned short int>& vIndices )
	{
		Block::program->use( true );

		Block::vertices->setData( vPoints );
		Block::textureCoordinates->setData( vTexCoords );
		Block::colors->setData( vColors );
		Block::indices->setData( vIndices );

		Block::program->sendUniform( "projection_matrix", projection, false );
		Block::program->sendUniform( "modelview_matrix", modelview, false );
		Block::program->sendUniform( "texture", *(Block::texture), 0 );
		Block::program->sendVertexPointer( "a_Vertex", Block::vertices );
		Block::program->sendColorPointer( "a_Color", Block::colors );
		Block::program->sendAttributePointer( "a_TexCoord", Block::textureCoordinates, 2 );

		Block::indices->draw( OpenGL::Triangles );
	}
}

