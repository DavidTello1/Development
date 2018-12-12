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
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UI_Element.h"
#include "InteractiveButton.h"
#include "JustSimpleUI.h"

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

	player_lives = config.child("player_lives").attribute("value").as_int();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = false;

	to_end = false;
	App->map->Load(map_names.start->data->GetString());
	currentMap = 0;

	//int w, h;
	//uchar* data = NULL;
	//if (App->map->CreateWalkabilityMap(w, h, &data))
	//{
	//	App->pathfinding->SetMap(w, h, data);

	//	LOG("Create walkability");
	//}
	//RELEASE_ARRAY(data);
	//debug_tex = App->tex->Load("maps/pathfinding.png");

	ResetBoxPos();
	SpawnEntities();

	//create gui
	SDL_RenderGetViewport(App->render->renderer, &App->render->viewport);

	ui_life1 = App->gui->AddUIElement(UI_Element::UI_type::IMAGE, UI_Element::Action::NONE, { 0,0 }, { 54,45 }, nullptr, true);
	ui_life2 = App->gui->AddUIElement(UI_Element::UI_type::IMAGE, UI_Element::Action::NONE, { 55,0 }, { 54,45 }, nullptr, true);
	ui_life3 = App->gui->AddUIElement(UI_Element::UI_type::IMAGE, UI_Element::Action::NONE, { 110,0 }, { 54,45 }, nullptr, true);
	ui_coins = App->gui->AddUIElement(UI_Element::UI_type::IMAGE, UI_Element::Action::NONE, { 200,0 }, { 47,47 }, nullptr, true);
	ui_coins_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 250,0 }, { 0,0 }, nullptr, true, { false, false }, "x0");
	ui_score = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 500,0 }, { 0,0 }, nullptr, true, { false, false }, "Score: 0");
	ui_timer = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 950,0 }, { 0,0 }, nullptr, true, { false, false }, "Timer: 0s");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene PreUpdate", Profiler::Color::DarkOrange);

	if (player_lives <= 0)
	{
		return false;
	}
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
	}
	else if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) //Debug UI
	{
		App->gui->UI_Debug = !App->gui->UI_Debug;
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

	// Debug pathfinding ------------------------------
	//int x, y;
	//App->input->GetMousePosition(x, y);
	//iPoint p = App->render->ScreenToWorld(x, y);
	//p = App->map->WorldToMap(p.x, p.y);
	//p = App->map->MapToWorld(p.x, p.y);
	//App->render->Blit(debug_tex, p.x, p.y);
	//if (App->entitycontroller->draw_path) {
	//	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
	//	for (uint i = 0; i < path->Count(); ++i)
	//	{
	//		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
	//		App->render->Blit(debug_tex, pos.x, pos.y);
	//	}
	//}

	// gui update
	p2List_item<UI_Element*>* item = App->gui->UI_elements.end;
	while (item != nullptr)
	{
		//if(item->data->type != UI_Element::UI_type::IMAGE && item->data->type != UI_Element::UI_type::TEXT)
		if (App->gui->CheckMousePos(item->data) == true && App->gui->dragging == false) //hovering
		{
			item->data->state = UI_Element::State::HOVER;
			if (App->gui->CheckClick(item->data) == true) //on-click
			{
				if (item->data->dragable.x == false && item->data->dragable.y == false)
				{
					item->data->state = UI_Element::State::LOGIC;
					item->data->DoLogic(item->data->action);

					if (item->data->children.start != nullptr)
					{
						item->data->children.start->data->Center(true, true);
					}
				}
				else
				{
					item->data->state = UI_Element::State::DRAG;
				}
			}
		}
		if (item->data->state == UI_Element::State::DRAG && App->gui->CheckClick(item->data) == true)
		{
			App->gui->dragging = true;
			item->data->Drag();
			App->gui->UpdateChildren();
		}
		else if (App->gui->CheckMousePos(item->data) == false && item->data->state != UI_Element::State::DRAG)
		{
			item->data->state = UI_Element::State::IDLE; //change to idle
		}
		item = item->prev;
	}
	coins++;
	UpdateSimpleUI();
	App->gui->Draw();

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
		pause = true;
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
		data.child("player_lives").attribute("value").set_value(player_lives);

		LOG("Calling switch maps");
		currentMap = data.child("currentMap").attribute("num").as_int();
		App->map->SwitchMaps(map_names[data.child("currentMap").attribute("num").as_int()]);

		player_lives = data.child("player_lives").attribute("value").as_int();
	}
	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	data.append_child("currentMap").append_attribute("num") = currentMap;
	data.append_child("player_lives").append_attribute("value") = player_lives;
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

