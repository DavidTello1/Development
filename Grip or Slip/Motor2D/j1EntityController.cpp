#include "j1EntityController.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"
#include "j1SceneChange.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "j1Grid.h"
#include "j1FlyingEnemy.h"
#include "j1LandEnemy.h"
#include "j1Textures.h"
#include "Brofiler\Brofiler.h"

#include "PugiXml/src/pugixml.hpp"

j1EntityController::j1EntityController()
{
	name.create("entitycontroller");
}

j1EntityController::~j1EntityController()
{
}

bool j1EntityController::Awake(pugi::xml_node &config)
{
	bool ret = true;
	folder.create(config.child("folder").child_value());
	texture_path = config.child("sprite_sheet").attribute("source").as_string();

	return ret;
}

bool j1EntityController::Start()
{
	bool ret = true;
	texture = App->tex->Load(PATH(folder.GetString(), texture_path.GetString()));
	debug_tex = App->tex->Load("maps/pathfinding.png");

	return ret;
}

bool j1EntityController::Update(float dt)
{
	BROFILER_CATEGORY("EntityController Update", Profiler::Color::Blue);

	bool ret = true;

	if (App->map->debug)
	{
		DebugDraw();
	}
	EnemyColliderCheck();

	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		ret = tmp->data->Update(dt);
		tmp = tmp->next;
	}

	return ret;
}

bool j1EntityController::PostUpdate()
{
	BROFILER_CATEGORY("EntityController PostUpdate", Profiler::Color::Blue);

	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->PostUpdate();

		if (App->entitycontroller->draw_path) {
			const p2DynArray<iPoint>* path = &tmp->data->entityPath;
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				App->render->Blit(debug_tex, pos.x, pos.y);
			}
		}
		tmp = tmp->next;
	}

	
	return ret;
}

bool j1EntityController::CleanUp()
{
	DeleteEntities();

	return true;
}

bool j1EntityController::Save(pugi::xml_node& file) const
{
	bool ret = true;
	if (App->scene->currentMap == 0)
	{
		file = file.append_child("map_1");
		p2List_item<Entity*>* tmp = Entities.start;
		while (tmp != nullptr)
		{
			tmp->data->Save(file);
			tmp = tmp->next;
		}
	}
	else if (App->scene->currentMap == 1)
	{
		file = file.append_child("map_2");
		p2List_item<Entity*>* tmp = Entities.start;
		while (tmp != nullptr)
		{
			tmp->data->Save(file);
			tmp = tmp->next;
		}
	}
	return ret;
}

bool j1EntityController::Load(pugi::xml_node& file)
{
	bool ret = true;

	DeleteEntities();
	App->scene->SpawnEntities();

	if (App->scene->currentMap == 0)
	{
		file = file.child("map_1");
	}
	else if (App->scene->currentMap == 1)
	{
		file = file.child("map_2");
	}

	p2List_item<Entity*>* tmp = Entities.start;
	pugi::xml_node box = file.child("box");
	pugi::xml_node grid = file.child("grid");
	pugi::xml_node flying_enemy = file.child("flying_enemy");
	pugi::xml_node land_enemy = file.child("land_enemy");

	while (tmp != nullptr)
	{
		if (tmp->data->type == Entity::entityType::BOX)
		{
			tmp->data->Load(box);
			box = box.next_sibling("box");
		}
		else if (tmp->data->type == Entity::entityType::GRID)
		{
			tmp->data->Load(grid);
			grid = grid.next_sibling("grid");
		}
		else if (tmp->data->type == Entity::entityType::FLYING_ENEMY)
		{
			tmp->data->Load(flying_enemy);
			flying_enemy = flying_enemy.next_sibling("flying_enemy");
		}
		else if (tmp->data->type == Entity::entityType::LAND_ENEMY)
		{
			tmp->data->Load(land_enemy);
			land_enemy = land_enemy.next_sibling("land_enemy");
		}
		else if (tmp->data->type == Entity::entityType::PLAYER)
		{
			tmp->data->Load(file.child("player"));
		}
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Restart()
{
	bool ret = true;

	DeleteEnemies();
	App->scene->SpawnEnemies();
	p2List_item<Entity*>* tmp = Entities.end;
	while (tmp != nullptr)
	{
		if (tmp->data->type == Entity::entityType::PLAYER)
		{
			tmp->data->Restart();
			break;
		}
		tmp = tmp->prev;
	}
	return ret;
}

void j1EntityController::DeleteEnemies()
{
	p2List_item<Entity*>* tmp = Entities.end;
	while (tmp != nullptr)
	{
		p2List_item<Entity*>* tmp2 = tmp;
		if (tmp->data->type != Entity::entityType::PLAYER)
		{
			RELEASE(tmp->data);
			Entities.del(tmp2);
			tmp = tmp->prev;
		}
		else
			tmp = tmp->prev;
	}
}

void j1EntityController::DeleteEntities()
{
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		RELEASE(tmp->data);
		tmp = tmp->next;
	}
	Entities.clear();
}

bool j1EntityController::Draw(float dt)
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	p2List_item<Entity*>* tmp2 = nullptr;

	while (tmp != nullptr)
	{
		if (tmp->data->type == Entity::entityType::PLAYER)
		{
			tmp2 = tmp;
		}
		else
		{
			tmp->data->Draw(dt);
		}
		tmp = tmp->next;
	}
	if (tmp2 != nullptr)
	{
		tmp2->data->Draw(dt); //draw player last
	}

	return ret;
}

