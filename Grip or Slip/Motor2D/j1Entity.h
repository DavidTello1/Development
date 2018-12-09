#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "p2Point.h"
#include "p2Log.h"
#include "p2DynArray.h"
#include "p2Animation.h"
#include "j1Map.h"
#include "j1App.h"


class Entity
{
public:
	enum entityType
	{
		NO_TYPE,
		GRID,
		BOX,
		PLAYER,
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

	virtual void CleanUp() {};
	virtual void Save(pugi::xml_node& file) const {};
	virtual void Load(pugi::xml_node& file) {};
	virtual void Restart() {};
	virtual void LoadAnimations() {};
	virtual void ChangeAnimation() {};
	
	void Collider_Overlay();
	void PositionCollider();
	void CheckBounds();
	void Draw(float dt);

public:
	Animation* Current_Animation = nullptr;
	entityType type;
	
	iPoint position;
	iPoint size;
	iPoint speed;
	fPoint final_speed;
	fPoint current_speed;
	int direction; // 1 right, -1 left
	int gravity;
	int lives;

	SDL_Rect Collider;
	SDL_Rect SightCollider;
	
	p2DynArray<iPoint> entityPath;

	bool flip = false;
	bool chasing_player = false;
	bool hurt;

	bool moving_left = false;
	bool moving_right = true;

	int jumpSpeed;
	bool left;
	bool right;
	bool grid_moving;
	bool dead;
	bool grounded;
	bool jumping;
	bool grid;
	bool sliding;
	bool landing;
	bool falling;
	bool gripping;
	bool wall_left, wall_right;
	bool gravity_active;
	bool grid_collision;
	bool top_grid;
	bool ceiling;
	bool landed;
	bool attack;
	bool attack_able;
	bool box_collision;
	bool gripped;
	iPoint grid_speed;
	iPoint box_speed;
	int grid_direction;
	int box_direction;

	p2SString grid_type;
	iPoint initial_size;
	iPoint prev_size;
	int frame = 0;
	SDL_Rect rect;
	bool flip_ver = false;
	bool flip_hor = false;
	bool vertical;
	bool is_static;
	int type_int;

	bool box_moving;
	bool end_moving = false;
	SDL_Rect Bounds;
};

#endif