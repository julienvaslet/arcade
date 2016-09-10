#include <labophoto/Labophoto.h>

#include <opengl/Screen.h>
#include <opengl/ui/BitmapFont.h>
#include <opengl/ui/Button.h>
#include <opengl/ui/DropDownList.h>

#include <sstream>

using namespace opengl;
using namespace std;

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#define WINDOW_BACKGROUND_COLOR		"888888"
#define UI_ELEMENT_BACKGROUND_COLOR	"668b8b"

namespace labophoto
{
	Labophoto * Labophoto::instance = NULL;
	
	Labophoto::Labophoto() : camera(NULL), ui(NULL)
	{
		if( Labophoto::instance != NULL )
		{
			delete Labophoto::instance;
			Labophoto::instance = NULL;
		}
		
		Labophoto::instance = this;
		
		Screen::get()->setTitle( "Labophoto" );

		Color backgroundColor( WINDOW_BACKGROUND_COLOR );
		Screen::get()->setClearColor( backgroundColor );
	
		// This should be enabled to render image, tools, etc.
		// Set the orthogonal origin at the top-left corner
		//Matrix::projection = Matrix::ortho( 0, Screen::get()->getWidth(), Screen::get()->getHeight(), 0, -1, 1 );
	
		new ui::BitmapFont( "data/fonts/bitmap.tga", 32, 32, 7, 1 );
	
		// This should be activated by Font or BitmapFont
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		
		// This should be activated by UserInterface
		glEnable( GL_DEPTH_TEST );
		
		ui::UserInterface::create( 800, 600 );
		this->ui = ui::UserInterface::get();
		this->ui->setFont( "bitmap", 0.5f );
		
		GphotoCamera::initializeContext();
		
		this->loadModeSelectionUI();
		this->loadCameraSelectionUI();
		this->loadCameraConfigurationUI();
		
		this->setMode( 0 );
	}
	
	Labophoto::~Labophoto()
	{
		if( this->ui != NULL )
		{
			delete this->ui;
			this->ui = NULL;
		}
		
		if( this->camera != NULL )
		{
			delete this->camera;
			this->camera = NULL;
		}
	
		GphotoCamera::destroyContext();
		ui::Font::destroy();
	}
	
	Labophoto * Labophoto::get()
	{
		return Labophoto::instance;
	}
			
	bool Labophoto::isRunning()
	{
		return true;
	}
	
	void Labophoto::render( unsigned int ticks )
	{
		this->ui->render( ticks );
	}
	
	void Labophoto::handleEvent( SDL_Event * event )
	{
		this->ui->dispatchEvent( event );
	}
	
	void Labophoto::loadModeSelectionUI()
	{
		ui::DropDownList * currentMode = new ui::DropDownList( "current_mode", "Mode de travail" );
		currentMode->setBackgroundColor( UI_ELEMENT_BACKGROUND_COLOR );
		currentMode->resize( 300, 20 );
		
		currentMode->addItem( "1 - Choix de l'appareil" );
		currentMode->addItem( "2 - Cadrage" );
		currentMode->addItem( "3 - Lecture" );
		currentMode->addItem( "4 - Tirage" );
		
		currentMode->addEventHandler( "selectionchanged", Labophoto::currentModeChangedEvent );
		
		this->ui->addElement( currentMode );
	}
	
	bool Labophoto::currentModeChangedEvent( Element * element, const event::Event * event )
	{
		Labophoto * labophoto = Labophoto::get();
		ui::DropDownList * currentMode = reinterpret_cast<ui::DropDownList *>( element );
		
		if( labophoto != NULL )
		{
			string item = currentMode->getSelectedItem();
			
			switch( item.at( 0 ) )
			{
				case '1':
				{
					labophoto->setMode( 0 );
					break;
				}
				
				case '2':
				{
					labophoto->setMode( 1 );
					break;
				}
				
				case '3':
				{
					labophoto->setMode( 2 );
					break;
				}
				
				case '4':
				{
					labophoto->setMode( 3 );
					break;
				}
				
				default:
					break;
			}
		}
		
		return true;
	}
	
