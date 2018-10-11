#ifndef VEC3_H
#define VEC3_H

#include <cstdio>
#include <cstdlib>

using namespace std;

template <class Type> class vec3 {
private:
	Type x, y, z;

public:
	//constructors
	vec3();
	vec3(const Type &x, const Type &y, const Type &z) : x(x), y(y), z(z) {}
	vec3(const vec3 &vec) { this->x = vec.x, this->y = vec.y, this->z; }

	~vec3() {}

	//methods
	void normalize();
	void zero();
	bool is_zero();
	Type distance_to(const vec3 &vec);

	//operators
	vec3 op + (const vec3 &vec)const {
		return vec3(x + vec.x, y + vec.y, z + vec.z);
	}

	vec3 op - (const vec3 &vec)const {

	}

	void op+= (const vec3 &vec);

	void op-= (const vec3 &vec);

	void op= (const vec3 &vec);
	bool op== (const vec3 &vec);
};
#endif