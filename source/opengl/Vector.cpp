#include <opengl/Vector.h>
#include <cmath>
#include <iostream>

namespace opengl
{
	Vector::Vector( float x, float y, float z ) : x(x), y(y), z(z)
	{
	}
	
	Vector::Vector( const Vector& vector ) : x(vector.x), y(vector.y), z(vector.z)
	{
	}
	
	Vector::~Vector()
	{
	}

	float Vector::getX()
	{
		return this->x;
	}
	
	float Vector::getY()
	{
		return this->y;
	}
	
	float Vector::getZ()
	{
		return this->z;
	}

	void Vector::show() const
	{
		std::cout << "[ " << this->x << "\t]" << std::endl;
		std::cout << "[ " << this->y << "\t]" << std::endl;
		std::cout << "[ " << this->z << "\t]" << std::endl;
	}

	float Vector::norm() const
	{
		return sqrt( this->x * this->x + this->y * this->y + this->z * this->z );
	}
	
	Vector& Vector::normalize()
	{
		*this /= this->norm();
		return *this;
	}

	Vector& Vector::operator*=( float n )
	{
		this->x *= n;
		this->y *= n;
		this->z *= n;
	
		return *this;
	}
	
	Vector& Vector::operator*=( const Vector& vector )
	{
		float x = this->x;
		float y = this->y;
		float z = this->z;
	
		this->x = y * vector.z - z * vector.y;
		this->y = z * vector.x - x * vector.z;
		this->z = x * vector.y - y * vector.x;
	
		return *this;
	}
	
	Vector Vector::operator*( const Vector& vector )
	{
		Vector v = *this;
		v *= vector;
	
		return v;
	}
	
	Vector& Vector::operator/=( float n )
	{
		this->x /= n;
		this->y /= n;
		this->z /= n;
	
		return *this;
	}
	
	Vector Vector::operator/( float n )
	{
		Vector v = *this;
		v /= n;
	
		return v;
	}	
}
