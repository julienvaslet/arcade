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
		CheckOpenGLError(glActiveTexture);
		glBindTexture( GL_TEXTURE_2D, this->id );
		CheckOpenGLError(glBindTexture);
	}
	
	void Texture2D::setData( unsigned char * data, unsigned int width, unsigned int height, GLenum format )
	{
		glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
		CheckOpenGLError(glTexImage2D);
	}
	
	void Texture2D::setWrapMode( GLenum mode )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode );
		CheckOpenGLError(glTexParameteri);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode );
		CheckOpenGLError(glTexParameteri);
	}
	
	void Texture2D::setFiltering( GLenum minification, GLenum magnification )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minification );
		CheckOpenGLError(glTexParameteri);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnification );
		CheckOpenGLError(glTexParameteri);
	}
}

