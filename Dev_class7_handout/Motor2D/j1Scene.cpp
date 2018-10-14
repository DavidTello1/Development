#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1SceneChange.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	bool ret = true;

	LOG("Loading Scene");

	fade_time = config.child("fade_time").attribute("value").as_float();

	for (pugi::xml_node map = config.child("map_name"); map; map = map.next_sibling("map_name"))
	{
		p2SString* data = new p2SString;

		data->create(map.attribute("name").as_string());
		map_names.add(data);
	}

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = false;

	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);

	to_end = false;
	ret = App->map->Load(map_names.start->data->GetString());
	currentMap = 0;

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	//LOG("IsChanging: %i", App->scenechange->IsChanging());
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) //Start from first level
	{
		Load_level(0);
		currentMap = 0;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) //Start from beginning of current map
	{
		App->player->Restart();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) //Start from second level
	{
		Load_level(1);
		currentMap = 1;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) //Load game state
	{
		App->LoadGame();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) //Save game state
	{
		App->SaveGame();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) //View colliders
	{
		App->map->debug = !App->map->debug;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) //Godmode
	{
		App->player->godmode = !App->player->godmode;
	}

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (change == true)
	{
		if (currentMap == 0)
		{
			Load_level(1);
			currentMap = 1;
		}
		else if (currentMap == 1)
		{
			Load_level(0);
			currentMap = 0;
		}
		change = false;
	}

	if (to_end && App->scenechange->IsChanging() == false)
	{
		if (currentMap < map_names.count() - 1)
		{
			ret = App->scenechange->ChangeMap(++currentMap, fade_time);
		}
		else
		{
			currentMap = 0;
			ret = App->scenechange->ChangeMap(currentMap, fade_time);
		}
		to_end = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	return true;
}


bool j1Scene::Load(pugi::xml_node& data)
{
	LOG("IsChanging: %i", App->scenechange->IsChanging());

	if (currentMap != data.child("currentMap").attribute("num").as_int())
	{
		LOG("Calling switch maps");
		currentMap = data.child("currentMap").attribute("num").as_int();
		App->map->SwitchMaps(map_names[data.child("currentMap").attribute("num").as_int()]);

	}
	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	data.append_child("currentMap").append_attribute("num") = currentMap;
	return true;
}

bool j1Scene::Load_level(int time)
{
	App->map->SwitchMaps(map_names[time]);
	App->player->Restart();
	return true;
}