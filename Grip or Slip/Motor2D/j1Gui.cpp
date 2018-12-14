#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_Element.h"
#include "InteractiveButton.h"
#include "JustSimpleUI.h"
#include "j1Scene.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	UpdateChildren();
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	
	p2List_item<UI_Element*>*item = UI_elements.start;
	while (item != nullptr)
	{
		RELEASE(item->data);
		item = item->next;
	}
	UI_elements.clear();

	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

UI_Element* j1Gui::AddUIElement(UI_Element::UI_type type, UI_Element::Action action, iPoint pos, iPoint size, UI_Element* parent, bool visible, Dragable dragable, const char* label)
{
	UI_Element* UI_elem = nullptr;

	switch (type)
	{
	case UI_Element::UI_type::TEXT:
		UI_elem = new JustSimpleUI(type, pos, size, parent, visible, dragable, label, action);
		break;
	
	case UI_Element::UI_type::IMAGE:
		UI_elem = new JustSimpleUI(type, pos, size, parent, visible, dragable);
		break;

	case UI_Element::UI_type::BACKGROUND:
		UI_elem = new JustSimpleUI(type, pos, size, parent, visible, dragable);
		break;

	case UI_Element::UI_type::PUSHBUTTON:
		UI_elem = new InteractiveButton(type, action, pos, size, parent, visible, dragable);
		break;
												  
	case UI_Element::UI_type::WINDOW:
		UI_elem = new InteractiveButton(type, action, pos, size, parent, visible, dragable);
		break;

	case UI_Element::UI_type::SLIDER:
		UI_elem = new InteractiveButton(type, action, pos, size, parent, visible, dragable);
		break;
	}

	if (UI_elem)
		UI_elements.add(UI_elem);

		return UI_elem;
}

bool j1Gui::Draw()
{
	bool ret = true;
	p2List_item<UI_Element*>*UI_elem = UI_elements.start;

	while (UI_elem != nullptr)
	{
		if (UI_elem->data->visible == true)
		{
			if (UI_elem->data->type == UI_Element::UI_type::TEXT) //text
			{
				App->tex->UnLoad(UI_elem->data->texture);
				UI_elem->data->texture = App->font->Print(UI_elem->data->label, UI_elem->data->color);
				App->font->CalcSize(UI_elem->data->label, UI_elem->data->size.x, UI_elem->data->size.y);

				App->render->Blit(UI_elem->data->texture, UI_elem->data->globalpos.x, UI_elem->data->globalpos.y, 0, SDL_FLIP_NONE, 0);
			}
			else if (UI_elem->data->type != UI_Element::UI_type::BACKGROUND) //rest of ui
			{
				App->render->Blit(GetAtlas(), UI_elem->data->globalpos.x, UI_elem->data->globalpos.y, &UI_elem->data->rect, SDL_FLIP_NONE, 0);
			}
		}
		if (UI_Debug == true)
		{
			App->render->DrawQuad(UI_elem->data->collider, 255, 0, 0, 255, false, false);
		}

			UI_elem = UI_elem->next;
	}

	return ret;
}


bool j1Gui::CheckMousePos(UI_Element* data)
{
	bool ret = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	SDL_Rect MouseCollider = { x,y,1,1 };
	if (SDL_HasIntersection(&MouseCollider, &data->collider))
	{
		ret = true;
	}

	return ret;
}

bool j1Gui::CheckClick(UI_Element* data)
{
	bool ret = false;

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		App->input->GetMousePosition(data->click_pos.x, data->click_pos.y);
		data->start_drag_pos = data->globalpos;

		ret = true;
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		ret = true;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
	{
		data->dragging = false;
		return false;
	}

	return ret;
}

void j1Gui::UpdateChildren()
{
	p2List_item<UI_Element*>*item = UI_elements.start;
	while (item != nullptr)
	{
		if (item->data->parent != nullptr)
		{
			if (item->data->parent->visible == false)
			{
				item->data->visible = item->data->parent->visible; //update visibility
			}
			item->data->globalpos = item->data->parent->globalpos + item->data->position; //update position
		}

		item->data->collider = { item->data->globalpos.x, item->data->globalpos.y, item->data->size.x, item->data->size.y }; //update collider

		item = item->next;
	}
}
