#include "COrbWalker.h"

DWORD LastAATick = 0;
DWORD OrbCanAttack = true;
DWORD OrbCanMove = true;

void COrbWalker::OrbWalk(CObject* target) {
	if (target != nullptr) {
		if (CanAttack()) {
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

bool COrbWalker::CanMove() {
	return GetTickCount() + 50 / 2 >= LastAATick + me->GetAttackCastDelay() * 1000 && OrbCanMove;
}

bool COrbWalker::CanAttack() {
	return GetTickCount() + 50 / 2 + 25 >= LastAATick + me->GetAttackDelay() * 1000 && OrbCanAttack;
}

float GetDistance(CObject* target, CObject* target2)
{
	return target->GetPos().DistTo(target2->GetPos());

}