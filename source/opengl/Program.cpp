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
	
	bool Program::loadVertexShaderFile( const string& filename )
	{
		bool status = false;
		
		VertexShader * shader = new VertexShader();
		if( shader->loadFile( filename ) )
			status = this->attachShader( shader, true );
		
		return status;
	}
	
	bool Program::loadFragmentShaderFile( const string& filename )
	{
		bool status = false;
		
		FragmentShader * shader = new FragmentShader();
		if( shader->loadFile( filename ) )
			status = this->attachShader( shader, true );
		
		return status;
	}
	
	bool Program::loadVertexShader( const string& source )
	{
		bool status = false;
		
		VertexShader * shader = new VertexShader();
		if( shader->load( source ) )
			status = this->attachShader( shader, true );
		
		return status;
	}
	
	bool Program::loadFragmentShader( const string& source )
	{
		bool status = false;
		
		FragmentShader * shader = new FragmentShader();
		if( shader->load( source ) )
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
	
	bool Program::link( bool cacheLocations )
	{
		glLinkProgram( this->id );
		
		#ifdef DEBUG1
		if( this->getLinkStatus() )
			Logger::get() << "[Program#" << this->id << "] Successfuly linked." << Logger::endl;
		else
			Logger::get() << "[Program#" << this->id << "] Linking error: " << this->getInfoLog() << Logger::endl;
		#endif
		
		if( cacheLocations && this->getLinkStatus() )
		{
			// Attributes
			int count = this->getActiveAttributes();
			int maxlength = this->getActiveAttributeMaxLength();
			string name( maxlength, 0 );
			
			for( int i = 0 ; i < count ; i++ )
			{
				GLsizei size = 0;
				GLenum type = GL_FLOAT;
				glGetActiveAttrib( this->id, i, bufSize, NULL, &size, &type, &name[0] );
				
				// TODO: Check errors
				this->attributes[name] = i;
				
				#ifdef DEBUG0
				Logger::get() << "[Program#" << this->id << "] Cached attribute \"" << name << ""\"'s location (" << i << ")." << Logger::endl;
				#endif
			}
			
			// Uniforms
			count = this->getActiveUniforms();
			maxlength = this->getActiveUniformMaxLength();
			name.reserve( maxlength );
			
			for( int i = 0 ; i < count ; i++ )
			{
				GLsizei size = 0;
				GLenum type = GL_FLOAT;
				glGetActiveUniform( this->id, i, bufSize, NULL, &size, &type, &name[0] );
				
				// TODO: Check errors
				this->uniforms[name] = i;
				
				#ifdef DEBUG0
				Logger::get() << "[Program#" << this->id << "] Cached uniform \"" << name << ""\"'s location (" << i << ")." << Logger::endl;
				#endif
			}
		}
		
		return this->getLinkStatus();
	}

	void Program::bindAttribute( const string& name, unsigned int index )
	{
		glBindAttribLocation( this->id, index, name.c_str() );
		
		if( OpenGL::getError() == GL_NO_ERROR )
		{
			this->attributes[name] = index;
			
			#ifdef DEBUG0
			Logger::get() << "[Program#" << this->id << "] Attribute \"" << name << "\" bound at location " << index << "." << Logger::endl;
			#endif
		}
		#ifdef DEBUG0
		else
			Logger::get() << "[Program#" << this->id << "] Could not bind attribute \"" << name << "\" at index " << index << "." << Logger::endl;
		#endif
	}
	
	void Program::enableAttribute( const string& name )
	{
		glEnableVertexAttribArray( this->getAttributeLocation( name ) );
	}
	
	int Program::getAttributeLocation( const string& name )
	{
		int location = -1;
		
		map<string, int>::const_iterator it = this->attributes.find( name );
		
		if( it != this->attributes.end() )
			location = it->second;
		else
		{
			location = glGetAttribLocation( this->id, name.c_str() );
			
			if( location != -1 )
			{
				this->attributes[name] = location;
				
				#ifdef DEBUG0
				Logger::get() << "[Program#" << this->id << "] Caching attribute \"" << name << "\"'s location (" << location << ")." << Logger::endl;
				#endif
			}
			#ifdef DEBUG0
			else
			{
				Logger::get() << "[Program#" << this->id << "] Attribute \"" << name << "\" not found." << Logger::endl;
			}
			#endif
		}
			
		return location;
	}
	
	int Program::getUniformLocation( const string& name )
	{
		int location = -1;
		
		map<string, int>::const_iterator it = this->uniforms.find( name );
		
		if( it != this->uniforms.end() )
			location = it->second;
		else
		{
			location = glGetUniformLocation( this->id, name.c_str() );
			
			if( location != -1 )
			{
				this->uniforms[name] = location;
				
				#ifdef DEBUG0
				Logger::get() << "[Program#" << this->id << "] Caching uniform \"" << name << "\"'s location (" << location << ")." << Logger::endl;
				#endif
			}
			#ifdef DEBUG0
			else
			{
				Logger::get() << "[Program#" << this->id << "] Uniform \"" << name << "\" not found." << Logger::endl;
			}
			#endif
		}
			
		return location;
	}
	
	void Program::use()
	{
		glUseProgram( this->id );
	}
	
	
			
	void Program::sendUniform( const string& uniform, float v0 )
	{
		glUniform1f( this->getUniformLocation( uniform ), v0 );
	}
	
	void Program::sendUniform( const string& uniform, float v0, float v1 )
	{
		glUniform2f( this->getUniformLocation( uniform ), v0, v1 );
	}
	
	void Program::sendUniform( const string& uniform, float v0, float v1, float v2 )
	{
		glUniform3f( this->getUniformLocation( uniform ), v0, v1, v2 );
	}
	
	void Program::sendUniform( const string& uniform, float v0, float v1, float v2, float v3 )
	{
		glUniform4f( this->getUniformLocation( uniform ), v0, v1, v2, v3 );
	}
	
	void Program::sendUniform( const string& uniform, float * values, unsigned int size, unsigned int count )
	{
		switch( size )
		{
			case 1:
				glUniform1fv( this->getUniformLocation( uniform ), count, values );
				break;
				
			case 2:
				glUniform2fv( this->getUniformLocation( uniform ), count, values );
				break;
			
			case 3:
				glUniform3fv( this->getUniformLocation( uniform ), count, values );
				break;

			case 4:		
				glUniform4fv( this->getUniformLocation( uniform ), count, values );
				break;
		}
	}
	
	void Program::sendUniform( const string& uniform, int v0 )
	{
		glUniform1i( this->getUniformLocation( uniform ), v0 );
	}
	
	void Program::sendUniform( const string& uniform, int v0, int v1 )
	{
		glUniform2i( this->getUniformLocation( uniform ), v0, v1 );
	}
	
	void Program::sendUniform( const string& uniform, int v0, int v1, int v2 )
	{
		glUniform3i( this->getUniformLocation( uniform ), v0, v1, v2 );
	}
	
	void Program::sendUniform( const string& uniform, int v0, int v1, int v2, int v3 )
	{
		glUniform4i( this->getUniformLocation( uniform ), v0, v1, v2, v3 );
	}
	
	void Program::sendUniform( const string& uniform, int * values, unsigned int size, unsigned int count )
	{
		switch( size )
		{
			case 1:
				glUniform1iv( this->getUniformLocation( uniform ), count, values );
				break;
				
			case 2:
				glUniform2iv( this->getUniformLocation( uniform ), count, values );
				break;
			
			case 3:
				glUniform3iv( this->getUniformLocation( uniform ), count, values );
				break;

			case 4:		
				glUniform4iv( this->getUniformLocation( uniform ), count, values );
				break;
		}
	}
	
	void Program::sendUniform( const string& uniform, unsigned int v0 )
	{
		glUniform1ui( this->getUniformLocation( uniform ), v0 );
	}
	
	void Program::sendUniform( const string& uniform, unsigned int v0, unsigned int v1 )
	{
		glUniform2ui( this->getUniformLocation( uniform ), v0, v1 );
	}
	
	void Program::sendUniform( const string& uniform, unsigned int v0, unsigned int v1, unsigned int v2 )
	{
		glUniform3ui( this->getUniformLocation( uniform ), v0, v1, v2 );
	}
	
	void Program::sendUniform( const string& uniform, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3 )
	{
		glUniform4ui( this->getUniformLocation( uniform ), v0, v1, v2, v3 );
	}
	
	void Program::sendUniform( const string& uniform, unsigned int * values, unsigned int size, unsigned int count )
	{
		switch( size )
		{
			case 1:
				glUniform1uiv( this->getUniformLocation( uniform ), count, values );
				break;
				
			case 2:
				glUniform2uiv( this->getUniformLocation( uniform ), count, values );
				break;
			
			case 3:
				glUniform3uiv( this->getUniformLocation( uniform ), count, values );
				break;

			case 4:		
				glUniform4uiv( this->getUniformLocation( uniform ), count, values );
				break;
		}
	}
	
	void Program::sendUniform( const string& uniform, const Matrix& matrix, bool transpose )
	{
		glUniformMatrix4fv( this->getUniformLocation( uniform ), 1, transpose, matrix.get() );
	}
	
	void Program::sendModelviewMatrix( const string& uniform )
	{
		float matrix[16];
		glGetFloatv( GL_MODELVIEW_MATRIX, matrix );
		glUniformMatrix4fv( this->getUniformLocation( uniform ), 1, GL_FALSE, matrix );
	}
	
	void Program::sendProjectionMatrix( const string& uniform )
	{
		float matrix[16];
		glGetFloatv( GL_PROJECTION_MATRIX, matrix );
		glUniformMatrix4fv( this->getUniformLocation( uniform ), 1, GL_FALSE, matrix );
	}
		
	void Program::sendAttributePointer( const string& name, ArrayBufferObject * buffer, unsigned int size, unsigned int stride )
	{
		glBindBuffer( GL_ARRAY_BUFFER, buffer->getBuffer() );
		glVertexAttribPointer( this->getAttributeLocation( name ), size, GL_FLOAT, GL_FALSE, stride, 0 );
	}
	
	void Program::sendVertexPointer( const string& name, ArrayBufferObject * vbo, unsigned int stride )
	{
		this->sendAttributePointer( name, vbo, 3, stride );
	}
	
	void Program::sendColorPointer( const string& name, ArrayBufferObject * cbo, unsigned int stride )
	{
		this->sendAttributePointer( name, cbo, 4, stride );
	}
}

