#include <opengl/Shader.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	Shader::Shader( Shader::Type type ) : id(0)
	{
		this->id = glCreateShader( static_cast<GLenum>( type ) );
		
		#ifdef DEBUG0
		Logger::get() << "[" << ( type == Shader::Vertex ? "Vertex" : ( type == Shader::Fragment ? "Fragment" : "" ) ) << "Shader#" << this->id << "] Created." << Logger::endl;
		#endif
	}
	
	Shader::~Shader()
	{
		glDeleteShader( this->id );
		
		#ifdef DEBUG0
		if( this->getDeleteStatus() )
			Logger::get() << "[Shader#" << this->id << "] Deleted." << Logger::endl;
		else
			Logger::get() << "[Shader#" << this->id << "] Could not be marked for deletion." << Logger::endl;
		#endif
	}
	
	bool Shader::getCompileStatus() const
	{
		GLint status;
		glGetProgramiv( this->id, GL_COMPILE_STATUS, &status );
		return status == GL_TRUE;
	}
	
	bool Shader::getDeleteStatus() const
	{
		GLint status;
		glGetProgramiv( this->id, GL_DELETE_STATUS, &status );
		return status == GL_TRUE;
	}

	Shader::Type Shader::getShaderType() const
	{
		GLint type;
		glGetShaderiv( this->id, GL_SHADER_TYPE, &type );
		return static_cast<Shader::Type>( type );
	}
	
	int Shader::getInfoLogLength() const
	{
		GLint length;
		glGetShaderiv( this->id, GL_INFO_LOG_LENGTH, &length );
		return static_cast<int>( length );
	}
	
	int Shader::getShaderSourceLength() const
	{
		GLint length;
		glGetShaderiv( this->id, GL_SHADER_SOURCE_LENGTH, &length );
		return static_cast<int>( length );
	}
	
	string Shader::getInfoLog() const
	{
		int length = this->getInfoLogLength();
		string log( length, ' ' );
		glGetShaderInfoLog( this->id, length, &length, &log[0] );
		return log;
	}
	
	unsigned int Shader::getId() const
	{
		return this->id;
	}
	
	bool Shader::load( const string& file )
	{
		return false;
	}
	
	bool Shader::compile( const string& content )
	{
		return false;
	}
}
