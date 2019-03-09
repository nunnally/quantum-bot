#pragma once
#include <time.h>
#include "stdafx.h"
#include "guicon.h"
#include "Engine.h"
#include "Hooks.h"
#include "detours.h"

namespace te {
	class Gosu;

}
class te::Gosu
{

public:

	Gosu();
	~Gosu();
	static void OrbWalk(CObject * target);
	static bool CanMove();
	static bool CanAttack();
	static float GetDistance(CObject* target, CObject* target2);


};

