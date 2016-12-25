#pragma warning(disable: 4996)
#include"Windows.h"
#include<iostream>
#include<stdio.h>
using namespace std;

DWORD WINAPI reader(CONST LPVOID lpParam);
DWORD WINAPI writer(CONST LPVOID lpParam);
CONST HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, 0);
CONST HANDLE hEvent_r = CreateEvent(NULL, TRUE, FALSE, 0);
int main(){
	HANDLE hThread[2];
	SetEvent(hEvent);
	hThread[0] = CreateThread(NULL, 0, &writer, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, &reader, NULL, 0, NULL);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	for (int i = 0; i < 2; i++)
		CloseHandle(hThread[i]);
	getchar();
	return 0;
}

DWORD WINAPI writer(CONST LPVOID lpParam)
{
	WaitForSingleObject(hEvent, INFINITE);
	ResetEvent(hEvent);
	HANDLE hCons = GetStdHandle(STD_INPUT_HANDLE);
	DWORD count = 0;
	char str[BUFSIZ] = { 0 };
	HANDLE hFile = CreateFile("out.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
	//ReadConsole(hCons, str, sizeof(str), &count, NULL);
	//SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, str, strlen(str), &count, NULL);
	CloseHandle(hFile);
	SetEvent(hEvent_r);
	ExitThread(0);
}

DWORD WINAPI reader(CONST LPVOID lpParam)
{
	WaitForSingleObject(hEvent_r, INFINITE);
	ResetEvent(hEvent_r);
	int n = 0;
	DWORD count = 0;
	HANDLE hCons = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hFile = CreateFile("in.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	char str[BUFSIZ] = { 0 }, num[256] = { 0 };

	ReadFile(hFile, str, sizeof(str), &count, NULL);
	CloseHandle(hFile);
	n = atoi(str);
	n = (n*n);
	_itoa(n, num, 10);
	HANDLE hOut = CreateFile( "out.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL );
	WriteConsole(hCons, num, strlen(num), &count, NULL);
	WriteConsole(hCons, "\n", 1, &count, NULL);
	WriteFile( hOut, num, strlen( num ), NULL, NULL );
	SetEvent(hEvent);
	return 0;
}

