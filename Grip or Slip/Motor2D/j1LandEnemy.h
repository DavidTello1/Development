#ifndef __J1LAND_ENEMY_H__
#define __J1LAND_ENEMY_H__

#include "j1Entity.h"

class LandEnemy : public Entity
{
public:
	LandEnemy();
	LandEnemy(iPoint position);
	~LandEnemy();

	bool Update(float dt);
	void CleanUp();

	void Load(pugi::xml_node&);
	void Save(pugi::xml_node&) const;

	void followPath(float dt);

	void ChangeAnimation();
	void LoadAnimations();

public:
	p2DynArray<iPoint> entityPath;

	Animation		idle;
	Animation		attacking;
	Animation		damage;
	Animation		dying;
};

#endif // !__J1FLYING_ENEMY_H__