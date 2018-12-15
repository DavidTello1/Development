#ifndef __INTERACTIVEBUTTON_H__
#define __INTERACTIVEBUTTON_H__

#include "UI_Element.h"
#include "j1SceneChange.h"
#include "j1MainMenu.h"
#include "j1Scene.h"
#include "j1Audio.h"

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
				App->scenechange->ContinueGame = true;
				App->scenechange->SwitchScene(App->scene, App->main_menu);
			}
			break;

			case UI_Element::Action::NEW_GAME:
			{
				App->scenechange->ContinueGame = false;
				App->scenechange->SwitchScene(App->scene, App->main_menu);
				App->scene->ResetBoxPos();
				App->scene->ResetIngameUI();
				App->scene->Load_level(0);
				App->scene->currentMap = 0;
			}
			break;

			case UI_Element::Action::SETTINGS:
			{
				App->main_menu->settings = true;
				App->main_menu->credits = false;
				App->main_menu->MoveUI_Left();
			}
			break;

			case UI_Element::Action::CREDITS:
			{
				App->main_menu->credits = true;
				App->main_menu->settings = false;
				App->main_menu->MoveUI_Left();
			}
			break;

			case UI_Element::Action::EXIT:
			{
				App->main_menu->quit_game = true;
			}
			break;

			case UI_Element::Action::BACK:
			{
				App->main_menu->ResetUI_pos();
			}
			break;

			case UI_Element::Action::ADJUST_VOL:
			{
				App->audio->AdjustMusicVol(App->main_menu->vol_value);
			}
			break;

			case UI_Element::Action::ADJUST_FX:
			{
				App->audio->AdjustSoundVol(App->main_menu->sfx_value);
			}
			break;

			case UI_Element::Action::SAVE:
			{
				App->SaveGame();
			}
			break;

			case UI_Element::Action::RESUME:
			{
				App->scene->pause = false;
			}
			break;

			case UI_Element::Action::MAIN_MENU:
			{
				App->scenechange->SwitchScene(App->main_menu, App->scene);
			}
			break;
		}
	};

public:
	UI_Element* text;
	UI_Element* image;
};

#endif // __INTERACTIVEBUTTON_H__