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
	gravity = config.child("gravity").attribute("value").as_int();
	gravity_active = config.child("gravity_active").attribute("value").as_bool();
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
	playerCollider.x = playerPos.x;
	playerCollider.y = playerPos.y;
	playerCollider.w = playerSize.w;
	playerCollider.h = playerSize.h;

	App->collider->Collider_Overlay();

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) //space
	{	
		if (jumping == false)
		{
			jumping = true;
			grounded = false;
			grid = false;
			jumpSpeed.y = playerSpeed.y;
		}
	} 

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) //left
	{
		if (grid == true)
		{
			playerPos.x -= playerSpeed.x / 2;
		}
		left = true;
		if (playerPos.x - playerSpeed.x <= 0) {
			playerPos.x = 0;
		}
		else
		{
			playerPos.x -= playerSpeed.x;
		}
		if (wall_left == true && grounded == false) {
			sliding = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) //right
	{
		if (grid == true) {
			playerPos.x += playerSpeed.x / 2;
		}
		else
		{
			right = true;
			playerSpeed.x = playerSpeed.x;

			if (playerPos.x + playerSpeed.x >= (App->map->data.width - 1) * App->map->data.tile_width) {
				playerPos.x = (App->map->data.width - 1) * App->map->data.tile_width;
			}
			else
			{
				playerPos.x += playerSpeed.x;
			}
			if (wall_right == true && grounded == false) {
				sliding = true;
			}
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) //left release
	{ 
		left = false;
		sliding = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) //right release
	{
		right = false;
		sliding = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) //up
	{
		if (grid_collision == true) 
		{
			if (grid == false)
			{
				grid = true;
			}
		}
		if (grid == true)
		{
			playerPos.y -= playerSpeed.x / 2;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) //down
	{
		if (grid == true)
		{
			playerPos.y += playerSpeed.x / 2;
		}
	} 



	//----------------	
	if (grounded == true) //grounded
	{
		jumping = false;
		sliding = false;
	}

	if (grid_collision == false)
	{
		grid = false;
		gravity_active = true;
	}

	if (grid == true)
	{
		gravity_active = false;
		grounded = false;
		sliding = false;
		jumping = false;
	}

	if (sliding == true)
	{
		jumpSpeed.y = 0;
		grounded = false;
		grid = false;
	}

	if (jumping == true) //jumping
	{
		gravity_active = true;
		if (playerPos.y - jumpSpeed.y <= 0)
		{
			playerPos.y = 0;
		}
		if (jumpSpeed.y > 0)
		{
			jumpSpeed.y--;
			playerPos.y -= jumpSpeed.y;
		}
	}

	if (wall_left == true) {
		playerPos.x += playerSpeed.x;
		wall_left = false;
	}
	if (wall_right == true) {
		playerPos.x -= playerSpeed.x;
		wall_right = false;
	}

	if (gravity_active == true) //gravity
	{
		if (playerPos.y + gravity >= (App->map->data.height - 1) * App->map->data.tile_height) {
			playerPos.y = (App->map->data.height - 1) * App->map->data.tile_height;
		}
		else
		{
			if (grounded == false && grid == false)
			{
				if (sliding == true)
				{
					playerPos.y += gravity / 4;
				}
				if (sliding == false)
				{
					playerPos.y += gravity;
				}
			}
		}
	}

	sliding = false;
	grounded = false;
	grid_collision = false;
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
	bool ret = true;

	playerSize.w = data.child("size").attribute("width").as_int();
	playerSize.h = data.child("size").attribute("height").as_int();
	playerPos.x = data.child("position").attribute("x").as_int();
	playerPos.y = data.child("position").attribute("y").as_int();
	gravity = data.child("gravity").attribute("value").as_int();
	playerSpeed.x = data.child("speed").attribute("x").as_int();
	playerSpeed.y = data.child("speed").attribute("y").as_int();
	grounded = data.child("grounded").attribute("value").as_bool();
	sliding = data.child("sliding").attribute("value").as_bool();
	jumping = data.child("jumping").attribute("value").as_bool();
	grid = data.child("grid").attribute("value").as_bool();
	gravity_active = data.child("gravity_active").attribute("value").as_bool();

	playerRect.x = 0;
	playerRect.y = 0;
	playerRect.h = playerSize.h;
	playerRect.w = playerSize.w;

	LOG("--- Player Loaded");
	return ret;
}

// Save Game State
bool j1Player::Save(pugi::xml_node& data) const
{
	bool ret = true;
	data.append_child("player");

	data.child("player").append_child("size").append_attribute("width") = playerSize.w;
	data.child("player").append_child("size").append_attribute("height") = playerSize.h;
	data.child("player").append_child("position").append_attribute("x") = playerPos.x;
	data.child("player").append_child("position").append_attribute("y") = playerPos.y;
	data.child("player").append_child("gravity").append_attribute("value") = gravity;
	data.child("player").append_child("speed").append_attribute("x") = playerSpeed.x;
	data.child("player").append_child("speed").append_attribute("y") = playerSpeed.y;
	data.child("player").append_child("grounded").append_attribute("value") = grounded;
	data.child("player").append_child("sliding").append_attribute("value") = sliding;
	data.child("player").append_child("jumping").append_attribute("value") = jumping;
	data.child("player").append_child("grid").append_attribute("value") = grid;
	data.child("player").append_child("gravity_active").append_attribute("value") = gravity_active;

	LOG("---Player Saved");
	return ret;
}

bool j1Player::CameraOnPlayer()
{
	uint winWidth = 0, winHeight = 0;
	App->win->GetWindowSize(winWidth, winHeight); // w = 1024, h = 768
	SDL_Rect mapSize;
	App->render->GetViewPort(mapSize);

	App->render->camera.x = playerPos.x - winWidth / 2 + playerRect.w / 2;
	App->render->camera.y = playerPos.y - winHeight / 2 + playerRect.h / 2;

	if (App->render->camera.x < mapSize.x) // left limit
	{ 
		App->render->camera.x = mapSize.x;
	}
	if (App->render->camera.x + mapSize.w > (App->map->data.width) * App->map->data.tile_width) //right limit
	{
		App->render->camera.x = App->map->data.width * App->map->data.tile_width - mapSize.w;
	}

	if (App->render->camera.y < mapSize.y) //up limit
	{
		App->render->camera.y = mapSize.y;
	}	
	if (App->render->camera.y + mapSize.h > (App->map->data.height) * App->map->data.tile_height) //down limit
	{
		App->render->camera.y = App->map->data.height * App->map->data.tile_height - mapSize.h;
	}

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Deleting player");
	App->tex->UnLoad(graph);

	return true;

}
