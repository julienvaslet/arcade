#include <labophoto/Negative.h>

namespace labophoto
{
	unsigned int Negative::instances = 0;
	Program * Negative::program = NULL;
	ArrayBufferObject * Negative::vertices = NULL;
	ArrayBufferObject * Negative::textureCoordinates = NULL;
	ElementArrayBufferObject * Negative::indices = NULL;
	
	Negative::Negative() : TexturedRectangle( false ), invertColors(true)
	{
		this->initializeRendering();
	}
	
	Negative::~Negative()
	{
		Negative::instances--;
	
		if( Negative::instances == 0 )
		{
			if( Negative::indices != NULL )
			{
				delete Negative::indices;
				Negative::indices = NULL;
			}
		
			if( Negative::textureCoordinates != NULL )
			{
				delete Negative::textureCoordinates;
				Negative::textureCoordinates = NULL;
			}
		
			if( Negative::vertices != NULL )
			{
				delete Negative::vertices;
				Negative::vertices = NULL;
			}
		
			if( Negative::program != NULL )
			{
				delete Negative::program;
				Negative::program = NULL;
			}
		}
	}
	
	void Negative::initializeRendering()
	{
		Negative::instances++;
		
		if( Negative::program == NULL )
		{
			Negative::program = new Program();
			
			Negative::program->loadVertexShaderFile( "data/labophoto/Negative.vs" );
			Negative::program->loadFragmentShaderFile( "data/labophoto/Negative.fs" );
			
			Negative::program->link( true );
		}
		
		if( Negative::vertices == NULL )
			Negative::vertices = new ArrayBufferObject();
			
		if( Negative::textureCoordinates == NULL )
			Negative::textureCoordinates = new ArrayBufferObject();
			
		if( Negative::indices == NULL )
			Negative::indices = new ElementArrayBufferObject();
	}
	
	void Negative::render( vector<Point3D>& vertices, vector<Point2D>& textureCoordinates, vector<unsigned short int>& indices, Texture2D * texture, unsigned int textureUnit )
	{
		if( Negative::program != NULL )
		{
			Negative::program->use( true );
			
			Negative::vertices->setData( vertices );
			Negative::textureCoordinates->setData( textureCoordinates );
			Negative::indices->setData( indices );
			
			Negative::program->sendUniform( "projection_matrix", Matrix::projection, false );
			Negative::program->sendUniform( "modelview_matrix", Matrix::modelview, false );
			Negative::program->sendUniform( "texture0", *texture, textureUnit );
			Negative::program->sendVertexPointer( "a_Vertex", Negative::vertices );
			Negative::program->sendTextureCoordinatesPointer( "a_TexCoord0", Negative::textureCoordinates );

			Negative::indices->draw( OpenGL::Triangles );
		}
	}
}

