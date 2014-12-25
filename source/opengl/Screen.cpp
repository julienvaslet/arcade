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

	Screen::Screen() : window(NULL), context(NULL), x(0), y(0), width(0), height(0), clearColor(0.0,0.0,0.0,1.0)
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
		
	bool Screen::initialize( int width, int height, int majorVersion, int minorVersion )
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

		Screen * screen = new Screen();

		SDL_DisplayMode displayMode;
		SDL_GetDesktopDisplayMode( 0, &displayMode );
		
		// TODO: resize the viewport if larger
		screen->width = width;
		screen->height = height;
		screen->x = static_cast<int>( (displayMode.w - width) / 2.0 );
		screen->y = static_cast<int>( (displayMode.h - height) / 2.0 );

		screen->window = SDL_CreateWindow(
			NULL,
			0,
			0,
			displayMode.w,
			displayMode.h,
			SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
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
			#ifdef DEBUG0
			Logger::get() << "[Screen] Window created." << Logger::endl;
			#endif
			
			// Set the OpenGL version		
	#ifdef __PI__
			SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES );
	#else
			SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	#endif

			SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion );
			SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, minorVersion );

			SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
			SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
		
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
		glViewport( this->x, this->y, this->width, this->height );
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
