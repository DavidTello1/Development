#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
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
	speed.x = config.child("speed").attribute("x").as_int();
	speed.y = config.child("speed").attribute("y").as_int();
	LOG("pos.x : %d, pos.y: %d, speed.x: %d, speed.y: %d", playerPos.x, playerPos.y);

	
	playerRect.h = 64;
	playerRect.w = 64;

	return ret;
}

bool j1Player::Start()
{

	graph = App->tex->Load("textures/penguin_player.png");
	

	/*
	//Sets the player in the start position
	for (p2List_item<ObjectsGroup*>* obj = App->map->data.objLayers.start; obj; obj = obj->next)
	{
		if (obj->data->name == ("Collisions"))
		{
			for (p2List_item<ObjectsData*>* objdata = obj->data->objects.start; objdata; objdata = objdata->next)
			{
				if (objdata->data->name == 6)
				{
					Collider.h = objdata->data->height;
					Collider.w = objdata->data->width;
					Collider.x = objdata->data->x;
					Collider.y = objdata->data->y;
				}
				else if (objdata->data->name == 4)
				{
					position = { objdata->data->x, objdata->data->y };
					Collider.x = position.x + colOffset.x;
					Collider.y = position.y + colOffset.y;
				}
			}
		}
	}
	*/
	return true;
}

bool j1Player::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		App->player->playerPos.y -= speed.y;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		App->player->playerPos.y += speed.y;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		App->player->playerPos.x -= speed.x;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		App->player->playerPos.x += speed.x;

	return true;
}


bool j1Player::PostUpdate()
{

	App->render->Blit(graph, playerPos.x, playerPos.y, &playerRect);
	LOG("Drawing Player");
	//CameraOnPlayer();

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



/*

bool j1Player::CameraOnPlayer()
{
	uint winWidth, winHeight;
	winWidth = 0;
	winHeight = 0;
	App->win->GetWindowSize(winWidth, winHeight);
	
	App->render->camera.x = playerPos.x - App->render->camera.w / 3;
	if (App->render->camera.x < 0)App->render->camera.x = 0;
	App->render->camera.y = playerPos.y - App->render->camera.h / 2;
	if (App->render->camera.y + winHeight > App->map->data.height*App->map->data.tile_height)App->render->camera.y = App->map->data.height*App->map->data.tile_height - winWidth;
	return true;
}

*/

bool j1Player::CleanUp()
{
	LOG("Deleting player");
	App->tex->UnLoad(graph);

	return true;

}
