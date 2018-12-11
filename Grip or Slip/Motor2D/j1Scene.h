#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "UI_Element.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node& data);
	bool Save(pugi::xml_node& data) const;

	bool Load_level(int map);
	void SpawnEnemies();
	void SpawnEntities();
	void SaveAndChange();
	void ResetBoxPos();

public:

	p2List<p2SString*> map_names;
	uint currentMap = 0;
	float fade_time = 0.0;
	bool to_end = false;
	bool change = false;
	bool pause = false;

	bool godmode = false;

	bool box_1_side = false;
	bool box_2_side = false;
	bool box_3_side = false;
	bool box_4_side = false;

private:

	SDL_Texture* debug_tex;
	bool copy = false;

public:
	//all gui elements
	UI_Element* button;
	UI_Element* button_text;

	UI_Element* button2;
	UI_Element* button_text2;

	UI_Element* window_ui;


};

#endif // __j1SCENE_H__