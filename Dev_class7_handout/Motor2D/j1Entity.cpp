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
	if (flip)
	{
		App->render->Blit(App->entitycontroller->texture, position.x, position.y, &(Current_Animation->GetCurrentFrame(dt)), SDL_FLIP_HORIZONTAL);
	}
	else
	{
		App->render->Blit(App->entitycontroller->texture, position.x, position.y, &(Current_Animation->GetCurrentFrame(dt)), SDL_FLIP_NONE);
	}
}

void Entity::Collider_Overlay()
{
	SDL_Rect ObjectRect;
	SDL_Rect result;

	for (p2List_item<ObjectsGroup*>* object = App->map->data.objLayers.start; object; object = object->next)
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
						ceiling = true;
					}
					else if (objectdata->data->name == "Grid")
					{
						grid_collision = true;
						if (Collider.y + (3 * Collider.h / 4) <= objectdata->data->y)
						{
							top_grid = true;
						}
					}
					else if (objectdata->data->name == "Finish")
					{
						App->scene->change = true;
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

	if (type != PLAYER)
	{
		SightCollider.x = position.x;
		SightCollider.y = position.y;
	}
}