	void Labophoto::setMode( short int mode )
	{
		if( mode != 0 && this->camera == NULL )
			mode = 0;
		
		ui::DropDownList * currentMode = reinterpret_cast<ui::DropDownList *>( this->ui->getElement( "current_mode" ) );
		currentMode->setSelectedItem( mode );
		
		this->ui->hideElement( "btn_reload_camera_list" );
		this->ui->hideElement( "camera_list" );
		
		switch( mode )
		{
			case 0:
			{
				this->ui->showElement( "btn_reload_camera_list" );
				this->ui->showElement( "camera_list" );
				break;
			}
			
			case 1:
			{
				break;
			}
			
			case 2:
			{
				break;
			}
			
			case 3:
			{
				break;
			}
		}
	}
	
	/*
	 * Step 1 - Camera selection.
	 */
	
	void Labophoto::loadCameraSelectionUI()
	{
		ui::Button * reloadCameraList = new ui::Button( "btn_reload_camera_list", "Recharger la liste" );
		reloadCameraList->setBackgroundColor( UI_ELEMENT_BACKGROUND_COLOR );
		reloadCameraList->moveTo( 0, 30 );
		reloadCameraList->resize( 300, 20 );
		reloadCameraList->addEventHandler( "mousedown", Labophoto::reloadCameraListEvent );
		
		ui::DropDownList * cameraList = new ui::DropDownList( "camera_list", "Liste des appareils" );
		cameraList->setBackgroundColor( UI_ELEMENT_BACKGROUND_COLOR );
		cameraList->moveTo( 0, 50 );
		cameraList->resize( 300, 20 );
		cameraList->addEventHandler( "selectionchanged", Labophoto::cameraChangedEvent );
		
		this->ui->addElement( reloadCameraList );
		this->ui->addElement( cameraList );
		
		this->reloadAvailableCameras();
	}
	
	void Labophoto::reloadAvailableCameras()
	{
		ui::DropDownList * cameraList = reinterpret_cast<ui::DropDownList *>( this->ui->getElement( "camera_list" ) );
		
		if( cameraList != NULL )
		{
			map<string, string> cameras;
			GphotoCamera::list( cameras );
			
			cameraList->clear();
			
			if( cameras.size() > 0 )
			{
				for( map<string, string>::iterator it = cameras.begin() ; it != cameras.end() ; it++ )
				{
					stringstream ss;
					ss << it->second << " (" << it->first << ")";
					cameraList->addItem( ss.str() );
				}
			}
			else
				cameraList->addItem( "Aucun appareil disponible", true );
		}
	}
	
	bool Labophoto::reloadCameraListEvent( Element * element, const event::Event * event )
	{
		Labophoto * labophoto = Labophoto::get();
		
		if( labophoto != NULL )
			labophoto->reloadAvailableCameras();
		
		return true;
	}
	
	bool Labophoto::cameraChangedEvent( Element * element, const event::Event * event )
	{
		Labophoto * labophoto = Labophoto::get();
		
		if( labophoto != NULL )
		{
			ui::DropDownList * ddl = reinterpret_cast<ui::DropDownList *>( element );
			string cameraName = ddl->getSelectedItem();
			size_t openingParenthesis = cameraName.find( '(' ) + 1;
			
			if( openingParenthesis != string::npos )
			{
				string port = cameraName.substr( openingParenthesis, cameraName.find( ')' ) - openingParenthesis );
				labophoto->selectCamera( port );
			}
		}
		
		return true;
	}
	
	void Labophoto::selectCamera( const string& port )
	{
		GphotoCamera * camera = GphotoCamera::getCamera( port );
		
		if( camera != NULL )
		{
			if( this->camera != NULL )
				delete this->camera;
			
			this->camera = camera;
			
			// Go to camera configuration
			this->setMode( 1 );
		}
	}
	
	/*
	 * Step 2 - Camera configuration.
	 */
	 
	void Labophoto::loadCameraConfigurationUI()
	{
	}
}

