#include "j1EntityController.h"
#include "j1Coins.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Input.h"
#include "Brofiler\Brofiler.h"


j1Coins::j1Coins() : Entity(entityType::COIN)
{
}

j1Coins::~j1Coins()
{
}

j1Coins::j1Coins(iPoint pos, iPoint Size) : Entity(entityType::COIN)
{
	LOG("Loading Coin");

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = App->LoadConfig(config_file);
	config = config.child("entitycontroller").child("coin");

	position = pos;
	size = Size;
	dead = config.child("dead").attribute("value").as_bool();

	rect = { 416, 96, size.x, size.y };
}

bool j1Coins::Update(float dt)
{
	BROFILER_CATEGORY("Coin Update", Profiler::Color::Gold);

	if (dead)
	{
		App->entitycontroller->DeleteEntity(this);
	}
	else
	{
		PositionCollider();
	}

	return true;
}

void j1Coins::CleanUp()
{
	LOG("---Coin Deleted");
}

void j1Coins::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	size.x = data.child("size").attribute("width").as_int();
	size.y = data.child("size").attribute("height").as_int();
	dead = data.child("dead").attribute("value").as_bool();

	LOG("--- Coin Loaded");
}

// Save Game State
void j1Coins::Save(pugi::xml_node& data) const
{
	pugi::xml_node coin = data.append_child("coin");

	coin.append_child("position").append_attribute("x") = position.x;
	coin.child("position").append_attribute("y") = position.y;
	coin.append_child("size").append_attribute("width") = size.x;
	coin.child("size").append_attribute("height") = size.y;
	coin.append_child("dead").append_attribute("value") = dead;

	LOG("---Coin Saved");
}