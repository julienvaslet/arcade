#ifndef __LABOPHOTO_CROPTOOL_H
#define __LABOPHOTO_CROPTOOL_H	1

#include <opengl/ColoredRectangle.h>
#include <labophoto/Negative.h>

using namespace opengl;

namespace labophoto
{
	enum MouseCropPosition
	{
		Outside,
		Inside,
		NorthWest,
		North,
		NorthEast,
		East,
		SouthEast,
		South,
		SouthWest,
		West
	};
	
	class CropTool : public ColoredRectangle
	{
		protected:
			bool active;
			Negative * negative;
		
		public:
			CropTool( Negative * negative );
			virtual ~CropTool();
			
			bool isActive() const;
			void activate( bool status );
			
			MouseCropPosition getCropPosition( const Point2D& point, const Negative& negative );
			void render();
	};
}

#endif
