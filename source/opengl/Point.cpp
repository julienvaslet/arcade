#include <opengl/Point.h>

namespace opengl
{
	Point::Point( float x, float y, float z ) : x(x), y(y), z(z)
	{
	}
	
	Point::Point( const Point& point ) : x(point.x), y(point.y), z(point.z)
	{
	}
	
	Point::~Point()
	{
	}
	
	void Point::setX( float x )
	{
		this->x = x;
	}
	
	void Point::setY( float y )
	{
		this->y = y;
	}
	
	void Point::setZ( float z )
	{
		this->z = z;
	}
	
	void Point::moveTo( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
	void Point::moveBy( float dx, float dy, float dz )
	{
		this->x += dx;
		this->y += dy;
		this->z += dz;
	}
	
	float Point::getX() const
	{
		return this->x;
	}
	
	float Point::getY() const
	{
		return this->y;
	}
	
	float Point::getZ() const
	{
		return this->z;
	}
}
