#include "vec3.h"
#include <iostream>
#include <cmath>

using namespace std;

template<class Type>
inline vec3<Type>::vec3()
{
}

template<class Type>
vec3<Type>::vec3(Type _x, Type _y, Type _z)
{
	x = _x;
	y = _y;
	z = _z;
}

template<class Type>
vec3<Type>::vec3(const vec3 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

template<class Type>
void vec3<Type>::normalize()
{
	Type mod = sqrt(this->x ^ 2 + this->y ^ 2 + this->z ^ 2);
	this->x = x / mod;
	this->y = y / mod;
	this->z = z / mod;
}

template<class Type>
void vec3<Type>::zero()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

template<class Type>
bool vec3<Type>::is_zero()
{
	if (this->x == 0 && this->y == 0 && this->z == 0) { return true;}
	return false;
}

template<class Type>
Type vec3<Type>::distance_to(const vec3 &vec)
{
	
	return Type(sqrt((vec.x - this->x) ^ 2 + (vec.y - this->y) ^ 2 + (vec.z - this->z) ^ 2));
}


template<class Type>
vec3 vec3<Type>::op+(Type sum)
{
	return vec3<Type>(x + sum, y + sum, z + sum);
}
template<class Type>
vec3 vec3<Type>::op+(const vec3 &vec)
{
	return vec3<Type>(x + vec.x, y + vec.y, z + vec.z);
}

template<class Type>
vec3 vec3<Type>::op- (Type sum)
{
	return vec3<Type>(x - sum, y - sum, z - sum);
}
template<class Type>
vec3 vec3<Type>::op- (const vec3 &vec)
{
	return vec3<Type>(x - vec.x, y - vec.y, z - vec.z);
}

template<class Type>
void vec3<Type>::op += (Type sum) 
{
	x += sum;
	y += sum;
	z += sum;
}
template<class Type>
void vec3<Type>::op += (const vec3 &vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
}

template<class Type>
void vec3<Type>::op-= (Type sum)
{
	x -= sum;
	y -= sum;
	z -= sum;
}
template<class Type>
void vec3<Type>::op-= (const vec3 &vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}

void vec3<Type>::op= (const vec3 &vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}
void vec3<Type>::op == (const vec3 &vec) {
	if (x == vec.x && y == vec.y && z == vec.z) {return true;}
	return false;
}
