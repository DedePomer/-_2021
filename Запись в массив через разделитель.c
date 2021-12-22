#include "Header.h"


void LpwstrSpace(LPWSTR str)// в ней какая то проблема
{
	HINSTANCE MyDll = LoadLibrary(DLLPATH);// подключаем DLL
	StrToFloat stf = (StrToFloat)GetProcAddress(MyDll, "LpwstrTofloat");	
	int y = 0, indArr = 0;
	
	for (int i = 0; i < wcslen(str); i++) // считает кол во пробелов
	{
		if (str[i] == L' ')
		{
			y = 0;
			indArr++;
		}
	}
	indArr+=2;// +1 за число +1 за хранение размерности
	float* outputArr = calloc(indArr, sizeof(float));
	outputArr[0] = indArr;
	y = 0;
	int z = 0;
	LPWSTR changeStr = (LPWSTR)malloc(sizeof(str));
	for (int i = 1; i < indArr; i++)// записываю в массив
	{	
		while (1)// считаю числа из строки
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
		stf(changeStr, &(outputArr[i]));// записываю в массив
		y = 0;
		z++;
		for (size_t u = 0; u < indArr+1; u++)// очищаю временную строку
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