void j1Scene::UpdateState(UI_Element* data)
{
	switch(data->type)
	{
	case UI_Element::UI_type::PUSHBUTTON: //push button
		switch (data->state)
		{
		case UI_Element::State::LOCKED:
			data->rect = { 0,0,0,0 };
			break;

		case UI_Element::State::IDLE:
			data->rect = { 642,170,230,64 };
			break;

		case UI_Element::State::HOVER:
			data->rect = { 0,114,230,64 };
			break;

		case UI_Element::State::LOGIC:
			data->rect = { 411,170,230,64 };
			break;

		case UI_Element::State::DRAG:
			data->rect = { 411,170,230,64 };
			break;
		}
		break;

	case UI_Element::UI_type::WINDOW: //window
		switch (data->state)
		{
		case UI_Element::State::LOCKED:
			data->rect = { 0,0,0,0 };
			break;

		case UI_Element::State::IDLE:
			data->rect = { 26,536,424,458 };
			break;

		case UI_Element::State::HOVER:
			data->rect = { 26,536,424,458 };
			break;

		case UI_Element::State::LOGIC:
			data->rect = { 26,536,424,458 };
			break;

		case UI_Element::State::DRAG:
			data->rect = { 26,536,424,458 };
			break;
		}
		break;
	}
}

void j1Scene::UpdateSimpleUI()
{
	p2List_item<UI_Element*>* item = App->gui->UI_elements.start;
	while (item != nullptr)
	{
		switch (item->data->type)
		{
		case UI_Element::UI_type::IMAGE: //image

			if (item->data == ui_life1) //Life 1
			{
				if (player_lives >= 2)
				{
					item->data->rect = { 190,0,53,45 }; //full
					break;
				}
				else if (player_lives == 1)
				{
					item->data->rect = { 242,0,54,45 }; //half
					break;
				}
				else
				{
					item->data->rect = { 295,0,54,45 }; //empty
					break;
				}
			}
			else if (item->data == ui_life2) //Life 2
			{
				if (player_lives >= 4)
				{
					item->data->rect = { 190,0,53,45 }; //full
					break;
				}
				else if (player_lives == 3)
				{
					item->data->rect = { 242,0,54,45 }; //half
					break;
				}
				else
				{
					item->data->rect = { 295,0,54,45 }; //empty
					break;
				}
			}
			else if (item->data == ui_life3) //Life 3
			{
				if (player_lives == 6)
				{
					item->data->rect = { 190,0,53,45 }; //full
					break;
				}
				else if (player_lives == 5)
				{
					item->data->rect = { 242,0,54,45 }; //half
					break;
				}
				else
				{
					item->data->rect = { 295,0,54,45 }; //empty
					break;
				}
			}
			else if (item->data == ui_coins) //Coins
			{
				item->data->rect = { 190,45,47,47 };
				break;
			}

		case UI_Element::UI_type::TEXT: //text

			if (item->data == ui_coins_text) //coins number
			{
				item->data->label = "x0";
				break;
			}
			else if (item->data == ui_score) //score
			{
				item->data->label = "SCORE: 0000";
				break;
			}
			else if (item->data == ui_timer) //timer
			{
				item->data->label = "TIME: 300s";
				break;
			}
		}
		item = item->next;
	}
}