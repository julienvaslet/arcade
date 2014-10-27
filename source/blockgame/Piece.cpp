#include <blockgame/Piece.h>
#include <tools/logger/Logger.h>

#include <cstdlib>
#include <ctime>

using namespace opengl;
using namespace tools::logger;
using namespace std;

namespace blockgame
{
	Piece::Piece() : Blocks( 4 )
	{
	}
	
	Piece::~Piece()
	{
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
	
	Piece * Piece::generate()
	{
		Piece * piece = new Piece();
		Point2D position( 5.0f, 19.0f );
		Color color( 0.0f, 0.0f, 0.0f );
		
		srand( time(NULL) );
		int index = rand() % 7;
		
		switch( index )
		{
			default:
			case 0:
			{
				// O
				color.setColor( "ff0000" );
				piece->insert( new Block( position, color ) );
				position.moveBy( -1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( 0.0f, -1.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( 1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				
				break;
			}
		
			case 1:
			{
				// I
				color.setColor( "00ff00" );
				piece->insert( new Block( position, color ) );
				position.moveBy( 1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( -2.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( -1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				
				break;
			}
			
			case 2:
			{
				// T
				color.setColor( "0000ff" );
				piece->insert( new Block( position, color ) );
				position.moveBy( -1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( 2.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( -1.0f, -1.0f );
				piece->insert( new Block( position, color ) );
				
				break;
			}
		
			case 3:
			{
				// J
				color.setColor( "ffff00" );
				piece->insert( new Block( position, color ) );
				position.moveBy( -1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( 2.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( 0.0f, -1.0f );
				piece->insert( new Block( position, color ) );
				
				break;
			}
		
			case 4:
			{
				// L
				color.setColor( "ff00ff" );
				piece->insert( new Block( position, color ) );
				position.moveBy( 1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( -2.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( 0.0f, -1.0f );
				piece->insert( new Block( position, color ) );
				
				break;
			}
			
			case 5:
			{
				// S
				color.setColor( "00ffff" );
				piece->insert( new Block( position, color ) );
				position.moveBy( 1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( -1.0f, -1.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( -1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				
				break;
			}
			
			case 6:
			{
				// Z
				color.setColor( "f0f0ff" );
				piece->insert( new Block( position, color ) );
				position.moveBy( -1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( 1.0f, -1.0f );
				piece->insert( new Block( position, color ) );
				position.moveBy( 1.0f, 0.0f );
				piece->insert( new Block( position, color ) );
				
				break;
			}
		}
		
		return piece;
	}
}

