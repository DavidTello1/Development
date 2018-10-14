#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Colliders.h"


j1Colliders::j1Colliders()
{
	name.create("colliders");
}

j1Colliders::~j1Colliders()
{
}

bool j1Colliders::Update(float dt) //Draws the colliders
{
	bool ret = true;

	for (p2List_item<ObjectsGroup*>* object = App->map->data.objLayers.start; object; object = object->next)
	{
		if (object->data->name == ("Collision"))
		{
			for (p2List_item<ObjectsData*>* objectdata = object->data->objects.start; objectdata; objectdata = objectdata->next)
			{
				SDL_Rect Collider_new;
				Collider_new = Collider;
				Collider_new.x = objectdata->data->x;
				Collider_new.y = objectdata->data->y;
				Collider_new.w = objectdata->data->width;
				Collider_new.h = objectdata->data->height;
			}
		}
	}

	return ret;
}


bool j1Colliders::Collider_Overlay()
{
	bool ret = true;
	SDL_Rect ObjectRect;
	SDL_Rect result;

	p2List_item<ObjectsGroup*>* object;
	p2List_item<ObjectsData*>* objectdata;

	for (object = App->map->data.objLayers.start; object; object = object->next)
	{
		if (object->data->name == ("Collision"))
		{
			for (objectdata = object->data->objects.start; objectdata; objectdata = objectdata->next)
			{
				ObjectRect.x = objectdata->data->x;
				ObjectRect.y = objectdata->data->y;
				ObjectRect.w = objectdata->data->width;
				ObjectRect.h = objectdata->data->height;

				if (SDL_IntersectRect(&ObjectRect, &App->player->playerCollider, &result))
				{
					if (objectdata->data->name == "Wall")
					{
						if (objectdata->data->type == "Wall_left")
						{
							App->player->wall_left = true;
						}
						if (objectdata->data->type == "Wall_right")
						{
							App->player->wall_right = true;
						}
					}
					else if (objectdata->data->name == "Floor") //
					{
						if (App->player->playerPos.y + App->player->playerCollider.h - App->player->gravity <= ObjectRect.y)
						{
							if (result.h <= result.w || App->player->playerPos.x + App->player->playerCollider.w >= ObjectRect.x)
							{
								App->player->playerPos.y -= result.h - 1;
								App->player->grounded = true;
							}
						}
						else 
						{
							App->player->jumpSpeed.y = 0;
						}
					}
					else if (objectdata->data->name == "Spikes")
					{
						App->player->dead = true;

					}
					else if (objectdata->data->name == "Ceiling")
					{
 						App->player->jumpSpeed.y = 0;
					}
					else if (objectdata->data->name == "Grid")
					{
						App->player->grid_collision = true;
					}
					else if (objectdata->data->name == "Finish")
					{
						App->scene->change = true;
					}
				}
			}
		}
	}
	return ret;
}

bool j1Colliders::CleanUp()
{
	LOG("Unloading colliders");
	p2List_item<ObjectsGroup*>* object;
	p2List_item<ObjectsData*>* objectdata;

	object = App->map->data.objLayers.start;
	objectdata = object->data->objects.start;

	while (object != NULL)
	{
		while (objectdata != NULL) 
		{
			RELEASE(objectdata->data);
			objectdata = objectdata->next;
		}
		RELEASE(object->data);
		object = object->next;
	}
	App->map->data.objLayers.start;

	return true;
}