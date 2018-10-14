#ifndef __j1Colliders_H__
#define __j1Colliders_H__

#include "p2Point.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1App.h"

class j1Colliders :public j1Module
{

public:
	j1Colliders();

	// Destructor
	virtual ~j1Colliders();

	//Called each loop iteration
	bool Update(float dt);

	//Detects if the player is colliding with something
	bool Collider_Overlay();

	// Called before quitting
	bool CleanUp();

public:
	int points;
	SDL_Rect Collider;
};

#endif