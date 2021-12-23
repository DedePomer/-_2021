#pragma once

#include <stdio.h>
#include <windows.h>
#include <Math.h>

#define DLLPATH L"DLL.dll"
#define FILEPATH L"InputOutput.txt"

// ������� �� DLL
typedef LPWSTR(_cdecl* FloatToStr)(float*);
typedef LPWSTR (_cdecl* Con)(LPWSTR, LPWSTR);
typedef BOOL(_cdecl* StrToFloat)(LPWSTR, float*);

struct LinList// ��������� ��� �������������� ������
{
	int value;
	struct LinList* NextStruct;
};
typedef struct LinList SList; // �������� ���� �� ���� ���������

// ���������� �������
int ArgumentFunction(int (*f)(int));
SList* CreateList(int);
void ShowList(SList*);
DWORD WINAPI Fact(LPVOID);
DWORD WINAPI CritSection(LPVOID);
void LpwstrSpace(LPWSTR);
DWORD WINAPI WatchSec(LPVOID);
DWORD WINAPI WatchButtonPressing(LPVOID);
