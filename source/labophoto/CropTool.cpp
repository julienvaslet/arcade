#include <labophoto/CropTool.h>
#include <opengl/Texture2D.h>
#include <cmath>

#ifdef DEBUG0
#include <tools/logger/Logger.h>
using namespace tools::logger;
#endif

#define CROP_RECTANGLE_BORDER_COLOR		"ffffff"
#define CROP_RECTANGLE_BORDER_SIZE		1.0f
#define CROP_RECTANGLE_BACKGROUND_COLOR	"00000088"

namespace labophoto
{	
	CropTool::CropTool( Negative * negative ) : ColoredRectangle(), active(false), negative(negative)
	{
	}
	
	CropTool::~CropTool()
	{
	}
	
	MouseCropPosition CropTool::getCropPosition( const Point2D& point, const Negative& negative )
	{
		MouseCropPosition position = MouseCropPosition::Outside;
		
		
		
		return position;
	}
	
	bool CropTool::isActive() const
	{
		return this->active;
	}
	
	void CropTool::activate( bool status )
	{
		this->active = status;
	}
	
	void CropTool::render()
	{
		vector<Point3D> vertices;
		vector<Color> colors;
		vector<unsigned short int> indices;
		
		unsigned int textureWidth = this->negative->getNegativeWidth();

		if( textureWidth > 0 )
		{
			float ratio = static_cast<float>( this->getWidth() ) / static_cast<float>( textureWidth );
			
			Rectangle view;
			view.getOrigin().moveTo( this->getOrigin().getX() + round( this->negative->getView().getX() * ratio ), this->getOrigin().getY() + round( this->negative->getView().getY() * ratio ), this->getOrigin().getZ() );
			view.resize( round( this->negative->getView().getWidth() * ratio ), round( this->negative->getView().getHeight() * ratio ) );
			
			Color borderColor( CROP_RECTANGLE_BORDER_COLOR );
			Color backgroundColor( CROP_RECTANGLE_BACKGROUND_COLOR );
			
			ColoredRectangle rectangle( view, backgroundColor );
			
			// Cropped top area
			rectangle.getOrigin().moveTo( this->getOrigin().getX(), this->getOrigin().getY(), this->getOrigin().getZ() );
			rectangle.resize( this->getWidth(), view.getOrigin().getY() - this->getOrigin().getY() );
			rectangle.prepareRendering( vertices, colors, indices );
			
			// Cropped right area
			rectangle.getOrigin().moveTo( view.getOrigin().getX() + view.getWidth(), view.getOrigin().getY(), this->getOrigin().getZ() );
			rectangle.resize( this->getWidth() - (view.getWidth() + view.getOrigin().getX() - this->getOrigin().getX()), view.getHeight() );
			rectangle.prepareRendering( vertices, colors, indices );
			
			// Cropped bottom area
			rectangle.getOrigin().moveTo( this->getOrigin().getX(), view.getOrigin().getY() + view.getHeight(), this->getOrigin().getZ() );
			rectangle.resize( this->getWidth(), this->getHeight() - view.getHeight() - ( view.getOrigin().getY() - this->getOrigin().getY() ) );
			rectangle.prepareRendering( vertices, colors, indices );
			
			// Cropped left area
			rectangle.getOrigin().moveTo( this->getOrigin().getX(), view.getOrigin().getY(), this->getOrigin().getZ() );
			rectangle.resize( view.getOrigin().getX() - this->getOrigin().getX(), view.getHeight() );
			rectangle.prepareRendering( vertices, colors, indices );
			
			rectangle.setColor( borderColor );
			
			// Top border
			rectangle.getOrigin().moveTo( view.getOrigin().getX(), view.getOrigin().getY() - CROP_RECTANGLE_BORDER_SIZE, this->getOrigin().getZ() + 0.1f );
			rectangle.resize( view.getWidth(), CROP_RECTANGLE_BORDER_SIZE );
			rectangle.prepareRendering( vertices, colors, indices );
			
			// Right border
			rectangle.getOrigin().moveTo( view.getOrigin().getX() + view.getWidth(), view.getOrigin().getY() - CROP_RECTANGLE_BORDER_SIZE, this->getOrigin().getZ() + 0.1f );
			rectangle.resize( CROP_RECTANGLE_BORDER_SIZE, view.getHeight() + 2 * CROP_RECTANGLE_BORDER_SIZE );
			rectangle.prepareRendering( vertices, colors, indices );
			
			// Bottom border
			rectangle.getOrigin().moveTo( view.getOrigin().getX(), view.getOrigin().getY() + view.getHeight(), this->getOrigin().getZ() + 0.1f );
			rectangle.resize( view.getWidth(), CROP_RECTANGLE_BORDER_SIZE );
			rectangle.prepareRendering( vertices, colors, indices );
			
			// Left border
			rectangle.getOrigin().moveTo( view.getOrigin().getX() - CROP_RECTANGLE_BORDER_SIZE, view.getOrigin().getY() - CROP_RECTANGLE_BORDER_SIZE, this->getOrigin().getZ() + 0.1f );
			rectangle.resize( CROP_RECTANGLE_BORDER_SIZE, view.getHeight() + 2 * CROP_RECTANGLE_BORDER_SIZE );
			rectangle.prepareRendering( vertices, colors, indices );
			
			ColoredRectangle::render( vertices, colors, indices );
		}
	}
}