bool j1EntityController::DebugDraw()
{
	p2List_item<Entity*>* tmp = Entities.start;
	SDL_Rect col;
	SDL_Rect col2;
	while (tmp != nullptr)
	{
		col.x = tmp->data->Collider.x;
		col.y = tmp->data->Collider.y;
		col.h = tmp->data->Collider.h;
		col.w = tmp->data->Collider.w;
		App->render->DrawQuad(col, 0, 0, 255, 50); //blue

		if (tmp->data->type == Entity::entityType::FLYING_ENEMY || tmp->data->type == Entity::entityType::LAND_ENEMY)
		{
			if (tmp->data->dead == false)
			{
				col2.x = tmp->data->SightCollider.x;
				col2.y = tmp->data->SightCollider.y;
				col2.h = tmp->data->SightCollider.h;
				col2.w = tmp->data->SightCollider.w;
				App->render->DrawQuad(col2, 255, 0, 0, 50);
			}
		}
		tmp = tmp->next;
	}

	return true;
}

Entity* j1EntityController::AddEntity(Entity::entityType type, iPoint position, iPoint Size, p2SString Type)
{
	Entity* tmp = nullptr;

	switch (type)
	{
	case Entity::entityType::BOX:
		//tmp = new box(position);
		break;

	case Entity::entityType::FLYING_ENEMY:
		tmp = new FlyingEnemy(position);
		break;

	case Entity::entityType::LAND_ENEMY:
		tmp = new LandEnemy(position);
		break;

	case Entity::entityType::GRID:
		tmp = new j1Grid(position, Size, Type);
		break;

	case Entity::entityType::PLAYER:
		tmp = new j1Player();
		break;
	}

	if (tmp)
		Entities.add(tmp);

	return tmp;
}

bool j1EntityController::DeleteEntity(Entity * entity)
{
	entity->CleanUp();
	Entities.del(Entities.At(Entities.find(entity)));
	return true;
}


void j1EntityController::EnemyColliderCheck()
{
	p2List_item<Entity*>* player = nullptr;
	for (p2List_item<Entity*>* i = Entities.start; i != nullptr; i = i->next)
	{
		if (i->data->type == Entity::entityType::PLAYER)
		{
			player = i;
			break;
		}
	}
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		if (tmp->data->type == Entity::entityType::LAND_ENEMY || tmp->data->type == Entity::entityType::FLYING_ENEMY)
		{
			if (tmp->data->dead == false)
			{

				if (SDL_HasIntersection(&tmp->data->SightCollider, &player->data->Collider))
				{
					tmp->data->chasing_player = true;
				}
				else
				{
					tmp->data->chasing_player = false;
				}
				if (SDL_HasIntersection(&tmp->data->Collider, &player->data->Collider) && !App->scenechange->IsChanging())
				{
					if (player->data->attack)
					{
						if (tmp->data->hurt == false)
						{
							tmp->data->lives--;
							tmp->data->hurt = true;
						}
					}
					else
					{
						if (!App->scene->godmode)
						{
							player->data->dead = true;
						}
					}
				}
			}
		}
		tmp = tmp->next;
	}
}
