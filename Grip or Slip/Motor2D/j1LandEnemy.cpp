#include "j1App.h"
#include "j1Player.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"
#include "j1EntityController.h"
#include "j1LandEnemy.h"
#include "p2Log.h"
#include "j1PathFinding.h"
#include "Brofiler\Brofiler.h"
#include <time.h>

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
	
	gravity = 100;

	entityPath.Clear();
	LoadAnimations();
	Current_Animation = &idle;
}

bool LandEnemy::Update(float dt)
{
	BROFILER_CATEGORY("LandEnemy Update", Profiler::Color::DarkSlateGray);

	if (!dead)
	{
		PositionCollider();
		App->entitycontroller->EnemyColliderCheck();
		Collider_Overlay();

		p2List_item<Entity*>* player = nullptr;
		for (p2List_item<Entity*>* i = App->entitycontroller->Entities.start; i != nullptr; i = i->next)
		{
			if (i->data->type == Entity::entityType::PLAYER)
			{
				player = i;
				break;
			}
		}

		if (grounded == true)
		{
			gravity_active = false;
		}
		else
		{
			gravity_active = true;
		}

		if (gravity_active == true)
		{
			if (position.y + gravity*dt >= (App->map->data.height - 1) * App->map->data.tile_height)
			{
				position.y = (App->map->data.height - 1) * App->map->data.tile_height;
			}
			else
			{
				position.y += gravity*dt;
			}
		}

		if (hurt == true)
		{
			if (player->data->attack == false)
			{
				hurt = false;
			}
		}
		if (lives <= 0)
		{
			dead = true;
			App->scene->score += 1000;
		}

		if (App->entitycontroller->draw_path) //draw path
		{
			const p2DynArray<iPoint>* path = &entityPath;
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				App->render->Blit(App->entitycontroller->debug_tex, pos.x, pos.y);
			}
		}

		if (chasing_player) 
		{
			followPath();
		}
		else
		{
			entityPath.Clear();
		}
	}

	ChangeAnimation();
	grounded = false;

	return true;
}

void LandEnemy::CleanUp()
{
	LOG("---LandEnemy Deleted");
	entityPath.Clear();
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
	p2List_item<Entity*>* player = nullptr;
	for (p2List_item<Entity*>* i = App->entitycontroller->Entities.start; i != nullptr; i = i->next)
	{
		if (i->data->type == Entity::entityType::PLAYER)
		{
			player = i;
			break;
		}
	}
	if (&player->data->position != entityPath.At(entityPath.Count())) 
	{
		entityPath.Clear();
		App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y),
			App->map->WorldToMap(player->data->position.x + (player->data->Collider.w / 2), player->data->position.y + (player->data->Collider.w / 2)), entityPath);
	}

	iPoint curr_cell;
	iPoint* next_cell = nullptr;
	if (entityPath.Count() > 0)
	{
		if (App->map->WorldToMap(position.x, position.y) !=
			App->map->WorldToMap(player->data->position.x + (player->data->Collider.w / 2), player->data->position.y + (player->data->Collider.w / 2)))
		{
			curr_cell = *entityPath.At(1);
			if (entityPath.Count() > 1)
			{
				next_cell = entityPath.At(2);
			}

			iPoint map_pos = App->map->WorldToMap(position.x + rect.w / 2, position.y + rect.h / 2);
			if (curr_cell.x > map_pos.x) //going right
			{
				position.x += speed.x / 4;
			}
			else if (curr_cell.x < map_pos.x) //going left
			{
				position.x -= speed.x / 4;
			}
		}
	}
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
	idle.speed = 5.0f;

	attacking.PushBack({ 448, 128, size.x, size.y });
	attacking.PushBack({ 480, 128, size.x, size.y });
	attacking.PushBack({ 512, 128, size.x, size.y });
	attacking.loop = true;
	attacking.speed = 5.0f;

	damage.PushBack({ 544, 160, size.x, size.y });

	dying.PushBack({ 544, 128, size.x, size.y });
}