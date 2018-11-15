#ifndef __j1FlyingEnemyH__
#define __j1FlyingEnemyH__
#include "j1Enemy.h"


class FlyingEnemy : public Enemy
{
public:
	FlyingEnemy();
	FlyingEnemy(iPoint position);
	~FlyingEnemy();

	bool Update(float dt);
	bool Start();
	bool Move();
private:

	void LoadPushbacks();
	void CleanUp();
public:


private:

	SDL_Texture* texture = nullptr;

	Animation idle;
	Animation moving;
	Animation attacking;
};

#endif