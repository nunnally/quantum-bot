#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include "guicon.h"

using namespace std;

//numero maximo de linhas que a saida do console deve ter

static const WORD MAX_CONSOLE_LINES = 500;

FILE* __fStdOut = NULL;
HANDLE __hStdOut = NULL;


//fname (tamanho se vc especificar)
//O file pointer é fechado automaticamente qunado fechar o app.

void CConsole::startConsoleWin(int width, int height, char* fname)
{
	AllocConsole();
	SetConsoleTitle("Debug Window");
	__hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD co = { width,height };
	SetConsoleScreenBufferSize(__hStdOut, co);
	if (fname)
		__fStdOut = fopen(fname, "w");
}
// Usar o print como TRACE0, TRACE1, ... (Os argumentos sao os mesmos como no print)
int CConsole::print(char *fmt, ...)
{
	char s[3000];
	va_list argptr;
	int cnt;
	va_start(argptr, fmt);
	cnt = vsprintf(s, fmt, argptr);
	va_end(argptr);
	DWORD cCharsWritten;
	if (__hStdOut)
		WriteConsole(__hStdOut, s, strlen(s), &cCharsWritten, NULL);
	if (__fStdOut)
		fprintf(__fStdOut, s);
	return(cnt);
}
