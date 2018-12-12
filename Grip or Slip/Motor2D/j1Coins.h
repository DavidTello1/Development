#ifndef __j1Coins_H__
#define __j1Coins_H__
#include "j1Entity.h"

class j1Coins : public Entity
{
public:
	j1Coins();
	j1Coins(iPoint position, iPoint Size);
	~j1Coins();

	void CleanUp();

	void Load(pugi::xml_node& file);
	void Save(pugi::xml_node& file) const;
};

#endif