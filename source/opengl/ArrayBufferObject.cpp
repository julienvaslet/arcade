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
}
