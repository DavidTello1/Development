#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "UI_Element.h"

struct SDL_Texture;

class j1MainMenu : public j1Module
{
public:

	j1MainMenu();

	// Destructor
	virtual ~j1MainMenu();

	// Called before render is available
	bool Awake();

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

public:	//all gui elements
	UI_Element* button;
	UI_Element* button_text;

	UI_Element* button2;
	UI_Element* button_text2;

	UI_Element* window_ui;

};

#endif // __j1SCENE_H__