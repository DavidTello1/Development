#ifndef __UI_ELEMENT_H__
#define __UI_ELEMENT_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "SDL\include\SDL_rect.h"

struct Dragable
{
	bool x, y;
};

class UI_Element
{
public:
	enum UI_type
	{
		UNKNOWN = 0,
		TEXT,
		IMAGE,
		PUSHBUTTON,
		CHECKBUTTON,
		INPUTTEXT,
		WINDOW
	};

	enum Action
	{
		NONE = 0,
		CONTINUE,
		NEW_GAME,
		SETTINGS,
		CREDITS,
		EXIT,
		PAUSE,
		ADJUST_VOL,
		RESUME,
		MAIN_MENU,

		MOVE_LEFT,
		DRAGABLE
	};

	enum State
	{
		NOTHING = 0,
		IDLE,
		HOVER,
		LOGIC,
		DRAG
	};

public:
	// Constructor
	UI_Element() {};
	UI_Element(UI_type type, UI_Element* parent, bool visible, Dragable dragable) {};

	// Destructor
	~UI_Element() {};

	virtual void DoLogic(Action action) {};

	void Center(bool x, bool y)
	{
		if (x)
		{
			position.x = parent->size.x / 2 - size.x / 2;
		}
		if (y)
		{
			position.y = parent->size.y / 2 - size.y / 2;
		}
	}

	void SetVisible()
	{
		visible = true;
	}

	void SetDragable(bool x, bool y)
	{
		if (x)
		{
			dragable.x = true;
		}
		if (y)
		{
			dragable.y = true;
		}
	}

	void Drag()
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		
		if (dragable.x == true)
		{
			globalpos.x = x - click_pos.x + start_drag_pos.x;
		}
		if (dragable.y == true)
		{
			globalpos.y = y - click_pos.y + start_drag_pos.y;
		}
	}

public:
	SDL_Rect rect;
	SDL_Texture* texture;
	SDL_Color color;

	UI_type type;
	Action action;
	State state;

	Dragable dragable;
	iPoint position;
	iPoint globalpos;
	iPoint start_drag_pos;
	iPoint size;
	SDL_Rect collider;
	const char* label = nullptr;
	bool visible = true;
	bool locked = false;

	UI_Element* parent;
	p2List<UI_Element*> children;

	iPoint click_pos;
	
};

#endif // __UI_ELEMENT__