#include <opengl/Screen.h>
#include <SDL2/SDL_image.h>
#include <opengl/OpenGL.h>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
#include <iostream>
using namespace std;
using namespace tools::logger;
#endif

namespace opengl
{
	Screen * Screen::instance = NULL;

	Screen::Screen() : window(NULL), context(NULL), width(0), height(0), clearColor(0.0,0.0,0.0,1.0)
	{
	}

	Screen::~Screen()
	{
		if( this->context != NULL )
		{
			SDL_GL_DeleteContext( this->context );
		
			#ifdef DEBUG0
			Logger::get() << "[Screen] Context destroyed." << Logger::endl;
			#endif
		}
		
		if( this->window != NULL )
		{
			SDL_DestroyWindow( this->window );
		
			#ifdef DEBUG0
			Logger::get() << "[Screen] Window destroyed." << Logger::endl;
			#endif
		}
	}
		
	bool Screen::initialize( const char * title, int width, int height, bool resizable, int majorVersion, int minorVersion )
	{
		bool success = true;
		int imageFlags = IMG_INIT_PNG;
		
		SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO );
		int initFlags = IMG_Init( imageFlags );
		
		if( (initFlags & imageFlags) != imageFlags )
		{
			#ifdef DEBUG0
			Logger::get() << "[Screen] Unable to init SDL_image library: " << IMG_GetError() << Logger::endl;
			#endif
		}
		
		// Set the OpenGL version
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minorVersion );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	
		Screen * screen = new Screen();

		screen->window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			( width == 0 || height == 0 ) ? SDL_WINDOW_FULLSCREEN : ( resizable ? SDL_WINDOW_RESIZABLE : 0 ) | SDL_WINDOW_OPENGL
		);

		if( screen->window == NULL )
		{
			#ifdef DEBUG0
			Logger::get() << "[Screen] Unable to create window: " << SDL_GetError() << Logger::endl;
			#endif
		
			success = false;
			delete screen;
		}
		else
		{
			// TODO: See the fullscreen case
			// Theses values may be called by Font subclasses in order to render 2D overlay fonts
			screen->width = width;
			screen->height = height;
			
			#ifdef DEBUG0
			Logger::get() << "[Screen] Window created." << Logger::endl;
			#endif
		
			screen->context = SDL_GL_CreateContext( screen->window );
	
			if( screen->context == NULL )
			{
				#ifdef DEBUG0
				Logger::get() << "[Screen] Unable to create the context: " << SDL_GetError() << Logger::endl;
				#endif
		
				success = false;
				delete screen;
			}
			else
			{
				#ifdef DEBUG0
				Logger::get() << "[Screen] Context created." << Logger::endl;
				#endif
			
				Screen::instance = screen;
				
				OpenGL::initialize();
			
				screen->clear();
				screen->render();
			}
		}
	
		return success;
	}

	Screen * Screen::get()
	{
		return Screen::instance;
	}

	void Screen::destroy()
	{
		Screen * screen = Screen::get();
		delete screen;
		Screen::instance = NULL;
	
		IMG_Quit();
		SDL_Quit();
	}

	SDL_GLContext Screen::getContext()
	{
		return this->context;
	}

	void Screen::clear()
	{
		glClearColor( this->clearColor.getRed(), this->clearColor.getGreen(), this->clearColor.getBlue(), this->clearColor.getAlpha() );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLoadIdentity();
	}

	void Screen::render()
	{
		SDL_GL_SwapWindow( this->window );
	}
	
	void Screen::setClearColor( const Color& color )
	{
		this->clearColor.setColor( color );
	}
	
	int Screen::getWidth()
	{		
		return this->width;
	}
	
	int Screen::getHeight()
	{
		return this->height;
	}
}
