#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1SceneChange.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1EntityController.h"
#include "j1Entity.h"


Entity::Entity()
{
}

Entity::Entity(entityType type) : type(type)
{
}

Entity::~Entity()
{
}

void Entity::Draw(float dt)
{
	if (type == GRID)
	{
		if (flip_ver)
		{
			App->render->Blit(App->entitycontroller->texture, position.x, position.y, &rect, SDL_FLIP_VERTICAL);
		}
		if (flip_hor)
		{
			App->render->Blit(App->entitycontroller->texture, position.x, position.y, &rect, SDL_FLIP_HORIZONTAL);
		}
		if (!flip_ver && !flip_hor)
		{
			App->render->Blit(App->entitycontroller->texture, position.x, position.y, &rect, SDL_FLIP_NONE);
		}

	}
	else
	{
		if (flip)
		{
			App->render->Blit(App->entitycontroller->texture, position.x, position.y, &(Current_Animation->GetCurrentFrame(dt)), SDL_FLIP_HORIZONTAL);
		}
		else
		{
			App->render->Blit(App->entitycontroller->texture, position.x, position.y, &(Current_Animation->GetCurrentFrame(dt)), SDL_FLIP_NONE);
		}
	}

}

void Entity::Collider_Overlay()
{
	SDL_Rect ObjectRect;
	SDL_Rect result;

	p2List_item<Entity*>* tmp = App->entitycontroller->Entities.start; //grid
	while(tmp != nullptr)
	{
		if (tmp->data->type == Entity::entityType::GRID)
		{
			if (SDL_IntersectRect(&Collider, &tmp->data->Collider, &result))
			{
				grid_collision = true;
				is_static = false;

				if (Collider.y + (3 * Collider.h / 4) <= tmp->data->position.y)
				{
					top_grid = true;
				}

				if (tmp->data->grid_type == "Hide_up") 
				{
					vertical = true;
					flip_ver = true;
				}
				else if (tmp->data->grid_type == "Hide_down")
				{
					vertical = true;
					flip_ver = false;
				}
				else if (tmp->data->grid_type == "Hide_left")
				{
					vertical = false;
					flip_hor = true;
				}
				else if (tmp->data->grid_type == "Hide_right")
				{
					vertical = false;
					flip_hor = false;
				}

				grid_speed.x = tmp->data->speed.x;
				grid_speed.y = tmp->data->speed.y;

			}
		}
		tmp = tmp->next;
	}

	for (p2List_item<ObjectsGroup*>* object = App->map->data.objLayers.start; object; object = object->next) //objects colliders
	{
		if (object->data->name == ("Collision"))
		{
			for (p2List_item<ObjectsData*>* objectdata = object->data->objects.start; objectdata; objectdata = objectdata->next)
			{
				ObjectRect.x = objectdata->data->x;
				ObjectRect.y = objectdata->data->y;
				ObjectRect.w = objectdata->data->width;
				ObjectRect.h = objectdata->data->height;

				if (SDL_IntersectRect(&Collider, &ObjectRect, &result))
				{
					if (objectdata->data->name == "Wall")
					{
						if (objectdata->data->type == "Wall_left")
						{
							wall_left = true;
						}
						if (objectdata->data->type == "Wall_right")
						{
							wall_right = true;
						}
					}
					else if (objectdata->data->name == "Floor")
					{
						if (position.y + Collider.h - gravity <= ObjectRect.y)
						{
							if (result.h <= result.w || position.x + Collider.w >= ObjectRect.x)
							{
								position.y -= result.h - 1;
								if (landed == false)
								{
									landing = true;
								}
								grounded = true;
							}
						}
						else
						{
							jumpSpeed = 0;
						}
					}
					else if (objectdata->data->name == "Spikes")
					{
						if (App->scene->godmode == false)
						{
							dead = true;
						}
					}
					else if (objectdata->data->name == "Ceiling")
					{
						if (App->scene->godmode == false)
						{
							ceiling = true;
						}
					}
					else if (objectdata->data->name == "Box")
					{
						grid_collision = true;
						is_static = true;

						if (Collider.y + (3 * Collider.h / 4) <= objectdata->data->y)
						{
							top_grid = true;
						}
					}
					else if (objectdata->data->name == "Finish")
					{
						App->scene->change = true;
					}
					else if (objectdata->data->name == "Grid" && objectdata->data->type == "Static") 
					{
						grid_collision = true;
						is_static = true;

						if (Collider.y + (3 * Collider.h / 4) <= objectdata->data->y)
						{
							top_grid = true;
						}

					}
				}
			}
		}
	}
}

void Entity::PositionCollider()
{
	Collider.x = position.x;
	Collider.y = position.y;
	Collider.w = size.x;
	Collider.h = size.y;

	if (type != PLAYER && type != GRID)
	{
		SightCollider.x = position.x;
		SightCollider.y = position.y;
	}
}
