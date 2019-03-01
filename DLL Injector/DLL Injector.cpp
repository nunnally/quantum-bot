#include "pch.h"
#include <iostream>
#include <Windows.h>

int main()
{
	//Caminho da nossa URL (TEM QUE SER O FULL PATH)
	LPCSTR DllPath = "";

	// Handle
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 000);

	//Alocar memoria do path no processo alvo
	// (tamanho da string + null)
	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(DllPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);

	WriteProcessMemory(hProcess, pDllPath, (LPVOID)DllPath,
		strlen(DllPath) + 1, 0);

	// Cria Remote thread
	HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),
			"LoadLibraryA"), pDllPath, 0, 0);

	// Espera pela execução 
	WaitForSingleObject(hLoadThread, INFINITE);

	//std::cout << "Alocado em: " << std::hex << pDllPath << std::endl;
	std::cout << "Ta la \n";
	std::cin.get();

	//Limpa a memoria alocada pro path da dll
	VirtualFreeEx(hProcess, pDllPath, strlen(DllPath) + 1, MEM_RELEASE);

	return 0;


}
