#ifndef __J1PLAYER_H__
#define __J1PLAYER_H__

#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_surface.h"

struct SDL_Texture;
struct ObjectsData;


class j1Player : public j1Module
{
	//-------------Functions-----------------
public:

	j1Player();

	// Destructor
	~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	//Called before the first frame
	bool Start();

	bool Update(float dt);
	bool PostUpdate();

	// Called each loop iteration

	bool Load(pugi::xml_node& file);
	bool Save(pugi::xml_node& file) const;

	// Called before quitting
	bool CleanUp();


	// Positions the player in the start of the current level
	 void Restart();


	// Centers the camera on the player
	bool CameraOnPlayer();

public: 
	bool godmode;

	iPoint playerPos;
	iPoint playerSpeed;
	SDL_Surface playerSize;
	iPoint jumpSpeed;
	int gravity;

	bool left;
	bool right;

	bool dead;

	bool grounded;
	bool jumping;
	bool sliding;
	bool grid;

	bool wall_left, wall_right;
	bool gravity_active;
	bool grid_collision;

	SDL_Rect playerCollider;

private:

	// Player graphs
	SDL_Texture* graph = nullptr;

	SDL_Rect playerRect;
	p2SString playerFile;

	pugi::xml_document	player_file;
};

#endif // !__J1PLAYER_H__