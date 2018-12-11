#include "p2Log.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "UI_Element.h"
#include "InteractiveButton.h"

InteractiveButton::InteractiveButton()
{
}

InteractiveButton::~InteractiveButton()
{
}

InteractiveButton::InteractiveButton(UI_type Type, Action Action, iPoint pos, iPoint Size, UI_Element* Parent, bool Visible, Dragable Dragable)
{
	type = Type;
	position.x = pos.x;
	position.y = pos.y;
	globalpos = position;
	size = Size;
	collider = { position.x, position.y, size.x, size.y };

	parent = Parent;
	visible = Visible;
	dragable = Dragable;

	if (parent != nullptr)
	{
		parent->children.add(this);
	}

	state = IDLE;
	action = Action;
}