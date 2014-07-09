#include <opengl/ArrayBufferObject.h>

namespace opengl
{
	ArrayBufferObject::ArrayBufferObject() : BufferObject()
	{
	}
	
	ArrayBufferObject::~ArrayBufferObject()
	{
	}
	
	void ArrayBufferObject::bind()
	{
		glBindBuffer( GL_ARRAY_BUFFER, this->buffer );
	}
	
	void ArrayBufferObject::setData( const vector<float>& data, Frequency frequency, Access access )
	{
		this->bind();
		glBufferData( GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], BufferObject::getUsage( frequency, access ) );
	}
	
	void ArrayBufferObject::setData( const vector<Point>& data, Frequency frequency, Access access )
	{
		vector<float> vertices;
		
		for( vector<Point>::const_iterator it = data.begin() ; it != data.end() ; it++ )
		{
			vertices.push_back( it->getX() );
			vertices.push_back( it->getY() );
			vertices.push_back( it->getZ() );
		}
		
		this->setData( vertices, frequency, access );
	}
	
	void ArrayBufferObject::setData( const vector<Color>& data, Frequency frequency, Access access )
	{
		vector<float> colors;
		
		for( vector<Color>::const_iterator it = data.begin() ; it != data.end() ; it++ )
		{
			colors.push_back( it->getRed() );
			colors.push_back( it->getGreen() );
			colors.push_back( it->getBlue() );
			colors.push_back( it->getAlpha() );
		}
		
		this->setData( colors, frequency, access );
	}
}
