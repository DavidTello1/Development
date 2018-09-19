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
	vec3(Type _x, Type _y, Type _z);
	vec3(const vec3 &vec);

	~vec3() {}

	//methods
	void normalize();
	void zero();
	bool is_zero();
	Type distance_to(const vec3 &vec);

	//operators
	vec3 op+ (Type sum);
	vec3 op+ (const vec3 &vec);

	vec3 op- (Type sum);
	vec3 op- (const vec3 &vec);

	void op+= (Type sum);
	void op+= (const vec3 &vec);

	void op-= (Type sum);
	void op-= (const vec3 &vec);

	void op= (const vec3 &vec);
	bool op== (const vec3 &vec);
};
#endif