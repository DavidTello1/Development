#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1EntityController.h"
#include "j1FlyingEnemy.h"
#include "p2Log.h"
//#include "j1PathFinding.h"
//#include "Brofiler\Brofiler.h"
//#include <time.h>

FlyingEnemy::FlyingEnemy() : Entity(entityType::FLYING_ENEMY)
{
}

FlyingEnemy::~FlyingEnemy()
{
}

FlyingEnemy::FlyingEnemy(iPoint pos) : Entity(entityType::FLYING_ENEMY)
{
	LOG("Loading FlyingEnemy");

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = App->LoadConfig(config_file);
	config = config.child("entitycontroller").child("flying_enemy");

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

	idle.PushBack({ 448, 96, size.x, size.y });
	idle.PushBack({ 480, 96, size.x, size.y });
	idle.PushBack({ 512, 96, size.x, size.y });
	idle.PushBack({ 544, 96, size.x, size.y });
	idle.loop = true;
	idle.speed = 0.15f;

	Current_Animation = &idle;
}

bool FlyingEnemy::Update(float dt)
{
	if (!dead)
	{
		PositionCollider();
		App->entitycontroller->EnemyColliderCheck();

		if (lives <= 0)
		{
			dead = true;
			return true;
		}

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
	else
	{
		App->entitycontroller->DeleteEntity(this);
	}

	return true;
}


void FlyingEnemy::CleanUp()
{
	LOG("---FlyingEnemy Deleted");
}

void FlyingEnemy::Load(pugi::xml_node& data)
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

	LOG("--- FlyingEnemy Loaded");
}

void FlyingEnemy::Save(pugi::xml_node& data) const
{
	pugi::xml_node flying_enemy = data.append_child("flying_enemy");

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

	LOG("--- FlyingEnemy Saved");
}

void FlyingEnemy::followPath()
{
	//iPoint curr_cell;
	//iPoint next_cell;

	//curr_cell = *entityPath.At(1);
	//if (entityPath.Count() > 2)
	//{
	//	next_cell = *entityPath.At(2);
	//}
	//iPoint map_pos(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);
	//iPoint map_cell = App->map->WorldToMap(map_pos.x, map_pos.y);

	//iPoint curr_pos = App->map->MapToWorld(curr_cell.x, curr_cell.y);
	//curr_pos = { curr_pos.x + App->map->data.tile_width / 2, curr_pos.y + App->map->data.tile_height / 2 };

	//iPoint next_pos = App->map->MapToWorld(next_cell.x, next_cell.y);
	//next_pos = { next_pos.x + App->map->data.tile_width / 2, next_pos.y + App->map->data.tile_height / 2 };

	//float usingSpeed = (slowerPath) ? (speed.x / 2) : speed.x;

	//if (curr_pos.x > map_pos.x)
	//{
	//	v.x = usingSpeed;
	//	if (curr_cell.x > map_cell.x)
	//		state = RIGHT;
	//}
	//else if (curr_pos.x < map_pos.x)
	//{
	//	v.x = -usingSpeed;
	//	if (curr_cell.x < map_cell.x)
	//		state = LEFT;
	//}
	//else
	//	v.x = 0;

	//if (curr_pos.y > map_pos.y || (next_pos.y > map_pos.y && !App->pathfinding->isTouchingGround({ map_cell.x, map_cell.y + 1 })))
	//{
	//	v.y = -usingSpeed;
	//}
	//else if (curr_pos.y < map_pos.y || (next_pos.y < map_pos.y && !App->pathfinding->isTouchingGround({ map_cell.x, map_cell.y - 2 })))
	//{
	//	v.y = usingSpeed;
	//}
	//else
	//	v.y = 0;
}

void FlyingEnemy::standardPath()
{
	//srand(time(NULL));

	//int newPosX = rand() % 10;
	//newPosX = (newPosX < 5) ? 1 : -1;
	//int newPosY = rand() % 10;
	//newPosY = (newPosY < 5) ? 1 : -1;

	//iPoint curr_pos = App->map->WorldToMap(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);

	//curr_pos = { curr_pos.x + newPosX, curr_pos.y + newPosY }; // 1
	//if (App->pathfinding->isWalkable(curr_pos))
	//	entityPath.PushBack(curr_pos);

	//curr_pos = { curr_pos.x + newPosX, curr_pos.y + newPosY }; // 2
	//if (App->pathfinding->isWalkable(curr_pos))
	//	entityPath.PushBack(curr_pos);
}