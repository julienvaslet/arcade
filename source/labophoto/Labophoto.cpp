#include <labophoto/Labophoto.h>

#include <opengl/Screen.h>
#include <opengl/ui/BitmapFont.h>
#include <opengl/ui/Button.h>
#include <opengl/ui/DropDownList.h>
#include <game/Resource.h>

#include <sstream>

using namespace game;
using namespace opengl;
using namespace std;

#ifdef DEBUG1
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#define CONTROL_PANEL_WIDTH			310.0f
#define WORKSPACE_BACKGROUND_COLOR	"444444"
#define WINDOW_BACKGROUND_COLOR		"888888"
#define UI_ELEMENT_BACKGROUND_COLOR	"668b8b"

namespace labophoto
{
	Labophoto * Labophoto::instance = NULL;
	
	Labophoto::Labophoto() : camera(NULL), ui(NULL), image(NULL), workspace(NULL)
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
		
		this->image = new TexturedRectangle( 1, 1 );
		
		Color workspaceColor( WORKSPACE_BACKGROUND_COLOR );
		this->workspace = new ColoredRectangle( 1, 1, workspaceColor );
		
		this->resizeWorkspace();
	
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
		if( this->workspace != NULL )
		{
			delete this->workspace;
			this->workspace = NULL;
		}
		
		if( this->image != NULL )
		{
			delete this->image;
			this->image = NULL;
		}
		
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
	
	void Labophoto::resizeWorkspace()
	{
		unsigned int width = Screen::get()->getWidth();
		unsigned int height = Screen::get()->getHeight();
		
		// Set the orthogonal origin at the top-left corner
		Matrix::projection = Matrix::ortho( 0, width, height, 0, -1.0f, 1.0f );
		
		this->workspace->getOrigin().moveTo( CONTROL_PANEL_WIDTH, 0.0f, 0.0f );
		this->workspace->resize( width - CONTROL_PANEL_WIDTH, height );
		
		// Resize image to fit workspace
		Texture2D * texture = this->image->getTile()->getTexture();
		
		if( texture != NULL )
		{
			// Review ratio computation
			float imageRatio = static_cast<float>( texture->getWidth() ) / static_cast<float>( texture->getHeight() );
			unsigned int imageWidth = 0;
			unsigned int imageHeight = 0;
			
			if( imageRatio > 1.0f )
			{
				imageWidth = this->workspace->getWidth();
				imageHeight = imageWidth / imageRatio;
				
				if( imageHeight > this->workspace->getHeight() )
				{
					imageHeight = this->workspace->getHeight();
					imageWidth = imageHeight * imageRatio;
				}
			}
			else
			{
				imageHeight = this->workspace->getHeight();
				imageWidth = imageHeight * imageRatio;
				
				if( imageWidth > this->workspace->getWidth() )
				{
					imageWidth = this->workspace->getWidth();
					imageHeight = imageWidth / imageRatio;
				}
			}
			
			this->image->resize( imageWidth, imageHeight );
			this->image->getOrigin().moveTo( CONTROL_PANEL_WIDTH + ( this->workspace->getWidth() - imageWidth ) / 2.0f, (this->workspace->getHeight() - imageHeight ) / 2.0f, 0.5f );
		}
	}
	
	void Labophoto::render( unsigned int ticks )
	{
		vector<Point3D> vertices;
		vector<Color> colors;
		vector<Point2D> textureCoordinates;
		vector<unsigned short int> indices;
		
		this->workspace->prepareRendering( vertices, colors, indices );
		ColoredRectangle::render( vertices, colors, indices );
	
		Texture2D * texture = static_cast<Texture2D *>( Resource::get( "labophoto.image" ) );
		
		if( texture != NULL )
		{
			vertices.clear();
			indices.clear();
			
			this->image->prepareRendering( vertices, textureCoordinates, indices );
			TexturedRectangle::render( vertices, textureCoordinates, indices, texture );
		}
		
		this->ui->render( ticks );
	}
	
