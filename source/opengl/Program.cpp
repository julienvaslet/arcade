#include <opengl/Program.h>
#include <opengl/OpenGL.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	Program::Program() : id(0)
	{
		this->id = glCreateProgram();
		
		#ifdef DEBUG0
		Logger::get() << "[Program#" << this->id << "] Created." << Logger::endl;
		#endif
	}
	
	Program::~Program()
	{
		glDeleteProgram( this->id );
		
		#ifdef DEBUG0
		if( this->getDeleteStatus() )
			Logger::get() << "[Program#" << this->id << "] Deleted." << Logger::endl;
		else
			Logger::get() << "[Program#" << this->id << "] Could not be marked for deletion." << Logger::endl;
		#endif
	}
	
	bool Program::getLinkStatus() const
	{
		GLint status;
		glGetProgramiv( this->id, GL_LINK_STATUS, &status );
		return status == GL_TRUE;
	}
	
	bool Program::getDeleteStatus() const
	{
		GLint status;
		glGetProgramiv( this->id, GL_DELETE_STATUS, &status );
		return status == GL_TRUE;
	}
	
	bool Program::getValidateStatus() const
	{
		GLint status;
		glGetProgramiv( this->id, GL_VALIDATE_STATUS, &status );
		return status == GL_TRUE;
	}
	
	int Program::getInfoLogLength() const
	{
		GLint length;
		glGetProgramiv( this->id, GL_INFO_LOG_LENGTH, &length );
		return static_cast<int>( length );
	}
	
	int Program::getAttachedShaders() const
	{
		GLint count;
		glGetProgramiv( this->id, GL_ATTACHED_SHADERS, &count );
		return static_cast<int>( count );
	}
	
	int Program::getActiveAttributes() const
	{
		GLint count;
		glGetProgramiv( this->id, GL_ACTIVE_ATTRIBUTES, &count );
		return static_cast<int>( count );
	}
	
	int Program::getActiveAttributeMaxLength() const
	{
		GLint length;
		glGetProgramiv( this->id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length );
		return static_cast<int>( length );
	}
	
	int Program::getActiveUniforms() const
	{
		GLint count;
		glGetProgramiv( this->id, GL_ACTIVE_UNIFORMS, &count );
		return static_cast<int>( count );
	}
	
	int Program::getActiveUniformMaxLength() const
	{
		GLint length;
		glGetProgramiv( this->id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length );
		return static_cast<int>( length );
	}
	
	string Program::getInfoLog() const
	{
		int length = this->getInfoLogLength();
		string log( length, ' ' );
		glGetProgramInfoLog( this->id, length, &length, &log[0] );
		return log;
	}
	
	unsigned int Program::getId() const
	{
		return this->id;
	}
	
	bool Program::loadVertexShader( const string& filename )
	{
		bool status = false;
		
		VertexShader * shader = new VertexShader();
		if( shader->load( filename ) )
			status = this->attachShader( shader, true );
		
		return status;
	}
	
	bool Program::loadFragmentShader( const string& filename )
	{
		bool status = false;
		
		FragmentShader * shader = new FragmentShader();
		if( shader->load( filename ) )
			status = this->attachShader( shader, true );
		
		return status;
	}
	
	bool Program::attachShader( Shader * shader, bool deleteShader )
	{
		bool status = true;
		glAttachShader( this->id, shader->getId() );
		status = (OpenGL::getError() == GL_NO_ERROR);
		
		#ifdef DEBUG0
		if( status )
			Logger::get() << "[Program#" << this->id << "] Shader#" << shader->getId() << " attached." << Logger::endl;
		else
			Logger::get() << "[Program#" << this->id << "] Could not attach Shader#" << shader->getId() << "." << Logger::endl;
		#endif
		
		if( deleteShader )
			delete shader;
			
		return status;
	}
	
	bool Program::link()
	{
		glLinkProgram( this->id );
		
		#ifdef DEBUG1
		if( this->getLinkStatus() )
			Logger::get() << "[Program#" << this->id << "] Successfuly linked." << Logger::endl;
		else
			Logger::get() << "[Program#" << this->id << "] Linking error: " << this->getInfoLog() << Logger::endl;
		#endif
		
		return this->getLinkStatus();
	}
	
	void Program::use()
	{
		glUseProgram( this->id );
	}
}

