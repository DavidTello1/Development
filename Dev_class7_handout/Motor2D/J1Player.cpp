#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1SceneChange.h"
#include "j1EntityController.h"
#include "p2Log.h"


j1Player::j1Player() : Entity(entityType::PLAYER)
{
}

j1Player::~j1Player()
{
}

bool j1Player::Awake(pugi::xml_node & config)
{
	LOG("Loading Player");
	bool ret = true;

	// Player starting point
	config = config.child("player");

	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();
	size.x = config.child("size").attribute("width").as_int();
	size.y = config.child("size").attribute("height").as_int();
	speed.x = config.child("speed").attribute("x").as_int();
	speed.y = config.child("speed").attribute("y").as_int();
	gravity = config.child("gravity").attribute("value").as_int();
	gravity_active = config.child("gravity_active").attribute("value").as_bool();

	return ret;
}

bool j1Player::Start()
{	
	LoadAnimation();
	Current_Animation = &idle;

	//Sets the player in the start position
	for (p2List_item<ObjectsGroup*>* object = App->map->data.objLayers.start; object; object = object->next)
	{
		if (object->data->name == ("Collision"))
		{
			for (p2List_item<ObjectsData*>* objectdata = object->data->objects.start; objectdata; objectdata = objectdata->next)
			{
				if (objectdata->data->name == "StartPosition")
				{
					position.x = objectdata->data->x;
					position.y = objectdata->data->y;
				}
			}
		}
	}

	return true;
}

