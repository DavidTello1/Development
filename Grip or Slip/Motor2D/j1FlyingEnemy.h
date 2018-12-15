#ifndef __J1FLYING_ENEMY_H__
#define __J1FLYING_ENEMY_H__

#include "j1Entity.h"

class FlyingEnemy : public Entity
{
public:
	FlyingEnemy();
	FlyingEnemy(iPoint position);
	~FlyingEnemy();

	bool Update(float dt);
	void CleanUp();

	void Load(pugi::xml_node&);
	void Save(pugi::xml_node&) const;

	void followPath();

public:

	Animation		idle;
	p2DynArray<iPoint> entityPath;
};

#endif // !__J1FLYING_ENEMY_H__