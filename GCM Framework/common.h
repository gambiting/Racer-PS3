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