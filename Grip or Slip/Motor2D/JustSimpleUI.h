#ifndef __JUSTSIMPLEUI_H__
#define __JUSTSIMPLEUI_H__

#include "UI_Element.h"

class JustSimpleUI : public UI_Element
{
public:
	JustSimpleUI();
	JustSimpleUI(UI_type type, iPoint position, iPoint size, UI_Element* parent, bool visible, Dragable dragable, const char* label = nullptr);
	~JustSimpleUI();
};

#endif // __JUSTSIMPLEUI_H__