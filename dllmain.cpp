// dllmain.cpp : Define o ponto de entrada para da aplica��o DLL.
#include <time.h>
#include "stdafx.h"
#include "guicon.h"
#include "Engine.h"
#include "Hooks.h"
#include "detours.h"
DWORD LastAATick = 0;

#pragma comment(lib, "detours.lib")

CObjectManager* ObjManager;
CConsole Console;
CFunctions Functions;

clock_t lastmove = NULL;
bool bInit = false;
typedef HRESULT(WINAPI *Prototype_Present)(DWORD, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
Prototype_Present Original_Present;

DWORD LastAATick = 0;

bool CanAttack()
{
	if (GetTickCount() + 50 / 2 + 25 >= LastAATick + me->GetAttackDelay() * 1000)
	{
		return true;
	}
	return false;
}

VOID teste() {
	Console.print("jus a test");
}

HRESULT WINAPI Hooked_Present(DWORD Device, CONST RECT *pSrcRect, CONST RECT *pDestRect, HWND hDestWindow, CONST RGNDATA *pDirtyRegion) {
	
	
	if (me) {
		if (!bInit) {
			Console.print("Nome: %s | Champion: %s | HP: %f\n", me->GetName(), me->GetChampionName(), me->GetHealth());
			bInit = true;
		}

		if (GetKeyState('D') & 0x8000) {

			Console.print("nick: %s | Campeao: %s | HP: %f\n", me->GetName(), me->GetChampionName(), me->GetHealth());
			teste();

		}


		/*so vamos andar se o farm ou o kite estiverem ativados atrav�s dos nossos hooks no processo*/
		if (GetKeyState('X') || GetAsyncKeyState(VK_SPACE) & 0x8000)/*Check if high-order bit is set (1 << 15)*/
		{
			//Soh se estiver vivo, acaba com o problema de draws indesejados.
			if (me->IsAlive()) {
				auto color = createRGB(0, 255, 0);
				Functions.DrawCircle(&me->GetPos(), me->GetAttackRange() + me->GetBoundingRadius(), &color, 0, 0.0f, 0, 0.5f);

				if (lastmove == NULL || clock() - lastmove > 30.0f) {
					lastmove = clock();
					Engine::MoveTo(&Engine::GetMouseWorldPosition());
				}
			}
		}


	}

	//Para prop�sitos de debug. Juntar v�rias informa��es poss�veis
	
	/*
	if (GetKeyState('D') & 0x8000) {

		if (ObjManager) {
			for (int i = 0; i < 10000; i++) {
				CObject* obj = Engine::GetObjectByID(i);
				if (obj) {
					if (obj->IsHero()) {
						if (obj->IsAlive() && obj->IsVisible() && obj->GetTeam() != me->GetTeam()) {
							//auto color = createRGB(255, 0, 0);
							//Functions.DrawCircle(&obj->GetPos(), obj->GetAttackRange() + obj->GetBoundingRadius(), &color, 0, 0.0f, 0, 0.5f); //Draw range (do inimigo?)


							//LastAATick = GetTickCount();
							//Engine::AttackTarget(obj);

						}
					}
				}
			}
		}

	}*/


	if (GetKeyState('A') & 0x8000) {

		if (ObjManager) {
			for (int i = 0; i < 10000; i++) {
				CObject* obj = Engine::GetObjectByID(i);
				if (obj) {
					if (obj->IsHero()) {
						if (obj->IsAlive() && obj->IsVisible() && obj->GetTeam() != me->GetTeam()) {
							auto color = createRGB(255, 0, 0);
							Functions.DrawCircle(&obj->GetPos(), obj->GetAttackRange() + obj->GetBoundingRadius(), &color, 0, 0.0f, 0, 0.5f); //Draw range (do inimigo?)
						
							
							//LastAATick = GetTickCount();
							Engine::AttackTarget(obj);
							
						}
					}
				}
			}
		}

	}
	/*lol*/
	if (ObjManager) {
		for (int i = 0; i < 10000; i++) {
			CObject* obj = Engine::GetObjectByID(i);
			if (obj) {
				if (obj->IsHero()) {
					if (obj->IsAlive() && obj->IsVisible() && obj->GetTeam() != me->GetTeam()) {
						auto color = createRGB(255, 0, 0);
						Functions.DrawCircle(&obj->GetPos(), obj->GetAttackRange() + obj->GetBoundingRadius(), &color, 0, 0.0f, 0, 0.5f); //Draw range
					}
				}
			}
		}
	}
	return Original_Present(Device, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
}

DWORD FindDevice(DWORD Len)
{
	DWORD dwObjBase = 0;

	dwObjBase = (DWORD)LoadLibrary("d3d9.dll");
	while (dwObjBase++ < dwObjBase + Len)
	{
		if ((*(WORD*)(dwObjBase + 0x00)) == 0x06C7
			&& (*(WORD*)(dwObjBase + 0x06)) == 0x8689
			&& (*(WORD*)(dwObjBase + 0x0C)) == 0x8689
			) {
			dwObjBase += 2; break;
		}
	}
	return(dwObjBase);
}

DWORD GetDeviceAddress(int VTableIndex)
{
	PDWORD VTable;
	*(DWORD*)&VTable = *(DWORD*)FindDevice(0x128000);
	return VTable[VTableIndex];
}

void __stdcall Start() {

	/*Iniciamos o nosso console para depura~��o*/
	Console.startConsoleWin(80, 25, NULL);
	
	/*Iniciar as fun��es do jogo apenas quando o jogo iniciar*/
	while (Engine::GetGameTime() < 1.0f || !me)
		Sleep(1);

	ObjManager = (CObjectManager*)(baseAddr + oObjManager);

	Functions.PrintChat = (Typedefs::fnPrintChat)(baseAddr + oPrintChat);
	Functions.IsTargetable = (Typedefs::fnIsTargetable)(baseAddr + oIsTargetable);
	Functions.IsAlive = (Typedefs::fnIsAlive)(baseAddr + oIsAlive);

	Functions.IsMinion = (Typedefs::fnIsMinion)(baseAddr + oIsMinion);
	Functions.IsTurret = (Typedefs::fnIsTurret)(baseAddr + oIsTurret);
	Functions.IsHero = (Typedefs::fnIsHero)(baseAddr + oIsHero);
	Functions.IsMissile = (Typedefs::fnIsMissile)(baseAddr + oIsMissile);
	Functions.IsNexus = (Typedefs::fnIsNexus)(baseAddr + oIsNexus);
	Functions.IsInhibitor = (Typedefs::fnIsInhibitor)(baseAddr + oIsInhib);
	Functions.IsTroyEnt = (Typedefs::fnIsTroyEnt)(baseAddr + oIsTroy);

	//Functions.CastSpell = (Typedefs::fnCastSpell)((DWORD)GetModuleHandle(NULL) + oCastSpell);
	Functions.IssueOrder = (Typedefs::fnIssueOrder)((DWORD)GetModuleHandle(NULL) + oIssueOrder);
	Functions.DrawCircle = (Typedefs::fnDrawCircle)((DWORD)GetModuleHandle(NULL) + oDrawCircle);

	Functions.GetAttackCastDelay = (Typedefs::fnGetAttackCastDelay)((DWORD)GetModuleHandle(NULL) + oGetAttackCastDelay);
	Functions.GetAttackDelay = (Typedefs::fnGetAttackDelay)((DWORD)GetModuleHandle(NULL) + oGetAttackDelay);

	Original_Present = (Prototype_Present)DetourFunction((PBYTE)GetDeviceAddress(17), (PBYTE)Hooked_Present);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Start, 0, 0, 0);
		return TRUE;
	}

	else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		return TRUE;
	}
	return FALSE;
}


