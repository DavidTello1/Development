#ifndef __JUSTSIMPLEUI_H__
#define __JUSTSIMPLEUI_H__

#include "UI_Element.h"

class JustSimpleUI : public UI_Element
{
public:
	JustSimpleUI();
	JustSimpleUI(UI_type type, iPoint position, iPoint size, UI_Element* parent, bool visible, Dragable dragable, const char* label = nullptr, Action action = NONE);
	~JustSimpleUI();

	void DoLogic(Action action)
	{
		switch (action)
		{
		case UI_Element::Action::WEB:
		{
			ShellExecuteA(NULL, "open", "www.google.com", NULL, NULL, SW_SHOWNORMAL);
		}
		break;
		}
	}
};

#endif // __JUSTSIMPLEUI_H__