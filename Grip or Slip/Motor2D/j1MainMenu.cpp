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
bool j1MainMenu::Awake(pugi::xml_node& conf)
{
	LOG("Loading Main Menu");
	bool ret = true;

	menu_bg_file_name = conf.child("menu_bg").attribute("file").as_string("");
	credits_file_name = conf.child("credits").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1MainMenu::Start()
{
	
	SDL_RenderGetViewport(App->render->renderer, &App->render->viewport);
	
	//WEB
	web_page = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::WEB, { 20, App->render->viewport.h - 50}, { 0,0 }, nullptr, true, { false, false }, "WEB PAGE - www.webpage.com");

	//BG
	menu_background = App->gui->AddUIElement(UI_Element::UI_type::BACKGROUND, UI_Element::Action::NONE, { 0,0 }, { App->win->width, App->win->height }, nullptr, true);
	menu_background->texture = App->tex->Load(menu_bg_file_name.GetString());
	menu_background->rect = { 0,0, App->win->width, App->win->height };

	//CONTINUE
	continue_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::CONTINUE, { App->render->viewport.w / 2 - 95, App->render->viewport.h / 3 +20 }, { 190,48 }, nullptr, true);
	continue_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, continue_button, true, { false, false }, "CONTINUE");
	
	//NEW GAME
	new_game_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::NEW_GAME, { App->render->viewport.w / 2 - 95, App->render->viewport.h / 3 + 80 }, { 190,48 }, nullptr, true);
	new_game_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, new_game_button, true, { false, false }, "NEW GAME");
	
	//EXIT
	exit_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::EXIT, { App->render->viewport.w / 2 - 95, App->render->viewport.h / 3 + 260 }, { 190,48 }, nullptr, true);
	exit_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, exit_button, true, { false, false }, "EXIT");
	
	//CREDITS
	credits_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::CREDITS, { App->render->viewport.w / 2 - 95, App->render->viewport.h /3 + 200 }, { 190,48 }, nullptr, true);
	credits_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, credits_button, true, { false, false }, "CREDITS");
	
	//SETTINGS
	settings_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::SETTINGS, { App->render->viewport.w / 2 - 95, App->render->viewport.h / 3 + 140 }, { 190,48 }, nullptr, true);
	settings_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, settings_button, true, { false, false }, "SETTINGS");
		
	//CREDITS WINDOW
	credits_window = App->gui->AddUIElement(UI_Element::UI_type::BACKGROUND, UI_Element::Action::NONE, { App->render->viewport.w / 2 - 40, App->render->viewport.h / 3 - 50 }, { 375,425 }, nullptr, false);
	credits_window->texture = App->tex->Load(credits_file_name.GetString());
	credits_window->rect = { 0, 0, 375, 425 };
	
	//SETTINGS WINDOW
	settings_window = App->gui->AddUIElement(UI_Element::UI_type::WINDOW, UI_Element::Action::NONE, { App->render->viewport.w / 2 - 40, App->render->viewport.h / 3 - 50 }, { 375,425 }, nullptr, false);
	settings_window->rect = { 628,0,375,425 };
	settings_window_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { App->render->viewport.w / 2 + 115, App->render->viewport.h / 3 - 20 }, { 0,0 }, nullptr, false, { false, false }, "*SETTINGS*");
	vol_slider_bar = App->gui->AddUIElement(UI_Element::UI_type::BACKGROUND, UI_Element::Action::NONE, { App->render->viewport.w / 2 + 150, 130 }, { 190,18 }, settings_window, false);
	sfx_slider_bar = App->gui->AddUIElement(UI_Element::UI_type::BACKGROUND, UI_Element::Action::NONE, { App->render->viewport.w / 2 + 150, 230 }, { 190,18 }, settings_window, false);
	vol_slider_bar->rect = { 0,26,190,18 };
	sfx_slider_bar->rect = { 0,44,190,18 };
	volume_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0, 100 }, { 0,0 }, settings_window, false, { false, false }, "MUSIC: 0");
	sfx_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0, 200 }, { 0,0 }, settings_window, false, { false, false }, "SOUND EFFECTS: 0");
	vol_slider_circle = App->gui->AddUIElement(UI_Element::UI_type::SLIDER, UI_Element::Action::ADJUST_VOL, { App->render->viewport.w / 2 + 52, App->render->viewport.h / 3 + 74 }, { 26,26 }, nullptr, false, { true, false });
	sfx_slider_circle = App->gui->AddUIElement(UI_Element::UI_type::SLIDER, UI_Element::Action::ADJUST_FX, { App->render->viewport.w / 2 + 52, App->render->viewport.h / 3 + 174 }, { 26,26 }, nullptr, false, { true, false });

	//BACK
	back_button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::BACK, { App->render->viewport.w / 2 + 55, App->render->viewport.h / 3 + 352 }, { 190,48 }, nullptr, false);
	back_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, back_button, false, { false, false }, "BACK");

	//TEXT COLOR
	continue_text->color = new_game_text->color = exit_text->color = credits_text->color = settings_text->color = back_text->color = { 255,255,255,255 };
	settings_window_text->color = volume_text->color = sfx_text->color = { 0,0,0,255 };

	credits = settings = false;

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
		if (item->data->children.start != nullptr) //center children
		{
			if (item->data->type != UI_Element::UI_type::PUSHBUTTON)
			{
				if (item->data->type == UI_Element::UI_type::WINDOW)
				{
					p2List_item<UI_Element*>* tmp = item->data->children.start;
					while (tmp != nullptr)
					{
						tmp->data->Center(true,false); //center X
						tmp = tmp->next;
					}
				}
				else if (item->data->type == UI_Element::UI_type::BACKGROUND)
				{
					p2List_item<UI_Element*>* tmp = item->data->children.start;
					while (tmp != nullptr)
					{
						if (tmp->data->type == UI_Element::UI_type::SLIDER)
						tmp->data->Center(false, true); //center Y
						tmp = tmp->next;
					}
				}
			}
			else
			{
				p2List_item<UI_Element*>* tmp = item->data->children.start;
				while (tmp != nullptr)
				{
					tmp->data->Center(true, true); //center X and Y
					tmp = tmp->next;
				}
			}
		}

		if (App->gui->CheckMousePos(item->data) == true && item->data->dragging == false && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KEY_REPEAT) //hovering
		{
			item->data->state = UI_Element::State::HOVER;
		}
		if (App->gui->CheckClick(item->data) == true && item->data->state == UI_Element::State::HOVER) //on-click
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
				item->data->DoLogic(item->data->action);
			}
		}
		if (item->data->state == UI_Element::State::DRAG && App->gui->CheckClick(item->data) == true)
		{
			item->data->dragging = true;
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

	App->render->Blit(menu_background->texture, 0,0, &menu_background->rect, SDL_FLIP_NONE, 0); //draw background first

	if (credits == true) //draw credits image
	{
		App->render->Blit(credits_window->texture, credits_window->globalpos.x, credits_window->globalpos.y, &credits_window->rect, SDL_FLIP_NONE, 0);
	}

	App->gui->Draw(); //draw gui
	
	if (settings == true) //draw settings slider bars
	{
		App->render->Blit(App->gui->GetAtlas(), vol_slider_bar->globalpos.x, vol_slider_bar->globalpos.y, &vol_slider_bar->rect, SDL_FLIP_NONE, 0);
		App->render->Blit(App->gui->GetAtlas(), sfx_slider_bar->globalpos.x, sfx_slider_bar->globalpos.y, &sfx_slider_bar->rect, SDL_FLIP_NONE, 0);
	}

	return true;
}

