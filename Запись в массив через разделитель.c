#include "Header.h"


void LpwstrSpace(LPWSTR str)// � ��� ����� �� ��������
{
	HINSTANCE MyDll = LoadLibrary(DLLPATH);// ���������� DLL
	StrToFloat stf = (StrToFloat)GetProcAddress(MyDll, "LpwstrTofloat");	
	int y = 0, indArr = 0;
	
	for (int i = 0; i < wcslen(str); i++) // ������� ��� �� ��������
	{
		if (str[i] == L' ')
		{
			y = 0;
			indArr++;
		}
	}
	indArr+=2;// +1 �� ����� +1 �� �������� �����������
	float* outputArr = calloc(indArr, sizeof(float));
	outputArr[0] = indArr;
	y = 0;
	int z = 0;
	LPWSTR changeStr = (LPWSTR)malloc(sizeof(str));
	for (int i = 1; i < indArr; i++)// ��������� � ������
	{	
		while (1)// ������ ����� �� ������
		{
			if (str[z] != L' ')
			{
				changeStr[y] = str[z];
				y++;
				z++;
			}
			else
			{
				break;
			}
		}					
		changeStr[y] = L'\0';
		stf(changeStr, &(outputArr[i]));// ��������� � ������
		y = 0;
		z++;
		for (size_t u = 0; u < indArr+1; u++)// ������ ��������� ������
		{
			changeStr[u] = L'\0';
		}
	}
	for (size_t i = 1; i < outputArr[0]; i++)
	{
		printf("%f\n", outputArr[i]);
	}
	free(outputArr);
	FreeLibrary(MyDll);
}