#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2Log.h"
#include "p2Animation.h"
#include "j1Map.h"
#include "j1App.h"
#include "SDL\include\SDL_rect.h"


class Entity
{
public:
	enum entityType
	{
		NO_TYPE,
		PLAYER,
		MOVING_GRID,
		BOX,
		FLYING_ENEMY,
		LAND_ENEMY,
		BOSS
	};

public:
	Entity();
	Entity(entityType type);
	~Entity();
	virtual bool Awake(pugi::xml_node & config) { return true; };
	virtual bool Start() { return true; };
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool Move() { return true; };

	virtual void CleanUp() {};
	virtual void Save(pugi::xml_node& file) const {};
	virtual void Load(pugi::xml_node& file) {};
	virtual void Restart() {};

	
	void Collider_Overlay(); //Detects if the player is colliding with something
	void PositionCollider();
	void Draw(float dt);

public:
	entityType type;
	
	iPoint position;
	iPoint size;
	iPoint speed;
	iPoint jumpSpeed;
	int gravity;

	int direction_x = 1; //-1 = left, 1 = right

	SDL_Rect Collider;
	SDL_Rect SightCollider;

	bool dead = true;
	bool isDying = false;
	bool grounded = false;
	bool flip = false;
	bool chasing_player = false;

	bool jumping;
	bool sliding;
	bool grid;

	bool wall_left, wall_right;
	bool gravity_active;
	bool grid_collision;
	bool top_grid;
	bool ceiling;

	float animationSpeed = 3;
	Animation* Current_Animation = nullptr;
};

#endif