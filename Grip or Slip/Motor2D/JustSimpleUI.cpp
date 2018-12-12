#include "p2Log.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "UI_Element.h"
#include "JustSimpleUI.h"
#include "j1Fonts.h"

JustSimpleUI::JustSimpleUI()
{
}

JustSimpleUI::~JustSimpleUI()
{
}

JustSimpleUI::JustSimpleUI(UI_type Type, iPoint pos, iPoint Size, UI_Element* Parent, bool Visible, Dragable Dragable, const char* Label)
{
	type = Type;
	position.x = pos.x;
	position.y = pos.y;
	globalpos = position;
	size = Size;
	label = Label;

	parent = Parent;
	visible = Visible;
	dragable = Dragable;

	texture = App->font->Print(label, color);
	App->font->CalcSize(label, size.x, size.y);

	collider = { position.x, position.y, size.x, size.y };

	if (parent != nullptr)
	{
		parent->children.add(this);
	}

	state = IDLE;
}