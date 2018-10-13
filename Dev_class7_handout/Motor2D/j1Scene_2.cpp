#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene_2.h"

j1Scene_2::j1Scene_2() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene_2::~j1Scene_2()
{}

// Called before render is available
bool j1Scene_2::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene_2::Start()
{
	App->map->Load("level2.tmx");

	return true;
}

// Called each loop iteration
bool j1Scene_2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene_2::Update(float dt)
{
	//if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	//	App->LoadGame();

	//if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	//	App->SaveGame();

	//if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//	App->render->camera.y += 1;

	//if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//	App->render->camera.y -= 1;

	//if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	//	App->render->camera.x += 1;

	//if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	//	App->render->camera.x -= 1;

	//	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	//		App->map->layer->properties.Get(App->map->layer->name.GetString(), App->map->layer->properties.Get);


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
bool j1Scene_2::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene_2::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