	void Labophoto::handleEvent( SDL_Event * event )
	{
		this->ui->dispatchEvent( event );
	}
	
	void Labophoto::captureImage()
	{
		this->camera->capture( "test.jpg" );
		Resource::loadTexture2D( "labophoto.image", "test.jpg", true );
		this->image->getTile()->setTexture( "labophoto.image" );
		this->resizeWorkspace();
	}
	
	void Labophoto::loadModeSelectionUI()
	{
		ui::DropDownList * currentMode = new ui::DropDownList( "current_mode", "Mode de travail" );
		currentMode->setBackgroundColor( UI_ELEMENT_BACKGROUND_COLOR );
		currentMode->moveTo( 5, 5 );
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
		this->ui->hideElement( "btn_take_preview" );
		this->ui->hideElement( "camera_iso" );
		this->ui->hideElement( "camera_aperture" );
		this->ui->hideElement( "camera_shutterspeed" );
		this->ui->hideElement( "camera_whitebalance" );
		
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
				this->ui->showElement( "btn_take_preview" );
				this->ui->showElement( "camera_iso" );
				this->ui->showElement( "camera_aperture" );
				this->ui->showElement( "camera_shutterspeed" );
				this->ui->showElement( "camera_whitebalance" );
				
				// Test if it useful to do this
				this->reloadCameraConfiguration();
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
		reloadCameraList->moveTo( 5, 35 );
		reloadCameraList->resize( 300, 20 );
		reloadCameraList->addEventHandler( "mousedown", Labophoto::reloadCameraListEvent );
		
		ui::DropDownList * cameraList = new ui::DropDownList( "camera_list", "Liste des appareils" );
		cameraList->setBackgroundColor( UI_ELEMENT_BACKGROUND_COLOR );
		cameraList->moveTo( 5, 55 );
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
		ui::Button * takePreview = new ui::Button( "btn_take_preview", "Prendre une photo" );
		takePreview->setBackgroundColor( UI_ELEMENT_BACKGROUND_COLOR );
		takePreview->moveTo( 5, 35 );
		takePreview->resize( 300, 20 );
		takePreview->addEventHandler( "mousedown", Labophoto::takePreviewEvent );
		
		ui::DropDownList * cameraShutterSpeed = new ui::DropDownList( "camera_shutterspeed", "Duree d'ouverture (s)" );
		cameraShutterSpeed->setBackgroundColor( UI_ELEMENT_BACKGROUND_COLOR );
		cameraShutterSpeed->moveTo( 5, 60 );
		cameraShutterSpeed->resize( 300, 20 );
		cameraShutterSpeed->addEventHandler( "selectionchanged", Labophoto::changeShutterSpeedEvent );
		
		ui::DropDownList * cameraAperture = new ui::DropDownList( "camera_aperture", "Ouverture (focale)" );
		cameraAperture->setBackgroundColor( UI_ELEMENT_BACKGROUND_COLOR );
		cameraAperture->moveTo( 5, 80 );
		cameraAperture->resize( 300, 20 );
		cameraAperture->addEventHandler( "selectionchanged", Labophoto::changeApertureEvent );
		
		ui::DropDownList * cameraIso = new ui::DropDownList( "camera_iso", "Sensibilite (ISO)" );
		cameraIso->setBackgroundColor( UI_ELEMENT_BACKGROUND_COLOR );
		cameraIso->moveTo( 5, 100 );
		cameraIso->resize( 300, 20 );
		cameraIso->addEventHandler( "selectionchanged", Labophoto::changeIsoEvent );
		
		ui::DropDownList * cameraWhiteBalance = new ui::DropDownList( "camera_whitebalance", "Balance des blancs" );
		cameraWhiteBalance->setBackgroundColor( UI_ELEMENT_BACKGROUND_COLOR );
		cameraWhiteBalance->moveTo( 5, 120 );
		cameraWhiteBalance->resize( 300, 20 );
		cameraWhiteBalance->addEventHandler( "selectionchanged", Labophoto::changeWhiteBalanceEvent );
		
		this->ui->addElement( takePreview );
		this->ui->addElement( cameraIso );
		this->ui->addElement( cameraAperture );
		this->ui->addElement( cameraShutterSpeed );
		this->ui->addElement( cameraWhiteBalance );
	}
	
