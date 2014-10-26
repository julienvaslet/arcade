#include <blockgame/Piece.h>
#include <tools/logger/Logger.h>

using namespace opengl;
using namespace tools::logger;
using namespace std;

namespace blockgame
{
	Piece::Piece( unsigned int reserveBlocks ) : vertices(NULL), textureCoordinates(NULL), colors(NULL), indices(NULL)
	{
		if( reserveBlocks > 0 )
			this->blocks.reserve( reserveBlocks );
	}
	
	Piece::~Piece()
	{
		if( this->vertices != NULL )
			delete vertices;
			
		if( this->textureCoordinates != NULL )
			delete this->textureCoordinates;
			
		if( this->colors != NULL )
			delete this->colors;
			
		if( this->indices != NULL )
			delete this->indices;
			
		for( vector<Block *>::iterator it = this->blocks.begin() ; it != this->blocks.end() ; it++ )
			delete (*it);
	}

	float Piece::getX() const
	{
		if( this->blocks.size() > 0 )
			return this->blocks[0]->getPosition().getX();
		else
			return -1.0f;
	}
	
	float Piece::getY() const
	{
		if( this->blocks.size() > 0 )
			return this->blocks[0]->getPosition().getY();
		else
			return -1.0f;
	}
	
	void Piece::insert( Block * block )
	{
		this->blocks.push_back( block );
	}
	
	void Piece::insert( Piece * piece )
	{
		for( vector<Block *>::iterator it = piece->blocks.begin() ; it != piece->blocks.end() ; it++ )
			this->insert( *it );
		
		piece->blocks.clear();
	}
	
	void Piece::render()
	{
		if( this->blocks.size() > 0 )
		{
			vector<Point3D> vPoints;
			vector<Point2D> vTexCoords;
			vector<Color> vColors;
			vector<unsigned int> vIndices;
		
			// Adding blocks to buffer objects
			for( vector<Block *>::iterator it = this->blocks.begin() ; it != this->blocks.end() ; it++ )
				(*it)->prepareRendering( vPoints, vTexCoords, vColors, vIndices );
		
			Block::renderBlocks( vPoints, vTexCoords, vColors, vIndices );
		}
	}

	void Piece::moveTo( float x, float y )
	{
		if( this->blocks.size() > 0 )
			this->moveBy( x - this->blocks[0]->getPosition().getX(), y - this->blocks[0]->getPosition().getY() );
	}
	
	void Piece::moveBy( float dx, float dy )
	{
		for( vector<Block *>::iterator it = this->blocks.begin() ; it != this->blocks.end() ; it++ )
			(*it)->getPosition().moveBy( dx, dy );
	}
	
	void Piece::correctPosition( float width, float height )
	{
		if( this->blocks.size() > 0 )
		{
			vector<Block *>::iterator it = this->blocks.begin();
			
			float up = (*it)->getPosition().getY();
			float down = (*it)->getPosition().getY();
			float left = (*it)->getPosition().getX();
			float right = (*it)->getPosition().getX();
			
			it++;
		
			for( ; it != this->blocks.end() ; it++ )
			{
				if( (*it)->getPosition().getY() > up ) up = (*it)->getPosition().getY();
				else if( (*it)->getPosition().getY() < down ) down = (*it)->getPosition().getY();
				
				if( (*it)->getPosition().getX() < left ) left = (*it)->getPosition().getX();
				else if( (*it)->getPosition().getX() > right ) right = (*it)->getPosition().getX();
			}
			
			if( up >= height )
				this->moveBy( 0.0f, height - up - 1.0f );
			else if( down < 0.0f )
				this->moveBy( 0.0f, -1.0f * down );
			
			if( left < 0.0f )
				this->moveBy( -1.0f * left, 0.0f );
			else if( right >= width )
				this->moveBy( width - right - 1.0f, 0.0f );
		}
	}
	
	void Piece::rotate()
	{
	}
	
	bool Piece::isAtGround()
	{
		bool touchingGround = false;
		
		if( this->blocks.size() > 0 )
		{
			vector<Block *>::iterator it = this->blocks.begin();
			
			float down = (*it)->getPosition().getY();
			it++;
		
			for( ; it != this->blocks.end() ; it++ )
			{
				if( (*it)->getPosition().getY() < down )
					down = (*it)->getPosition().getY();
			}
			
			if( down < 0.0f )
				touchingGround = true;
		}
		
		return touchingGround;
	}
	
	bool Piece::isInCollision( Piece * piece )
	{
		bool hasCollision = false;
		
		if( this->blocks.size() > 0 )
		{
			for( vector<Block *>::iterator it1 = this->blocks.begin() ; it1 != this->blocks.end() ; it1++ )
			{
				for( vector<Block *>::iterator it2 = piece->blocks.begin() ; it2 != piece->blocks.end() ; it2++ )
				{
					if( (*it1)->getPosition().getX() == (*it2)->getPosition().getX() && (*it1)->getPosition().getY() == (*it2)->getPosition().getY() )
					{
						hasCollision = true;
						break;
					}
				}
				
				if( hasCollision )
					break;
			}
		}
		
		return hasCollision;
	}
	
	Piece * Piece::generate()
	{
		Piece * piece = new Piece( 4 );
		Point2D position( 5.0f, 19.0f );
		Color color( 1.0f, 0.0f, 0.0f );
		
		// Square (O)
		piece->insert( new Block( position, color ) );
		position.moveBy( -1.0f, 0.0f );
		piece->insert( new Block( position, color ) );
		position.moveBy( 0.0f, -1.0f );
		piece->insert( new Block( position, color ) );
		position.moveBy( 1.0f, 0.0f );
		piece->insert( new Block( position, color ) );
		
		return piece;
	}
}
