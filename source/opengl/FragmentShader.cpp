#include <opengl/FragmentShader.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	FragmentShader::FragmentShader() : Shader( Shader::Fragment )
	{
	}
	
	FragmentShader::~FragmentShader()
	{
		glDeleteShader( this->id );
		
		#ifdef DEBUG0
		if( this->getDeleteStatus() )
			Logger::get() << "[FragmentShader#" << this->id << "] Deleted." << Logger::endl;
		else
			Logger::get() << "[FragmentShader#" << this->id << "] Could not be marked for deletion." << Logger::endl;
		#endif
	}
	
	Shader::Type FragmentShader::getShaderType() const
	{
		return Shader::Fragment;
	}
}

