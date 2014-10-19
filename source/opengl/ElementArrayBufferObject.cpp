#include <opengl/ElementArrayBufferObject.h>

namespace opengl
{
	ElementArrayBufferObject::ElementArrayBufferObject() : BufferObject()
	{
	}
	
	ElementArrayBufferObject::~ElementArrayBufferObject()
	{
	}
	
	void ElementArrayBufferObject::bind()
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, this->buffer );
	}
	
	void ElementArrayBufferObject::setData( const vector<unsigned int>& data, Frequency frequency, Access access )
	{
		this->bind();
		this->size = data.size();
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.size(), &data[0], BufferObject::getUsage( frequency, access ) );
	}
	
	void ElementArrayBufferObject::draw( OpenGL::DrawMode mode, unsigned int start, unsigned int end )
	{
		this->bind();
		glDrawRangeElements( mode, start, end, ( end == 0 || end - start > this->size ? this->size - start : end - start), GL_UNSIGNED_INT, NULL );
	}
	
	void ElementArrayBufferObject::draw( unsigned int start, unsigned int end )
	{
		this->draw( OpenGL::Triangles, start, end );
	}
}
