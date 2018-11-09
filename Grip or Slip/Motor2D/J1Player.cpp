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
	LoadAnimations();
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
				landed = false;
				jumpSpeed = speed.y;
			}
			if (App->scene->godmode == true)
			{
				jumpSpeed = speed.y;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) //left
		{
			if (grid == true)
			{
				grid_moving = true;
				position.x -= speed.x / 2;
			}
			else
			{
				flip = false;
				left = true;
				if (position.x - speed.x <= 0) {
					position.x = 0;
				}
				else
				{
					position.x -= speed.x;
				}
				if (wall_left == true && grounded == false) {
					flip = false;
					sliding = true;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) //right
		{
			if (grid == true) {
				grid_moving = true;
				position.x += speed.x / 2;
			}
			else
			{
				flip = true;
				right = true;
				if (position.x + speed.x >= (App->map->data.width - 1) * App->map->data.tile_width) {
					position.x = (App->map->data.width - 1) * App->map->data.tile_width;
				}
				else
				{
					position.x += speed.x;
				}
				if (wall_right == true && grounded == false) {
					flip = true;
					sliding = true;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) //up
		{
			if (grid == true && ceiling == false && top_grid == false)
			{
				grid_moving = true;
				position.y -= speed.x / 2;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) //down
		{
			if (grid == true)
			{
				grid_moving = true;
				position.y += speed.x / 2;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) //grip
		{
			if (grid_collision == true && grid == false)
			{
				gripping = true;
				grid = true;
				jumping = false;
			}
		}


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP) //left release
		{
			left = false;
			sliding = false;
			grid_moving = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP) //right release
		{
			right = false;
			sliding = false;
			grid_moving = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_UP) //up release
		{
			grid_moving = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_UP) //down release
		{
			grid_moving = false;
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
			landed = false;

			if (!is_static) 
			{
				if (vertical == true)
				{
					if (flip_ver == true) //hide_up
					{
						if (!ceiling)
						{
							position.y -= grid_speed.y;
						}
						else
						{
							position.y += grid_speed.y;
						}
					}
					if (flip_ver == false) //hide_down
					{
						position.y += grid_speed.y;
					}
				}
				if (vertical == false)
				{
					if (flip_hor == true) //hide_left
					{
						position.x -= grid_speed.x;
					}
					if (flip_hor == false) //hide_right
					{
						position.x += grid_speed.x;
					}
				}
			}
		}

		if (sliding == true) //sliding
		{
			jumpSpeed = 0;
			grounded = false;
			grid = false;
		}

		if (jumping == true) //jumping
		{
			gravity_active = true;
			if (position.y - jumpSpeed <= 0) //up-limit
			{
				position.y = 0;
			}
			if (jumpSpeed > 0)
			{
				jumpSpeed--;
				position.y -= jumpSpeed;
			}
		}

		if (ceiling == true)
		{
			jumpSpeed = 0;
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

		if (landing == true && landing_anim.Finished())
		{
			landing_anim.Reset();
			landing_anim.ResetLoops();
			landed = true;
			landing = false;
		}
		if (gripping == true && grip_anim.Finished())
		{
			grip_anim.Reset();
			grip_anim.ResetLoops();
			gripping = false;
		}

		ChangeAnimation();

		sliding = false;
		grounded = false;
		grid_collision = false;
		top_grid = false;
		ceiling = false;
	}
	else
	{
		ChangeAnimation();
		left = right = false;
	}

	return true;
}


bool j1Player::PostUpdate()
{
	if (dead && !App->scenechange->IsChanging())
	{
		App->scenechange->ChangeMap(App->scene->currentMap, App->scene->fade_time);
	}

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


void j1Player::CleanUp()
{
	LOG("---Player Deleted");

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
	if (!dead)
	{
		if (grounded && landed)
		{
			if (!left && !right)
			{
				Current_Animation = &idle;
			}
			else
			{
				Current_Animation = &move_anim;
			}
		}
		if (jumping)
		{
			if (jumpSpeed > 0)
			{
				Current_Animation = &jump_anim;
			}
			else
			{
				Current_Animation = &falling_anim;
			}
		}
		if (grid && !gripping)
		{
			if (!grid_moving)
			{
				Current_Animation = &grip_idle;
			}
			else
			{
				Current_Animation = &grip_move;
			}
		}
		if (sliding)
		{
			Current_Animation = &slide_anim;
		}
		if (landing)
		{
			Current_Animation = &landing_anim;
		}
		if (gripping)
		{
			Current_Animation = &grip_anim;
		}
	}
	else
	{
		Current_Animation = &dying;
	}
}

void j1Player::LoadAnimations()
{
	idle.PushBack({ 0, 0, size.x, size.y });
	idle.PushBack({ 32, 0, size.x, size.y });
	idle.PushBack({ 64, 0, size.x, size.y });
	idle.PushBack({ 96, 0, size.x, size.y });
	idle.loop = true;
	idle.speed = 0.2f;

	move_anim.PushBack({ 0, 32, size.x, size.y });
	move_anim.PushBack({ 32, 32, size.x, size.y });
	move_anim.PushBack({ 64, 32, size.x, size.y });
	move_anim.PushBack({ 96, 32, size.x, size.y });
	move_anim.PushBack({ 128, 32, size.x, size.y });
	move_anim.PushBack({ 160, 32, size.x, size.y });
	move_anim.PushBack({ 192, 32, size.x, size.y });
	move_anim.PushBack({ 224, 32, size.x, size.y });
	move_anim.loop = true;
	move_anim.speed = 0.2f;

	jump_anim.PushBack({ 224, 64, size.x, size.y });
	jump_anim.PushBack({ 192, 64, size.x, size.y });
	jump_anim.loop = true;
	jump_anim.speed = 0.2f;

	falling_anim.PushBack({ 160, 64, size.x, size.y });
	falling_anim.PushBack({ 128, 64, size.x, size.y });
	falling_anim.loop = false;
	falling_anim.speed = 0.2f;

	landing_anim.PushBack({ 96, 64, size.x, size.y });
	landing_anim.PushBack({ 64, 64, size.x, size.y });
	landing_anim.PushBack({ 32, 64, size.x, size.y });
	landing_anim.PushBack({ 0, 64, size.x, size.y });
	landing_anim.loop = false;
	landing_anim.speed = 0.2f;

	grip_idle.PushBack({ 160, 0, size.x, size.y });

	grip_anim.PushBack({ 128, 64, size.x, size.y });
	grip_anim.PushBack({ 224, 96, size.x, size.y });
	grip_anim.PushBack({ 128, 0, size.x, size.y });
	grip_anim.PushBack({ 192, 0, size.x, size.y });
	grip_anim.loop = false;
	grip_anim.speed = 0.4f;

	grip_move.PushBack({ 224, 0, size.x, size.y });
	grip_move.PushBack({ 192, 0, size.x, size.y });
	grip_move.PushBack({ 160, 0, size.x, size.y });
	grip_move.PushBack({ 160, 96, size.x, size.y });
	grip_move.PushBack({ 192, 96, size.x, size.y });
	grip_move.loop = true;
	grip_move.speed = 0.2f;

	slide_anim.PushBack({ 128, 96, size.x, size.y });

	dying.PushBack({ 64, 96, size.x, size.y });

}
