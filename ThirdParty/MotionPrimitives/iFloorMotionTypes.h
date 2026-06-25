#ifndef _IFLOORMOTIONTYPES_H
#define _IFLOORMOTIONTYPES_H

//----------------------------------------------------------
// iFloorPoint
//----------------------------------------------------------
class iFloorPoint
{
public:
	iFloorPoint(float _x=0.0f, float _y=0.0f, float _z=0.0f) ;
	iFloorPoint(const iFloorPoint & pnt);
	void set(float _x, float _y, float _z = 0);
	//------ Operators:
	//Negative
	iFloorPoint operator-() const;

	//equality
	bool operator==(const iFloorPoint& pnt);

	//inequality
	bool operator!=(const iFloorPoint& pnt);

	//Set
	iFloorPoint & operator=(const iFloorPoint& pnt);
	iFloorPoint & operator=(const float& val);

	// Add
	iFloorPoint operator+(const iFloorPoint& pnt) const;
	iFloorPoint operator+(const float& val) const;

	iFloorPoint & operator+=(const iFloorPoint& pnt);
	iFloorPoint & operator+=(const float & val);

	// Subtract
	iFloorPoint operator-(const iFloorPoint& pnt) const;
	iFloorPoint operator-(const float& val) const;
	iFloorPoint & operator-=(const iFloorPoint& pnt);
	iFloorPoint & operator-=(const float & val);

	// Multiply
	iFloorPoint operator*(const iFloorPoint& pnt) const;
	iFloorPoint operator*(const float& val) const;
	iFloorPoint & operator*=(const iFloorPoint& pnt);
	iFloorPoint & operator*=(const float & val);


	// Divide
	iFloorPoint operator/(const iFloorPoint& pnt) const;
	iFloorPoint operator/(const float &val) const;
	iFloorPoint& operator/=(const iFloorPoint& pnt);
	iFloorPoint& operator/=(const float &val);
	// union allows us to access the coordinates through
	// both an array 'v' and 'x', 'y', 'z' member varibles
#pragma warning (disable: 4201)
	union  {
		struct {
			float x;
			float y;
			float z;
		};
		float v[3];
	};
#pragma warning (default: 4201)
};

//----------------------------------------------------------
// iFloorRectangle
//----------------------------------------------------------

class iFloorRectangle
{
public:
	iFloorRectangle();
	iFloorRectangle(float _x, float _y, float _w, float _h);

	float x;
	float y;
	float width;
	float height;
};



//----------------------------------------------------------
// iFloorColor
//----------------------------------------------------------

class iFloorColor
{
public:
	iFloorColor();
	virtual ~iFloorColor();
	float r, g, b, a;
};

#endif // _IFLOORMOTIONTYPES_H
