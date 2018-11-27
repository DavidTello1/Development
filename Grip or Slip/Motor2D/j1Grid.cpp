#include "j1EntityController.h"
#include "j1Grid.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Map.h"
#include "Brofiler\Brofiler.h"


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
	
	config = config.child("entitycontroller").child("grid");

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

	direction = 1;

	ChangeAnimation();
	PositionCollider();
}

bool j1Grid::Update(float dt)
{
	BROFILER_CATEGORY("Grid Update", Profiler::Color::SandyBrown);

	GetType();
	prev_size = size;

	if (size.x - speed.y*dt * direction > initial_size.x || size.y - speed.y*dt * direction > initial_size.y)
	{
		direction = -1;
	}
	if (size.x - speed.y*dt * direction <= 32 || size.y - speed.y*dt * direction <= 32)
	{
		direction = 1;
	}

	if (grid_type == "Hide_up" || grid_type == "Hide_down")
	{
		flip_ver = true;

		final_speed.y += speed.y*dt;
		if (final_speed.y >= 1)
		{
			if (direction == 1)
			{
				size.y++;
			}
			else if (direction == -1)
			{
				size.y--;
			}
			final_speed.y--;
		}
		if (grid_type == "Hide_down")
		{
			flip_ver = false;
			position.y -= size.y - prev_size.y;
		}
	}

	if (grid_type == "Hide_left" || grid_type == "Hide_right")
	{
		flip_hor = true;

		final_speed.x += speed.x*dt;
		if (final_speed.x >= 1)
		{
			if (direction == 1)
			{
				size.x++;
			}
			else if (direction == -1)
			{
				size.x--;
			}
			final_speed.x--;
		}
		if (grid_type == "Hide_right")
		{
			flip_hor = false;
			position.x -= size.x - prev_size.x;
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