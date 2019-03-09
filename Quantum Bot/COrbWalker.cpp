#include "COrbWalker.h"

unsigned LastAATick = 0;
DWORD OrbCanAttack = true;
DWORD OrbCanMove = true;
using te::Gosu;

float Gosu::GetDistance(CObject* target, CObject* target2)
{
	return target->GetPos().DistTo(target2->GetPos());

}

void Gosu::OrbWalk(CObject* target) {

	if (GetAsyncKeyState(VK_SPACE)) {
		Functions.PrintChat(*(DWORD*)(baseAddr + oChatClientPtr), "<font color='#C1FFAF'>[GONNA ATTACK]</font>", 1);

		if (target != nullptr) {
			Functions.PrintChat(*(DWORD*)(baseAddr + oChatClientPtr), "<font color='#C1FFAF'>[GONNA ATTACK2]</font>", 1);

			if (CanAttack()) {
				Functions.PrintChat(*(DWORD*)(baseAddr + oChatClientPtr), "<font color='#C1FFAF'>[GONNA ATTAC3]</font>", 1);

				if (GetDistance(target, me)) {
					Engine::AttackTarget(target);
					LastAATick = GetTickCount();
					OrbCanAttack = false;
					OrbCanMove = true;
				}
			}
			if (CanMove()) {
				Engine::MoveTo(&Engine::GetMouseWorldPosition());
				OrbCanAttack = true;
				if (CanAttack()) OrbCanMove = false;
			}
		}
		else {
			Engine::MoveTo(&Engine::GetMouseWorldPosition());
		}
	}
}

bool Gosu::CanMove() {
	return GetTickCount() + 50 / 2 >= LastAATick + me->GetAttackCastDelay() * 1000 && OrbCanMove;
}

bool Gosu::CanAttack() {
	return GetTickCount() + 50 / 2 + 25 >= LastAATick + me->GetAttackDelay() * 1000 && OrbCanAttack;
}
