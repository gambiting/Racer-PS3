/******************************************************************************
Author:Rich Davison
Description: Random bits and pieces - too lazy to see if Sony have equivelents

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vectormath/cpp/vectormath_aos.h>

using namespace Vectormath::Aos;

//It's pi(ish)...
static const float		PI = 3.14159265358979323846f;	

//It's pi...divided by 360.0f!
static const float		PI_OVER_360 = PI / 360.0f;

//Radians to degrees
static inline double RadToDeg(const double rad)	{
	return rad * 180.0 / PI;
};

//Degrees to radians
static inline double DegToRad(const double deg)	{
	return deg * PI / 180.0;
};

static inline Matrix4 QuatToMatrix(Quat q) {
	Matrix4 mat;

	float w = q.getW();
	float y = q.getY();
	float x = q.getX();
	float z = q.getZ();

	float yy = y*y;
	float zz = z*z;
	float xy = x*y;
	float zw = z*w;
	float xz = x*z;
	float yw = y*w;
	float xx = x*x;
	float yz = y*z;
	float xw = x*w;

	mat.setRow(0,Vector4(1- 2*yy - 2*zz, 2*xy + 2*zw,2*xz - 2*yw,1));
	mat.setRow(1,Vector4(2*xy - 2*zw,	1- 2*xx - 2*zz,2*yz + 2*xw,1));
	mat.setRow(2,Vector4(2*xz + 2*yw,2*yz - 2*xw,1 - 2*xx - 2*yy,1));
	mat.setRow(3,Vector4(0,0,0,1));
	return mat;
}

//check Vector3s for equality
static bool vector3Equal(Vector3 lhs, Vector3 rhs){
	if( lhs.getX() == rhs.getX() && 
		lhs.getY() == rhs.getY() &&
		lhs.getZ() == rhs.getZ()){
		//all are equal
		return true;
	}else{
		return false;
	}
}

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))