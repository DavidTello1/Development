#ifndef __j1MAIN_MENU_H__
#define __j1MAIN_MENU_H__

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
	bool Awake(pugi::xml_node& conf);

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

	void UpdateState(UI_Element* data);
	void MoveUI_Left();
	void ResetUI_pos();

public:	//all gui elements

	p2SString menu_bg_file_name;
	UI_Element* menu_background;

	UI_Element* continue_button;
	UI_Element* continue_text;

	UI_Element* new_game_button;
	UI_Element* new_game_text;
	
	UI_Element* exit_button;
	UI_Element* exit_text;

	UI_Element* credits_button;
	UI_Element* credits_text;

	UI_Element* settings_button;
	UI_Element* settings_text;

	UI_Element* back_button;
	UI_Element* back_text;

	bool quit_game = false;
};

#endif // __j1MAIN_MENU_H__