#include "iFloorMotionTypes.h"

iFloorPoint::iFloorPoint(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

iFloorPoint::iFloorPoint(const iFloorPoint & pnt)
{
	x = pnt.x;
	y = pnt.y;
	z = pnt.z;
}

void iFloorPoint::set(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}


//------ Operators:

//Negative
iFloorPoint iFloorPoint::operator-() const
{
	return iFloorPoint(-x, -y, -z);
}

//equality
bool iFloorPoint::operator==(const iFloorPoint& pnt)
{
	return (x == pnt.x) && (y == pnt.y) && (z == pnt.z);
}

//inequality
bool iFloorPoint::operator!=(const iFloorPoint& pnt)
{
	return (x != pnt.x) || (y != pnt.y) || (z != pnt.z);
}

//Set
iFloorPoint & iFloorPoint::operator=(const iFloorPoint& pnt)
{
	x = pnt.x;
	y = pnt.y;
	z = pnt.z;
	return *this;
}

iFloorPoint & iFloorPoint::operator=(const float& val)
{
	x = val;
	y = val;
	z = val;
	return *this;
}

// Add
iFloorPoint iFloorPoint::operator+(const iFloorPoint& pnt) const
{
	return iFloorPoint(x+pnt.x, y+pnt.y, z+pnt.z);
}

iFloorPoint iFloorPoint::operator+(const float& val) const
{
	return iFloorPoint(x+val, y+val, z+val);
}

iFloorPoint & iFloorPoint::operator+=(const iFloorPoint& pnt)
{
	x+=pnt.x;
	y+=pnt.y;
	z+=pnt.z;
	return *this;
}

iFloorPoint & iFloorPoint::operator+=(const float & val)
{
	x+=val;
	y+=val;
	z+=val;
	return *this;
}

// Subtract
iFloorPoint iFloorPoint::operator-(const iFloorPoint& pnt) const
{
	return iFloorPoint(x-pnt.x, y-pnt.y, z-pnt.z);
}

iFloorPoint iFloorPoint::operator-(const float& val) const
{
	return iFloorPoint(x-val, y-val, z-val);
}

iFloorPoint & iFloorPoint::operator-=(const iFloorPoint& pnt)
{
	x -= pnt.x;
	y -= pnt.y;
	z -= pnt.z;
	return *this;
}

iFloorPoint & iFloorPoint::operator-=(const float & val)
{
	x -= val;
	y -= val;
	z -= val;
	return *this;
}

// Multiply
iFloorPoint iFloorPoint::operator*(const iFloorPoint& pnt) const
{
	return iFloorPoint(x*pnt.x, y*pnt.y, z*pnt.z);
}

iFloorPoint iFloorPoint::operator*(const float& val) const
{
	return iFloorPoint(x*val, y*val, z*val);
}

iFloorPoint & iFloorPoint::operator*=(const iFloorPoint& pnt)
{
	x*=pnt.x;
	y*=pnt.y;
	z*=pnt.z;
	return *this;
}

iFloorPoint & iFloorPoint::operator*=(const float & val)
{
	x*=val;
	y*=val;
	z*=val;
	return *this;
}


// Divide
iFloorPoint iFloorPoint::operator/(const iFloorPoint& pnt) const
{
	return iFloorPoint(pnt.x!=0 ? x/pnt.x : x , pnt.y!=0 ? y/pnt.y : y, pnt.z!=0 ? z/pnt.z : z);
}

iFloorPoint iFloorPoint::operator/(const float &val) const
{
	if (val != 0)
	{
		return iFloorPoint(x/val, y/val, z/val);
	}
	return iFloorPoint(x, y, z);
}

iFloorPoint& iFloorPoint::operator/=(const iFloorPoint& pnt)
{
	pnt.x!=0 ? x/=pnt.x : x;
	pnt.y!=0 ? y/=pnt.y : y;
	pnt.z!=0 ? z/=pnt.z : z;

	return *this;
}

iFloorPoint& iFloorPoint::operator/=(const float &val)
{
	if (val != 0)
	{
		x /= val;
		y /= val;
		z /= val;
	}

	return *this;
}

iFloorRectangle::iFloorRectangle()
{
	x = y = width = height = 0;
}

iFloorRectangle::iFloorRectangle(float _x, float _y, float _w, float _h)
{
	x = _x;
	y = _y;
	width = _w;
	height = _h;
}

iFloorColor::iFloorColor()
{
	r = 255;
	g = 255;
	b = 255;
	a = 255;
}

iFloorColor::~iFloorColor() {}
