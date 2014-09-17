#include <opengl/VertexShader.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	VertexShader::VertexShader() : Shader( Shader::Vertex )
	{
	}
	
	VertexShader::~VertexShader()
	{
		glDeleteShader( this->id );
		
		#ifdef DEBUG0
		if( this->getDeleteStatus() )
			Logger::get() << "[VertexShader#" << this->id << "] Deleted." << Logger::endl;
		else
			Logger::get() << "[VertexShader#" << this->id << "] Could not be marked for deletion." << Logger::endl;
		#endif
	}
	
	Shader::Type VertexShader::getShaderType() const
	{
		return Shader::Vertex;
	}
}

