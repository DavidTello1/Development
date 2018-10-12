#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Colliders.h"
#include "p2Log.h"


j1Player::j1Player() : j1Module()
{
	name.create("player");
}

j1Player::~j1Player()
{
}

bool j1Player::Awake(pugi::xml_node & config)
{
	bool ret = true;

	// Player starting point
	playerPos.x = config.child("position").attribute("x").as_int();
	playerPos.y = config.child("position").attribute("y").as_int();
	playerSpeed.x = config.child("speed").attribute("x").as_int();
	playerSpeed.y = config.child("speed").attribute("y").as_int();
	playerSize.w = config.child("size").attribute("width").as_int();
	playerSize.h = config.child("size").attribute("height").as_int();
	LOG("pos.x : %d, pos.y: %d, speed.x: %d, speed.y: %d", playerPos.x, playerPos.y, playerSpeed.x, playerSpeed.y);

	playerRect.x = 0;
	playerRect.y = 0;
	playerRect.h = playerSize.h;
	playerRect.w = playerSize.w;

	return ret;
}

bool j1Player::Start()
{
	graph = App->tex->Load("textures/penguin_player.png");
	
	//Sets the player in the start position
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("Collision"))
		{
			for (p2List_item<ObjectsData*>* objectdata = obj->data->objects.start; objectdata; objectdata = objectdata->next)
			{
				if (objectdata->data->name == "StartPosition")
				{
					playerPos.x = objectdata->data->x;
					playerPos.y = objectdata->data->y;
				}
			}
		}
	}

	return true;
}

bool j1Player::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) //up
	{
		if (jumping == false) {
			jumping = true;
			speed_y = 22;
		}
		
	}
	if (jumping == true)
	{
		playerPos.y -= speed_y;
		speed_y -= 2;
		if (speed_y <= -24) {
			speed_y = 0;
			jumping = false;
		}
	}
	

	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) //up
	//{
	//	up = true;
	//	if (playerPos.y - playerSpeed.y <= 0) {
	//		playerPos.y = 0;
	//	}
	//	else 
	//	{
	//		playerPos.y -= playerSpeed.y;
	//	}
	//}

	//if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) //down
	//{
	//	down = true;
	//	if (playerPos.y + playerSpeed.y >= (App->map->data.height -1) * App->map->data.tile_height) {
	//		playerPos.y = (App->map->data.height -1) * App->map->data.tile_height;
	//	}
	//	else
	//	{
	//		playerPos.y += playerSpeed.y;
	//	}
	//}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) //left
	{
		left = true;
		if (playerPos.x - playerSpeed.x <= 0) {
			playerPos.x = 0;
		}
		else
		{
			playerPos.x -= playerSpeed.x;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) //right
	{
		right = true;
		if (playerPos.x + playerSpeed.x >= (App->map->data.width -1) * App->map->data.tile_width) {
			playerPos.x = (App->map->data.width -1) * App->map->data.tile_width;
		}
		else
		{
			playerPos.x += playerSpeed.x;
		}
	}
	App->collider->Collider_Overlay();

	CameraOnPlayer();
	return true;
}


bool j1Player::PostUpdate()
{
	App->render->Blit(graph, playerPos.x, playerPos.y, &playerRect);

	return true;
}



bool j1Player::Load(pugi::xml_node& data)
{
	return false;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	data.append_child("player");

	data.child("player").append_child("position").append_attribute("x") = playerPos.x;
	data.child("player").child("position").append_attribute("y") = playerPos.y;
	
	return true;
}


bool j1Player::CameraOnPlayer()
{
	// Camera Position Update
	App->render->camera.x = App->win->screen_surface->w / 2 - playerPos.x - playerRect.w / 2;
	App->render->camera.y = App->win->screen_surface->h / 2 - playerPos.y - playerRect.h / 2;

	// Left Limit
	if (App->render->camera.x > 0) App->render->camera.x = 0;
	
	// Top Limit
	if (App->render->camera.y > 0) App->render->camera.y = 0;
	
	// Right Limit


	//Bottom Limit


	return true;
}

bool j1Player::CleanUp()
{
	LOG("Deleting player");
	App->tex->UnLoad(graph);

	return true;

}
