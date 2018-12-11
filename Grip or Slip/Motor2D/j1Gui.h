#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "UI_Element.h"
#include "SDL/include/SDL_rect.h"

#define CURSOR_WIDTH 2

class UI_Element;
// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Draw();
	bool CheckMousePos(UI_Element* data);
	bool CheckClick(UI_Element* data);
	void UpdateState(UI_Element* data);
	void UpdateChildren();

	// Gui creation functions
	UI_Element* AddUIElement(UI_Element::UI_type Type, UI_Element::Action Action, iPoint pos, iPoint size, UI_Element* parent = nullptr, bool visible = false, Dragable dragable = { false, false }, const char* label = nullptr);

	SDL_Texture* GetAtlas() const;

private:

	p2SString atlas_file_name;
	SDL_Texture* atlas;

public:

	p2List<UI_Element*> UI_elements;
	SDL_Rect* section;

	bool UI_Debug = false;
	bool dragging = false;

};

#endif //__J1_GUI_H__