	void Labophoto::reloadCameraConfiguration()
	{
		if( this->camera != NULL )
		{
			map<string, string> configurations;
			configurations["aperture"] = "camera_aperture";
			configurations["shutterspeed"] = "camera_shutterspeed";
			configurations["iso"] = "camera_iso";
			configurations["whitebalance"] = "camera_whitebalance";
			
			vector<string> values;
			string currentValue;
			
			for( map<string,string>::const_iterator itConfiguration = configurations.begin() ; itConfiguration != configurations.end() ; itConfiguration++ )
			{
				currentValue = this->camera->getSetting( itConfiguration->first );
				this->camera->getSettingValues( itConfiguration->first, values );
			
				ui::DropDownList * ddl = static_cast<ui::DropDownList *>( this->ui->getElement( itConfiguration->second ) );
			
				if( ddl != NULL )
				{
					ddl->clear();
				
					for( vector<string>::iterator it = values.begin() ; it != values.end() ; it++ )
						ddl->addItem( *it, (*it == currentValue) );
				}
			
				values.clear();
			}
		}
	}
	
	void Labophoto::setCameraShutterSpeed( const string& value )
	{
		if( this->camera != NULL )
		{
			this->camera->setSetting( "shutterspeed", value );
		}
	}
	
	void Labophoto::setCameraAperture( const string& value )
	{
		if( this->camera != NULL )
		{
			this->camera->setSetting( "aperture", value );
		}
	}
	
	void Labophoto::setCameraIso( const string& value )
	{
		if( this->camera != NULL )
		{
			this->camera->setSetting( "iso", value );
		}
	}
	
	void Labophoto::setCameraWhiteBalance( const string& value )
	{
		if( this->camera != NULL )
		{
			this->camera->setSetting( "whitebalance", value );
		}
	}
	
	bool Labophoto::takePreviewEvent( Element * element, const event::Event * event )
	{
		Labophoto * labophoto = Labophoto::get();
		
		if( labophoto != NULL )
		{
			element->setDisabledState( true );
			labophoto->captureImage();
			element->setDisabledState( false );
		}
		
		return true;
	}
	
	bool Labophoto::changeShutterSpeedEvent( Element * element, const event::Event * event )
	{
		Labophoto * labophoto = Labophoto::get();
		ui::DropDownList * ddl = static_cast<ui::DropDownList *>( element );
		
		if( labophoto != NULL )
			labophoto->setCameraShutterSpeed( ddl->getSelectedItem() );
		
		return true;
	}
	
	bool Labophoto::changeApertureEvent( Element * element, const event::Event * event )
	{
		Labophoto * labophoto = Labophoto::get();
		ui::DropDownList * ddl = static_cast<ui::DropDownList *>( element );
		
		if( labophoto != NULL )
			labophoto->setCameraAperture( ddl->getSelectedItem() );
		
		return true;
	}
	
	bool Labophoto::changeIsoEvent( Element * element, const event::Event * event )
	{
		Labophoto * labophoto = Labophoto::get();
		ui::DropDownList * ddl = static_cast<ui::DropDownList *>( element );
		
		if( labophoto != NULL )
			labophoto->setCameraIso( ddl->getSelectedItem() );
		
		return true;
	}
	
	bool Labophoto::changeWhiteBalanceEvent( Element * element, const event::Event * event )
	{
		Labophoto * labophoto = Labophoto::get();
		ui::DropDownList * ddl = static_cast<ui::DropDownList *>( element );
		
		if( labophoto != NULL )
			labophoto->setCameraWhiteBalance( ddl->getSelectedItem() );
		
		return true;
	}
}

