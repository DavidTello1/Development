#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Colliders.h"


j1Colliders::j1Colliders()
{
}

j1Colliders::j1Colliders(ColliderType type) : type(type)
{
}

j1Colliders::~j1Colliders()
{
}

bool j1Colliders::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
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

				if (OnCollision(App->player->playerRect, ObjectRect) == true)
				{
					if (objectdata->data->name == "Wall")
					{
						App->player->playerSpeed.x = 0;
						if (!App->player->grounded)
						{
							if (objectdata->data->type == "Wall_left" && App->player->left == true
								|| objectdata->data->type == "Wall_right" && App->player->right == true)
							{
								App->player->playerSpeed.y /= 2;
								App->player->sliding = true;
							}
						}
					}
					else if (objectdata->data->name == "Floor")
					{
						App->player->playerSpeed.y = 0;
						App->player->grounded = true;
					}
					else if (objectdata->data->name == "Spikes")
					{
						App->player->dead = true;
					}
					else if (objectdata->data->name == "Ceiling")
					{
						App->player->playerSpeed.y = 0;
					}
					else if (objectdata->data->name == "Grid")
					{
						App->player->playerSpeed.x = 0;
						App->player->playerSpeed.y = 0;
						App->player->grid = true;
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

	return true;
}

bool j1Colliders::OnCollision(SDL_Rect rect1, SDL_Rect rect2)
{
	bool ret = false;
	if (rect2.x <= rect1.x + rect1.w && rect2.x >= rect1.x && rect2.y <= rect1.y + rect1.h && rect2.y >= rect1.y) 
	{
		ret = true;
	}
	return ret;
}
