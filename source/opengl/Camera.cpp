#include <opengl/Camera.h>
#include <opengl/Matrix.h>

#include <SDL2/SDL.h>
#include <iostream>
#include <GL/glu.h>

using namespace std;

namespace opengl
{
	Camera::Camera() : eye(0.0f,0.0f,0.0f), center(0.0f,0.0f,-10.0f), up(0.0f,1.0f,0.0f)
	{
	}
	
	Camera::~Camera()
	{
	}
	
	Point& Camera::getEye()
	{
		return this->eye;
	}
	
	Point& Camera::getCenter()
	{
		return this->center;
	}
	
	Vector& Camera::getUp()
	{
		return this->up;
	}
	
	void Camera::look() const
	{
		Matrix view = Matrix::lookAt( this->eye.getX(), this->eye.getY(), this->eye.getZ(), this->center.getX(), this->center.getY(), this->center.getZ(), this->up.getX(), this->up.getY(), this->up.getZ() );
	
		glMatrixMode( GL_MODELVIEW );
		view.load();
	}
	
	void Camera::setOrthogonal( float left, float right, float bottom, float top, float near, float far )
	{
		Matrix ortho = Matrix::ortho( left, right, bottom, top, near, far );
		
		glMatrixMode( GL_PROJECTION );
		ortho.load();
	}
	
	void Camera::setPerspective( float fovy, float aspect, float near, float far )
	{
		Matrix perspective = Matrix::perspective( fovy, aspect, near, far );
		
		glMatrixMode( GL_PROJECTION );
		perspective.load();
	}
	
	void Camera::setFrustum( float left, float right, float bottom, float top, float near, float far )
	{
		Matrix frustum = Matrix::frustum( left, right, bottom, top, near, far );
		
		glMatrixMode( GL_PROJECTION );
		frustum.load();
	}
}
