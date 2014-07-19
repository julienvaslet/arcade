#include <opengl/Screen.h>
#include <SDL2/SDL_image.h>

#ifdef DEBUG0
#include <iostream>
using namespace std;
#endif

namespace opengl
{
	Screen * Screen::instance = NULL;

	Screen::Screen() : window(NULL), context(NULL)
	{
	}

	Screen::~Screen()
	{
		if( this->context != NULL )
		{	
			SDL_GL_DeleteContext( this->context );
		
			#ifdef DEBUG0
			cout << "[Screen] Context destroyed." << endl;
			#endif
		}
		
		if( this->window != NULL )
		{
			SDL_DestroyWindow( this->window );
		
			#ifdef DEBUG0
			cout << "[Screen] Window destroyed." << endl;
			#endif
		}
	}
		
	bool Screen::initialize( const char * title, int width, int height, bool resizable )
	{
		bool success = true;
		int imageFlags = IMG_INIT_PNG;
		
		SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK );
		int initFlags = IMG_Init( imageFlags );
		
		if( (initFlags & imageFlags) != imageFlags )
		{
			#ifdef DEBUG0
			cout << "[Screen] Unable to init SDL_image library: " << IMG_GetError() << endl;
			#endif
		}
	
		Screen * screen = new Screen();

		screen->window = SDL_CreateWindow(
			title,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			( width == 0 || height == 0 ) ? SDL_WINDOW_FULLSCREEN : ( resizable ? SDL_WINDOW_RESIZABLE : 0 ) | SDL_WINDOW_OPENGL
		);
	
		// do no specify width & height and put in fullscreen
		// then => SDL_RenderSetLogicalSize( renderer, 800, 600 ); ?

		if( screen->window == NULL )
		{
			#ifdef DEBUG0
			cout << "[Screen] Unable to create window: " << SDL_GetError() << endl;
			#endif
		
			success = false;
			delete screen;
		}
		else
		{
			#ifdef DEBUG0
			cout << "[Screen] Window created." << endl;
			#endif
		
			screen->context = SDL_GL_CreateContext( screen->window );
	
			if( screen->context == NULL )
			{
				#ifdef DEBUG0
				cout << "[Screen] Unable to create the context: " << SDL_GetError() << endl;
				#endif
		
				success = false;
				delete screen;
			}
			else
			{
				#ifdef DEBUG0
				cout << "[Screen] Context created." << endl;
				#endif
			
				screen->clear();
				screen->render();
			
				Screen::instance = screen;
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
		//this->resetRenderColor();
		//glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLoadIdentity();
	}

	void Screen::render()
	{
		SDL_GL_SwapWindow( this->window );
	}
	
	void Screen::resetRenderColor()
	{
		//SDL_SetRenderDrawColor( this->renderer, 0, 0, 0, 255 );
	}
	
	void Screen::setRenderColor( const Color& color )
	{
		//SDL_SetRenderDrawColor( this->renderer, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha() );
	}
	
	int Screen::getWidth()
	{
		int width = 0;
		//int height = 0;
		
		/*if( SDL_GetRendererOutputSize( this->renderer, &width, &height ) != 0 )
		{
			width = 0;
			#ifdef DEBUG0
			cout << "[Screen] An error has occured when output size was requested: " << SDL_GetError() << endl;
			#endif
		}*/
		
		return width;
	}
	
	int Screen::getHeight()
	{
		//int width = 0;
		int height = 0;
		/*
		if( SDL_GetRendererOutputSize( this->renderer, &width, &height ) != 0 )
		{
			height = 0;
			#ifdef DEBUG0
			cout << "[Screen] An error has occured when output size was requested: " << SDL_GetError() << endl;
			#endif
		}*/
		
		return height;
	}
}
