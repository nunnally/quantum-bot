#pragma once
#include <time.h>
#include "stdafx.h"
#include "guicon.h"
#include "Engine.h"
#include "Hooks.h"
#include "detours.h"
CObjectManager* ObjManager;
CConsole Console;
CFunctions Functions;
class COrbWalker
{
public:
	COrbWalker();
	~COrbWalker();
	void OrbWalk(CObject * target);
	bool CanMove();
	bool CanAttack();
};

