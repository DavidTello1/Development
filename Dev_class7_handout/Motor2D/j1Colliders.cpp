/*#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Colliders.h"


Colliders::Colliders()
{
}

Colliders::Colliders(ColliderType type) : type(type)
{
}

Colliders::~Colliders()
{
}

void Colliders::Draw(float dt)
{
	//if (flip)
	//	App->render->Blit(App->entitycontroller->texture, position.x, position.y, &(Current_Animation->GetCurrentFrame(dt)), SDL_FLIP_HORIZONTAL, -1.0);
	//else
	//	App->render->Blit(App->entitycontroller->texture, position.x, position.y, &(Current_Animation->GetCurrentFrame(dt)), SDL_FLIP_NONE, -1.0);
}


fPoint Colliders::Collider_Overlay(fPoint vector)
{

	SDL_Rect Collider_new;
	Collider_new = Collider;
	Collider_new.x += vector.x;
	Collider_new.y += vector.y;

	SDL_Rect result;

	fPoint newvector = vector;
	for (p2List_item<ObjectsGroup*>* object = App->map->data.objLayers.start; object; object = object->next)
	{
		if (object->data->name == ("Collisions"))
		{
			for (p2List_item<ObjectsData*>* objectdata = object->data->objects.start; objectdata; objectdata = objectdata->next)
			{
				if (SDL_IntersectRect(&Collider_new, &CreateRect_FromObjectData(objectdata->data), &result))
				{
					if (objectdata->data->name == "Wall")
					{
						//speed.x = 0;
						//if (!grounded){
						//	speed.y /= 2;
						//	sliding = true;}
					}
					else if (objectdata->data->name == "Floor")
					{
						//if (position.y + Collider.h <= objectdata->data->y)
						//	if (result.h <= result.w || position.x + Collider.w + ColliderOffset.x >= objectdata->data->x)
						//		newvector.y -= result.h, BecomeGrounded();
					}
					else if (objectdata->data->name == "Spikes")
					{
						dead = true;
					}
					else if (objectdata->data->name == "Ceiling")
					{
						//speed.y = 0;
					}
					else if (objectdata->data->name == "Grid")
					{
						//speed.y = speed.x = 0;
						//grid = true;
					}
				}
			}
		}
	}


	return newvector;
}*/
/*
fPoint Colliders::AvoidCollision(fPoint newvector, const SDL_Rect result, p2List_item<ObjectsData*>* objectdata)
{
	if (type == PLAYER)
	{
		type = PLAYER;
	}
	if (newvector.y > 0)
	{
		if (position.y + Collider.h + ColliderOffset.y <= objectdata->data->y)
		{
			if (newvector.x > 0)
			{
				if (result.h <= result.w || position.x + Collider.w + ColliderOffset.x > objectdata->data->x)
					newvector.y -= result.h, BecomeGrounded();
				else
					newvector.x -= result.w;
			}
			else if (newvector.x < 0)
			{
				if (result.h <= result.w || position.x + ColliderOffset.x > objectdata->data->x + objectdata->data->width)
					newvector.y -= result.h, BecomeGrounded();
				else
					newvector.x += result.w;
			}
			else
				newvector.y -= result.h, BecomeGrounded();
		}
		else
		{
			if (newvector.x > 0)
				newvector.x -= result.w;
			else
				newvector.x += result.w;
		}

	}
	else if (newvector.y < 0)
	{
		if (position.y + ColliderOffset.y >= objectdata->data->y + objectdata->data->height)
		{
			if (newvector.x > 0)
			{
				if (result.h <= result.w || position.x + Collider.w + ColliderOffset.x >= objectdata->data->x)
					newvector.y += result.h;
				else
					newvector.x -= result.w;
			}
			else if (newvector.x < 0)
			{
				if (result.h <= result.w || position.x + ColliderOffset.x <= objectdata->data->x + objectdata->data->width)
					newvector.y += result.h;
				else
					newvector.x += result.w;
			}
			else
				newvector.y += result.h;
		}
		else
		{
			if (newvector.x > 0)
				newvector.x -= result.w;
			else if (newvector.x < 0)
				newvector.x += result.w;
			else
				newvector.y += result.h;
		}
	}
	else
	{
		if (newvector.x > 0)
			newvector.x -= result.w;
		else if (newvector.x < 0)
			newvector.x += result.w;
	}

	if (newvector.y < 1 && newvector.y > -1)
	{
		newvector.y = 0;
	}
	if (newvector.x < 1 && newvector.x > -1)
	{
		newvector.x = 0;
	}

	return newvector;
}*/
/*
SDL_Rect Colliders::CreateRect_FromObjectData(ObjectsData * data)
{
	SDL_Rect ret;
	ret.x = data->x;
	ret.y = data->y;
	ret.h = data->height;
	ret.w = data->width;
	return ret;
}


void Colliders::PositionCollider()
{
	Collider.x = position.x + ColliderOffset.x;
	Collider.y = position.y + ColliderOffset.y;
	if (type != PLAYER)
	{
		if (direction_x == 1)
		{
			SightCollider.x = position.x - SightOffset.x;
			SightCollider.y = position.y - SightOffset.y;
		}
		else
		{
			SightCollider.x = position.x - SightCollider.w + SightOffset.x;
			SightCollider.y = position.y - SightOffset.y;
		}

	}
}
*/