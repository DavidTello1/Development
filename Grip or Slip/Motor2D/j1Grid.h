#ifndef __j1Grid_H__
#define __j1Grid_H__
#include "j1Entity.h"

class j1Grid : public Entity
{
public:
	j1Grid();
	j1Grid(iPoint position, iPoint Size, p2SString Type);
	~j1Grid();

	bool Update(float dt);
	void CleanUp();

	void Load(pugi::xml_node& file);
	void Save(pugi::xml_node& file) const;

	void GetType();
	void ChangeAnimation();

};

#endif