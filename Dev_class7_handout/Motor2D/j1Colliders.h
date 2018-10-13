#ifndef __j1Colliders_H__
#define __j1Colliders_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"

class j1Colliders :public j1Module
{
public:
	enum ColliderType
	{
		NO_TYPE,
		PLAYER,
		SPIKES,
		FLOOR,
		CEILING,
		WALL,
		GRID
	};

public:
	j1Colliders();
	j1Colliders(ColliderType type);

	// Destructor
	virtual ~j1Colliders();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	//Called each loop iteration
	bool Update(float dt);

	//Detects if the player is colliding with something
	bool Collider_Overlay();

	// Called before quitting
	bool CleanUp();

	bool OnCollision(SDL_Rect cldr, SDL_Rect cldr_player);

	void PositionCollider();

public:
	int points;
	ColliderType type;
	iPoint ColliderOffset;
	iPoint SightOffset;
	SDL_Rect Collider;
	SDL_Rect Collider_floor;
};

#endif