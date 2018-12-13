#ifndef __INTERACTIVEBUTTON_H__
#define __INTERACTIVEBUTTON_H__

#include "UI_Element.h"
#include "j1SceneChange.h"
#include "j1MainMenu.h"
#include "j1Scene.h"

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
			case UI_Element::Action::CONTINUE:
			{
				App->scenechange->SwitchScene(App->scene, App->main_menu);
				App->scenechange->ContinueGame = true;
			}
			break;

			case UI_Element::Action::NEW_GAME:
			{
				App->scenechange->SwitchScene(App->scene, App->main_menu);
				App->scene->ResetBoxPos();
				App->scene->ResetIngameUI();
				App->scene->Load_level(0);
				App->scene->currentMap = 0;

			}
			break;

			case UI_Element::Action::SETTINGS:
			{

			}
			break;

			case UI_Element::Action::CREDITS:
			{

			}
			break;

			case UI_Element::Action::EXIT:
			{
				App->main_menu->quit_game = true;
			}
			break;
		}
	};

public:
	UI_Element* text;
	UI_Element* image;
};

#endif // __INTERACTIVEBUTTON_H__