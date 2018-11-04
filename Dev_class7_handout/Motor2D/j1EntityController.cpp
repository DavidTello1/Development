#include "j1EntityController.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1SceneChange.h"
#include "j1Entity.h"
#include "j1Player.h"
#include "j1Textures.h"

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

	return ret;
}

bool j1EntityController::Update(float dt)
{
	bool ret = true;

	if (App->map->debug)
	{
		DebugDraw();
	}
	EnemyColliderCheck();

	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		if (tmp->data->type == Entity::entityType::PLAYER)
			ret = tmp->data->Update(dt);
		else
			ret = tmp->data->Update(enemy_dt);
		tmp = tmp->next;
	}

	return ret;
}

bool j1EntityController::PostUpdate()
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->PostUpdate();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::CleanUp()
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->CleanUp();
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Save(pugi::xml_node& file) const
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Save(file);
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Load(pugi::xml_node& file)
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	pugi::xml_node box = file.child("box");
	pugi::xml_node moving_grid = file.child("moving_grid");
	while (tmp != nullptr)
	{
		if (tmp->data->type == Entity::entityType::PLAYER)
		{
			tmp->data->Load(file.child("player"));
		}
		else if (tmp->data->type == Entity::entityType::BOX)
		{
			tmp->data->Load(box);
			box = box.next_sibling("box");

		}
		else if (tmp->data->type == Entity::entityType::MOVING_GRID)
		{
			tmp->data->Load(moving_grid);
			moving_grid = moving_grid.next_sibling("moving_grid");
		}
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::Restart()
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Restart();
		tmp = tmp->next;
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
	p2List_item<Entity*>* tmp = Entities.end;
	while (tmp != nullptr)
	{
		p2List_item<Entity*>* tmp2 = tmp;
		RELEASE(tmp->data);
		Entities.del(tmp2);
		tmp = tmp->prev;
	}

}

bool j1EntityController::Draw(float dt)
{
	bool ret = true;
	p2List_item<Entity*>* tmp = Entities.start;
	while (tmp != nullptr)
	{
		tmp->data->Draw(dt);
		tmp = tmp->next;
	}
	return ret;
}

bool j1EntityController::DebugDraw()
{
	//p2List_item<Entity*>* tmp = Entities.start;
	//SDL_Rect col;
	//SDL_Rect col2;
	//while (tmp != nullptr)
	//{
	//	col.h = tmp->data->Collider.h, col.w = tmp->data->Collider.w, col.x = tmp->data->Collider.x, col.y = tmp->data->Collider.y;
	//	App->render->DrawQuad(col, 255, 0, 0, 50);
	//	if (tmp->data->type == Entity::entityType::FLYING_ENEMY || tmp->data->type == Entity::entityType::LAND_ENEMY)
	//	{
	//		col2.h = tmp->data->SightCollider.h, col2.w = tmp->data->SightCollider.w, col2.x = tmp->data->SightCollider.x, col2.y = tmp->data->SightCollider.y;
	//		App->render->DrawQuad(col2, 255, 0, 0, 50);
	//	}
	//	tmp = tmp->next;
	//}

	return true;
}

Entity* j1EntityController::AddEntity(Entity::entityType type, iPoint position)
{
	Entity* tmp = nullptr;

	switch (type)
	{
	case Entity::entityType::PLAYER:
		tmp = new j1Player();

		break;
	case Entity::entityType::BOX:
		//tmp = new box(position);
		break;
	case Entity::entityType::MOVING_GRID:
		//tmp = new moving_grid(position);
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
			//if (SDL_HasIntersection(&tmp->data->SightCollider, &player->data->Collider))
			//{
			//	tmp->data->chasing_player = true;
			//}
			//else
			//{
				tmp->data->chasing_player = false;
			//}
			if (SDL_HasIntersection(&tmp->data->Collider, &player->data->Collider) && !godmode && !App->scenechange->IsChanging())
			{
				player->data->isDying = true;
			}
		}
		tmp = tmp->next;
	}
}
