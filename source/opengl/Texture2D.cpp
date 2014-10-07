#include <opengl/Texture2D.h>
#include <tools/logger/Logger.h>

using namespace tools::logger;

namespace opengl
{
	Texture2D::Texture2D() : Texture()
	{
		this->bind();
		this->setWrapMode( GL_REPEAT );
		this->setFiltering( GL_NEAREST, GL_NEAREST );
	}
	
	Texture2D::~Texture2D()
	{
	}
	
	void Texture2D::bind( unsigned int unit ) const
	{
		glActiveTexture( GL_TEXTURE0 + unit );
		glBindTexture( GL_TEXTURE_2D, this->id );
	}
	
	void Texture2D::setData( unsigned char * data, unsigned int width, unsigned int height, GLenum type )
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, type, GL_UNSIGNED_BYTE, data );
	}
	
	void Texture2D::setWrapMode( GLenum mode )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode );
	}
	
	void Texture2D::setFiltering( GLenum minification, GLenum magnification )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minification );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnification );
	}
}

