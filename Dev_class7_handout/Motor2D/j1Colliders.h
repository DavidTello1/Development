#ifndef __j1COLLIDERS_H__
#define __j1COLLIDERS_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Module.h"

class Colliders :public j1Module
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
	Colliders();
	Colliders(ColliderType type);

	// Destructor
	virtual ~Colliders();

	// Called before render is available
	bool Awake(pugi::xml_node & config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	//Detects if the player's collider is going to collide in the next frame and acts in consequence
	fPoint Collider_Overlay(fPoint originalvec);

	//Called if the player's collider is going to collide with unpassable terrain, adjusts the player's speed to avoid it
	fPoint AvoidCollision(fPoint newvec, const SDL_Rect result, p2List_item<ObjectsData*>* objdata);

	//Returns the rect of the Object passed using its data
	SDL_Rect CreateRect_FromObjectData(ObjectsData* data);

	void PositionCollider();
	void Draw(float dt);

public:
	int points;
	ColliderType type;
	fPoint speed;
	iPoint position;
	iPoint maxSpeed;
	float gravity;
	int direction_x = 1; //-1 = left, 1 = right
	iPoint ColliderOffset;
	iPoint SightOffset;
	SDL_Rect Collider;
	SDL_Rect SightCollider;
	bool dead = false;
	bool grounded = false;
	bool sliding = false;
	bool grid = false;
};

#endif