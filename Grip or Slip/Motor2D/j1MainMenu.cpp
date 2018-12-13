#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1SceneChange.h"
#include "j1Scene.h"
#include "j1MainMenu.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "UI_Element.h"
#include "InteractiveButton.h"
#include "JustSimpleUI.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.create("main_menu");
}

// Destructor
j1MainMenu::~j1MainMenu()
{}

// Called before render is available
bool j1MainMenu::Awake()
{
	LOG("Loading Main Menu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1MainMenu::Start()
{
	//create gui
	SDL_RenderGetViewport(App->render->renderer, &App->render->viewport);

	continue_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::CONTINUE, { App->render->viewport.w / 2 - 95, App->render->viewport.h / 3 +20 }, { 190,48 }, nullptr, true);
	continue_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, continue_button, true, { false, false }, "CONTINUE");
	new_game_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::NEW_GAME, { App->render->viewport.w / 2 - 95, App->render->viewport.h / 3 + 80 }, { 190,48 }, nullptr, true);
	new_game_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, new_game_button, true, { false, false }, "NEW GAME");
	exit_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::EXIT, { App->render->viewport.w / 2 - 95, App->render->viewport.h / 3 + 260 }, { 190,48 }, nullptr, true);
	exit_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, exit_button, true, { false, false }, "EXIT");
	credits_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::CREDITS, { App->render->viewport.w / 2 - 95, App->render->viewport.h /3 + 200 }, { 190,48 }, nullptr, true);
	credits_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, credits_button, true, { false, false }, "CREDITS");
	settings_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::SETTINGS, { App->render->viewport.w / 2 - 95, App->render->viewport.h / 3 + 140 }, { 190,48 }, nullptr, true);
	settings_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, settings_button, true, { false, false }, "SETTINGS");

	continue_text->color = new_game_text->color = exit_text->color = credits_text->color = settings_text->color = { 255,255,255,255 };
	return true;
}

// Called each loop iteration
bool j1MainMenu::PreUpdate()
{
	pugi::xml_document data;
	pugi::xml_node root;
	data.load_file("save_game.xml");
	root = data.child("game_state");

	if (root == NULL)
	{
		continue_button->locked = true;
	}
	else
	{
		continue_button->locked = false;
	}

	return true;
}

// Called each loop iteration
bool j1MainMenu::Update(float dt)
{
	// -------
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		App->gui->UI_Debug = !App->gui->UI_Debug;

	// gui update
	p2List_item<UI_Element*>* item = App->gui->UI_elements.end;
	while (item != nullptr)
	{
		if (item->data->children.start != nullptr)
		{
			item->data->children.start->data->Center(true, true); //center children
		}

		if (App->gui->CheckMousePos(item->data) == true && App->gui->dragging == false) //hovering
		{
			item->data->state = UI_Element::State::HOVER;
			if (App->gui->CheckClick(item->data) == true) //on-click
			{
				if (item->data->dragable.x == false && item->data->dragable.y == false)
				{
					item->data->state = UI_Element::State::LOGIC;
					if (item->data->locked == false)
					{
						item->data->DoLogic(item->data->action);
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
		UpdateState(item->data);
		item = item->prev;
	}
	App->gui->Draw();

	return true;
}

// Called each loop iteration
bool j1MainMenu::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit_game == true)
		ret = false;

	return ret;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	LOG("Freeing Main Menu");

	return true;
}

void j1MainMenu::UpdateState(UI_Element* data)
{
	switch (data->type)
	{
	case UI_Element::UI_type::PUSHBUTTON: //push button
		switch (data->state)
		{
		case UI_Element::State::IDLE:
			if (data->locked == true)
			{
				data->rect = { 0,139,190,48 };
			}
			else
			{
				data->rect = { 380,139,190,48 };
			}
			break;

		case UI_Element::State::HOVER:
			if (data->locked == true)
			{
				data->rect = { 0,139,190,48 };
			}
			else
			{
				data->rect = { 190,139,190,48 };
			}
			break;

		case UI_Element::State::LOGIC:
			if (data->locked == true)
			{
				data->rect = { 0,90,190,48 };
			}
			else
			{
				data->rect = { 190,90,190,48 };
			}
			break;
		}
		break;
	}
}
