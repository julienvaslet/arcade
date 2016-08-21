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
	
	bool Rectangle::isInCollision( const Rectangle& rectangle ) const
	{
		bool hasCollision = false;
		
		// X axis collision
		if( ( this->origin.getX() <= rectangle.origin.getX() && this->origin.getX() + this->width > rectangle.origin.getX() )
		 || ( rectangle.origin.getX() <= this->origin.getX() && rectangle.origin.getX() + rectangle.width > this->origin.getX() )
		 || ( this->origin.getX() >= rectangle.origin.getX() && this->origin.getX() + this->width <= rectangle.origin.getX() + rectangle.width ) )
		{
			// Y axis collision
			if( ( this->origin.getY() <= rectangle.origin.getY() && this->origin.getY() + this->height > rectangle.origin.getY() )
			 || ( rectangle.origin.getY() <= this->origin.getY() && rectangle.origin.getY() + rectangle.height > this->origin.getY() )
	 		 || ( this->origin.getY() >= rectangle.origin.getY() && this->origin.getY() + this->height <= rectangle.origin.getY() + rectangle.height ))
			{
				// Z axis collision
				// No depth for rectangle, simply compare Z axis
				if( this->origin.getZ() == rectangle.origin.getZ() )
					hasCollision = true;
			}
		}
		
		return hasCollision;
	}
	
	bool Rectangle::isInCollision( const Point2D& point ) const
	{
		bool hasCollision = false;
		
		if( point.getX() >= this->origin.getX() && point.getX() <= this->origin.getX()+ this->width )
		{
			if( point.getY() >= this->origin.getY() && point.getY() <= this->origin.getY() + this->height )
			{
				hasCollision = true;
			}
		}
		
		return hasCollision;
	}
	
	bool Rectangle::isInCollision( const Point3D& point ) const
	{
		bool hasCollision = false;
		
		// Rectangle has no depth, simply compare Z axis
		if( point.getZ() == this->origin.getZ() )
		{
			if( point.getX() >= this->origin.getX() && point.getX() <= this->origin.getX()+ this->width )
			{
				if( point.getY() >= this->origin.getY() && point.getY() <= this->origin.getY() + this->height )
				{
					hasCollision = true;
				}
			}
		}
		
		return hasCollision;
	}
	
	void Rectangle::prepareRendering( vector<Point3D>& vertices, vector<unsigned short int>& indices, bool clockwise ) const
	{
		unsigned short int j = static_cast<unsigned short int>( vertices.size() );
		 
		// Vertices
		vertices.push_back( Point3D( this->origin.getX() - this->anchor.getX(), this->origin.getY() - this->anchor.getY(), this->origin.getZ() ) );
		vertices.push_back( Point3D( this->origin.getX() - this->anchor.getX() + width, this->origin.getY() - this->anchor.getY(), this->origin.getZ() ) );
		vertices.push_back( Point3D( this->origin.getX() - this->anchor.getX() + width, this->origin.getY() - this->anchor.getY() + height, this->origin.getZ() ) );
		vertices.push_back( Point3D( this->origin.getX() - this->anchor.getX(), this->origin.getY() - this->anchor.getY() + height, this->origin.getZ() ) );

		// Indices		
		if( clockwise )
		{
			indices.push_back( j );
			indices.push_back( j + 1 );
			indices.push_back( j + 2 );
			indices.push_back( j );
			indices.push_back( j + 2 );
			indices.push_back( j + 3 );
		}
		else
		{
			indices.push_back( j );
			indices.push_back( j + 3 );
			indices.push_back( j + 2 );
			indices.push_back( j );
			indices.push_back( j + 2 );
			indices.push_back( j + 1 );
		}
	}
}

