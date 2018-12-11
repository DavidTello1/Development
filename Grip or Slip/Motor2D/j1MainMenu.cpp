#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1MainMenu.h"
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
	window_ui = App->gui->AddUIElement(UI_Element::UI_type::WINDOW, UI_Element::Action::NONE, { App->render->viewport.w / 2, App->render->viewport.h / 12 }, { 424,458 }, nullptr, true, { true, true });
	button = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::MOVE_LEFT, { 112,229 }, { 230,64 }, window_ui, true);
	button_text = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, button, true, { false, false }, "Hello World");
	button2 = App->gui->AddUIElement(UI_Element::UI_type::PUSHBUTTON, UI_Element::Action::DRAGABLE, { App->render->viewport.w / 6, App->render->viewport.h / 12 + 75}, { 230,64 }, nullptr, true);
	button_text2 = App->gui->AddUIElement(UI_Element::UI_type::TEXT, UI_Element::Action::NONE, { 0,0 }, { 0,0 }, button2, true, { false, false }, "Dragable");

	return true;
}

// Called each loop iteration
bool j1MainMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1MainMenu::Update(float dt)
{
	// -------
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_REPEAT)
		App->gui->UI_Debug = !App->gui->UI_Debug;

	// gui update
	p2List_item<UI_Element*>* item = App->gui->UI_elements.end;
	while (item != nullptr)
	{
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
	App->gui->Draw();

	return true;
}

// Called each loop iteration
bool j1MainMenu::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	LOG("Freeing Main Menu");

	return true;
}
