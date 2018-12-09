#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"
#include "j1SceneChange.h"
#include "j1EntityController.h"
#include "Brofiler\Brofiler.h"

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

	to_end = false;
	App->map->Load(map_names.start->data->GetString());
	currentMap = 0;

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
	{
		App->pathfinding->SetMap(w, h, data);

		LOG("Create walkability");
	}


	RELEASE_ARRAY(data);

	debug_tex = App->tex->Load("maps/pathfinding.png");

	SpawnEntities();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene PreUpdate", Profiler::Color::DarkOrange);

	//LOG("IsChanging: %i", App->scenechange->IsChanging());

	//// debug pathfing ------------------
	//static iPoint origin;
	//static bool origin_selected = false;
	//int x, y;
	//App->input->GetMousePosition(x, y);
	//iPoint p = App->render->ScreenToWorld(x, y);
	//p = App->map->WorldToMap(p.x, p.y);
	//if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	//{
	//	if (origin_selected == true)
	//	{
	//		App->pathfinding->CreatePath(origin, p);
	//		origin_selected = false;
	//	}
	//	else
	//	{
	//		origin = p;
	//		origin_selected = true;
	//	}
	//}
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::DarkOrange);

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) //Start from first level
	{
		ResetBoxPos();
		Load_level(0);
		currentMap = 0;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) //Start from beginning of current map
	{
		ResetBoxPos();
		App->entitycontroller->Restart();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) //Start from second level
	{
		ResetBoxPos();
		Load_level(1);
		currentMap = 1;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) //Save game state
	{
		App->SaveGame();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) //Load game state
	{
		App->LoadGame();
		SetBoxFlag();
		App->LoadGame();
	}
	else if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) //View colliders
	{
		App->map->debug = !App->map->debug;
		App->entitycontroller->draw_path = !App->entitycontroller->draw_path;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) //Godmode
	{
		godmode = !godmode;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) //Framerate Cap On/Off
	{
		App->fpsCapON = !App->fpsCapON;
	}

	//----
	App->map->Draw();
	if (change == false)
	{
		App->entitycontroller->Draw(dt);
	}

	//----
	int x, y;
	App->input->GetMousePosition(x, y);

	// Debug pathfinding ------------------------------
	//int x, y;
	/*App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);
	App->render->Blit(debug_tex, p.x, p.y);
	if (App->entitycontroller->draw_path) {
		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}*/

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Scene PostUpdate", Profiler::Color::DarkOrange);

	bool ret = true;

	if (change == true) //rotate map and change
	{
		if (App->map->rotate == false)
		{
			if (App->map->rotate_end == true)
			{
				change = false;
			}
			else if (App->map->rotate_back == false && App->map->rotate_end == false)
			{
				SaveAndChange();
			}
		}
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

bool j1Scene::Load_level(int map)
{
	App->entitycontroller->DeleteEntities();
	App->map->SwitchMaps(map_names[map]);
	SpawnEntities();

	return true;
}

void j1Scene::SpawnEntities()
{
	App->entitycontroller->DeleteEntities();
	SpawnEnemies();

	pugi::xml_document	config_file;
	pugi::xml_node		config;

	config = App->LoadConfig(config_file);

	Entity* player = App->entitycontroller->AddEntity(Entity::entityType::PLAYER, { 0,0 }, { 0,0 });
	player->Awake(config.child(App->entitycontroller->name.GetString()));
	player->Start();
}


void j1Scene::SpawnEnemies()
{
	for (p2List_item<ObjectsGroup*>* object = App->map->data.objLayers.start; object; object = object->next)
	{
		if (object->data->name == ("Enemies"))
		{
			for (p2List_item<ObjectsData*>* objectdata = object->data->objects.start; objectdata; objectdata = objectdata->next)
			{
				if (objectdata->data->name == "FlyingEnemy")
				{
					App->entitycontroller->AddEntity(Entity::entityType::FLYING_ENEMY, { objectdata->data->x, objectdata->data->y }, { 0,0 });
				}

				else if (objectdata->data->name == "LandEnemy")
				{
					App->entitycontroller->AddEntity(Entity::entityType::LAND_ENEMY, { objectdata->data->x, objectdata->data->y }, { 0,0 });
				}
			}
		}
		if (object->data->name == ("Collision"))
		{
			for (p2List_item<ObjectsData*>* objectdata = object->data->objects.start; objectdata; objectdata = objectdata->next)
			{
				if (objectdata->data->name == "Grid" && objectdata->data->type != "Static") 
				{
					App->entitycontroller->AddEntity(Entity::entityType::GRID, { objectdata->data->x, objectdata->data->y }, { objectdata->data->width, objectdata->data->height }, objectdata->data->type);
				}
				else if (objectdata->data->name == "Box1" || objectdata->data->name == "Box2" || objectdata->data->name == "Box3" || objectdata->data->name == "Box4")
				{
					App->entitycontroller->AddEntity(Entity::entityType::BOX, { objectdata->data->x, objectdata->data->y }, { objectdata->data->width, objectdata->data->height }, objectdata->data->name, objectdata->data->type);
				}

			}
		}
	}
}

void j1Scene::SaveAndChange()
{
	pugi::xml_document data;  //savegame.xml data
	pugi::xml_node root;
	pugi::xml_parse_result result = data.load_file("save_game.xml");
	data.child("game_state").child("scene").child("currentMap").attribute("num").set_value(currentMap);
	data.save_file("save_game.xml");

	root = data.child("game_state").child("entitycontroller");

	pugi::xml_document copy_data; //copysave.xml data
	pugi::xml_node file;
	file = copy_data.append_child("game_state");

	App->entitycontroller->CopySave(file); //write currentmap data to copysave.xml
	copy_data.save_file("copysave.xml");

	if (currentMap == 0) //change to map2
	{
		currentMap = 1; //switch to and load map2
		App->entitycontroller->DeleteEntities();
		App->map->SwitchMaps2(map_names[1]);
		SpawnEntities();

		if (root.child("map_2").child("player") != NULL) // if map2 data exists loads it from savegame.xml
		{
			App->entitycontroller->Load(root);
			root = data.child("game_state").child("entitycontroller");
		}
	}
	else if (currentMap == 1) //change to map1
	{
		currentMap = 0; //switch to and load map1
		App->entitycontroller->DeleteEntities();
		App->map->SwitchMaps2(map_names[0]);
		SpawnEntities();

		if (root.child("map_1").child("player") != NULL) //if map1 data exists loads it from savegame.xml
		{
			App->entitycontroller->Load(root);
			root = data.child("game_state").child("entitycontroller");
		}
	}

	file = copy_data.child("game_state");
	App->entitycontroller->AppendSave(file, root); //write copysave.xml data to savegame.xml
	data.save_file("save_game.xml");
	copy_data.reset();
	data.reset();
}

void j1Scene::ResetBoxPos()
{
	box_1_side = false;
	box_2_side = false;
	box_3_side = true;
	box_4_side = false;
}

void j1Scene::SetBoxFlag()
{
	p2List_item<Entity*>* tmp = App->entitycontroller->Entities.end;
	while (tmp != nullptr)
	{
		if (tmp->data->type == Entity::entityType::BOX)
		{
			tmp->data->LoadSide();
		}
		tmp = tmp->prev;
	}
}