// Called each loop iteration
bool j1MainMenu::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit_game == true)
	{
		ret = false;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		p2List_item<UI_Element*>* item = App->gui->UI_elements.start;
		while (item != nullptr)
		{
			if (item->data->dragging == false)
			{
				item->data->state = UI_Element::State::IDLE;
			}
			item = item->next;
		}
	}

	return ret;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	LOG("Freeing Main Menu");

	App->tex->UnLoad(menu_background->texture);
	App->tex->UnLoad(credits_window->texture);

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

	case UI_Element::UI_type::TEXT: //web text
		switch (data->action)
		{
		case UI_Element::Action::WEB:
		{
			switch (data->state)
			{
			case UI_Element::State::IDLE:
			{
				data->color = { 0,0,0,255 };
			}
			break;

			case UI_Element::State::HOVER:
			{
				data->color = { 0,0,255,255 };
			}
			break;
			}
		}break;
		}
		break;

	case UI_Element::UI_type::SLIDER: //slider
		switch (data->state)
		{
		case UI_Element::State::IDLE:
			{
				data->rect = { 0,0,26,26 };
			}
			break;

		case UI_Element::State::HOVER:
			{
				data->rect = { 52,0,26,26 };
			}
			break;

		case UI_Element::State::DRAG:
			{
				data->rect = { 78,90,26,26 };
			}
			break;
		}
		break;

	}
}

void j1MainMenu::MoveUI_Left()
{
	p2List_item<UI_Element*>* item = App->gui->UI_elements.start;
	while (item != nullptr)
	{
		if (item->data->type == UI_Element::UI_type::PUSHBUTTON && item->data != back_button)
		{
			item->data->globalpos.x = 200;
		}
		item = item->next;
	}
	back_button->visible = true;
	back_text->visible = true;

	if (credits == true)
	{
		credits_window->visible = true;	

		settings_window->visible = false;
		SetChildrenVisible(settings_window);
		settings_window_text->visible = false;
		vol_slider_bar->visible = false;
		sfx_slider_bar->visible = false;
		vol_slider_circle->visible = false;
		sfx_slider_circle->visible = false; 
	}
	else if (settings == true)
	{
		credits_window->visible = false;

		settings_window->visible = true;
		SetChildrenVisible(settings_window);
		settings_window_text->visible = true;
		vol_slider_bar->visible = true;
		sfx_slider_bar->visible = true;
		vol_slider_circle->visible = true;
		sfx_slider_circle->visible = true;
	}
}

void j1MainMenu::ResetUI_pos()
{
	p2List_item<UI_Element*>* item = App->gui->UI_elements.start;
	while (item != nullptr)
	{
		if (item->data->type == item->data->PUSHBUTTON)
		{
			item->data->globalpos.x = item->data->position.x;
		}
		item = item->next;
	}
	back_button->visible = false;
	back_text->visible = false;

	credits_window->visible = false;
	settings_window->visible = false;
	SetChildrenVisible(settings_window);
	settings_window_text->visible = false;
	vol_slider_bar->visible = false;
	sfx_slider_bar->visible = false;
	vol_slider_circle->visible = false;
	sfx_slider_circle->visible = false;

	credits = false;
	settings = false;
}

void j1MainMenu::SetChildrenVisible(UI_Element* elem)
{
	p2List_item<UI_Element*>* item = elem->children.start;
	while (item != nullptr)
	{
		item->data->visible = elem->visible;
		item = item->next;
	}
}