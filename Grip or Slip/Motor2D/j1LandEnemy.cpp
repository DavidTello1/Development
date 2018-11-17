#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1EntityController.h"
#include "j1LandEnemy.h"
#include "p2Log.h"
//#include "j1PathFinding.h"
//#include "Brofiler\Brofiler.h"
//#include <time.h>

LandEnemy::LandEnemy() : Entity(entityType::LAND_ENEMY)
{
}

LandEnemy::~LandEnemy()
{
}

LandEnemy::LandEnemy(iPoint pos) : Entity(entityType::LAND_ENEMY)
{
	LOG("Loading LandEnemy");

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = App->LoadConfig(config_file);
	config = config.child("entitycontroller").child("land_enemy");

	position.x = pos.x;
	position.y = pos.y;
	speed.x = config.child("speed").attribute("x").as_int();
	speed.y = config.child("speed").attribute("y").as_int();
	size.x = config.child("size").attribute("width").as_int();
	size.y = config.child("size").attribute("height").as_int();
	SightCollider.w = config.child("sightCollider").attribute("width").as_int();
	SightCollider.h = config.child("sightCollider").attribute("height").as_int();
	dead = config.child("dead").attribute("value").as_bool();
	lives = config.child("lives").attribute("value").as_int();

	LoadAnimations();
	Current_Animation = &idle;
}

bool LandEnemy::Update(float dt)
{
	if (!dead)
	{
		PositionCollider();
		App->entitycontroller->EnemyColliderCheck();

		if (hurt == true)
		{
			if (lives <= 0)
			{
				dead = true;
				return true;
			}
			p2List_item<Entity*>* player = nullptr;
			for (p2List_item<Entity*>* i = App->entitycontroller->Entities.start; i != nullptr; i = i->next)
			{
				if (i->data->type == Entity::entityType::PLAYER)
				{
					player = i;
					break;
				}
			}

			if (player->data->attack == false)
			{
				hurt = false;
			}
		}
		else
		{
			//if (!Calculate_Path())
			//{
			//	if (!counting)
			//	{
			//		doStandardPath.Start();
			//		counting = true;
			//	}
			//	if (doStandardPath.Read() >= 1500)
			//	{
			//		standardPath();
			//		slowerPath = true;
			//	}
			//}
			//else
			//{
			//	counting = false;
			//	slowerPath = false;
			//}

			//if (entityPath.Count() > 1)
			//	followPath();
		}
	}

	ChangeAnimation();

	return true;
}
void LandEnemy::CleanUp()
{
	LOG("---LandEnemy Deleted");
}

void LandEnemy::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	speed.x = data.child("speed").attribute("x").as_int();
	speed.y = data.child("speed").attribute("y").as_int();
	size.x = data.child("size").attribute("width").as_int();
	size.y = data.child("size").attribute("height").as_int();
	SightCollider.w = data.child("sightCollider").attribute("width").as_int();
	SightCollider.h = data.child("sightCollider").attribute("height").as_int();
	dead = data.child("dead").attribute("value").as_bool();
	lives = data.child("lives").attribute("value").as_int();

	LOG("--- LandEnemy Loaded");
}

void LandEnemy::Save(pugi::xml_node& data) const
{
	pugi::xml_node flying_enemy = data.append_child("land_enemy");

	flying_enemy.append_child("position").append_attribute("x") = position.x;
	flying_enemy.child("position").append_attribute("y") = position.y;
	flying_enemy.append_child("size").append_attribute("width") = size.x;
	flying_enemy.child("size").append_attribute("height") = size.y;
	flying_enemy.append_child("speed").append_attribute("x") = speed.x;
	flying_enemy.child("speed").append_attribute("y") = speed.y;
	flying_enemy.append_child("sightCollider").append_attribute("width") = SightCollider.w;
	flying_enemy.child("sightCollider").append_attribute("height") = SightCollider.h;
	flying_enemy.append_child("dead").append_attribute("value") = dead;
	flying_enemy.append_child("lives").append_attribute("value") = lives;

	LOG("--- LandEnemy Saved");
}

void LandEnemy::followPath()
{
	//iPoint next_cell;
	//next_cell = *entityPath.At(1);
	//next_cell = App->map->MapToWorld(next_cell.x, next_cell.y);
	//next_cell = { next_cell.x + App->map->data.tile_width / 2, next_cell.y };
	//iPoint map_pos(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);

	//float usingSpeed = (slowerPath) ? (speed / 2) : speed;

	//if (next_cell.x > map_pos.x)
	//{
	//	v.x = usingSpeed;
	//	state = RIGHT;
	//	going_right = true;
	//	going_left = false;
	//}
	//else if (next_cell.x < map_pos.x)
	//{
	//	v.x = -usingSpeed;
	//	state = LEFT;
	//	going_left = true;
	//	going_right = false;
	//}
	//else
	//{
	//	v.x = 0;
	//	going_right = false;
	//	going_left = false;
	//}

	//if (next_cell.y > map_pos.y)
	//{
	//	state = FALLING;
	//	going_down = true;
	//}
}

void LandEnemy::standardPath()
{
	//iPoint curr_pos = App->map->WorldToMap(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);
	//iPoint rightCell(curr_pos.x - 1, curr_pos.y);
	//iPoint leftCell(curr_pos.x + 1, curr_pos.y);
	//entityPath.PushBack(curr_pos);

	//if (moving_right && App->pathfinding->isWalkable(rightCell) && App->pathfinding->isTouchingGround(rightCell))
	//	entityPath.PushBack(rightCell);
	//else if (!moving_left)
	//{
	//	entityPath.PushBack(curr_pos);
	//	moving_right = false;
	//	moving_left = true;
	//}
	//if (moving_left && App->pathfinding->isWalkable(leftCell) && App->pathfinding->isTouchingGround(leftCell))
	//	entityPath.PushBack(leftCell);
	//else if (!moving_right)
	//{
	//	entityPath.PushBack(curr_pos);
	//	moving_right = true;
	//	moving_left = false;
	//}
}

void LandEnemy::ChangeAnimation()
{
	if (!dead)
	{
		if (!chasing_player && !hurt)
		{
			Current_Animation = &idle;
		}
		if (chasing_player)
		{
			Current_Animation = &attacking;
		}
		if (hurt)
		{
			Current_Animation = &damage;
		}
	}
	else
	{
		Current_Animation = &dying;
	}
}

void LandEnemy::LoadAnimations()
{
	idle.PushBack({ 448, 160, size.x, size.y });
	idle.PushBack({ 480, 160, size.x, size.y });
	idle.PushBack({ 512, 160, size.x, size.y });
	idle.loop = true;
	idle.speed = 0.15f;

	attacking.PushBack({ 448, 128, size.x, size.y });
	attacking.PushBack({ 480, 128, size.x, size.y });
	attacking.PushBack({ 512, 128, size.x, size.y });
	attacking.loop = true;
	attacking.speed = 0.15f;

	damage.PushBack({ 544, 160, size.x, size.y });

	dying.PushBack({ 544, 128, size.x, size.y });
}