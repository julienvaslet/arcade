#include <opengl/Rectangle.h>

namespace opengl
{
	Rectangle::Rectangle() : origin(), anchor(), width(0), height(0)
	{
	}
	
	Rectangle::Rectangle( const Rectangle& rectangle ) : origin(rectangle.origin), anchor(rectangle.anchor), width(0), height(0)
	{
	}
	
	Rectangle::Rectangle( unsigned width, unsigned height ) : origin(), anchor(), width(width), height(height)
	{
	}
	
	Rectangle::~Rectangle()
	{
	}
	
	Point3D& Rectangle::getOrigin()
	{
		return this->origin;
	}
	
	Point2D& Rectangle::getAnchor()
	{
		return this->anchor;
	}
	
	unsigned int Rectangle::getWidth()
	{
		return this->width;
	}
	
	unsigned int Rectangle::getHeight()
	{
		return this->height;
	}
	
	void Rectangle::setWidth( unsigned int width )
	{
		this->width = width;
	}
	
	void Rectangle::setHeight( unsigned int height )
	{
		this->height = height;
	}
	
	void Rectangle::resize( unsigned int width, unsigned int height )
	{
		this->width = width;
		this->height = height;
	}
	
	void Rectangle::resizeBy( int deltaWidth, unsigned int deltaHeight )
	{
		this->width += deltaWidth;
		this->height += deltaHeight;
	}
}

