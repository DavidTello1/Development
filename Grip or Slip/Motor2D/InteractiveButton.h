#ifndef __INTERACTIVEBUTTON_H__
#define __INTERACTIVEBUTTON_H__

#include "UI_Element.h"

class InteractiveButton : public UI_Element
{
public:
	InteractiveButton();
	InteractiveButton(UI_type type, Action action, iPoint position, iPoint size, UI_Element* parent, bool visible, Dragable dragable);
	~InteractiveButton();

	void DoLogic(Action action) 
	{
		switch (action)
		{
		case UI_Element::Action::MOVE_LEFT:
			if (parent != nullptr)
			{
				position.x -= 50;
			}
			else
			{
				globalpos.x -= 50;
			}
			break;

		case UI_Element::Action::DRAGABLE:
			dragable.x = true;
			dragable.y = true;
		}
	};

public:
	UI_Element* text;
	UI_Element* image;
};

#endif // __INTERACTIVEBUTTON_H__