bool j1Player::Update(float dt)
{
	if (!dead)
	{
		PositionCollider();
		Collider_Overlay();

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) //space
		{
			if (jumping == false)
			{
				jumping = true;
				grounded = false;
				grid = false;
				jumpSpeed.y = speed.y;
			}
			if (App->scene->godmode == true)
			{
				jumpSpeed.y = speed.y;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) //left
		{
			if (grid == true)
			{
				position.x -= speed.x / 2;
			}
			else
			{
				left = true;
				if (position.x - speed.x <= 0) {
					position.x = 0;
				}
				else
				{
					position.x -= speed.x;
				}
				if (wall_left == true && grounded == false) {
					sliding = true;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) //right
		{
			if (grid == true) {
				position.x += speed.x / 2;
			}
			else
			{
				right = true;
				speed.x = speed.x;

				if (position.x + speed.x >= (App->map->data.width - 1) * App->map->data.tile_width) {
					position.x = (App->map->data.width - 1) * App->map->data.tile_width;
				}
				else
				{
					position.x += speed.x;
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
					jumping = false;
				}
			}
			if (grid == true && ceiling == false)
			{
				if (top_grid == false)
				{
					position.y -= speed.x / 2;
				}
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) //down
		{
			if (grid == true)
			{
				position.y += speed.x / 2;
			}
		}



		//----------------	
		if (grounded == true) //grounded
		{
			jumping = false;
			sliding = false;
		}

		if (grid_collision == false) //grid collision
		{
			grid = false;
			gravity_active = true;
		}

		if (grid == true) //grid
		{
			gravity_active = false;
			grounded = false;
			sliding = false;
		}

		if (sliding == true) //sliding
		{
			jumpSpeed.y = 0;
			grounded = false;
			grid = false;
		}

		if (jumping == true) //jumping
		{
			gravity_active = true;
			if (position.y - jumpSpeed.y <= 0)
			{
				position.y = 0;
			}
			if (jumpSpeed.y > 0)
			{
				jumpSpeed.y--;
				position.y -= jumpSpeed.y;
			}
		}

		if (ceiling == true)
		{
			jumpSpeed.y = 0;
		}

		if (wall_left == true) //wall left
		{
			position.x += speed.x;
			wall_left = false;
		}

		if (wall_right == true) //wall right
		{
			position.x -= speed.x;
			wall_right = false;
		}

		if (gravity_active == true) //gravity
		{
			if (position.y + gravity >= (App->map->data.height - 1) * App->map->data.tile_height) 
			{
				position.y = (App->map->data.height - 1) * App->map->data.tile_height;
			}
			else
			{
				if (grounded == false && grid == false)
				{
					if (sliding == true)
					{
						position.y += gravity / 4;
					}
					if (sliding == false)
					{
						position.y += gravity;
					}
				}
			}
		}

		sliding = false;
		grounded = false;
		grid_collision = false;
		top_grid = false;
		ceiling = false;
	}
	ChangeAnimation();

	return true;
}


bool j1Player::PostUpdate()
{
	if (dead && !App->scenechange->IsChanging())
	{
		App->scenechange->ChangeMap(App->scene->currentMap, App->scene->fade_time);
	}

	CameraOnPlayer();

	return true;
}



void j1Player::Load(pugi::xml_node& data)
{
	size.x = data.child("size").attribute("width").as_int();
	size.y = data.child("size").attribute("height").as_int();
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	gravity = data.child("gravity").attribute("value").as_int();
	speed.x = data.child("speed").attribute("x").as_int();
	speed.y = data.child("speed").attribute("y").as_int();
	grounded = data.child("grounded").attribute("value").as_bool();
	sliding = data.child("sliding").attribute("value").as_bool();
	jumping = data.child("jumping").attribute("value").as_bool();
	grid = data.child("grid").attribute("value").as_bool();
	gravity_active = data.child("gravity_active").attribute("value").as_bool();
	
	LOG("--- Player Loaded");
}

// Save Game State
void j1Player::Save(pugi::xml_node& data) const
{
	data.append_child("size").append_attribute("width") = size.x;
	data.append_child("size").append_attribute("height") = size.y;
	data.append_child("position").append_attribute("x") = position.x;
	data.child("position").append_attribute("y") = position.y;
	data.append_child("gravity").append_attribute("value") = gravity;
	data.append_child("speed").append_attribute("x") = speed.x;
	data.child("speed").append_attribute("y") = speed.y;
	data.append_child("grounded").append_attribute("value") = grounded;
	data.append_child("sliding").append_attribute("value") = sliding;
	data.append_child("jumping").append_attribute("value") = jumping;
	data.append_child("grid").append_attribute("value") = grid;
	data.append_child("gravity_active").append_attribute("value") = gravity_active;

	LOG("---Player Saved");
}

bool j1Player::CameraOnPlayer()
{
	App->render->camera.x = position.x - App->render->camera.w / 3;
	App->render->camera.y = position.y - App->render->camera.h / 2;

	if (App->render->camera.x < 0) // left limit
	{ 
		App->render->camera.x = 0;
	}
	if (App->render->camera.x + App->win->width > App->map->data.width * App->map->data.tile_width) //right limit
	{
		App->render->camera.x = App->map->data.width * App->map->data.tile_width - App->win->width;
	}

	if (App->render->camera.y < 0) //up limit
	{
		App->render->camera.y = 0;
	}	
	if (App->render->camera.y + App->render->camera.h > App->map->data.height * App->map->data.tile_height) //down limit
	{
		App->render->camera.y = App->map->data.height * App->map->data.tile_height - App->win->height;
	}

	return true;
}

void j1Player::CleanUp()
{
	LOG("Deleting player");
}

void j1Player::Restart()
{
	for (p2List_item<ObjectsGroup*>* object = App->map->data.objLayers.start; object; object = object->next)
	{
		if (object->data->name == ("Collision"))
		{
			for (p2List_item<ObjectsData*>* objectdata = object->data->objects.start; objectdata; objectdata = objectdata->next)
			{
				if (objectdata->data->name == "StartPosition")
				{
					position.x = objectdata->data->x;
					position.y = objectdata->data->y;
				}
			}
		}
	}
	dead = false;
}

void j1Player::ChangeAnimation()
{
	Current_Animation = &idle;
}

void j1Player::LoadAnimation()
{
	idle.PushBack({ 0, 0, 32, 32 });
	idle.PushBack({ 32, 0, 32, 32 });
	idle.PushBack({ 64, 0, 32, 32 });
	idle.loop = true;
	idle.speed = 5.0f;
}
