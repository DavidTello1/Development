#include "j1EntityController.h"
#include "j1Grid.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Map.h"


j1Grid::j1Grid() : Entity(entityType::GRID)
{
}

j1Grid::~j1Grid()
{
}

j1Grid::j1Grid(iPoint pos, iPoint Size, p2SString Type) : Entity(entityType::GRID)
{
	LOG("Loading Grid");

	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);
	if (App->scene->currentMap == 0)
	{
		config = config.child("entitycontroller").child("map_1").child("grid");
	}
	else if (App->scene->currentMap == 1)
	{
		config = config.child("entitycontroller").child("map_2").child("grid");
	}

	position.x = pos.x;
	position.y = pos.y;
	speed.x = config.child("speed").attribute("x").as_int();
	speed.y = config.child("speed").attribute("y").as_int();
	initial_size = Size;
	size.x = Size.x;
	size.y = Size.y;
	grid_type = Type;

	if (grid_type == "Hide_up")
	{
		type_int = 1;
	}
	if (grid_type == "Hide_down")
	{
		type_int = 2;
	}
	if (grid_type == "Hide_left")
	{
		type_int = 3;
	}
	if (grid_type == "Hide_right")
	{
		type_int = 4;
	}

	ChangeAnimation();
	PositionCollider();
}

bool j1Grid::Update(float dt)
{
	GetType();

	if (size.x > initial_size.x || size.x <= 32)
	{
		speed.x *= -1;
	}
	if (size.y > initial_size.y || size.y <= 32)
	{
		speed.y *= -1;
	}

	if (grid_type == "Hide_up" || grid_type == "Hide_down")
	{
		size.y -= speed.y;
		flip_ver = true;
		if (grid_type == "Hide_down")
		{
			position.y += speed.y;
			flip_ver = false;
		}
	}
	if (grid_type == "Hide_left" || grid_type == "Hide_right")
	{
		size.x -= speed.x;
		flip_hor = true;
		if (grid_type == "Hide_right")
		{
			position.x += speed.x;
			flip_hor = false;
		}

	}
	ChangeAnimation();
	PositionCollider();

	return true;
}

void j1Grid::CleanUp()
{
	LOG("---Grid Deleted");
}

void j1Grid::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	size.x = data.child("size").attribute("width").as_int();
	size.y = data.child("size").attribute("height").as_int();
	speed.x = data.child("speed").attribute("x").as_int();
	speed.y = data.child("speed").attribute("y").as_int();
	type_int = data.child("type").attribute("value").as_int();

	LOG("--- Grid Loaded");
}

// Save Game State
void j1Grid::Save(pugi::xml_node& data) const
{
	pugi::xml_node grid = data.append_child("grid");

	grid.append_child("position").append_attribute("x") = position.x;
	grid.child("position").append_attribute("y") = position.y;
	grid.append_child("size").append_attribute("width") = size.x;
	grid.child("size").append_attribute("height") = size.y;
	grid.append_child("speed").append_attribute("x") = speed.x;
	grid.child("speed").append_attribute("y") = speed.y;
	grid.append_child("type").append_attribute("value") = type_int;

	LOG("---Grid Saved");
}

void j1Grid :: ChangeAnimation()
{
	if (grid_type == "Hide_up" || grid_type == "Hide_down")
	{
		rect = { 256, 0, size.x, size.y };
	}
	else if (grid_type == "Hide_left" || grid_type == "Hide_right")
	{
		rect = { 352, 0, size.x, size.y };
	}
}

void j1Grid::GetType()
{
	if (type_int == 1)
	{
		grid_type = "Hide_up";
	}
	if (type_int == 2)
	{
		grid_type = "Hide_down";
	}
	if (type_int == 3)
	{
		grid_type = "Hide_left";
	}
	if (type_int == 4)
	{
		grid_type = "Hide_right";
	}
}

